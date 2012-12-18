#include  <iostream>
#include	"net.h"
#include	"global.h"

//int init_net(int count , char *ip, int port);
//int net_loop();
int main(int argc,char** argv)
{
	char ip[10]="127.0.0.1";
	int port=10002;
	int MAX_EPOLL_CNT=100;
	if(init_proc()){
		return -1;
	}

	init_net(MAX_EPOLL_CNT, ip, 10000);
	for(int loop=0 ; loop < MAX_PID_CNT ; loop++ ){
		int pid=fork();
		if( pid>0 ){
			std::cout<<"parent start:"<<getpid()<<std::endl;
		}else if(pid==0){
			std::cout<<"child start"<<" pid: "<<getpid()<<" parent:"<<getppid()<<std::endl;
			exit_net();
			while(1){
				net_loop(0);
			}
		}else{
			exit(-1);
		}
	}
	while(1){
		net_loop(1);
	}
	exit_net();
	return 0;
}
