#include "scheduler.h"
#include <stdio.h>
#include <fcntl.h>

void read_from_reader(void * pn){
  printf("goooo\n");
  int c = 1, i = 1;
  char buf[20];
  size_t nbytes = sizeof(buf-1);
  ssize_t bytesread;
  int filedesc = open("reader.txt",O_RDONLY);
  bytes read= (filedesc, buf, nbytes);
  for (i = 0; i < nbytes; i++) {
    printf(" %02hhX", buf[i]);
  }
}


int main(void) {
  scheduler_begin();

  int n1 = 20000, n2 = 10000, n3 = 30000;
  thread_fork(read_from_reader, &n1);
  thread_fork(read_from_reader, &n2);
  thread_fork(read_from_reader, &n3);

  scheduler_end();
}
