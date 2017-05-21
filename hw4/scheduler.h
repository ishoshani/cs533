/****************************************/
/* Liz Lawrens                          */
/* CS533 Assignment 2                   */
/* Spring 2017                          */
/* Round Robin Scheduling               */
/* Header file with function prototypes */
/****************************************/

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
  state_t state;
  unsigned char* temp_sp_holder;
  int name;
}thread;

extern struct thread * current_thread;

//struct Mutex
struct mutex {
     int held;
     struct queue waiting_threads;
   };
//Struct conditional variable
struct condition {
     struct queue waiting_threads;
   };
//Function prototypes
void scheduler_begin();
void thread_fork(void(*target)(void*), void * arg);
void yield();
void scheduler_end();

void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new);
//void thread_wrap();

void mutex_init(struct mutex *)
void mutex_lock(struct mutex *)
void mutex_unlock(struct mutex *)

void condition_init(struct condition *)
void condition_wait(struct condition *, struct mutex *)
void condition_signal(struct condition *)
void condition_broadcast(struct condition *)
