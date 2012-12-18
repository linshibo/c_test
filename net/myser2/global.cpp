#include    "global.h"
epoll_info_t einfo;
const int MAXLEN=128;
int listenfd=0;
const int MAX_PID_CNT=3;
const int MAX_FD_CNT=1024;
server_info_t g_servers[MAX_PID_CNT];
fd_info_t fds[1024];
//pipeinfo_t pinfo[MAX_PID_CNT];
