/*
 *  rec.c
 *
 *  Created by Vania Marangozova.
 *
 */

#include <stdio.h>
#include <stdlib.h>

int power(int a, int n) {
  if (n != 0)
    return a * power(a, n - 1);
  else
    return 1;
}

int main() {
  int pt = power(2, 3);
  printf("The power 3 of 2 is: %d\n", pt);
  exit(0);
}
