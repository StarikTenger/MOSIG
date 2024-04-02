/*
 *  ex2.c
 */

#include <stdio.h>
#include <stdlib.h>

void print_vect(int *v, int size) {
  int i;
  printf("[ ");
  for (i = 0; i < size; i++) {
    printf("%d ", v[i]);
  }
  printf("]\n");
}

int *vect_sum(int *v1, int *v2, int size) {
  int *r, i;
  r = malloc(sizeof(int) * size);
  for (i = 0; i < size; i++) {
    r[i] = v1[i] + v2[i];
  }
  return r;
}

int main() {
  int v1[] = {1, 2, 4, 7};
  int v2[] = {3, 4, 9, 2};
  int *p_result = vect_sum(v1, v2, 4);
  print_vect(p_result, 4); /* prints the content of the given vector */
  exit(0);
}
