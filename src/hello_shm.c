#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define COPY_FROM_HOST 0x10303
#define COPY_TO_HOST 0x10505

#define errExit(msg)                                                           \
  do {                                                                         \
    printf("%d\n", errno);                                                     \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#define BUF_SIZE (1024 * 4) /* Maximum size for exchanged string */

struct intermem {
  uint8_t buf[BUF_SIZE];
  // size_t size;
};

struct intermem im;

int main(int argc, char **argv) {
  // TODO: vmcall to get data from host
  int err = syscall(COPY_FROM_HOST, im.buf, BUF_SIZE);
  if (err == -1) {
    errExit("COPY_FROM_HOST");
  }
  fprintf(stderr, "%s\n", im.buf);

  char str[11] = "hello back";
  memcpy(im.buf, str, 11);

  // TODO: vmcall to send data back to host
  err = syscall(COPY_TO_HOST, im.buf, BUF_SIZE);
  if (err == -1) {
    errExit("COPY_TO_HOST");
  }

  // if (sem_post(&shmp->sem1) == -1)
  //   errExit("sem_post");

  /* Wait until peer says that it has finished accessing
     the shared memory. \*/

  //  if (sem_wait(&shmp->sem2) == -1)
  //    errExit("sem_wait");

  /* Write modified data in shared memory to standard output. */

  return 0;
}
