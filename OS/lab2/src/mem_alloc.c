#include "mem_alloc.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "mem_alloc_types.h"
#include "my_mmap.h"

/* pointer to the beginning of the memory region to manage */
void *heap_start;           // static var, means it's been allocated somewhere
                            // should use this as starting address for the memory region
/* Pointer to the first free block in the heap */
mb_free_t *first_free;
mb_free_t *start_free;
mb_allocated_t *first_alloc;


#define ULONG(x)((long unsigned int)(x))

// todo: rm this later
#define FIRST_FIT

#if defined(FIRST_FIT)

void *memory_alloc(size_t size)
{
    mb_free_t* current_block;
    mb_free_t* prev_block = NULL;
    
    current_block = first_free;

    /* Finding suitable free block */
    while(current_block != NULL){
        if (current_block->size - sizeof(mb_allocated_t) >= size) { // 4
            break;
        }
        prev_block = current_block;
        current_block = current_block->next_free;
    }

	/* No suitable free block is found */
    if (!current_block) {
		print_alloc_info(NULL, size);
        return NULL;
    }

	/* 
	 * |--------------------| <- free block
	 * |----------|           <- allocated part
	 *            |---------| <- remainder
	 */
	size_t remainder = current_block->size - size - sizeof(mb_allocated_t);

    mb_allocated_t* new_alloc = (mb_allocated_t*)current_block;

	/* Check if the remainder is big enough to fit a new free block */
    if (remainder > sizeof(mb_free_t) && remainder > sizeof(mb_allocated_t)) {     
        mb_free_t* new_free = 
            (mb_free_t*)((void*)current_block + size + sizeof(mb_allocated_t));
        new_free->size = remainder;
        new_free->next_free = current_block->next_free;

        if (prev_block) {
            prev_block->next_free = new_free;
        } else {
            first_free = new_free;
        }

        new_alloc->size = size + sizeof(mb_allocated_t);
    } else {
        new_alloc->size = current_block->size;

        if (prev_block) {
            prev_block->next_free = current_block->next_free;
        } else {
            first_free = current_block->next_free;
        }
    }

	/* Link new_alloc to the alloc list */
	if (!first_alloc) {
		/* No alloc list yet. Our new_alloc will be the first */
		first_alloc = new_alloc;
		new_alloc->next_alloc = NULL;
	}
	else if (first_alloc > new_alloc) {
		/* new_alloc before the head of the list */
		new_alloc->next_alloc = first_alloc;
		first_alloc = new_alloc;
	}
	else {
		/* new_alloc inside or after the list 
		 * here we find the block which goes right before the new_alloc
		 */
		mb_allocated_t *cur_alloc;
		for(cur_alloc = first_alloc; cur_alloc; 
			cur_alloc = cur_alloc->next_alloc) {
			if ((void*)new_alloc > (void*)cur_alloc && 
				(!cur_alloc->next_alloc || (void*)new_alloc < (void*)cur_alloc->next_alloc)) {
				break;
			}
		}
		
		new_alloc->next_alloc = cur_alloc->next_alloc;
		cur_alloc->next_alloc = new_alloc;
	}
	

	print_alloc_info((void*)current_block + sizeof(mb_allocated_t), size);
    
	print_mem_state();

    return (void*)current_block + sizeof(mb_allocated_t);
}

#elif defined(BEST_FIT)


#elif defined(NEXT_FIT)


#endif


void run_at_exit(void)
{    
    if (first_alloc) {
		size_t wasted_memory = 0;
		size_t wasted_user_memory = 0;
		size_t wasted_blocks = 0;
		for (mb_allocated_t* cur_alloc = first_alloc; cur_alloc; cur_alloc = cur_alloc->next_alloc) {
			wasted_blocks++;
			wasted_memory += cur_alloc->size;
			wasted_user_memory += cur_alloc->size - sizeof(mb_allocated_t);
		}
		fprintf(
			stderr, "Memory leak detected\nwasted %ld blocks with total size %ld (%ld allocated by user)\n", 
			wasted_blocks, wasted_memory, wasted_user_memory);
	}
}

