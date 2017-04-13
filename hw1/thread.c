#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
struct thread * current_thread;
struct thread * inactive_thread;
int STACK_SIZE = 1024 * 1024;
int main(){
	current_thread = malloc(sizeof(struct thread));
	inactive_thread = malloc(sizeof(struct thread));
	current_thread->initial_function = threadincrement;
	int * p = malloc(sizeof(int));
 	*p = 5;
  	current_thread->initial_argument = p;
  	current_thread->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;
  	//current_thread->initial_function(current_thread->initial_argument);
  	thread_start(inactive_thread,current_thread);
	free(current_thread)
	free(inactive_thread)
	free(p)
}
int increment(int a){
	return a+1;
}
int powerofTwo(int a){
	int p = 1;
	for(int i =0; i<a;i++){
		p *=2;
	}
	return p;
}

void threadincrement(void * a){
	int n = *(int*) a;
	printf("%d is %d power of two",powerofTwo(n),n);
}
void thread_wrap() {
    current_thread->initial_function(current_thread->initial_argument);
		struct thread * temp = current_thread;
		free(temp);
    yield();
  }
void yield() {
    struct thread * temp = current_thread;
    current_thread = inactive_thread;
    inactive_thread = temp;
    thread_switch(inactive_thread, current_thread);
  }
