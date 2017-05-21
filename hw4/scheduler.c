/****************************************/
/* Scheduler based on solution by Liz Lawrens
/****************************************/

#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"
#include "queue.h"

#define STACK_SIZE 1024*1024

struct thread * current_thread;
struct thread * temp_thread;
struct queue * ready_list;
struct queue * done_list;
int thNum = 1;                                                               //global variable just to keep track of which thread is running and for                                                                                      printing purpose when each thread is forked and DONE. This will be stored                                                                                  as current_thread->name and will act as a unique identifier

/*******************************************************************/
/*                  Function thread_wrap                           */
/* This function is called inside thread_start to get the thread   */
/* running at the specified initial_function                       */
/*******************************************************************/

void thread_wrap() {
    printf("Starting thread_wrap ...\n\n");
    current_thread->initial_function(current_thread->initial_argument);
    current_thread->state = DONE;
    thread_enqueue(done_list,current_thread);
    printf("Thread %d is DONE ...\n\n",current_thread->name);
    yield();
    printf("Finish thread_wrap ...\n");                                       //this is never printed because scheduler doesn't execute a thread once its                                                                                 state is DONE
}

/*******************************************************************/
/*                   Function scheduler_begin                      */
/* This function initializes scheduler by allocating the           */
/* current_thread thread control block and set its state to RUNNING*/
/* The other fields need not be initialized; at the moment, this is*/
/* an empty shell that will hold the main thread's stack pointer   */
/* when it first gets switched out.                                */
/*******************************************************************/

void scheduler_begin(){
  printf("Scheduler initialized ...\n");
  current_thread = (struct thread*)malloc(sizeof(struct thread));             //allocate space for current_thread TCB
  temp_thread = current_thread;                                               //temporary holder so it can be free'd the end
  current_thread->state = RUNNING;                                            //initialize state to RUNNING
  ready_list=malloc(sizeof(struct queue));                                    //initialize ready_list and set head and tail to NULL
  done_list=malloc(sizeof(struct queue));                                     //initialize done_list and set head and tail to NULL
  ready_list->head = NULL;
  ready_list->tail = NULL;
  done_list->head = NULL;
  done_list->tail = NULL;
}

/*******************************************************************/
/*                   Function thread_fork                          */
/* This function encapsulates everything necessary to allocate a   */
/* new thread and then jump to it. thread_fork should:             */
/* 1) Allocate a new thread control block, and allocate its stack. */
/* 2) Set the new thread's initial argument and initial function.  */
/* 3) Set the current thread's state to READY and enqueue it on the*/
/* ready list.                                                     */
/* 4) Set the new thread's state to RUNNING.                       */
/* 5) Save a pointer to the current thread in a temporary variable,*/
/* then set the current thread to the new thread.                  */
/* 6) Call thread_start with the old current thread as old and the */
/* new current thread as new                                       */
/*******************************************************************/

thread * thread_fork(void(*target)(void*), void * arg){

  printf("Thread %d is forked ...\n", thNum);

  struct thread * new_thread = (struct thread*)malloc(sizeof(struct thread));

  new_thread->stack_pointer = (unsigned char*)malloc(STACK_SIZE);
  new_thread->temp_sp_holder = new_thread->stack_pointer;                  //temp pointer to stack pointer in order to free it at the end of execution
  new_thread->stack_pointer = new_thread->temp_sp_holder + STACK_SIZE;
  new_thread->initial_argument=arg;
  new_thread->initial_function=target;
  new_thread->name = thNum;
  thNum++;

  current_thread->state = READY;
  thread_enqueue(ready_list,current_thread);

  new_thread->state = RUNNING;

  struct thread * temp = current_thread;
  current_thread =  new_thread;

  thread_start(temp,current_thread);
  return new_thread;
}

/*******************************************************************/
/*                   Function yield                                */
/* This function pulls the next thread to run off of the ready list*/
/* It will do the following:                                       */
/* 1) If the current thread is not DONE, set its state to READY and*/
/* enqueue it on the ready list.                                   */
/* 2) Dequeue the next thread from the ready list and set its state*/
/* to RUNNING.                                                     */
/* 3) Save a pointer to the current thread in a temporary variable,*/
/* then set the current thread to the next thread.                 */
/* 4) Call thread_switch with the old current thread as old and the*/
/* new current thread as new.                                      */
/*******************************************************************/

void yield(){
  if( current_thread->state == BLOCKED){
    if(is_empty(ready_list)){
      printf("%s\n", "Cannot yield a blocking thread with nothing in ready list");
      exit(1);
    }
  }                                                        //can't really put any print statement here as its going to be printed a million                                                                             times :)
  else if (current_thread->state != DONE){
    current_thread->state = READY;
    thread_enqueue(ready_list,current_thread);
  }
  else if(current_thread->state == DONE){
    condition_broadcast(current_thread->thread_conditional);
  }

  struct thread *next_thread = thread_dequeue(ready_list);
  if(next_thread){
    struct thread * temp = current_thread;
    next_thread->state = RUNNING;
    current_thread = next_thread;
    thread_switch(temp,current_thread);
  }

}

/*******************************************************************/
/*                   Function scheduler_end                        */
/* This function checks the ready list and if any thread is still  */
/* running it will yield to it. We need a way to prevent the main  */
/* thread from terminating prematurely if there are other threads  */
/* still running.                                                  */
/*******************************************************************/

void scheduler_end(){
  while(!is_empty(ready_list)){
    yield();
  }

  printf("Cleaning up memory ...\n\n");
  while(!is_empty(done_list)){
        struct thread * tmp_thread = thread_dequeue(done_list);
        free(tmp_thread->temp_sp_holder);
        free(tmp_thread);
  }
  free(temp_thread);
  free(ready_list);
  free(done_list);

  printf("Scheduler ends here ....\n");                                    //this line is printed at the end when all threads have finished running
}

void mutex_init(struct mutex * mtx){
  mtx->held = 0;
  mtx->waiting_threads = malloc(sizeof(struct queue));
}

void mutex_lock(struct mutex * mtx){
  if(mtx->held == 1){
    current_thread->state = BLOCKED;
    thread_enqueue(mtx->waiting_threads,current_thread);
    yield();
  }
  mtx->held = 1;
}

void mutex_unlock(struct mutex * mtx){
  struct thread * next_thread = thread_dequeue(mtx->waiting_threads);
  if(next_thread){
    next_thread->state = READY;
    thread_enqueue(ready_list,next_thread);
  }else{
    mtx->held = 0;
  }
}

void condition_init(struct condition * cond){
  cond->waiting_threads = malloc(sizeof(struct queue));
}

void condition_wait(struct condition * cond, struct mutex * mtx){
  mutex_unlock(mtx);
  current_thread->state = BLOCKED;
  yield();
  mutex_lock(mtx);
}

void condition_signal(struct condition * cond){
  struct thread * next_thread = thread_dequeue(cond->waiting_threads);
  next_thread->state = READY;
  thread_enqueue(ready_list, next_thread);
}

void condition_broadcast(struct condition * cond){
  while(!is_empty(cond->waiting_threads)){
    condition_signal(cond);
  }
}

void thread_join(struct thread* joining_thread){
  while(joining_thread->state != DONE){
    condition_wait(joining_thread->thread_conditional, joining_thread->thread_mutex);
  }
}
