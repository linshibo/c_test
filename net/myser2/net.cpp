#include    "net.h"
#include    "global.h"
void sigterm_handler(int sig)
{
	std::cerr<<"sigterm_handler"<<" pid: "<<getpid()<<std::endl;
}

void sighup_handler(int sig)
{
	std::cerr<<"sighup_handler"<<" pid: "<<getpid()<<std::endl;
}

void sigpipe_handler(int sig)
{
	int32_t status,pid;
	while((pid = waitpid (-1, &status, WNOHANG)) > 0) 
		std::cerr<<"sigpipe_handler"<<" pid: "<<pid<<" status:"<<status<<std::endl;
}

void sigchild_handler(int sig, siginfo_t *ps ,void *p)
{
	pid_t pid;
	int32_t status;
	while((pid = waitpid (-1, &status, WNOHANG)) > 0) 
		std::cerr<<"sigchld_handler"<<" pid: "<<pid<<" status:"<<status<<std::endl;
}

int init_proc()
{
	for( int loop=0 ; loop<MAX_PID_CNT ; loop++){
		create_stack(g_servers[loop].recv_q,1024*1024);
	}
    struct sigaction sa;
	memset(&sa, 0, sizeof(sa));

	//signal(SIGPIPE,SIG_IGN);
    sa.sa_handler = sigterm_handler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sa.sa_handler = sigpipe_handler;
    sigaction(SIGPIPE, &sa, NULL);
    
    sa.sa_handler = sighup_handler;
    sigaction(SIGHUP, &sa, NULL);

    sa.sa_flags = SA_RESTART|SA_SIGINFO;
    sa.sa_sigaction = sigchild_handler;
    sigaction(SIGCHLD, &sa, NULL);

	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGBUS);
    sigaddset(&sset, SIGILL);
    sigaddset(&sset, SIGCHLD);
    sigaddset(&sset, SIGFPE);
	sigprocmask(SIG_UNBLOCK, &sset, &sset);
	return 0;
}

int readn(int fd, char *bp, size_t len)
{
	int cnt=len,rc;
	while(cnt>0){
		rc=recv(fd, bp, cnt, 0);
		if(rc < 0){
			if(errno == EINTR){
				continue;				
			}
			std::cout<<"readn error:"<<errno<<std::endl;
			return -1;
		}
		if( rc == 0){
			return len-cnt;
		}
		bp += rc;
		cnt -= rc;
	}
	return len;
}

int recv_data(int fd, char* data, uint32_t len)
{
	proto_t pkg;
	int rc=readn(fd, (char*)&pkg, sizeof(proto_t));
	if( rc != sizeof(proto_t) ){
		return rc<0?-1:0;
	}
	uint32_t reclen=ntohl(pkg.len);
	if(1  ){
	}
}

