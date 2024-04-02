#include "ht48.h"
#include "keyrec.h"
#include <stdio.h>
#include <string.h>

void keyrec_demo() {
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

	keyrec_demo();
}