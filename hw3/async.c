#include "scheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

ssize_t read_wrap(int fd, void * buf, size_t count){
  struct aiocb * controlblock = malloc(sizeof(struct aiocb));
  controlblock->aio_fildes=fd;
  controlblock->aio_buf= buf;
  controlblock->aio_nbytes= count;
  controlblock->aio_sigevent->sigevent_notify=SIGEV_NONE;

}
