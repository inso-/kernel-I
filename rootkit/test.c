/*
 * Test.c - Thomas Moussajee 
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

int main() {
  int fd, ret = 0;
  
  fd = open("/dev/urandom", O_RDWR);

  fd = open("/proc/modules", O_RDWR);

  printf("file descriptor fd(%d)\n", fd);
  if (fd < 0)
    return(printf("File open error\n"));
  
  close(fd);
}
