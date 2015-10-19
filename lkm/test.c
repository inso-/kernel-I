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

#define VERSION_MAGIC 42
#define VERSION_MODIFIED _IO(VERSION_MAGIC, 0)
#define VERSION_RESET  _IO(VERSION_MAGIC, 1)

void ioctl_test(char *name, unsigned long cmd, int fd)
{
  int ret = 0;
  
  printf("Sending ioctl %s\n", name); 
  ret = ioctl(fd, cmd);
  printf("ioctl ret val (%d) errno (%d)\n", ret, errno); 
  perror("IOCTL error: "); 
}

void ioctl_version_modified(int fd)
{
  ioctl_test("VERSION_MODIFIED", VERSION_MODIFIED, fd);
}

void ioctl_version_reset(int fd)
{
  ioctl_test("VERSION_RESET", VERSION_RESET, fd);
}

void readnprint_version(int fd)
{
  int nb = 0;
  int ret = 1;
  char buff[255] = {0};
  
  while (ret > 0)
    {
      ret = read(fd, &buff, 254);
    }
  buff[254] = 0;
  printf("%s\n", buff);
}

void writeon_version(int fd, char *buff)
{
  write(fd, buff, strlen(buff));
}

int main() {
  int fd, ret = 0;

  fd = open("/dev/version", O_RDWR);

  printf("file descriptor fd(%d)\n", fd);
  if (fd < 0)
    return(printf("File open error\n"));

  readnprint_version(fd);
  ioctl_version_modified(fd);

  writeon_version(fd, "toto\n");
  readnprint_version(fd);
  ioctl_version_modified(fd);
  
  ioctl_version_reset(fd);
  readnprint_version(fd);
  
  close(fd);
}
