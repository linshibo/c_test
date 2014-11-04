#ifndef EVENT_LOOP_H 
#define EVENT_LOOP_H

#include <memory>
#include <vector>
#include <functional>
#include <mutex>
#include "Noncopyable.hpp"
class Poller;

class EventLoop:public noncopyable{
    public:
        EventLoop ();
    using Functor = std::function<void()>;
        //typedef std:function<void()>  Functor
        virtual ~EventLoop ();
        void quit();
        void loop();
        int pollReturnTime() const { return pollReturnTime_; }
        void updateChannel(Channel* channel);
        void runInLoop(Functor &&cb);
        void removeChannel(Channel* channel);
    private:
        void handleRead();  // waked up

        void doPendingFunctors();

        void printActiveChannels() const; // DEBUG

        typedef std::vector<Channel*> ChannelList;

        bool looping_; /* atomic */
        bool quit_; /* atomic and shared between threads, okay on x86, I guess. */
        int32_t  pollReturnTime_;
        std::shared_ptr<Poller> poller_;
        ChannelList activeChannels_;
        //std::mutex mutex_;
        std::vector<Functor > pendingFunctors_; // @GuardedBy mutex_
};


#endif /*EVENT_LOOP_H*/
