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


#define ULONG(x)((long unsigned int)(x))

// todo: rm this later

#if defined(FIRST_FIT)

void *memory_alloc(size_t size)
{
    mb_free_t* current_block;
    mb_free_t* prev_block = NULL;
    
    current_block = first_free;

    // first fit
    while(current_block != NULL){
        if (current_block->size - sizeof(mb_allocated_t) >= size) { // 4
            break;
        }
        prev_block = current_block;
        current_block = current_block->next_free;
    }

    if (!current_block) {
        // print error
		print_alloc_info(NULL, size);
        return NULL;
    }

	printf("start of free%ld\n", (void*)current_block - heap_start);

    mb_free_t current_block_backup = *current_block;

	size_t remainder = current_block->size - size - sizeof(mb_allocated_t);

    mb_allocated_t* new_alloc = (mb_allocated_t*)current_block;
    if (remainder > sizeof(mb_free_t) && remainder > sizeof(mb_allocated_t)) {     
		printf("A\n");
        mb_free_t* new_free = 
            (mb_free_t*)((void*)current_block + size + sizeof(mb_allocated_t));
        new_free->size = remainder;
        new_free->next_free = current_block_backup.next_free;

        if (prev_block) {
            prev_block->next_free = new_free;
        } else {
            first_free = new_free;
        }

        new_alloc->size = size + sizeof(mb_allocated_t);
    } else {
		printf("B\n");
        new_alloc->size = current_block_backup.size;

        if (prev_block) {
            prev_block->next_free = current_block_backup.next_free;
        } else {
            first_free = current_block_backup.next_free;
        }
    }


	print_alloc_info((void*)current_block + sizeof(mb_allocated_t), size);
    
	print_mem_state();

    return (void*)current_block + sizeof(mb_allocated_t);
}

#elif defined(BEST_FIT)

void *memory_alloc(size_t size)
{
    /* TODO: insert your code here */
    mb_free_t* current_block;
    mb_free_t* prev_block = NULL;
    
    current_block = first_free;

    // Best fit
	size_t best_size = -1;
	mb_free_t* current_block_best;
	mb_free_t* prev_block_best = NULL;

    while(current_block != NULL){
        if (current_block->size - sizeof(mb_allocated_t) >= size) { // 4
            if (current_block->size < best_size) {
				best_size = current_block->size;
				current_block_best = current_block;
				prev_block_best = prev_block;
			}
        }
        prev_block = current_block;
        current_block = current_block->next_free;
    }

	current_block = current_block_best;
	prev_block = prev_block_best;

    if (!current_block) {
        // print error
		print_alloc_info(NULL, size);
        return NULL;
    }

	printf("start of free%ld\n", (void*)current_block - heap_start);

    mb_free_t current_block_backup = *current_block;

    // update free list
    size_t remainder = current_block->size - size - sizeof(mb_allocated_t);
    // TODO: Check minimum free or allocated size of blocks
    // pseudocode

    mb_allocated_t* new_alloc = (mb_allocated_t*)current_block;
    if (remainder > sizeof(mb_free_t) && remainder > sizeof(mb_allocated_t)) {     
		printf("A\n");
        mb_free_t* new_free = 
            (mb_free_t*)((void*)current_block + size + sizeof(mb_allocated_t));
        new_free->size = remainder;
        new_free->next_free = current_block_backup.next_free;

        if (prev_block) {
            prev_block->next_free = new_free;
        } else {
            first_free = new_free;
        }

        new_alloc->size = size + sizeof(mb_allocated_t);
    } else {
		printf("B\n");
        new_alloc->size = current_block_backup.size;

        if (prev_block) {
            prev_block->next_free = current_block_backup.next_free;
        } else {
            first_free = current_block_backup.next_free;
        }
    }


	print_alloc_info((void*)current_block + sizeof(mb_allocated_t), size);
    
	print_mem_state();

    return (void*)current_block + sizeof(mb_allocated_t);
}

#elif defined(NEXT_FIT)

