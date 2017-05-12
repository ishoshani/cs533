#include "scheduler.h"
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

ssize_t read_wrap(int fd, void * buf, size_t count){
  struct aiocb * controlblock = malloc(sizeof(aiocb));
  controlblock->aio_fildes=fd;
  controlblock->aio_buf= buf;
  controlblock->aio_nbytes= count;
  controlblock->aio_sigevent->sigevent_notify=SIGEV_NONE;

}
