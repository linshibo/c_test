#include "Poller.hpp"
#include <sys/epoll.h>

namespace
{
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

Poller(EventLoop* loop)
  :epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(16)
{
  if (epollfd_ < 0)
  {
    LOG(FATAL) << "Poller::Poller";
  }
}

Poller::~Poller()
{
  ::close(epollfd_);
}

int Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
  int numEvents = ::epoll_wait(epollfd_,
                               &*events_.begin(),
                               static_cast<int>(events_.size()),
                               timeoutMs);
  int savedErrno = errno;
  int now = time(NULL)
  if (numEvents > 0)
  {
    LOG(TRACE) << numEvents << " events happended";
    fillActiveChannels(numEvents, activeChannels);
    events_.resize(events_.size()*2);
  } else if (numEvents == 0) {
    LOG(TRACE) << " nothing happended";
  } else {
    // error happens, log uncommon ones
    if (savedErrno != EINTR)
    {
      errno = savedErrno;
      LOG(ERROR) << "Poller::poll()";
    }
  }
  return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

void Poller::updateChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  LOG(TRACE) << "fd = " << channel->fd() << " events = " << channel->events();
  const int index = channel->index();
  if (index == kNew || index == kDeleted)
  {
    // a new one, add with EPOLL_CTL_ADD
    int fd = channel->fd();
    if (index == kNew)
    {
      assert(channels_.find(fd) == channels_.end());
      channels_[fd] = channel;
    }
    else // index == kDeleted
    {
      assert(channels_.find(fd) != channels_.end());
      assert(channels_[fd] == channel);
    }
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else
  {
    // update existing one with EPOLL_CTL_MOD/DEL
    int fd = channel->fd();
    (void)fd;
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    assert(index == kAdded);
    if (channel->isNoneEvent())
    {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    }
    else
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void Poller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  int fd = channel->fd();
  LOG_TRACE << "fd = " << fd;
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);
  assert(channel->isNoneEvent());
  int index = channel->index();
  assert(index == kAdded || index == kDeleted);
  size_t n = channels_.erase(fd);
  (void)n;
  assert(n == 1);

  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}

void Poller::update(int operation, Channel* channel)
{
  struct epoll_event event;
  bzero(&event, sizeof event);
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
  {
    if (operation == EPOLL_CTL_DEL)
    {
      LOG_SYSERR << "epoll_ctl op=" << operation << " fd=" << fd;
    }
    else
    {
      LOG_SYSFATAL << "epoll_ctl op=" << operation << " fd=" << fd;
    }
  }
}

