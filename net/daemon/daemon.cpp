#include   <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>
#include <stdint.h>
const int maxfd=1024;
void handle_sig(int sig)
{
	syslog(7,"handle signal:%u",sig);
}
int daemon_init(char* name, int facility)
{
	pid_t pid;	
	if((pid=fork()) >0 ){
		exit(0);		
	}
	setsid();
	signal(SIGHUP,handle_sig); 
	if((pid=fork()) >0 ){
		exit(0);		
	}
	//deamon_proc=1;
	chdir("/");
	umask(0);
	for( uint32_t loop=0 ; loop<maxfd ; loop++){
		close(loop);
	}
	openlog(name, LOG_PID, facility);
	return 0;	
}
