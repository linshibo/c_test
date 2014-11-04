#include <iostream>
#include "Poller.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"
#include "Buffer.hpp"
#include <glog/logging.h>
#include "Socket.hpp"
#include "TcpServer.hpp"

void message(const std::shared_ptr<Socket>& s){
    LOG(INFO)<<"message xxx"<<s->getAddr()->sin_addr.s_addr<<s->inputBuffer_.retrieveAllAsString();
    s->Send("aaaabbbb", sizeof("aaaabbbb"));
}
void cclose(const std::shared_ptr<Socket>& s){
    LOG(INFO)<<"close xxx"<<s->getAddr()->sin_addr.s_addr;
}
void newConn(const std::shared_ptr<Socket>& s){
    LOG(INFO)<<"newConn xxx"<<s->getAddr()->sin_addr.s_addr;
}
int main(int argc, const char *argv[])
{
    google::InitGoogleLogging("");  
    google::SetLogDestination(google::GLOG_INFO, "./");  
    //google::SetStderrLogging(google::GLOG_INFO);  
    FLAGS_logbufsecs = 0;  //日志实时输出         
    FLAGS_max_log_size=10; //最大日志文件大小 10M  
    TcpServer server("127.0.0.1:9999");
    server.setMessageCallback(message);
    server.setCloseCallback(cclose);
    server.setConnectionCallback(newConn);
    server.start();
    return 0;
}
