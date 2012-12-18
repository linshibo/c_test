#include  <iostream>
#include  <stdio.h>
#include  <cstdlib>
#include  <fcntl.h>
#include  <sys/types.h>  
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <sys/un.h>
#include  <sys/stat.h>
#include  <arpa/inet.h>
#include  <errno.h>
#include <sys/epoll.h>

using namespace std;
const int MAXLEN=128;

int main(int argc,char** argv)
{
	int connfd,listenfd,f;
	int epollfd=epoll_create(100);
	char buf[MAXLEN+1];
	struct sockaddr_in servaddr;
	if((listenfd=socket(AF_INET, SOCK_STREAM,0))==-1){
		cerr<<"sock error"<<endl;
		return 0;
	}
	fcntl(listenfd, F_SETFL, O_NONBLOCK | fcntl(listenfd, F_GETFL, 0));
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&f, sizeof(f))==-1){
   		cerr<<"setsockopt"<<endl;
    	return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
    if(inet_aton("127.0.0.1", &(servaddr.sin_addr))==-1){
		cerr<<"sock"<<endl;
		return 0;
	}
	servaddr.sin_port=htons(6543);
	if(bind(listenfd,(struct sockaddr* )&servaddr,sizeof(servaddr))==-1){
		cerr<<"bind erro"<<endl;
		return 0;
	}
    if(listen(listenfd,128)==-1){
		cerr<<ntohs(servaddr.sin_port)<<" has been listened"<<endl;
		return 0;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        cerr<<"epoll_ctl";
        return -1;
    }
	cout<<"start listen"<<inet_ntoa(servaddr.sin_addr)<<" "<<ntohs(servaddr.sin_port)<<endl;
	while( 1 ){
		struct epoll_event events[100];
		int nfds;
		nfds = epoll_wait(epollfd,events,100,100);	
		for( uint32_t loop=0 ; loop<nfds ; loop++ ){
			if(listenfd  ==  events[loop].data.fd){
				do{
					struct sockaddr_in cliaddr;
					socklen_t   len = sizeof(cliaddr);
					connfd  = accept(listenfd, (sockaddr*)&cliaddr, &len);
					if( connfd < 0 ){
						cerr<<"connfd";
						break;		
					}
					printf("accept %s:%u\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
					fcntl(connfd, F_SETFL, fcntl(connfd, F_GETFL)|O_NONBLOCK);
					ev.data.fd=connfd;
					ev.events=EPOLLIN | EPOLLET;	
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
						cerr<<"epoll_ctl1";
						return -1;
					}
				}while(connfd);
				
			}else{
                if (events[loop].events & EPOLLERR) {
                    printf("EPOLLERR: client linkdown\n");
                    close(events[loop].data.fd);
                    continue;
                }
                if (events[loop].events & EPOLLHUP) {
                    printf("EPOLLHUP: client linkdown\n");
                    close(events[loop].data.fd);
                    continue;
                }
                if (events[loop].events & EPOLLPRI) {
                    printf("EPOLLPRI: client linkdown\n");
                    close(events[loop].data.fd);
                    continue;
                }
                if (events[loop].events & EPOLLIN) {
                    printf("EPOLLIN start:\n");
                    memset(buf, 0, sizeof(buf));
					bool goon=false ,tag=true;
					do{
						int n = recv(events[loop].data.fd, buf, sizeof(buf), 0);
						if (n > 0) {
							cout<<"recv:"<<buf<<endl;
						} else if (n == 0) {
							printf("client linkdown\n");
							close(events[loop].data.fd);
							tag=false;
							break;
						} else { 
							if(errno == ECONNRESET) {
								close(events[loop].data.fd);
								//events[loop].data.fd = -1;
								tag=false;
								break;
							}else if(errno == EAGAIN || errno == EINTR) {
								break;
							}else{
								printf("client linkdown\n");
								close(events[loop].data.fd);
								tag=false;
								break;
							}
						}
						if(n >= MAXLEN)
							goon=true;
						else
							goon=false;
					}while(goon);
					if( tag ){
						ev.data.fd=events[loop].data.fd;	
						ev.events=EPOLLOUT | EPOLLET;
						if (epoll_ctl(epollfd, EPOLL_CTL_MOD,events[loop].data.fd , &ev) == -1) {
							cerr<<"epoll_ctl2";
							return -1;
						}
					}
                    printf("EPOLLIN end:\n");
                }else if (events[loop].events & EPOLLOUT) {
                    printf("EPOLLOUT start:\n");
					write(events[loop].data.fd, "nihao", sizeof("nihao"));
					ev.data.fd=events[loop].data.fd;	
					ev.events=EPOLLIN | EPOLLET;
				    if (epoll_ctl(epollfd, EPOLL_CTL_MOD,events[loop].data.fd , &ev) == -1) {
				        cerr<<"epoll_ctl3";
				        return -1;
				    }
                    printf("EPOLLOUT end:\n");
                }
			}	
		}
	}	
	return 0;
}
