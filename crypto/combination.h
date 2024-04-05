#pragma once

#include <stdio.h>
#include <string.h>

/* Initializes the combination with all ones being to the leftest positions
 * In: n, k (denote bit vector of size n with k ones)
 * Out: comb[k]
 */
void combination_init(uint64_t n, uint64_t k, uint64_t comb[k]) 
{
	for (uint64_t i = 0; i < k; i++) {
		comb[i] = i;
	}
}

/* Switches to the next combination. Returns 0 if unable to switch
 * In: n, k (denote bit vector of size n with k ones)
 * Inout: comb
 */
int combination_next(uint64_t n, uint64_t k, uint64_t comb[k]) 
{
	comb[k - 1]++;
	int i = 0;
	if (comb[k - 1] >= n) {
		while (i < k && comb[k - 1 - i] >= n - i - 1) i++;
		if (i == k) return 0;
		int reset_val = comb[k - 1 - i];
		for (int j = 0; j <= i; j++) {
			comb[k - 1 - i + j] = reset_val + 1 + j;
		}
	}
	return 1;
}

/* Converts combination object to the bit string
 * In: k, comb[k], buffsize
 * Out: buff
 */
void combination_bitstring(uint64_t k, const uint64_t comb[k], 
	uint64_t buffsize, uint8_t buff[buffsize]) 
{
	memset(buff, 0, buffsize);
	for (int i = 0; i < k; i++) {
		buff[buffsize - 1 - comb[i] / 8] ^= (1 << comb[i]%8);
	}
}

/* Debug printing of the bit string
 * In: buffsize, buff
 */
void bitstring_print(uint64_t buffsize, const uint8_t buff[buffsize]) 
{
    for (uint64_t i = 0; i < buffsize; i++) {
        uint8_t byte = buff[i];
        for (int j = 7; j >= 0; j--) {
			printf("%c", ((byte >> j) & 0x01) ? '1' : '_');
        }
    }
    printf("\n");
}

/* Debug printing of the combination
 * In: k, comb[k]
 */
void combination_print(uint64_t k, const uint64_t comb[k]) 
{
	for (uint64_t i = 0; i < k; i++) {
		printf("%d ", (int)comb[i]);
	}
	printf("\n");
}