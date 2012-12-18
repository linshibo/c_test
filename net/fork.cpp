#include  <iostream>
#include  <sys/types.h>  
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <sys/un.h>
#include  <sys/stat.h>
#include  <arpa/inet.h>
#include  <errno.h>
#include  <sys/epoll.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <linux/unistd.h>
using namespace std;
#define    MAX_PID_CNT 	2 
int main(int argc,char** argv)
{
	for(int loop=0 ; loop < MAX_PID_CNT ; loop++ ){
		int pid=fork();
		if( pid>0 ){
			std::cout<<"parent start:loop:"<<loop<<std::endl;
			cout<<getpid()<<" >>>>>>>>>>>>>>>>>>>>"<<std::endl;

		}else if(pid==0){
			//DEBUG_LOG("child start:%u",getpid());
			std::cout<<"child start :"<<loop<<" pid: "<<getpid()<<">>>>>>>>>>>>>>>>>>"<<std::endl;
		}
		cout<<"pid:"<<getpid()<<"<<<<<<<<<<<<<<<"<<endl;
	}
	return 0;
}
