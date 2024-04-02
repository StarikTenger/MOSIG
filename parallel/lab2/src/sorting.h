#ifndef __SORTING_H__
#define __SORTING_H__

#include "cpu_stats.h"
#include <stdlib.h>

#define NB_EXPERIMENTS 10
extern struct cpu_stats_report experiments[NB_EXPERIMENTS];

/* utility functions */
void init_array_sequence(uint64_t *T, uint64_t size);
void init_array_random(uint64_t *T, uint64_t size);
void print_array(uint64_t *T, uint64_t size);
int is_sorted_sequence(uint64_t *T, uint64_t size);
int is_sorted(uint64_t *T, uint64_t size);
int are_vector_equals(uint64_t *T1, uint64_t *T2, uint64_t size);

#endif /* __SORTING_H__ */
