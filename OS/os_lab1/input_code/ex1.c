/*
 *  ex1.c
 * As given in the lab text.
 */

#include <stdio.h>
#include <stdlib.h>

int min(int a, int b, int c) {
  int tmp_min;
  tmp_min = a <= b ? a : b;
  tmp_min = tmp_min <= c ? tmp_min : c;
  return tmp_min;
}
int main() {
  int min_val = min(3, 7, 5);
  printf("The min is: %d\n", min_val);
  exit(0);
}
