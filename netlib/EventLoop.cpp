#include <glog/logging.h>
#include "EventLoop.hpp"


const int kPollTimeMs = 10000;

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    poller_(Poller::newDefaultPoller(this)),
{
  LOG(DEBUG) << "EventLoop created " << this << " in thread " << threadId_;
}

EventLoop::~EventLoop()
{
  LOG(DEBUG) << "EventLoop " << this << " of thread " << threadId_
            << " destructs in thread " << CurrentThread::tid();
  ::close(wakeupFd_);
  t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
  assert(!looping_);
  looping_ = true;
  quit_ = false;  
  LOG(TRACE) << "EventLoop " << this << " start looping";
  while (!quit_)
  {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    ++iteration_;
    printActiveChannels();
    eventHandling_ = true;
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
        it->handleEvent(pollReturnTime_);
    }
    doPendingFunctors();
  }
  LOG(TRACE) << "EventLoop " << this << " stop looping";
  looping_ = false;
}

void EventLoop::quit()
{
  quit_ = true;
}

void EventLoop::runInLoop(const Functor& cb)
{
    cb();
}

//TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
//{
  //return timerQueue_->addTimer(cb, time, 0.0);
//}

//TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
//{
  //Timestamp time(addTime(Timestamp::now(), delay));
  //return runAt(time, cb);
//}

//TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
//{
  //Timestamp time(addTime(Timestamp::now(), interval));
  //return timerQueue_->addTimer(cb, time, interval);
//}

//void EventLoop::cancel(TimerId timerId)
//{
  //return timerQueue_->cancel(timerId);
//}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
  if (eventHandling_)
  {
    assert(currentActiveChannel_ == channel ||
        std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
  }
  poller_->removeChannel(channel);
}


void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
  MutexLockGuard lock(mutex_);
  functors.swap(pendingFunctors_);
  }

  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  callingPendingFunctors_ = false;
}

void EventLoop::printActiveChannels() const
{
  for (ChannelList::const_iterator it = activeChannels_.begin();
      it != activeChannels_.end(); ++it)
  {
    const Channel* ch = *it;
    LOG(TRACE) << "{" << ch->reventsToString() << "} ";
  }
}

