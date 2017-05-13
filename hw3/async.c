#include <stdlib.h>
#include <stdio.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

ssize_t read_wrap(int fd, void * buf, size_t count){
  struct aiocb * controlblock = malloc(sizeof(struct aiocb));
  ssize_t bytesread;
  controlblock->aio_fildes=fd;
  controlblock->aio_buf= buf;
  controlblock->aio_nbytes= count;
  controlblock->aio_sigevent.sigev_notify=SIGEV_NONE;
  aio_read(controlblock);
  int errorval;
  while((errorval = aio_error(controlblock))==EINPROGRESS){
    yield();
  }
  return aio_return(controlblock);

}
