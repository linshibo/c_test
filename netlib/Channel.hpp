#ifndef CHANNEL_HPP 
#define CHANNEL_HPP
#include <funtional>
#include "Noncopyable.hpp"

class Channel : noncopyable
{
 public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(int32_t)> ReadEventCallback;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void handleEvent(int32_t receiveTime);

  void setReadCallback(ReadEventCallback&& cb)
  { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback&& cb)
  { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback&& cb)
  { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback&& cb)
  { errorCallback_ = std::move(cb); }

  int fd() const { return fd_; }
  int events() const { return events_; }
  void set_revents(int revt) { revents_ = revt; } // used by pollers
  bool isNoneEvent() const { return events_ == kNoneEvent; }

  void enableReading() { events_ |= kReadEvent; update(); }
  void enableWriting() { events_ |= kWriteEvent; update(); }
  void disableWriting() { events_ &= ~kWriteEvent; update(); }
  void disableAll() { events_ = kNoneEvent; update(); }
  bool isWriting() const { return events_ & kWriteEvent; }

  // for Poller
  int index() { return index_; }
  void set_index(int idx) { index_ = idx; }

  // for debug
  std::string reventsToString() const;


  EventLoop* ownerLoop() { return loop_; }
  void remove();

 private:
  void update();

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;
  const int  fd_;
  int        events_;
  int        revents_;
  int        index_; // used by Poller.

  bool eventHandling_;
  ReadEventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

#endif /*CHANNEL_HPP*/
