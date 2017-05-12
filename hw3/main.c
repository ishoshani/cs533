#include "scheduler.h"
#include <stdio.h>
#include <fcntl.h>

void print_nth_prime(void * pn){
  printf("goooo");
  int c = 1, i = 1;
  char buf[20];
  size_t nbytes = sizeof(buf);
  ssize_t bytesread;
  int filedesc = open("reader.txt",O_RDONLY);
  bytesread= (filedesc, buf, nbytes);
  put(buf);
}


int main(void) {
  scheduler_begin();

  int n1 = 20000, n2 = 10000, n3 = 30000;
  thread_fork(print_nth_prime, &n1);
  thread_fork(print_nth_prime, &n2);
  thread_fork(print_nth_prime, &n3);

  scheduler_end();
}
