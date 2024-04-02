#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 16

int main() {
  char *buf0, *buf1, *buf2;
  char *string = "Hello";

  buf0 = malloc(sizeof(char) * BUF_SIZE);
  buf1 = malloc(sizeof(char) * BUF_SIZE);
  buf2 = malloc(sizeof(char) * BUF_SIZE);

  snprintf(buf0, BUF_SIZE, "%s %d\n", string, 0);
  printf("%s", buf0);
  free(buf1);
  snprintf(buf0, BUF_SIZE, "%s %d\n", string, 1);
  printf("%s", buf0);
  free(buf0);
  snprintf(buf0, BUF_SIZE, "%s %d\n", string, 2);
  printf("%s", buf0);
  free(buf2);

  exit(0);
}
