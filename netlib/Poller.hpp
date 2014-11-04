#ifndef POLLER_HPP 
#define POLLER_HPP
#include <vector>
#include <map>
#include "Noncopyable.hpp"

class EventLoop;
class Channel;
class Poller : public noncopyable
{
 public:
  Poller(EventLoop* loop);
  virtual ~Poller();

  typedef std::vector<Channel*> ChannelList;
  int poll(int timeoutMs, ChannelList* activeChannels);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  static Poller* newDefaultPoller(EventLoop* loop){
      return new Poller(loop);
  }
 private:

  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;
  void update(int operation, Channel* channel);

  typedef std::vector<struct epoll_event> EventList;

  typedef std::map<int, Channel*> ChannelMap;

  int epollfd_;
  EventList events_;
  ChannelMap channels_;
  EventLoop* ownerLoop_;
};

#endif /*POLLER_HPP*/
