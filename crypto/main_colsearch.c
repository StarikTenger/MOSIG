#include "colsearch.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int main() 
{
	uint8_t key[6]; 
	uint8_t m1[16]; 
	uint8_t m2[16];
	smht48ef(1<<28, key, m1, m2); // All printing is done inside the function
}

/* --- Some results --- *

Collision found
95508ABE32C1D046873253B0A36831CA
146448FD6669879E6AEA2C3A3A9D6465

Collision found:
656E24667AAD54812955A777D942BBDD
71DC6B973853F163CFB0F859C750FC88
smht48 tags (must be the same):
42443F38F759
42443F38F759

 * -------------------- */