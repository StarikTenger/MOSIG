#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concat(char *str1, char *str2) {
  char *buf = malloc(strlen(str1) + strlen(str2));

  strncpy(buf, str1, strlen(str1));
  strncpy(buf + strlen(str1), str2, strlen(str2));
  return buf;
}

int main() {

  char *str = concat("hello", "world");
  printf("RESULT : %s\n", str);

  return 0;
}
