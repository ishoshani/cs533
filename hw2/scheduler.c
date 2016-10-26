#include "scheduler.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
struct thread * current_thread;
struct queue * ready_list;
int STACK_SIZE=1024*1024;
void scheduler_begin(){
	current_thread=malloc(sizeof(struct thread));
	current_thread->state=RUNNING;
	ready_list=malloc(sizeof(struct queue));
	ready_list->head = NULL;
	ready_list->tail = NULL;

}
void scheduler_end(){
while(!is_empty(ready_list)){
yield();
}
return;
}
void thread_fork(void(*target)(void*), void * arg){
  struct thread * new; 
  new = malloc(sizeof(struct thread));
  new->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;
  new->initial_function=target;
  new->initial_argument=arg;
  current_thread->state=READY;
  thread_enqueue(ready_list,current_thread);
  new->state=RUNNING;
  struct thread * temp;
  temp = current_thread;
  current_thread = new;
  thread_start(temp, current_thread);
}

void yield() {
    if (current_thread->state!=DONE)
    {
      current_thread->state=READY;
      thread_enqueue(ready_list,current_thread);
    }
    struct thread * new = thread_dequeue(ready_list);
    new->state=RUNNING;
    struct thread * temp;
    temp = current_thread;
    current_thread = new;
    thread_switch(temp, current_thread);
	}
void thread_wrap() {
    current_thread->initial_function(current_thread->initial_argument);
    current_thread->state=DONE;
	 yield();
 }
