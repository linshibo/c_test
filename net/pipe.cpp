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
#include  <sys/epoll.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <linux/unistd.h>


#define     MAX_PID_CNT  2
int main(int argc,char** argv)
{
	int recv_pipefd[2];
	int send_pipefd[2];
	char msg[]="nihao";
	char buf[256];
   if (pipe(recv_pipefd) == -1) {
	   perror("pipe");
	   exit(EXIT_FAILURE);
   }
	if (pipe(send_pipefd) == -1) {
	   perror("pipe");
	   exit(EXIT_FAILURE);
   }
   int pid = fork();
   if (pid == -1) {
	   perror("fork");
	   exit(EXIT_FAILURE);
   }

   if (pid == 0) {    /* Child reads from pipe */
	   std::cout<<"child"<<std::endl;
	   close(recv_pipefd[1]);          /* Close unused write end */
	   close(send_pipefd[0]);          /* Close unused write end */
	   sleep(1);
		if(read(recv_pipefd[0], &buf, 1) > 0)
		   write(STDOUT_FILENO, &buf, 1);
	   std::cerr<<"read end:"<<errno<<std::endl;
	   write(STDOUT_FILENO, "\n", 1);
	   _exit(EXIT_SUCCESS);

   } else {            /* Parent writes argv[1] to pipe */
	   std::cout<<"parent"<<std::endl;
	   close(recv_pipefd[0]);          /* Close unused write end */
	   close(send_pipefd[1]);          /* Close unused write end */
	   int ret=0;
	   if( (ret=write(recv_pipefd[1], msg, strlen(msg))) < 0 ){
	   		std::cerr<<"write err:"<<errno<<"  ret:"<<ret<<std::endl;
	   }
	   std::cout<<"parent2222"<<std::endl;
	   wait(NULL);                /* Wait for child */
	   exit(EXIT_SUCCESS);
   }
	return 0;
}
