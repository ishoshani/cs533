#include "scheduler.h"
##include "async.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void print_nth_prime(void * pn){
  printf("goooo");
	int n = *(int *) pn;
  int c = 1, i = 1;
  while(c <= n) {
    ++i;
    int j, isprime = 1;
    for(j = 2; j < i; ++j) {
      if(i % j == 0) {
        isprime = 0;
        break;
      }
    }
    if(isprime) {
      ++c;
    }
    yield();
  }
  printf("%dth prime: %d\n", n, i);

}

void read_from_reader(void * pn){
  char buf[5];
  size_t nbytes = sizeof(buf-1);
  ssize_t bytesread;
  int filedesc = STDIN_FILENO;
  bytesread= read_wrap(filedesc, buf, nbytes);
  for (int i = 0; i < nbytes; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");
}




int main(void) {
  scheduler_begin();

  int n1 = 20000, n2 = 10000, n3 = 30000;
  thread_fork(print_nth_prime, &n1);
  thread_fork(print_nth_prime, &n2);
  thread_fork(print_nth_prime, &n3);
  thread_fork(read_from_reader, &n3);


  scheduler_end();
}