void memory_init(void)
{
    /* register the function that will be called when the programs exits */
    atexit(run_at_exit);

    heap_start = my_mmap(MEM_POOL_SIZE);

    first_free = (mb_free_t*)heap_start;
    first_free->size = MEM_POOL_SIZE;
    first_free->next_free = NULL;
     
	start_free = first_free;

	first_alloc = NULL;
    
}

void memory_free(void *p)
{
	/* void *p represetns the start of the user data, so we decrease it to get 
	 * the header address
	 */
	p -= sizeof(mb_allocated_t);

	/* We make the varaible for free block data to write it to the memory then
	 */
	mb_free_t free_block;
	free_block.size = ((mb_allocated_t*)p)->size;

	if (!first_alloc || p < (void*)first_alloc) {
		fprintf(stderr, "ERROR: invalid block address in memory_free\n");
		return;
	}

	/* Erase allocated block from alloc list */
	if (p == (void*)first_alloc) {
		first_alloc = ((mb_allocated_t*)p)->next_alloc;
	}
	else {
		mb_allocated_t *cur_alloc = first_alloc;
		for (;cur_alloc; cur_alloc = cur_alloc->next_alloc) {
			if (cur_alloc->next_alloc == ((mb_allocated_t*)p)) {
				cur_alloc->next_alloc = ((mb_allocated_t*)p)->next_alloc;
				break;
			}
		}
		if (!cur_alloc) {
			fprintf(stderr, "ERROR: invalid block address in memory_free\n");
			return;
		}
	}

	if (p < first_free) {
		/* Here the new free block will be the new head of the freelist */

		// Check for merge
		/* |---------|------|...c
		 * ^p        ^first_free
		 */
		if (p + free_block.size == (void*)first_free) {
			free_block.size += first_free->size;
			free_block.next_free = first_free->next_free;
		} else {
			free_block.next_free = first_free;
		}
		first_free = p;
	} else {
		/* If p is not before the first elem. in free list, it is somewhere 
		 * between two free blocks or after them.
		 *
		 * |------|    |------|   |----------|
		 * ^cur_free   ^p         ^cur_free->next
		 */
		for (mb_free_t* cur_free = first_free; 
		     cur_free; cur_free = cur_free->next_free) {

			// Check if p is between two current blocks
			if ((void*)cur_free < p && (!cur_free->next_free || 
				(void*)cur_free->next_free > p)) {
				
				/* Following bool varaibles represent the possibility of merging
				 * block at address p with left and right neighbors respectively
				 */
				bool can_merge_left = 
					(void*)cur_free + cur_free->size == (void*)p;
				bool can_merge_right = 
					cur_free->next_free && 
					p + free_block.size == (void*)cur_free->next_free;


				/* |---------|------|----------|
				 * ^cur_free ^p     ^cur_free->next
				*/
				if (can_merge_left && can_merge_right) {
					cur_free->size += 
						free_block.size + cur_free->next_free->size;
					cur_free->next_free = cur_free->next_free->next_free;
				}
				/* |---------|-----|    |----------|
				 * ^cur_free ^p         ^cur_free->next
				*/
				else if (can_merge_left) {
					cur_free->size += free_block.size;
				
				} 
				/* |---------|   |------|----------|
				 * ^cur_free     ^p     ^cur_free->next
				*/
				else if (can_merge_right) {
					free_block.size += cur_free->next_free->size;
					free_block.next_free = cur_free->next_free->next_free;
					cur_free->next_free = p;
				}
				/* |------|    |------|   |----------|
				 * ^cur_free   ^p         ^cur_free->next
				*/
				else {
					free_block.next_free = cur_free->next_free;
					cur_free->next_free = p;
				}
				break;
			}
		}
	}


	/* Finally we can write new free block in memory. We don't need in case of 
	 *right merge, but since it wouldn't spoil any data why not
	 */
	*(mb_free_t*)p = free_block;

	print_free_info(p + sizeof(mb_allocated_t));
	print_mem_state();

}

