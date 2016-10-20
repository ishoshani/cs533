struct thread{
	unsigned char* stack_pointer;
	void (*initial_function)(void*);
	void* initial_argument;
};
void threadincrement(void * a);
int increment(int a);
void thread_switch(struct thread * old, struct thread * new);
void thread_wrap();
void thread_start(struct thread * old, struct thread * new);
void yield();
