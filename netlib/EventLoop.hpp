#ifndef EVENT_LOOP_H 
#define EVENT_LOOP_H

#include <memory>
#include <vector>
#include <functional>
#include "Noncopyable.hpp"

class EventLoop:public noncopyable{
    public:
        EventLoop ();
        virtual ~EventLoop ();
        typedef std::function<void()> Functor;
        EventLoop();
        ~EventLoop(); 
        void quit();
        int pollReturnTime() const { return pollReturnTime_; }
        void updateChannel(Channel* channel);
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
        Channel* currentActiveChannel_;
        std::mutex_guard mutex_;
        std::vector<Functor> pendingFunctors_; // @GuardedBy mutex_
};


#endif /*EVENT_LOOP_H*/
