#include "ht48.h"
#include "keyrec.h"
#include <stdio.h>
#include <string.h>


int main() 
{
	{
		uint8_t key[6] = {0, 1, 2, 3, 4, 5};
		uint8_t message[6] = {9, 8, 7, 6, 5, 4};
		uint8_t tag[6];
		smht48(key, 6, message, tag);
		printhash(tag);
		printf("\nEE75794547B8 was expected\n");
	}

	{
		uint8_t key[6] = {0xE4, 0x16, 0x9F, 0x12, 0xD3, 0xBA};
		uint8_t message[6] = {9, 8, 7, 6, 5, 4};
		uint8_t tag[6];
		smht48(key, 6, message, tag);
		printhash(tag);
		printf("\n5F265B72B5EC was expected\n");
	}
}