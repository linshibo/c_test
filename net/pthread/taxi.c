#include  <stdio.h>
#include  <pthread.h>
pthread_cond_t taxiCond; 

 // 同步锁
pthread_mutex_t taxiMutex; 

// 旅客到达等待出租车
void * traveler_arrive(void * name) { 
   cout<< ” Traveler: ” <<(char *)name<< ” needs a taxi now! ” <<endl; 
   pthread_mutex_lock(&taxiMutex); 
   pthread_cond_wait (&taxiCond, &taxtMutex); 
   pthread_mutex_unlock (&taxtMutex); 
   cout<< ” Traveler: ” << (char *)name << ” now got a taxi! ” <<endl; 
   pthread_exit( (void *)0 ); 
} 

// 出租车到达
void * taxi_arrive(void *name) { 
   cout<< ” Taxi ” <<(char *)name<< ” arrives. ” <<endl; 
   pthread_cond_signal(&taxtCond); 
   pthread_exit( (void *)0 ); 
} 

void main() {  
   // 初始化
   taxtCond= PTHREAD_COND_INITIALIZER; 
   taxtMutex= PTHREAD_MUTEX_INITIALIZER; 
   pthread_t thread; 
   pthread_attr_t threadAttr; 
   pthread_attr_init(&threadAttr); 

   pthread_create(&thread, &threadAttr, taxt_arrive, (void *)( ” Jack ” )); 
   sleep(1); 
   pthread_create(&thread, &threadAttr, traveler_arrive, (void *)( ” Susan ” )); 
   sleep(1); 
   pthread_create(&thread, &threadAttr, taxi_arrive, (void *)( ” Mike ” )); 
   sleep(1); 

   return 0; 
}
