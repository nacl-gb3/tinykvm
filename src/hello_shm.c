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
  /* replace with switch statement to call into sample library */

  // TODO: vmcall to send data back to host
  err = syscall(COPY_TO_HOST, im.buf, BUF_SIZE);
  if (err == -1) {
    errExit("COPY_TO_HOST");
  }

  return 0;
}
