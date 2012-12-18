#include   <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <bits/string3.h>
#include <lcms.h>
#include <signal.h>
#include <errno.h>
#define UNIXSTR_PATH "/tmp/info00"
void handle_sigcld(int sig)
{
	perror("sigchld\n");
}
int main()
{
	int sockfd,connfd;	
	socklen_t clen;
	struct sockaddr_un saddr,caddr;
	bzero(&saddr,sizeof(saddr));
	if((sockfd == socket(AF_LOCAL, SOCK_DGRAM, 0))){
		return -1;
	}
	saddr.sun_family=AF_LOCAL;
	unlink(UNIXSTR_PATH);
	strcpy(saddr.sun_path,UNIXSTR_PATH);
	if( bind(sockfd,(struct sockaddr*)(&saddr), sizeof(saddr))){
		return -1;
	}
	if(listen(sockfd, 1024)){
		return -1;
	}
	if( signal(SIGCHLD,handle_sigcld) ){
		return -1;
	}
	while( 1 ){
		clen=sizeof(caddr);
		if( connfd=accept(sockfd, (struct sockaddr*)(&caddr),&clen)){
			if(errno==EINTR){
				continue;
			}else{
				printf("accept err");
			}
		}
		int n=0;
		char buf[2046];
		if( (n==recv(connfd,buf,sizeof(buf),0))<0){
			printf("recv err");
			continue;
		}
		if( send(connfd,buf,sizeof(buf),0)<0){
			printf("send err");
		}
	}
}

