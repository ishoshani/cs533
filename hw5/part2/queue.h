/*
 * Thread Queue ADT
 * queue.h
 *
 * Feel free to modify this file. Please thoroughly comment on
 * any changes you make.
 */

typedef struct queue_node {
  struct thread * t;
  struct queue_node * next;
}queue_node;

typedef struct queue {
  struct queue_node * head;
  struct queue_node * tail;
}queue;

void thread_enqueue(struct queue * q, struct thread * t);
struct thread * thread_dequeue(struct queue * q);
int is_empty(struct queue * q);
