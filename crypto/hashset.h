#pragma once

#include "ht48.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Basic list structure
 */
struct ListNode {
	uint8_t key[3];
	uint8_t value[16];
	struct ListNode *next;
};

/* Adds element to the head of the list, returns new head
 * In: list, key, value
 */
struct ListNode *lst_add(struct ListNode *list, const uint8_t key[static 3], 
	const uint8_t value[static 16]) 
{
	struct ListNode *new_head = (struct ListNode*)malloc(sizeof(struct ListNode));
	memcpy(new_head->key, key, 3);
	memcpy(new_head->value, value, 16);
	new_head->next = list;
	return new_head;
}

/* Finds elements in the list. If found, writes element to value and returns true,
   returns false otherwise
 * In: list, key
 * Out: value
 */
bool lst_find(const struct ListNode *list, const uint8_t key[static 3], uint8_t value[static 16]) 
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

/* Frees memory allocated by list
 * Inout: list
 */
void lst_free(struct ListNode *list) 
{
	if (!list) return;
	lst_free(list->next);
	free(list);
}

#define HS_SIZE (1<<24)

typedef struct ListNode *HashSet[HS_SIZE];

/* Initialization of empty hash table
 */
void hs_init(HashSet hs) 
{
	// TODO: memset
	for (uint64_t i = 0; i < HS_SIZE; i++) {
		hs[i] = 0;
	}
}

/* Adds the key to hash set
 * Inout: hs
 * In: h, value
 */
void hs_add(HashSet hs, const uint8_t h[static 6], const uint8_t value[static 16]) 
{
	uint8_t part_r[3];
	memcpy(part_r, h + 3, 3);

	uint64_t pos = h[0] + h[1] * (1<<8) + h[2] * (1<<16);

	hs[pos] = lst_add(hs[pos], part_r, value);
}

/* Finds elements in the table. If found, writes element to value and returns true,
   returns false otherwise
 * In: hs, h
 * Out: value
 */
bool hs_find(const HashSet hs, const uint8_t h[static 6], uint8_t value[static 16]) 
{
	uint8_t part_r[3];
	memcpy(part_r, h + 3, 3);

	uint64_t pos = h[0] + h[1] * (1<<8) + h[2] * (1<<16);
	return lst_find(hs[pos], part_r, value);
}

/* Frees memory allocated by elements of the table. 
   Does not free the table itself
 * In: hs
 */
void hs_cleanup(const HashSet hs) 
{
	for (int i = 0; i < HS_SIZE; i++) {
		lst_free(hs[i]);
	}
}

