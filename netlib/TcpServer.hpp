#ifndef TCPSERVER_HPP 
#define TCPSERVER_HPP
#include <string>
#include <functional>
#include <map>
#include "Socket.hpp"

class EventLoop;

class TcpServer{
    public:
        typedef std::function<void (const std::shared_ptr<Socket>&)> Callback;
        TcpServer(std::string host);
        void start();
        void accept(int32_t);
        void acceptorCloseCallback();
        void connectionClose(const std::shared_ptr<Socket>& conn);
        void setConnectionCallback(const Callback& cb)
        { connectionCallback_ = cb; }
        void setMessageCallback(const Callback& cb)
        { messageCallback_ = cb; }
        void setCloseCallback(const Callback& cb)
        { closeCallback_ = cb; }
    private:
        int bind();
        int listen();
        void setWriteCompleteCallback(const Callback& cb)
        { writeCompleteCallback_ = cb; }
        Callback connectionCallback_;
        Callback messageCallback_;
        Callback closeCallback_;
        Callback writeCompleteCallback_;
        std::shared_ptr<EventLoop> loop_;
        std::string host_;
        std::map<int, std::shared_ptr<Socket> > connections_;
        std::shared_ptr<Socket> acceptor;
};

#endif /*TCPSERVER_HPP*/
