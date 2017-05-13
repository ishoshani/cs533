#include <stdlib.h>
#include <stdio.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "scheduler.h"

ssize_t read_wrap(int fd, void * buf, size_t count){
  struct aiocb * controlblock = malloc(sizeof(struct aiocb));
  off_t offsetbytes = lseek(fd, 0, SEEK_CUR);
  ssize_t bytesread;
  controlblock->aio_fildes=fd;
  controlblock->aio_buf= buf;
  controlblock->aio_nbytes= count;
  if(offsetbytes!=-1){
    controlblock->aio_offset = offsetbytes;
  }
  controlblock->aio_sigevent.sigev_notify=SIGEV_NONE;
  aio_read(controlblock);
  int errorval;
  while((errorval = aio_error(controlblock))==EINPROGRESS){
    yield();
  }
  return aio_return(controlblock);

}
