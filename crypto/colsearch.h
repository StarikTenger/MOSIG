#pragma once

#include "ht48.h"
#include "smht48.h"
#include "hashset.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

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

bool colsearch(uint64_t iters, uint8_t m1[static 16], uint8_t m2[static 16]) {
	struct ListNode** hs = malloc(HS_SIZE * sizeof(struct ListNode*));
	hs_init(hs);
	srand(time(NULL));
	uint8_t h[6];
	
	for (uint64_t i = 0; i < iters; i++) {
		num_to_message(i, m1);
		h[0] = IVB0;
		h[1] = IVB1;
		h[2] = IVB2;
		h[3] = IVB3;
		h[4] = IVB4;
		h[5] = IVB5;
		tcz48_dm(m1, h);

		if(i % 100000 == 0) {
			printf("%lu / %lu\n", i, iters);
		}

		if (hs_find(hs, h, m2) && memcmp(m1, m2, 16) != 0) { 
			printf("Collision found:\n");
			print_message(m1);
			print_message(m2);
			hs_free(hs);
			free(hs);
			return true;
		}

		hs_add(hs, h, m1);
	}
	printf("No collision found, try again\n");
	hs_free(hs);
	free(hs);
	return false;
}

bool smht48ef(uint64_t iters, uint8_t key[static 6], uint8_t m1[static 16], uint8_t m2[static 16]) {
	for (int i = 0; i < 3; i++) {
		((uint16_t*)key)[i] = rand() % (1<<16);
	}
	colsearch(iters, m1, m2);
	uint8_t tag1[6];
	uint8_t tag2[6];
	smht48(key, 16, m1, tag1);
	smht48(key, 16, m2, tag2);

	printf("smht48 tags (must be the same):\n");
	printhash(tag1);
	printf("\n");
	printhash(tag2);
	printf("\n");
}