size_t memory_get_allocated_block_size(void *addr)
{

    /* TODO: insert your code here */

    return 0;
}


void print_mem_state(void)
{

	const size_t linesize = 172;
	char line[linesize];
	for (int i = 0; i < linesize; i++) {
		line[i] = '|';
	}
	line[linesize - 1] = '\0';

	for (mb_free_t* cur_free = first_free; cur_free; cur_free = cur_free->next_free) {
		for (int i = 0; i < cur_free->size && (void*)cur_free - heap_start + i < linesize; i++) {
			line[(void*)cur_free - heap_start + i] = '_';
		}
		for (int i = 0; i < sizeof(mb_free_t) && (void*)cur_free - heap_start + i < linesize; i++) {
			line[(void*)cur_free - heap_start + i] = 'f';
		}
	}

	for (mb_allocated_t* cur_alloc = first_alloc; cur_alloc; cur_alloc = cur_alloc->next_alloc) {
		for (int i = 0; i < sizeof(mb_free_t) && (void*)cur_alloc - heap_start + i < linesize; i++) {
			line[(void*)cur_alloc - heap_start + i] = 'A';
		}
	}

	printf("%s\n", line);
    /* TODO: insert your code here */
}


void print_info(void) {
    fprintf(stderr, "Memory : [%lu %lu] (%lu bytes)\n", (long unsigned int) heap_start, (long unsigned int) ((char*)heap_start+MEM_POOL_SIZE), (long unsigned int) (MEM_POOL_SIZE));
}

void print_free_info(void *addr) {
    if(addr){
        fprintf(stderr, "FREE  at : %lu \n", ULONG((char*)addr - (char*)heap_start));
    }
    else{
        fprintf(stderr, "FREE  at : %lu \n", ULONG(0));
    }
    
}

void print_alloc_info(void *addr, int size) {
  if(addr){
    fprintf(stderr, "ALLOC at : %lu (%d byte(s))\n", 
	    ULONG((char*)addr - (char*)heap_start), size);
  }
  else{
    fprintf(stderr, "Warning, system is out of memory\n"); 
  }
}

void print_alloc_error(int size) 
{
    fprintf(stderr, "ALLOC error : can't allocate %d bytes\n", size);
}


#define MAIN

/* Some functions for manual testing */

void test1() {
	void *a = memory_alloc(8);
	void *b = memory_alloc(8);
	void *c = memory_alloc(8);
	void *d = memory_alloc(8);

	memory_free(b);
	memory_free(a);
	memory_free(d);
	memory_free(c);

	// No errors
}

void test2() {
	void *a = memory_alloc(8);
	void *b = memory_alloc(8);
	void *c = memory_alloc(8);
	void *d = memory_alloc(8);

	memory_free(b);
	memory_free(b);
	memory_free(a);
	memory_free(d);
	memory_free(c);

	// Error of illegal free
}

void test3() {
	void *a = memory_alloc(8);
	void *b = memory_alloc(8);
	void *c = memory_alloc(8);
	void *d = memory_alloc(8);

	memory_free(heap_start - 10);
	memory_free(b);
	memory_free(a);
	memory_free(d);
	memory_free(c);

	// Error of illegal free
}

void test4() {
	void *a = memory_alloc(8);
	void *b = memory_alloc(8);
	void *c = memory_alloc(8);
	void *d = memory_alloc(8);

	memory_free(d);
	memory_free(c);

	// Memory leak error
}

void test5() {
	void *a = memory_alloc(8);
	void *b = memory_alloc(8);
	void *c = memory_alloc(8);
	void *d = memory_alloc(8);

	memory_free(heap_start);
	memory_free(c);

	// Memory leak error + illegal free error
}

#ifdef MAIN
int main(int argc, char **argv) {
	memory_init();
	print_info();

	// Put test function here
	test1();

	return EXIT_SUCCESS;
}
#endif 