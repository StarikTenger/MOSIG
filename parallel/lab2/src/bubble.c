#include <stdio.h>
#include <omp.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "sorting.h"



/* 
   bubble sort -- sequential, parallel -- 
*/

void swap(uint64_t *a, uint64_t *b) {
	uint64_t c = *a;
	*a = *b;
	*b = c;
}

void sequential_bubble_sort (uint64_t *T, const uint64_t size)
{
	size_t n_swaps = 1;
    for (uint64_t i = 0; i < size && n_swaps; i++) {
		n_swaps = 0;
		for (uint64_t j = 1; j < size; j++) {
			if (T[j - 1] > T[j]) {
				swap(&T[j - 1], &T[j]);
				n_swaps++;
			}
		}
	}
    
    return;
}

void parallel_bubble_sort (uint64_t *T, const uint64_t size)
{
    const uint64_t N_CHUNKS = omp_get_max_threads();
	omp_set_num_threads(N_CHUNKS);
	const uint64_t CHUNK_SIZE = size / N_CHUNKS;
	bool sorted = 0;

	#pragma omp parallel
	{
		int num_thread = omp_get_thread_num();

		// #pragma omp critical
		// printf("Worker %d\n", num_thread);

		for (uint64_t i = 0; !sorted; i++) {
			sequential_bubble_sort(T + num_thread * CHUNK_SIZE, CHUNK_SIZE);
			#pragma omp barrier

			sorted = 1;

			#pragma omp barrier
			if (num_thread > 0 && T[num_thread * CHUNK_SIZE - 1] > T[num_thread * CHUNK_SIZE]) {
				swap(&T[num_thread * CHUNK_SIZE - 1], &T[num_thread * CHUNK_SIZE]);
				sorted = 0;
			}

			// #pragma omp critical
			// print_array(T, size);

			#pragma omp barrier
		}
	}
	
    
    return;
}


int main (int argc, char **argv)
{
	


    // Init cpu_stats to measure CPU cycles and elapsed time
    struct cpu_stats *stats = cpu_stats_init();

    unsigned int exp ;

    /* the program takes one parameter N which is the size of the array to
       be sorted. The array will have size 2^N */
    if (argc != 2)
    {
        fprintf (stderr, "Usage: bubble.run N \n") ;
        exit (-1) ;
    }

    uint64_t N = 1 << (atoi(argv[1])) ;
    /* the array to be sorted */
    uint64_t *X = (uint64_t *) malloc (N * sizeof(uint64_t)) ;


    printf("--> Sorting an array of size %lu\n",N);
#ifdef RINIT
    printf("--> The array is initialized randomly\n");
#endif
    

    for (exp = 0 ; exp < NB_EXPERIMENTS; exp++){
#ifdef RINIT
        init_array_random (X, N);
#else
        init_array_sequence (X, N);
#endif        
    cpu_stats_begin(stats);
        
        sequential_bubble_sort (X, N) ;

    experiments[exp] = cpu_stats_end(stats);

        /* verifying that X is properly sorted */
#ifdef RINIT
        if (! is_sorted (X, N))
        {
            print_array (X, N) ;
            fprintf(stderr, "ERROR: the sequential sorting of the array failed\n") ;
            exit (-1) ;
	}
#else
        if (! is_sorted_sequence (X, N))
        {
            print_array (X, N) ;
            fprintf(stderr, "ERROR: the sequential sorting of the array failed\n") ;
            exit (-1) ;
	}
#endif
    }

    println_cpu_stats_report("bubble serial", average_report(experiments, NB_EXPERIMENTS));
  
    for (exp = 0 ; exp < NB_EXPERIMENTS; exp++)
    {
#ifdef RINIT
        init_array_random (X, N);
#else
        init_array_sequence (X, N);
#endif
        
        cpu_stats_begin(stats);

        parallel_bubble_sort (X, N) ;

        experiments[exp] = cpu_stats_end(stats);

        /* verifying that X is properly sorted */
#ifdef RINIT
        if (! is_sorted (X, N))
        {
            print_array (X, N) ;
            fprintf(stderr, "ERROR: the parallel sorting of the array failed\n") ;
            exit (-1) ;
	}
#else
        if (! is_sorted_sequence (X, N))
        {
            print_array (X, N) ;
            fprintf(stderr, "ERROR: the parallel sorting of the array failed\n") ;
            exit (-1) ;
	}
#endif
                
        
    }
    
    println_cpu_stats_report("bubble parallel", average_report(experiments, NB_EXPERIMENTS));
    
    /* print_array (X, N) ; */

    /* before terminating, we run one extra test of the algorithm */
    uint64_t *Y = (uint64_t *) malloc (N * sizeof(uint64_t)) ;
    uint64_t *Z = (uint64_t *) malloc (N * sizeof(uint64_t)) ;

#ifdef RINIT
    init_array_random (Y, N);
#else
    init_array_sequence (Y, N);
#endif

    memcpy(Z, Y, N * sizeof(uint64_t));

    sequential_bubble_sort (Y, N) ;
    parallel_bubble_sort (Z, N) ;

    if (! are_vector_equals (Y, Z, N)) {
        fprintf(stderr, "ERROR: sorting with the sequential and the parallel algorithm does not give the same result\n") ;
        exit (-1) ;
    }


    free(X);
    free(Y);
    free(Z);
    
}
