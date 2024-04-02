#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 16

void print_vertical(char *str, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf("%c\n", str[i]);
  }
}

int main() {
  int fd = open("/usr/hostname", O_RDONLY);
  char buf[BUF_SIZE] = {0};
  int r_count = read(fd, buf, BUF_SIZE - 1);
  buf[r_count] = '\0';
  printf("%s \n", buf);
  print_vertical(buf, r_count);
  close(fd);
  exit(EXIT_SUCCESS);
}
