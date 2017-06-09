#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <atomic_ops.h>
#include "async.h"
#include "scheduler.h"

AO_TS_t * print_lock;

void print_nth_prime(void * pn){
  spinlock_lock(&print_lock)
  printf("goooo");
  spinlock_unlock(&print_lock)

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
  spinlock_lock(&print_lock)
  printf("%dth prime: %d\n", n, i);
  spinlock_unlock(&print_lock)


}

void read_from_user(void * pn){
  char buf[5];
  size_t nbytes = sizeof(buf-1);
  ssize_t bytesread;
  int filedesc = STDIN_FILENO;
  bytesread= read_wrap(filedesc, buf, nbytes);
  for (int i = 0; i < nbytes; i++) {
    spinlock_lock(&print_lock);
    printf("%c", buf[i]);
    spinlock_unlock(&print_lock);

  }
  printf("\n");
}

void read_from_file(void * offsetpn) {
  int offset = *(int *) offsetpn;
  char buf[20];
  char abuf[20];
  size_t nbytes = sizeof(buf);
  ssize_t bytesread,asyncbytesRead;
  int filedesc = open("reader.txt", O_RDONLY);
  off_t offsetbytes = lseek(filedesc,offset,SEEK_CUR);
  bytesread = read(filedesc,buf,nbytes);
  asyncbytesRead= read_wrap(filedesc, abuf, nbytes);

  for (int i = 0; i < nbytes; i++) {
    spinlock_lock(&print_lock);
    printf("%c", buf[i]);
    spinlock_unlock(&print_lock);

  }
  spinlock_lock(&print_lock)
  printf("\n");
  spinlock_unlock(&print_lock)


  for (int i = 0; i < nbytes; i++) {
    spinlock_lock(&print_lock);
    printf("%c", abuf[i]);
    spinlock_unlock(&print_lock);

  }
  spinlock_lock(&print_lock);
  printf("\n");
  spinlock_unlock(&print_lock);


  fputs((asyncbytesRead==bytesread) ? "true" : "false", stdout);

  printf("\n");
}


int main(void) {
  print_lock = AO_TS_INITIALIZER
  scheduler_begin();

  int n1 = 20000, n2 = 10000, n3 = 30000, o5 = 3;
  thread_fork(print_nth_prime, &n1);
  thread_fork(print_nth_prime, &n2);
  thread_fork(print_nth_prime, &n3);
  thread_fork(read_from_user, &n3);
  thread_fork(read_from_file,&o5);


  scheduler_end();
}
