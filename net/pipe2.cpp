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
typedef struct pipeinfo {
	int recv_pipefd[2];
	int send_pipefd[2];
}pipeinfo_t;
#define    MAX_PID_CNT 	3 
pipeinfo_t pinfo[MAX_PID_CNT];
int main(int argc,char** argv)
{
	char ip[10]="127.0.0.1";
	int port=10002;
	char buf[256];
	int MAX_EPOLL_CNT=100;
	for(int loop=0 ; loop < MAX_PID_CNT ; loop++ ){
		if(pipe( pinfo[loop].recv_pipefd) ){
			return -1;
		}
		if(pipe( pinfo[loop].send_pipefd) ){
			return -1;
		}
	}
	for(int loop=0 ; loop < MAX_PID_CNT ; loop++ ){
		int pid=fork();
		if( pid>0 ){
			std::cout<<"parent start:loop:"<<loop<<std::endl;
			cout<<getpid()<<" >>>>>>>>>>>>>>>>>>>>"<<std::endl;

			//DEBUG_LOG("parent start");
			close(pinfo[loop].recv_pipefd[0]);
			//close(pinfo[loop].send_pipefd[1]);
			char msg[128]="i am your father";
			if(write(pinfo[loop].recv_pipefd[1],msg,strlen(msg))<0){
				std::cerr<<"write err:"<< errno<<std::endl;
			};
		}else if(pid==0){
			//DEBUG_LOG("child start:%u",getpid());
			std::cout<<"child start :"<<loop<<" pid: "<<getpid()<<">>>>>>>>>>>>>>>>>>"<<std::endl;
			for(uint32_t i=0 ;i < MAX_PID_CNT    ; i++ ){
				if( i != loop ){
					//close(pinfo[i].send_pipefd[0]);
					//close(pinfo[i].send_pipefd[1]);
					cout<<"kkk"<<endl;
					close(pinfo[i].recv_pipefd[0]);
					close(pinfo[i].recv_pipefd[1]);
				}else{
					cout<<"jjj"<<endl;
					close(pinfo[i].recv_pipefd[1]);
					//close(pinfo[i].send_pipefd[0]);
					//if(read(pinfo[i].recv_pipefd[0],buf,1 )<0 ){
						//std::cerr<<"xxxxxxxx:"<< errno<<std::endl;
					//}
				}
			}
			sleep(5);
			if(read(pinfo[loop].recv_pipefd[0],buf,1 )<0 ){
				std::cerr<<"xxxxxxxx:"<< errno<<std::endl;
			}else{
				cout<<"pid:"<<getpid()<<" receive:"<<buf<<endl;
			}
			while(1);
		}else{
			return -1;
		}
		cout<<"pid:"<<getpid()<<"<<<<<<<<<<<<<<<"<<endl;
	}
	return 0;
}
