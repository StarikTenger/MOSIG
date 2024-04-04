#include "ht48.h"
#include "hashset.h"
#include <stdio.h>
#include <string.h>

int main() 
{
	uint8_t v1[3] = {0,0,0};
	uint8_t v2[3] = {1,1,0};
	uint8_t v3[3] = {1,2,3};
	uint8_t v4[3] = {1,2,4};

	struct ListNode *lst = 0;
	lst = lst_add(lst, v3);
	lst = lst_add(lst, v1);
	lst = lst_add(lst, v2);
	

	printf(lst_find(lst, v1) ? "y" : "n");
	printf(lst_find(lst, v2) ? "y" : "n");
	printf(lst_find(lst, v3) ? "y" : "n");
	printf(lst_find(lst, v4) ? "y" : "n");

	lst_free(lst);

	printf("\nTABLES\n");

	uint8_t vv1[6] = {1,2,3,4,5,6};
	uint8_t vv2[6] = {1,2,3,5,5,6};
	uint8_t vv3[6] = {2,4,6,1,2,3};
	uint8_t vv4[6] = {2,4,6,1,2,4};

	struct ListNode** hs = malloc(HS_SIZE * sizeof(struct ListNode*));
	hs_init(hs);
	hs_add(hs, vv1);
	hs_add(hs, vv2);
	hs_add(hs, vv3);

	printf(hs_find(hs, vv1) ? "y" : "n");
	printf(hs_find(hs, vv2) ? "y" : "n");
	printf(hs_find(hs, vv3) ? "y" : "n");
	printf(hs_find(hs, vv4) ? "y" : "n");

	printf("\n");
}