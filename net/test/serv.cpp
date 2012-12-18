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

using namespace std;
const int MAXLEN=1024;

int main(int argc,char** argv)
{
	int connfd,listenfd,f;
	char buf[MAXLEN+1];
	struct sockaddr_in servaddr;
	if((listenfd=socket(AF_INET, SOCK_STREAM,0))==-1){
		cerr<<"sock error"<<endl;
		return 0;
	}
    //f = fcntl(listenfd, F_GETFL, 0);
	//f |= O_NONBLOCK;
	//fcntl(listenfd, F_SETFL, f);
	f = 1;
	//if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&f, sizeof(f))==-1){
   		//cerr<<"setsockopt"<<endl;
    	//return 0;
	//}

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
	cout<<"start listen"<<inet_ntoa(servaddr.sin_addr)<<" "<<ntohs(servaddr.sin_port)<<endl;
	while( 1 ){
		struct sockaddr_in cliaddr;
        socklen_t   len = sizeof(cliaddr);
        connfd  = accept(listenfd, (sockaddr*)&cliaddr, &len);
		printf("accept %s:%u\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
		int pid = fork();
		if (pid == -1) {
			perror("call to fork");
			exit(1);
		} else if (pid == 0) {
			close(listenfd);
			while(connfd){
				int n;
				if((n = read(connfd, buf, MAXLEN))>0){
					cout<<"recv:"<<buf<<endl;
					write(connfd, "nihao", sizeof("nihao"));
				}else{
					cout<<"recv error"<<endl;
					break;
				}
			}
			close(connfd);
			exit(0);
		} else
			close(connfd); 
	}	
	return 0;
}
