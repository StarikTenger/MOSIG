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
	memcpy((void*)conc, (void*)m, 6);
	memcpy((void*)conc + 6, (void*)k, 6);
	ht48(12, conc, h);
}