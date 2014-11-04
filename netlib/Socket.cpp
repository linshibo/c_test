#include <glog/logging.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "Socket.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"
void Socket::handleRead(int32_t){
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(chan_->fd(), &savedErrno);
    if (n > 0)
    {
        messageCallback_(shared_from_this());
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG(ERROR) << "TcpConnection::handleRead";
        handleError();
    }

}
void Socket::handleWrite(){
    ssize_t n = ::write(chan_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
    if (n > 0) {
        outputBuffer_.retrieve(n);
        if (outputBuffer_.readableBytes() == 0)
        {
            chan_->disableWriting();
            if (writeCallback_)
            {
                loop_->runInLoop(std::bind(writeCallback_, shared_from_this()));
            }
        }
    } else {
        LOG(ERROR)<< "TcpConnection::handleWrite";
    }

}

int Socket::Send(char* data, int len){
    ssize_t n = ::write(chan_->fd(), data,len);
    if (n == len) {
        return 0;
    } if(n>0) {
        this->outputBuffer_.append(data+n, len-n); 
        return 0;
    }else {
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            this->chan_->enableWriting();
        }
        return errno;
    }
}


void Socket::handleClose(){
   LOG(INFO)<<"Socket::handleClose";
   ::close(fd_);
   chan_->remove();
   closeCallback_(shared_from_this());
}

void Socket::handleError(){
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);
    if (::getsockopt(fd_, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        LOG(ERROR)<<"handleError"<<errno;
    }
    else
    {
        LOG(ERROR)<<"handleError"<<optval;
    }
}
    
void Socket::setChanReadCallback(ReadEventCallback&& cb){
    chan_->setReadCallback(std::move(cb));
}
void Socket::setChanCloseCallback(EventCallback&& cb){
    chan_->setCloseCallback(std::move(cb));
}

Socket::Socket(EventLoop*loop, int fd):fd_(fd), loop_(loop), chan_(new Channel(loop, fd)) {
    // non-block
    int flags = fcntl(fd_, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = fcntl(fd_, F_SETFL, flags);

    // close-on-exec
    flags = fcntl(fd_, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = fcntl(fd_, F_SETFD, flags);
    chan_->enableReading();
    chan_->setReadCallback(std::bind(&Socket::handleRead, this, std::placeholders::_1));
    chan_->setWriteCallback(std::bind(&Socket::handleWrite, this));
    chan_->setCloseCallback(std::bind(&Socket::handleClose, this));
}
