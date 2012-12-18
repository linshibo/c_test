#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>

int main()
{
    sem_t * mutex=(sem_t *) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mutex == MAP_FAILED)
            return -1;

    int count;
	pid_t pid;
	/*struct sched_param param;*/
    int prio;
    if(sem_init(mutex,1,0) < 0)
    {
        printf("init fail\n");
    }
	
    if((pid=fork())==0)
    {
		/*prio = sched_get_priority_min(SCHED_FIFO);*/
		/*param.sched_priority = prio;*/
		/*if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)*/
		/*{*/
		/*printf("Child Error\n");*/
		/*}*/
		printf("child xx %u %lu pid=%u %s\n",mutex,mutex->__align,getpid(),mutex->__size);
		
        sem_post(mutex);

		sleep(5);
		printf("child ---%u %lu\n",mutex,mutex->__align);
        sem_wait(mutex);
		printf("child ---%u %lu\n",mutex,mutex->__align);
		/*printf("child %d\n",sched_getscheduler(0));*/
        for(count=0;count < 10;count++)
        {
            printf("step %d in child %u %lu\n",count,mutex,mutex->__align);
			sleep(1);
        }
		printf("child +++ %u %lu\n",mutex,mutex->__align);
        sem_post(mutex);
		printf("child +++ %u %lu\n",mutex,mutex->__align);
    }
    else if(pid>0)
    {
		/*prio = sched_get_priority_max(SCHED_FIFO);*/
		/*param.sched_priority = prio;*/
		/*if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)*/
		/*{*/
		/*printf("Father Error\n");*/
		/*}*/
		
		printf("parent xx %u %lu pid=%u %s\n",mutex,mutex->__align,getpid(),mutex->__size);
		sleep(10);
		printf("parent --- %u  %lu\n",mutex,mutex->__align);
        sem_wait(mutex);
		printf("parent ---  %u %lu\n",mutex,mutex->__align);
        for(count=0;count < 10;count++)
        {
            printf("step %d in father %u %lu\n",count,mutex,mutex->__align);
        }
		printf("parent +++  %u %lu\n",mutex,mutex->__align);
        sem_post(mutex);
		printf("parent +++  %u %lu\n",mutex,mutex->__align);
    }
	sem_destroy(mutex);
	return 0;
}

