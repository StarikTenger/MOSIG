#include "ht48.h"
#include <stdio.h>
#include <string.h>

/*
 * Input k: a 48-bit key stored as an array of 6 bytes
 * Input blen: the byte length of m, stored on 64 bits
 * Input m: the message to be hashed, whose length is required to be an
   integral number of bytes
 * Input h: placeholder for the 48-bit resulting tag, to be stored as an
   array of 6 bytes. Must have been allocated by the caller.
 * Output: void, h is overwritten with the result ht48(m||k)
 * Warning: the key bytes in k must be appended _in order_ (k[0] first)
 */
void smht48(const uint8_t k[static 6], uint64_t blen, 
	const uint8_t m[blen], uint8_t h[static 6]) 
{
	const uint8_t conc[12];
	memcpy(conc, m, 6);
	memcpy(conc + 6, k, 6);
	//strcat(conc, );

	ht48(12, conc, h);
}

void combination_init(uint64_t n, uint64_t k, uint64_t comb[k]) 
{
	for (uint64_t i = 0; i < k; i++) {
		comb[i] = i;
	}
}

int combination_next(uint64_t n, uint64_t k, uint64_t comb[k]) 
{
	comb[k - 1]++;
	int i = 0;
	if (comb[k - 1] >= n) {
		while (i < k && comb[k - 1 - i] >= n - i - 1) i++;
		if (i == k) return 0;
		int reset_val = comb[k - 1 - i];
		//printf("i: %d, reset_val: %d\n", i, reset_val);
		for (int j = 0; j <= i; j++) {
			comb[k - 1 - i + j] = reset_val + 1 + j;
		}
	}
	return 1;
}

void combination_bitstring(uint64_t k, uint64_t comb[k], 
	uint64_t buffsize, uint8_t buff[buffsize]) 
{
	memset(buff, 0, sizeof(buff));
	for (int i = 0; i < k; i++) {
		buff[buffsize - 1 - comb[i] / 8] ^= (1 << comb[i]%8);
	}
}

void bitstring_print(uint64_t buffsize, uint8_t buff[buffsize]) 
{
    for (uint64_t i = 0; i < buffsize; i++) {
        uint8_t byte = buff[i];
        for (int j = 7; j >= 0; j--) {
            //printf("%d", (byte >> j) & 0x01);
			printf("%c", ((byte >> j) & 0x01) ? '1' : '_');
        }
    }
    printf("\n");
}

void combination_print(uint64_t k, uint64_t comb[k]) 
{
	for (uint64_t i = 0; i < k; i++) {
		printf("%d ", (int)comb[i]);
	}
	printf("\n");
}

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

int main() 
{
	// printf("Test\n");
	// uint8_t key[6] = {0xE4, 0x16, 0x9F, 0x12, 0xD3, 0xBA};
	// uint8_t message[6] = {9, 8, 7, 6, 5, 4};
	// uint8_t result[6];
	// smht48(key, 6, message, result);
	// printhash(result);
	// printf("\n");

	// uint64_t n = 48;
	// uint64_t k = 7;
	// uint64_t comb[k];

	// uint64_t buffsize = 6;
	// uint8_t buff[buffsize];
	

	// combination_init(n, k, comb);

	// //combination_print(k, comb);
	// combination_bitstring(k, comb, buffsize, buff);
	// bitstring_print(buffsize, buff);

	// while (combination_next(n, k, comb)) {
	// 	//combination_print(k, comb);
	// 	combination_bitstring(k, comb, buffsize, buff);
	// 	bitstring_print(buffsize, buff);
	// }

	uint8_t message[6] = {9, 8, 7, 6, 5, 4};
	uint8_t tag[6] = {0x7D, 0x1D, 0xEF, 0xA0, 0xB8, 0xAD};
	//uint8_t key[6] = {0, 1, 0, 0, 0, 1};
	//smht48(key, 6, message, tag);

	uint8_t key1[6];// = {0, 1, 0, 0, 0, 1};
	keyrec(6, message, tag, key1);
	// printhash(key);
	// printf("\n");
	printhash(key1);
	printf("\n");
}