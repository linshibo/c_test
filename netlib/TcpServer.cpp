#include "TcpServer.hpp"
#include "Socket.hpp"
#include "EventLoop.hpp"
#include "Channel.hpp"
#include <vector>
#include <glog/logging.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <stdlib.h>
#include <memory>
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

TcpServer::TcpServer(std::string host):loop_(new EventLoop()), host_(host) {
}

void TcpServer::start(){

    int fd = socket(AF_INET,SOCK_STREAM,0);
    std::shared_ptr<Socket> p(new Socket(loop_.get(), fd));
    acceptor = p;
    acceptor->setChanReadCallback(std::bind(&TcpServer::accept, this, std::placeholders::_1));
    acceptor->setChanCloseCallback(std::bind(&TcpServer::acceptorCloseCallback, this));
    int ret=bind();
    if(ret != 0) {
        LOG(ERROR)<<"bind error";
        return ;
    }
    ret=listen();
    if(ret != 0) {
        LOG(ERROR)<<"listen error";
        return ;
    }
    loop_->loop();
}

void TcpServer::acceptorCloseCallback(){
    loop_->quit();
}

int TcpServer::bind(){
    std::vector<std::string> elems;
    auto ch = ":";
    split(host_, ch[0] , elems);
    if(elems.size() != 2) {
        LOG(ERROR)<<"host ERROR";
        return -1;
    }
    printf("xxxxx\n");
    auto paddr= acceptor->getAddr();
    bzero(paddr,sizeof(*paddr));
    paddr->sin_family = AF_INET;  
    paddr->sin_port=htons(atoi(elems[1].c_str()));  
    paddr->sin_addr.s_addr = inet_addr(elems[0].c_str());  
    int ret = ::bind(acceptor->getFD(), static_cast<struct sockaddr*>(reinterpret_cast<void*>(paddr)), static_cast<socklen_t>(sizeof *paddr));
    return ret;
}

int TcpServer::listen(){
    int ret = ::listen(acceptor->getFD(), SOMAXCONN);
    if (ret < 0)
    {
        LOG(FATAL) << "sockets::listenOrDie";
    }
    return ret;
}

void TcpServer::connectionClose(const std::shared_ptr<Socket>& conn){
    connections_.erase(conn->getFD());
    LOG(INFO)<<"TcpServer::connectionClose"<<connections_.size();
    closeCallback_(conn);
}

void TcpServer::accept(int32_t){
    struct sockaddr_in addr;
    socklen_t addrlen = static_cast<socklen_t>(sizeof addr);
    int connfd = ::accept(acceptor->getFD(), static_cast<struct sockaddr*>(reinterpret_cast<void*>(&addr)), &addrlen);
    if (connfd < 0)
    {
        int savedErrno = errno;
        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: // ???
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                LOG(FATAL) << "unexpected error of ::accept " << savedErrno;
                break;
            default:
                LOG(FATAL) << "unknown error of ::accept " << savedErrno;
                break;
        }
    }
    std::shared_ptr<Socket> newSocket(new Socket(loop_.get(), connfd));
    newSocket->setAddr(addr);
    newSocket->setMessageCallback(messageCallback_) ;
    newSocket->setWriteCompleteCallback( writeCompleteCallback_) ;
    newSocket->setCloseCallback(std::bind(&TcpServer::connectionClose, this, std::placeholders::_1));
    connections_.insert(std::make_pair(newSocket->getFD(), newSocket));
    loop_->updateChannel(newSocket->getChan());
    connectionCallback_(newSocket);
    return;
}
