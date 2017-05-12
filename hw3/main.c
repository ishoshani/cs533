#include "scheduler.h"
#include <stdio.h>
void print_nth_prime(void * pn){
  printf("goooo");
  int c = 1, i = 1;

}


int main(void) {
  scheduler_begin();

  int n1 = 20000, n2 = 10000, n3 = 30000;
  thread_fork(print_nth_prime, &n1);
  thread_fork(print_nth_prime, &n2);
  thread_fork(print_nth_prime, &n3);

  scheduler_end();
}
