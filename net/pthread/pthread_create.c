#include  <stdio.h>
#include  <stdlib.h>
#include <pthread.h>
struct msg {
    struct msg *m_next;
	char *data;
    /* ... more stuff here ... */
};
struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void
process_msg(int thread)
{
    struct msg *mp;

    for (;;) {
        pthread_mutex_lock(&qlock);
        while (workq == NULL)
            pthread_cond_wait(&qready, &qlock);
        mp = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
		printf("thread %u message:%s \n",thread,mp->data);
		free(mp);
        /* now process the message mp */
    }
}

void
enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}



pthread_t ntid[2];

struct foo {
    int             f_count;
    pthread_mutex_t f_lock;
    /* ... more stuff here ... */
};

struct foo * foo_alloc(void) /* allocate the object */
{
    struct foo *fp;

    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return(NULL);
        }
        /* ... continue initialization ... */
    }
    return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0) { /* last reference */
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}



void
printids(const char *s)
{
    pid_t      pid;
    pthread_t  tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
      (unsigned int)tid, (unsigned int)tid);
}
void
cleanup(void *arg)
{
	printf("cleanup: %s\n", (char *)arg);
}
void * thr_func1(void *arg)
{
	struct foo* fp=(struct foo*)arg;
	pthread_cleanup_push(cleanup,"thread 1 cleanup");
    printids("new thread: ");
	pthread_cleanup_pop(0);
	process_msg(1);
	/*foo_hold(fp);*/
	/*printf("xxxx %u",fp->f_count);*/
    return((void *)2);
}
void * thr_func2(void *arg)
{
	struct foo* fp=(struct foo*)arg;
	pthread_cleanup_push(cleanup,"thread 2 cleanup");
    printids("new thread: ");
	pthread_cleanup_pop(0);
	/*foo_hold(fp);*/
	/*printf("xxxx %u",fp->f_count);*/
	process_msg(2);
	pthread_exit((void *)1);
}

int
main(void)
{
    int     err;
	void * ret;
	struct foo *fp=foo_alloc();
	char message[][5]={"1111", "2222", "3333", "4444", "5555", "6666", "7777", "8888", "9999", "000" };
	int loop=0;


    err = pthread_create(&ntid[0], NULL, thr_func1, fp);
    if (err != 0){
		exit(0);
	}
	err = pthread_create(&ntid[1], NULL, thr_func2, fp);
    if (err != 0){
		exit(0);
	}
	sleep(1);
	for(loop ; loop<10 ; loop++ ){
		struct msg * f=malloc(sizeof(struct msg));
		f->data=message[loop];
		enqueue_msg(f);
	}
	
    sleep(1);
	err = pthread_join(ntid[0], &ret);
	printf("thread 1 exit code %u\n",(int)ret);
	err = pthread_join(ntid[1], &ret);
	printf("thread 2 exit code %u\n",(int)ret);
    printids("main thread:");
    exit(0);
}

