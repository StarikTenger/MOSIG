#include "combination.h"
#include "smht48.h"
#include <stdio.h>
#include <string.h>

/*
 * In blen - size of message
 * In m - message
 * In h - tag value
 * Out k - key
 */
void keyrec(uint64_t blen, const uint8_t m[blen], 
	uint8_t h[static 6], uint8_t key[static 6]) 
{
	uint64_t n = 48;
	uint64_t k = 7;
	uint64_t comb[k];
	uint8_t result[6];

	combination_init(n, k, comb);

	int i = 0;

	while (combination_next(n, k, comb)) {
		//combination_print(k, comb);
		combination_bitstring(k, comb, 6, key);
		//bitstring_print(buffsize, buff);
		
		smht48(key, 6, m, result);

		if (i % 100000 == 0) {
			bitstring_print(6, key);
		}

		if (memcmp(result, h, 6) == 0) {
			printf("FOUND KEY:\n");
			bitstring_print(6, key);
			return;
		}
		i++;
	}
	printf("Keyrec was unable to find the key\n");
}