#include "Channel.hpp"

#include <glog/logging.h>
#include <sys/epoll.h>
#include "EventLoop.hpp"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd__)
  : loop_(loop),
    fd_(fd__),
    events_(0),
    revents_(0),
    index_(-1),
    eventHandling_(false)
{
}

Channel::~Channel()
{
}

void Channel::update()
{
  loop_->updateChannel(this);
}

void Channel::remove()
{
  loop_->removeChannel(this);
}

void Channel::handleEvent(int32_t receiveTime)
{
  eventHandling_ = true;
  LOG(INFO) << reventsToString();
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
  {
    LOG(INFO) << "Channel::handle_event() POLLHUP";
    if (closeCallback_) closeCallback_();
  }
  if (revents_ & (EPOLLERR))
  {
    if (errorCallback_) errorCallback_();
  }
  if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
  {
    if (readCallback_) readCallback_(receiveTime);
  }
  if (revents_ & EPOLLOUT)
  {
    if (writeCallback_) writeCallback_();
  }
  eventHandling_ = false;
}

std::string Channel::reventsToString() const
{
  std::ostringstream oss;
  oss << fd_ << ": ";
  if (revents_ & EPOLLIN)
    oss << "IN ";
  if (revents_ & EPOLLPRI)
    oss << "PRI ";
  if (revents_ & EPOLLOUT)
    oss << "OUT ";
  if (revents_ & EPOLLHUP)
    oss << "HUP ";
  if (revents_ & EPOLLRDHUP)
    oss << "RDHUP ";
  if (revents_ & EPOLLERR)
    oss << "ERR ";
  return oss.str().c_str();
}
