#include "ht48.h"
#include "smht48.h"
#include "hashset.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void print_message(const uint8_t m[static 16]) 
{
	for (int i = 0; i < 16; i++) {
		printf("%02X", m[i]);
	}
	printf("\n");
}

void num_to_message(uint64_t num, uint8_t m[static 16]) {
	for (int i = 0; i < 8; i++) {
		((uint16_t*)m)[i] = rand() % (1<<16);
	}
}

void colsearch(uint64_t iters) {
	struct ListNode** hs = malloc(HS_SIZE * sizeof(struct ListNode*));
	hs_init(hs);
	srand(time(NULL));
	uint8_t m[16];
	uint8_t m_col[16];
	uint8_t h[6];
	
	for (uint64_t i = 0; i < iters; i++) {
		num_to_message(i, m);
		h[0] = IVB0;
		h[1] = IVB1;
		h[2] = IVB2;
		h[3] = IVB3;
		h[4] = IVB4;
		h[5] = IVB5;
		tcz48_dm(m, h);
		// printhash(h);
		// printf("\n");
		if(i % 10000 == 0) {
			printf("%lu / %lu\n", i, iters);
		}

		if (hs_find(hs, h, m_col)) {
			printf("Collision found\n");
			print_message(m);
			print_message(m_col);
			break;
		}

		hs_add(hs, h, m);
	}
	printf("Stopping...\n");
}

// Collision found
// 95508ABE32C1D046873253B0A36831CA
// 146448FD6669879E6AEA2C3A3A9D6465


int main() 
{
	colsearch(1<<24);
	// {
	// 	uint8_t key[6] = {0, 1, 2, 3, 4, 5};
	// 	uint8_t message[16] = {1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
	// 	uint8_t tag[6];
	// 	smht48(key, 16, message, tag);
	// 	printhash(tag);
	// 	printf("\n");
	// }

	// {
	// 	uint8_t key[6] = {5,4,3,2,1,0};
	// 	uint8_t message[16] = {1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
	// 	uint8_t tag[6];
	// 	smht48(key, 16, message, tag);
	// 	printhash(tag);
	// 	printf("\n");
	// }
}