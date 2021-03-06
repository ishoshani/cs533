/****************************************/
/* Based on work by Liz Lawrens                          */
/* CS533 Assignment 2                   */
/* Spring 2017                          */
/* Round Robin Scheduling               */
/* Header file with function prototypes */
/****************************************/
extern void * safe_mem(int, void*);
#define malloc(arg) safe_mem(0, ((void*)(arg)))
#define free(arg) safe_mem(1, arg)
#define _GNU_SOURCE
#include <sched.h>
#define current_thread (get_current_thread())


typedef enum {
    RUNNING, // The thread is currently running.
    READY,   // The thread is not running, but is runnable.
    BLOCKED, // The thread is not running, and not runnable.
    DONE     // The thread has finished.
} state_t;






//Struct thread
typedef struct thread{
  unsigned char* stack_pointer;
  void (*initial_function)(void*);
  void* initial_argument;
  struct mutex * thread_mutex;
  struct condition * thread_conditional;
  state_t state;
  unsigned char* temp_sp_holder;
  int name;
}thread;

extern struct thread * current_thread;

//Kernel_thread stuff

extern struct thread * get_current_thread();
extern void set_current_thread(struct thread * t);
int kernel_thread_begin(void * arg);


//struct Mutex
typedef struct mutex {
     int held;
     struct queue * waiting_threads;
   }mutex;
//Struct conditional variable
typedef struct condition {
     struct queue * waiting_threads;
   }condition;
//Function prototypes
void scheduler_begin();
thread * thread_fork(void(*target)(void*), void * arg);
void yield();
void scheduler_end();

void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new);
//void thread_wrap();

void mutex_init(struct mutex * mtx);
void mutex_lock(struct mutex * mtx);
void mutex_unlock(struct mutex * mtx);

void condition_init(struct condition * cond);
void condition_wait(struct condition * cond, struct mutex * mtx);
void condition_signal(struct condition * cond);
void condition_broadcast(struct condition * cond);

void thread_join(struct thread* joining_thread);


//Spinlock interface
void spinlock_lock(AO_TS_t * spin);
void spinlock_unlock(AO_TS_t * spin);
