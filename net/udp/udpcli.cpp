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
const int MAXLEN=1024;

int main(int argc, char **argv)
{
	if( argc<2 ){
		cerr<<"arg not enough"<<endl;
		return 0;
	}
	int sockfd,n;
	char recvbuff[MAXLEN+1];
	char sendbuf[MAXLEN+1];
	struct sockaddr_in servaddr;
	if((sockfd=socket(AF_INET, SOCK_DGRAM,0))<0){
		cerr<<"socket error";
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	int port=atoi(argv[2]);
	servaddr.sin_port=htons(port);
	inet_aton(argv[1], &(servaddr.sin_addr));
	struct sockaddr_in cliaddr;
	socklen_t len;
	if(getsockname(sockfd, (struct sockaddr*) &cliaddr, &len)){
		cerr<<"get socket name error"<<endl;
	}
	//getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, &len);
	cout<<"udpcli:="<<sock_ntop((struct sockaddr*) &cliaddr, len)<<" "<<len<<endl;
	for( int i=0 ; i<2000 ; i++ ){
		if(sendto(sockfd, sendbuf,MAXLEN,0, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
			cerr<<"write error:"<<errno<<endl;
		}

	}
	
	//char ch='y';
	//while(ch == 'y'){
		//memset(sendbuf, 0, sizeof(sendbuf));
		//cout<<"input:"<<endl;
		//cin.getline(sendbuf,MAXLEN);
		////cin>>ch;
		//cout<<"input:"<<sendbuf<<endl;
		//if(sendto(sockfd, sendbuf,MAXLEN+1,0, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
			//cerr<<"write error:"<<errno<<endl;
		//}
		//memset(recvbuff,0,sizeof(recvbuff));
		//if((n=recvfrom(sockfd,recvbuff,MAXLEN,0,NULL,NULL))<0)
			//cerr<<"read error"<<endl;
		//cout<<"recv:"<<recvbuff<<endl;
	//}
	cout<<"close"<<endl;
	close(sockfd);
}