void *memory_alloc(size_t size)
{
    /* TODO: insert your code here */
    mb_free_t* current_block;
    mb_free_t* prev_block = NULL;
    
    current_block = start_free;

    while(current_block != NULL) {
        if (current_block->size - sizeof(mb_allocated_t) >= size) {
			break;
        }
        prev_block = current_block;
        current_block = current_block->next_free;

		if (current_block->next_free == start_free) {
			// print error
			print_alloc_info(NULL, size);
			return NULL;
		}

		if (current_block == NULL) {
			current_block = first_free;
		}
    }

	printf("start of free%ld\n", (void*)current_block - heap_start);

    mb_free_t current_block_backup = *current_block;

    // update free list
    size_t remainder = current_block->size - size - sizeof(mb_allocated_t);
    // TODO: Check minimum free or allocated size of blocks
    // pseudocode

    mb_allocated_t* new_alloc = (mb_allocated_t*)current_block;
    if (remainder > sizeof(mb_free_t) && remainder > sizeof(mb_allocated_t)) {     
		printf("A\n");
        mb_free_t* new_free = 
            (mb_free_t*)((void*)current_block + size + sizeof(mb_allocated_t));
        new_free->size = remainder;
        new_free->next_free = current_block_backup.next_free;

        if (prev_block) {
            prev_block->next_free = new_free;
        } else {
            first_free = new_free;
        }

        new_alloc->size = size + sizeof(mb_allocated_t);

		start_free = new_free;
    } else {
		printf("B\n");
        new_alloc->size = current_block_backup.size;

        if (prev_block) {
            prev_block->next_free = current_block_backup.next_free;
        } else {
            first_free = current_block_backup.next_free;
        }

		start_free = current_block->next_free;
    }


	print_alloc_info((void*)current_block + sizeof(mb_allocated_t), size);
    
	print_mem_state();

    return (void*)current_block + sizeof(mb_allocated_t);
}

#endif


void run_at_exit(void)
{
    fprintf(stderr,"YEAH B-)\n");
    
    /* TODO: insert your code here */
}




void memory_init(void)
{
    /* register the function that will be called when the programs exits */
    atexit(run_at_exit);

    /* TODO: insert your code here */
    heap_start = my_mmap(MEM_POOL_SIZE);

    first_free = (mb_free_t*)heap_start;
    first_free->size = MEM_POOL_SIZE;
    first_free->next_free = NULL;
     
	start_free = first_free;

    /* TODO: start by using the provided my_mmap function to allocate
     * the memory region you are going to manage */
    
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

	if (p < first_free) {
		/* Here the new free block will be the new head of the freelist */

		// Check for merge
		/* |---------|------|...
		 * ^p        ^first_free
		 */
		if (p + free_block.size == (void*)first_free) {
			printf("SUPERBIBA\n");
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
					printf("LR\n");
				}
				/* |---------|-----|    |----------|
				 * ^cur_free ^p         ^cur_free->next
				*/
				else if (can_merge_left) {
					cur_free->size += free_block.size;
					printf("L\n");
				} 
				/* |---------|   |------|----------|
				 * ^cur_free     ^p     ^cur_free->next
				*/
				else if (can_merge_right) {
					free_block.size += cur_free->next_free->size;
					free_block.next_free = cur_free->next_free->next_free;
					cur_free->next_free = p;
					printf("R\n");
				}
				/* |------|    |------|   |----------|
				 * ^cur_free   ^p         ^cur_free->next
				*/
				else {
					free_block.next_free = cur_free->next_free;
					cur_free->next_free = p;
					printf("_\n");
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


#ifdef MAIN
int main(int argc, char **argv){

  /* The main can be changed, it is *not* involved in tests */
  memory_init();
  print_info();
  int i ; 
  for( i = 0; i < 10; i++){
    char *b = memory_alloc(rand()%8);
    memory_free(b);
  }

  char * a = memory_alloc(15);
  memory_free(a);


  a = memory_alloc(10);
  memory_free(a);

  fprintf(stderr,"%lu\n",(long unsigned int) (memory_alloc(9)));
  return EXIT_SUCCESS;
}
#endif 