int init_net(int count , char *ip, int port)
{
	std::cout<<"init_net:pid="<<getpid()<<std::endl;
	einfo.epollfd = epoll_create(count);
	char buf[MAXLEN+1];
	int f;
	struct sockaddr_in servaddr;
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0))==-1){
		std::cerr<<"sock error"<<std::endl;
		return -1;
	}
	fcntl(listenfd, F_SETFL, O_NONBLOCK | fcntl(listenfd, F_GETFL, 0));
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&f, sizeof(f))==-1){
   		std::cerr<<"setsockopt"<<std::endl;
    	return -1;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
    if(inet_aton(ip, &(servaddr.sin_addr))==-1){
		std::cerr<<"sock"<<std::endl;
		return -1;
	}
	servaddr.sin_port=htons(port);
	if(bind(listenfd, (struct sockaddr* )&servaddr, sizeof(servaddr))==-1){
		std::cerr<<"bind erro"<<std::endl;
		return -1;
	}
    if(listen(listenfd,	128)==-1){
		std::cerr<<ntohs(servaddr.sin_port)<<" has been listened"<<std::endl;
		return -1;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
    if (epoll_ctl(einfo.epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        std::cerr<<"epoll_ctl";
        return -1;
    }
	std::cout<<"start listen "<<inet_ntoa(servaddr.sin_addr)<<" "<<ntohs(servaddr.sin_port)<<" fd:"<<listenfd<<std::endl;
	return 0;
}

int exit_net()
{
	close(einfo.epollfd);
	close(listenfd);
	return 0;
}
int do_add_event(int fd)
{
	struct epoll_event ev;
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)|O_NONBLOCK);
	ev.data.fd=fd;
	ev.events=EPOLLIN | EPOLLET;	
	if (epoll_ctl(einfo.epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr<<"do add event:epoll_ctl err:"<<errno<<std::endl;
		return -1;
	}
	std::cout<<getpid()<<" do add event"<<std::endl;
	return 0;
}

int do_accept(int listenfd)
{
	//std::cout<<"do accept"<<std::endl;
	struct epoll_event ev;
	struct sockaddr_in cliaddr;
	socklen_t   len = sizeof(cliaddr);
	int connfd  = accept(listenfd, (sockaddr*)&cliaddr, &len);
	if( connfd < 0 ){
		std::cerr<<getpid()<<" accpet error"<<std::endl;
		return connfd;
	}
	printf("accept %s:%u\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
	fcntl(connfd, F_SETFL, fcntl(connfd, F_GETFL)|O_NONBLOCK);
	ev.data.fd=connfd;
	ev.events=EPOLLIN | EPOLLET;	
	if (epoll_ctl(einfo.epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
		std::cerr<<"do accpet:epoll_ctl err"<<std::endl;
		return -1;
	}
	return 0;
}

int recv_data(int fd, bool &tag)
{
	char buf[512];
	int len=0;
	memset(buf,0,sizeof(buf));
	do{
		int n = recv(fd, buf, sizeof(buf), 0);
		if (n > 0) {
			std::cout<<"recv:"<<buf<<std::endl;
			int i=rand()%2;
			std::cout<<"write to child:"<<i<<std::endl;
			//if( write(pinfo[i].recv_pipefd[1], &buf[len], n) < 0){
				//std::cout<<"write to  error:"<<std::endl;
			//}
			len+=n;
		} else if (n == 0) {
			printf("client linkdown recv 0\n");
			close(fd);
			tag=false;
			break;
		} else { 
			if(errno == ECONNRESET) {
				close(fd);
				//events[loop].data.fd = -1;
				tag=false;
				break;
			}else if(errno == EAGAIN || errno == EINTR) {
				break;
			}else{
				printf("client linkdown:errno %u\n",errno);
				close(fd);
				tag=false;
				break;
			}
		}
		
		}while((n >= MAXLEN));
}

int net_loop(int parent)
{
	int nfds,connfd;
	struct epoll_event ev;
	nfds = epoll_wait(einfo.epollfd, einfo.events, 100, 100);	
	for( uint32_t loop=0 ; loop<nfds ; loop++ ){
		if(listenfd  ==  einfo.events[loop].data.fd){
			std::cout<<"ready accept isparent:"<<parent<<std::endl;
			while(!do_accept(listenfd));
		}else{
			if (einfo.events[loop].events & EPOLLERR) {
				printf("EPOLLERR: client linkdown:fd %u parent %u\n",einfo.events[loop].data.fd,parent);
				close(einfo.events[loop].data.fd);
				continue;
			}
			if (einfo.events[loop].events & EPOLLHUP) {
				printf("EPOLLHUP: client linkdown:fd %u parent %u\n",einfo.events[loop].data.fd,parent);
				close(einfo.events[loop].data.fd);
				continue;
			}
			if (einfo.events[loop].events & EPOLLPRI) {
				printf("EPOLLPRI: client linkdown:fd %u parent %u\n",einfo.events[loop].data.fd,parent);
				close(einfo.events[loop].data.fd);
				continue;
			}
			if (einfo.events[loop].events & EPOLLIN) {
				printf("\nEPOLLIN start:fd=%u   parent %u\n",einfo.events[loop].data.fd, parent);
				char buf[512];
				memset(buf, 0, sizeof(buf));
				bool goon=false ,tag=true;
				int len=0;
				if(parent){
					recv_from_cli(einfo.events[loop].data.fd ,tag);
				}else{
					read_from_parent(einfo.events[loop].data.fd,tag);
				}
				if(tag){
					ev.data.fd=einfo.events[loop].data.fd;	
					ev.events=EPOLLOUT | EPOLLET;
					if (epoll_ctl(einfo.epollfd, EPOLL_CTL_MOD, einfo.events[loop].data.fd, &ev) == -1) {
						std::cerr<<"epoll_ctl2";
						return -1;
					}
				}
				printf("EPOLLIN end:\n");
			}else if (einfo.events[loop].events & EPOLLOUT) {
				printf("EPOLLOUT start: fd:%u parent:%u\n",einfo.events[loop].data.fd,parent);
				write(einfo.events[loop].data.fd, "nihao", sizeof("nihao"));
				ev.data.fd=einfo.events[loop].data.fd;	
				ev.events=EPOLLIN | EPOLLET;
				if (epoll_ctl(einfo.epollfd, EPOLL_CTL_MOD, einfo.events[loop].data.fd , &ev) == -1) {
					std::cerr<<"epoll_ctl3";
					return -1;
				}
				printf("EPOLLOUT end:\n");
			}
		}	
	}
	return 0;
}
