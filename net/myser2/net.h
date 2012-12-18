#ifndef  NET_H
#define  NET_H
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
// /usr/include/asm-generic/errno-base.h
// /usr/include/asm-generic/errno.h
typedef struct epoll_info{
	int epollfd;
	struct epoll_event events[100];
	int max_cnt;
}epoll_info_t;

typedef struct proto{
	uint32_t len;
	uint32_t cmd;
	uint8_t   data[];
}__attribute__((packed)) proto_t;
typedef struct fd_info{
	int fd;
	int servid;
} fd_info_t;

typedef struct data_block{
	int fd;
	uint32_t len;
	uint8_t data[];
	struct data_block *self;
}data_block_t;

typedef struct data_block_queue{
	data_block_t *head,*tail;
	void *addr;
	uint32_t maxlen;
}data_block_queue_t;

static int create_stack(data_block_queue_t* pdbq, int len)
{
	pdbq->maxlen=len;
	pdbq->addr=mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	if (pdbq->addr == MAP_FAILED){
        return  -1;
    }
	return 0;
}

static int pop_stack(data_block_queue_t* pdbq, data_block_t* pdb)
{
	if(pdbq->head == pdbq->tail){
		return -1;
	}
	data_block_t* cur=(data_block_t*)(pdbq->head-4);
	memcpy(pdb, pdbq->head, sizeof(data_block_t)+pdbq->head->len);
	pdbq->tail = cur;
	return 0;	
}

static int push_stack(data_block_queue_t* pdbq, data_block_t* pdb)
{
	if(pdbq->head-pdbq->tail + sizeof(data_block_t)+pdb->len >= pdbq->maxlen ){
		return -1;
	}
	memcpy(pdbq->tail, pdb, sizeof(data_block_t)+pdb->len);
	pdb->self=pdbq->tail;
	pdbq->tail += sizeof(data_block_t)+pdb->len;
	return 0;	
}

typedef struct server_info{
	data_block_queue_t* recv_q;
}server_info_t;
int init_net(int count , char *ip, int port);
int net_loop(int parent);
int init_proc();
int exit_net();
int do_add_event(int fd);

#endif  /*NET_H*/
