#include  <iostream>
#include  <cstdlib>
#include  <cstdio>
#include  <sys/types.h>  
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <sys/un.h>
#include  <arpa/inet.h>
#include  <sys/stat.h>
#include  <errno.h>
using namespace std;
const int MAXLEN=256;
int main(int argc, char **argv)
{
	if( argc<2 ){
		cerr<<"arg not enough"<<endl;
		return 0;
	}
	int sockfd,n;
	char recvbuff[MAXLEN+1];
	char sendbuf[MAXLEN+1];
	for( int loop=0 ; loop<128 ; loop++ ){
		sendbuf[loop]='a';	
		sendbuf[loop+128]='b';	
	}
	sendbuf[512]='\0';	
	struct sockaddr_in servaddr;
	if((sockfd=socket(AF_INET, SOCK_STREAM,0))<0){
		cerr<<"socket error";
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	int port=atoi(argv[2]);
	servaddr.sin_port=htons(port);
	inet_aton(argv[1], &(servaddr.sin_addr));
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		if (errno == ETIMEDOUT)
			cerr<<"连接超时\n";
		else if (errno == ECONNREFUSED){
			cerr<<"服务器未监听端口\n";   
		}else{
			cerr<<"error:"<<errno<<endl;
		}
		return -1;
	}
	//struct sockaddr localaddr;
	struct sockaddr_storage ss;
	socklen_t addrlen;
	if( getsockname(sockfd, (struct sockaddr*)&ss, &addrlen)>=0 ){
		cout<<"sock data: "<<ss.__ss_padding<<" "<<ss.__ss_align<<" family:"<<ss.ss_family<<" len:"<<addrlen<<endl;
	}
	char ch='y';
	while(ch == 'y'){
		cout<<"start\n";
		if(write(sockfd,sendbuf,sizeof(sendbuf))<0)
			cerr<<"write error";
		memset(recvbuff,0,sizeof(recvbuff));
		if((n=read(sockfd,recvbuff,MAXLEN))<0)
			cerr<<"read error"<<endl;
		cout<<"recv:"<<recvbuff<<endl;
		cout<<"continue?";
		cin>>ch;
		cout<<"input:"<<ch<<endl;
	}
	cout<<"close"<<endl;
	close(sockfd);
}
