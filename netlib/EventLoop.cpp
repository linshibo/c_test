#include <glog/logging.h>
#include "Channel.hpp"
#include "Poller.hpp"
#include "EventLoop.hpp"


const int kPollTimeMs = 10000;

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    poller_(Poller::newDefaultPoller(this))
{
  LOG(INFO) << "EventLoop created " << this ;
}

EventLoop::~EventLoop()
{
  LOG(INFO) << "EventLoop " << this << " of thread " ;
}

void EventLoop::loop()
{
  looping_ = true;
  quit_ = false;  
  LOG(INFO) << "EventLoop " << this << " start looping";
  while (!quit_)
  {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    printActiveChannels();
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
        (*it)->handleEvent(pollReturnTime_);
    }
    doPendingFunctors();
  }
  LOG(INFO) << "EventLoop " << this << " stop looping";
  looping_ = false;
}

void EventLoop::quit()
{
  quit_ = true;
}


void EventLoop::updateChannel(Channel* channel)
{
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
  poller_->removeChannel(channel);
}

void EventLoop::runInLoop(Functor &&cb){
    pendingFunctors_.push_back(cb);
}

void EventLoop::doPendingFunctors()
{
  //std::vector<Functor> functors;
  //{
    //std::lock_guard<std::mutex> lock(mutex_);
    //functors.swap(pendingFunctors_);
  //}
  for (size_t i = 0; i < pendingFunctors_.size(); ++i)
  {
    pendingFunctors_[i]();
  }
}

void EventLoop::printActiveChannels() const
{
  for (ChannelList::const_iterator it = activeChannels_.begin();
      it != activeChannels_.end(); ++it)
  {
    const Channel* ch = *it;
    LOG(INFO) << "{" << ch->reventsToString() << "} ";
  }
}

