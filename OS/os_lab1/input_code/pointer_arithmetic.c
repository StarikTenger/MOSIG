#include <stdint.h>
#include <stdio.h>

int main() {
  unsigned long a = 10;
  unsigned long b = 20;
  unsigned long *p1 = &a;
  unsigned long *p2 = &b;
  unsigned char *p3 = (unsigned char *)&a;
  uint8_t *p4 = (uint8_t *)&a;
  void *p5 = (void *)&a;
  unsigned long *p6, *p7;
  unsigned long array[5] = {0, 10, 20, 30, 40};

  printf("\nsizeof(unsigned long) = %ld\n", sizeof(unsigned long));
  printf("sizeof(unsigned char) = %ld\n", sizeof(unsigned char));
  printf("sizeof(uint8_t) = %ld\n", sizeof(uint8_t));

  printf("\n\t\tp1 = %p\n", (void *)p1);
  p1++;
  printf("after p1++, \tp1 = %p \n", (void *)p1);

  printf("\n\t\tp2 = %p\n", (void *)p2);
  p2 = p2 + 10;
  printf("after p2+=10, \tp2 = %p\n", (void *)p2);

  printf("\n\t\tp3 = %p\n", (void *)p3);
  p3++;
  printf("after p3++, \tp3 = %p\n", (void *)p3);

  printf("\n\t\tp4 = %p\n", (void *)p4);
  p4++;
  printf("after p4++, \tp4 = %p\n", (void *)p4);

  printf("\n\t\tp5 = %p\n", p5);
  p5++;
  printf("after p5++, \tp5 = %p\n", p5);

  p6 = &(array[0]);
  p7 = &(array[4]);
  printf("\np6 =  &(array[0]) = %p\n", (void *)p6);
  printf("p7 =  &(array[4]) = %p\n", (void *)p7);
  printf("D = p7 - p6 = %lu\n", p7 - p6);

  return 0;
}
