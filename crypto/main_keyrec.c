#include "ht48.h"
#include "keyrec.h"
#include <stdio.h>
#include <string.h>

/*
	Recovers the key for the message-tag pair given in lab description
	Expected result: 001618009000
*/

int main() 
{
	uint8_t message[6] = {9, 8, 7, 6, 5, 4};
	uint8_t tag[6] = {0x7D, 0x1D, 0xEF, 0xA0, 0xB8, 0xAD};

	uint8_t key1[6];
	keyrec(6, message, tag, key1);
	printhash(key1);
	printf("\n");
}