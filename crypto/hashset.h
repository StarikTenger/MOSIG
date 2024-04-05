#include "ht48.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct ListNode {
	uint8_t key[3];
	uint8_t value[16];
	struct ListNode *next;
};

struct ListNode *lst_add(struct ListNode *list, const uint8_t key[static 3], 
	const uint8_t value[static 16]) 
{
	struct ListNode *new_head = (struct ListNode*)malloc(sizeof(struct ListNode));
	memcpy(new_head->key, key, 3);
	memcpy(new_head->value, value, 16);
	new_head->next = list;
	return new_head;
}

bool lst_find(struct ListNode *list, const uint8_t key[static 3], uint8_t value[static 16]) 
{
	for (;list; list = list->next) {
		if (list->key[0] == key[0] &&
			list->key[1] == key[1] &&
			list->key[2] == key[2]) {
			memcpy(value, list->value, 16);
			return true;
		}
	}
	return false;
}

void lst_free(struct ListNode *list) 
{
	if (!list) return;
	lst_free(list->next);
	free(list);
}

#define HS_SIZE (1<<24)

typedef struct ListNode *HashSet[HS_SIZE];

void hs_init(HashSet hs) 
{
	// TODO: memset
	for (uint64_t i = 0; i < HS_SIZE; i++) {
		hs[i] = 0;
	}
}

// Adds the key to hash set
void hs_add(HashSet hs, const uint8_t h[static 6], const uint8_t value[static 16]) 
{
	uint8_t part_r[3];
	memcpy(part_r, h + 3, 3);

	uint64_t pos = h[0] + h[1] * (1<<8) + h[2] * (1<<16);

	hs[pos] = lst_add(hs[pos], part_r, value);
}

// Checks whether hash set contains a key
bool hs_find(HashSet hs, const uint8_t h[static 6], uint8_t value[static 16]) 
{
	uint8_t part_r[3];
	memcpy(part_r, h + 3, 3);

	uint64_t pos = h[0] + h[1] * (1<<8) + h[2] * (1<<16);
	return lst_find(hs[pos], part_r, value);
}

void hs_free(HashSet hs) 
{
	for (int i = 0; i < HS_SIZE; i++) {
		lst_free(hs[i]);
	}
}

