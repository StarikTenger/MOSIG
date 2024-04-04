#include "ht48.h"
#include "keyrec.h"
#include <stdio.h>
#include <string.h>


int main() 
{
	{
		uint8_t key[6] = {0, 1, 2, 3, 4, 5};
		uint8_t message[16] = {1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
		uint8_t tag[6];
		smht48(key, 16, message, tag);
		printhash(tag);
		printf("\n");
	}

	{
		uint8_t key[6] = {5,4,3,2,1,0};
		uint8_t message[16] = {1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
		uint8_t tag[6];
		smht48(key, 16, message, tag);
		printhash(tag);
		printf("\n");
	}
}