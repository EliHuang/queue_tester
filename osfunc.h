#include <sys/types.h>
#include <sys/syscall.h> 
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
   
#define PORTDBG(cond, x ) \
   if(cond) \
   { \
      printf("%s:",__FUNCTION__); \
   		printf x; \
   }

/* mutex functions */
typedef pthread_mutex_t  mutex;
typedef pthread_mutex_t*  pmutex;

static inline int init_mutex(pmutex pmute) {
   pthread_mutex_t tmpMutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutexattr_t mta;
   memcpy(pmute,&tmpMutex,sizeof(*pmute));

   pthread_mutexattr_init(&mta);
   pthread_mutexattr_settype(&mta,PTHREAD_MUTEX_RECURSIVE);
   return pthread_mutex_init(pmute,&mta);
}
static inline void delete_mutex(pmutex pmute) { pthread_mutex_destroy(pmute); }
static inline void lock_mutex(pmutex pmute) { pthread_mutex_lock(pmute); }
static inline void unlock_mutex(pmutex pmute) { pthread_mutex_unlock(pmute); }   

/* event functions */
typedef sem_t event_handle;
typedef sem_t* pevent_handle;
typedef int event_wait_return;

#define event_invalid(event) 0
#define event_wait_success(return_value)	(return_value)

static inline int event_create(event_handle* pev_handle) { return sem_init(pev_handle,0,0); }
static inline int event_set(pevent_handle pev_handle ) { return sem_post(pev_handle); }
static inline int event_reset(pevent_handle pev_handle ) { 
   while(sem_trywait(pev_handle)==0);
   return 0;
}
static inline int event_wait(pevent_handle pev_handle ) { 
   sem_wait(pev_handle);
   return 0;
}
static inline int  event_timed_wait(pevent_handle pev_handle,int timeout) {
   struct timespec btime;	
   clock_gettime(CLOCK_REALTIME, &btime);
   btime.tv_sec += timeout/1000 ;
   btime.tv_nsec += (timeout%1000)*1000000;;
   int ret = sem_timedwait(pev_handle, &(btime));
   if(ret != 0 && (errno == ETIMEDOUT || errno == EINTR))
   {
      return 1;
   }
   return ret; 
}
static inline int event_destroy(pevent_handle pev_handle) { return sem_destroy(pev_handle); }