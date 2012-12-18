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
	int listenfd,f,n;
	char buf[MAXLEN+1];
	struct sockaddr_in servaddr;
	if((listenfd=socket(AF_INET, SOCK_DGRAM,0))==-1){
		cerr<<"sock error"<<endl;
		return 0;
	}
	f = 1;
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
	n=1024*10;
    if( setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n))!=0 ){
		cerr<<"opt set error";
	};
    socklen_t len=0;
    if( getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &n, &len)==-1 ){
		cerr<<"opt get error "<<errno<<endl;
	};
	cout<<"udpserv start"<<endl;
    cout<<"udp:recvbuf="<<n<<" "<<len<<endl;
	uint32_t count=0;
	while( 1 ){

		 struct sockaddr_in cliaddr;
		 socklen_t len = sizeof(servaddr);
		 memset(buf,0,sizeof(buf));
         if(n = recvfrom(listenfd, buf, MAXLEN, 0, (struct sockaddr* )&cliaddr, &len)){
			count++;
			cout<<"receive:"<<count<<endl;	
		 };
         //if(sendto(listenfd, buf, n, 0, (struct sockaddr* )&cliaddr, len)  ){
			 //cout<<"send succ"<<endl;
		 //};
	}	
	return 0;
}
