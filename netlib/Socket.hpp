#ifndef SOCKET_HPP 
#define SOCKET_HPP
#include "Buffer.hpp"
#include <memory>
class Channel;
class EventLoop;

class Socket:public std::enable_shared_from_this<Socket>{
    public:
        typedef std::function<void (const std::shared_ptr<Socket>&)> Callback;
        typedef std::function<void()> EventCallback;
        typedef std::function<void(int32_t)> ReadEventCallback;
        Socket(EventLoop*loop, int fd) ;
        
        void handleRead(int32_t);
        
        void handleWrite();
        
        void handleClose();

        void handleError();

        int Send(char* data, int len);
        void setMessageCallback(const Callback& cb)
        { messageCallback_ = cb; }

        void setWriteCompleteCallback(const Callback& cb)
        { writeCallback_ = cb; }

        void setCloseCallback(const Callback& cb)
        { closeCallback_ = cb; }
        int getFD(){return fd_;}
        void setChanReadCallback(ReadEventCallback&& cb);
        void setChanCloseCallback(EventCallback&& cb);
        struct sockaddr_in* getAddr(){return & addr_;}
        void setAddr(struct sockaddr_in & a){ addr_ = a;}
        Channel* getChan(){return chan_.get();}
        Buffer inputBuffer_;
        Buffer outputBuffer_;
    private:

        Callback messageCallback_;
        Callback closeCallback_;
        Callback writeCallback_;

        struct sockaddr_in addr_;
        int fd_;
        EventLoop* loop_;
        std::shared_ptr<Channel> chan_;
};

#endif /*SOCKET_HPP*/
