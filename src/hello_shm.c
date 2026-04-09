#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "sandbox.hpp"

int main(int argc, char **argv) {
  int fd;
  struct shmbuf *shmp;

  fprintf(stderr, "check args\n");
  if (argc < 1) {
    errExit("args");
  }

  char *shmpath = argv[1];

  fprintf(stderr, "%s\n", shmpath);

  fd = shm_open(shmpath, O_RDWR, 0777);
  if (fd == -1) {
    fprintf(stderr, "%d\n", errno);
    errExit("shm_open");
  } 

  shmp = (struct shmbuf *)mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
  if (shmp == MAP_FAILED)
    errExit("mmap");

  /* Copy data into the shared memory object. */

  char str[6] = "hello";
  size_t len = 6;
  shmp->cnt = len + 1;
  memcpy(&shmp->buf, str, len);

  /* Tell peer that it can now access shared memory. */

  if (sem_post(&shmp->sem1) == -1)
    errExit("sem_post");

  /* Wait until peer says that it has finished accessing
     the shared memory. \*/

  //  if (sem_wait(&shmp->sem2) == -1)
  //    errExit("sem_wait");

  /* Write modified data in shared memory to standard output. */

  return 0;
}
