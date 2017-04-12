#void thread_switch(void * old, void * new)
.globl thread_switch
thread_switch:
	pushq %rbx #push all callee-save registers
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, (%rdi)		#save stack pointer into old's thread control block
	movq (%rsi), %rsp 	#move new threads control block onto the stack pointer
	popq %r15 #pop all the callee-save registers
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
	ret
#void thread_start(void * old_thread, void * new_thread
.globl thread_start
thread_start:
	pushq %rbx #push all callee-save registers
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp, (%rdi)		#save stack pointer into old's thread control block
	movq (%rsi), %rsp 	#move new threads control block onto the stack pointer
	jmp thread_wrap
