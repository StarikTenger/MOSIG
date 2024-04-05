#include "ht48.h"
#include "hashset.h"
#include "combination.h"
#include <stdio.h>
#include <string.h>

/*
	This is the target for testing different functions
*/

void print_message(const uint8_t m[static 16]) 
{
	for (int i = 0; i < 16; i++) {
		printf("%02X", m[i]);
	}
	printf("\n");
}


int main() 
{
	int n = 5;
	int k = 3;
	uint64_t comb[k];
	combination_init(n, k, comb);
	combination_print(k, comb);
	while (combination_next(n, k, comb)) {
		combination_print(k, comb);
	}
	// uint8_t v1[3] = {0,0,0};
	// uint8_t v2[3] = {1,1,0};
	// uint8_t v3[3] = {1,2,3};
	// uint8_t v4[3] = {1,2,4};

	// struct ListNode *lst = 0;
	// lst = lst_add(lst, v3);
	// lst = lst_add(lst, v1);
	// lst = lst_add(lst, v2);
	

	// printf(lst_find(lst, v1) ? "y" : "n");
	// printf(lst_find(lst, v2) ? "y" : "n");
	// printf(lst_find(lst, v3) ? "y" : "n");
	// printf(lst_find(lst, v4) ? "y" : "n");

	// lst_free(lst);

	// printf("\nTABLES\n");

	uint8_t k1[6] = {1,2,3,4,5,6};
	uint8_t k2[6] = {1,2,3,5,5,6};
	uint8_t k3[6] = {2,4,6,1,2,3};
	uint8_t k4[6] = {2,4,6,1,2,4};

	uint8_t m1[16] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
	uint8_t m2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	struct ListNode** hs = malloc(HS_SIZE * sizeof(struct ListNode*));
	hs_init(hs);
	hs_add(hs, k1, m1);
	hs_add(hs, k2, m1);
	hs_add(hs, k3, m2);

	uint8_t m_r[16];
	hs_find(hs, k3, m_r);
	print_message(m_r);
	

	printf("\n");
}