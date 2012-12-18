#include  <stdio.h>
#include  <stdlib.h>
#include  <signal.h>
void kill_handler(int sig)
{
	printf("sigfpe pid=%u sig=%u \n",getpid(),sig);
}

void sigcld_handler(int sig)
{
	printf("sigchild pid=%u sig=%u \n",getpid(),sig);
}

int main()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler=kill_handler;
	sigaction(SIGFPE, &sa, NULL);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGFPE);
	sigprocmask(SIG_BLOCK,&set, &set);
	sa.sa_handler=sigcld_handler;
	sigaction(SIGCHLD, &sa, NULL);
	int pid=fork();
	if( pid==0 ){
		//son	
		printf("cccccc11\n");
		sleep(10);
		printf("cccccc22\n");
	}else if(pid>0){
		//parent	
		printf("pppppp11 pid=%u ppid=%u\n",pid,getpid());
		kill(pid, SIGFPE);
		sleep(10);
		printf("pppppp22\n");
	}else{
		return -1;
	}
	return 0;
}
