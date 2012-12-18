#ifndef  GLOBAL_H
#define  GLOBAL_H
#include	"net.h"
extern fd_info_t fds[];
extern epoll_info_t einfo;
extern const int MAXLEN;
extern int listenfd;
extern const int MAX_PID_CNT;
extern server_info_t g_servers[];
#endif  /*GLOBAL_H*/
