#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 16

void fill_array(int *buf, int max_len) {
  int i;
  for (i = 0; i < max_len; i++) {
    buf[i] = (double)rand() / RAND_MAX * 10;
  }
}

void print_array(int *array, int size) {
  int i;
  printf("[ ");
  for (i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }
  printf("]\n");
}

int main() {

  int *array = (int *)ARRAY_SIZE;
  fill_array(array, 10);
  print_array(array, 10);

  exit(EXIT_SUCCESS);
}
