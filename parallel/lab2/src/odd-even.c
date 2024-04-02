#include <stdio.h>
#include <omp.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <stdbool.h>

#include "sorting.h"

/* 
   odd-even sort -- sequential, parallel -- 
*/


void swap(uint64_t *a, uint64_t *b) {
	uint64_t c = *a;
	*a = *b;
	*b = c;
}

void sequential_oddeven_sort (uint64_t *T, const uint64_t size)
{
	bool sorted = 0;
    while (!sorted) {
		sorted = 1;
		for (uint64_t i = 1; i < size; i+=2) {
			if (T[i - 1] > T[i]) {
				swap(&T[i - 1], &T[i]);
				sorted = 0;
			}
		}

		for (uint64_t i = 2; i < size; i+=2) {
			if (T[i - 1] > T[i]) {
				swap(&T[i - 1], &T[i]);
				sorted = 0;
			}
		}
	}
	

    return ;
}


void parallel_oddeven_sort (uint64_t *T, const uint64_t size)
{
	/*
		This is anattempt to not start new tasks each time in the loop,
		but to start them all one time.
		However, it didn't work, performance was worse then with naive 
		implementation
	*/

	// int threads = omp_get_max_threads();
	// omp_set_num_threads(threads);
	// uint64_t chunk_size = size / threads;

    // bool sorted = 0;
	// #pragma omp parallel
	// {
	// 	int num_thread = omp_get_thread_num();
	// 	uint64_t my_begin = chunk_size * num_thread;
	// 	int my_sorted = 0;

	// 	// #pragma omp critical
	// 	// printf("Worker %d my_begin=%d \n", num_thread, (int)my_begin);

	// 	while (!sorted) {
	// 		// #pragma omp critical 
	// 		// {
	// 		// 	printf("Worker %d my_begin=%d \n", num_thread, (int)my_begin);
	// 		// 	print_array(T, size);
	// 		// }

	// 		#pragma omp barrier
	// 		my_sorted = 1;
	// 		sorted = 1;
	// 		#pragma omp barrier

	// 		for (uint64_t i = my_begin + 1; i < my_begin + chunk_size; i+=2) {
	// 			if (T[i - 1] > T[i]) {
	// 				swap(&T[i - 1], &T[i]);
	// 				my_sorted = 0;
	// 			}
	// 		}

	// 		#pragma omp barrier

	// 		for (uint64_t i = my_begin + 2; i < my_begin + chunk_size + 1; i+=2) {
	// 			if (T[i - 1] > T[i]) {
	// 				swap(&T[i - 1], &T[i]);
	// 				my_sorted = 0;
	// 			}
	// 		}

	// 		#pragma omp critical
	// 		sorted = sorted && my_sorted;

	// 		#pragma omp barrier
	// 	}

	// 	// #pragma omp critical
	// 	// printf("Worker %d finished");
	// }

    // return ;

	bool sorted = 0;
    while (!sorted) {
		sorted = 1;
		#pragma omp parallel for
		for (uint64_t i = 1; i < size; i+=2) {
			if (T[i - 1] > T[i]) {
				swap(&T[i - 1], &T[i]);
				sorted = 0;
			}
		}

		#pragma omp parallel for
		for (uint64_t i = 2; i < size; i+=2) {
			if (T[i - 1] > T[i]) {
				swap(&T[i - 1], &T[i]);
				sorted = 0;
			}
		}
	}
	

    return ;

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
        fprintf (stderr, "Usage: odd-even.run N \n") ;
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
        
        sequential_oddeven_sort (X, N) ;
     
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

    println_cpu_stats_report("odd-even serial", average_report(experiments, NB_EXPERIMENTS));

  
    for (exp = 0 ; exp < NB_EXPERIMENTS; exp++)
    {
#ifdef RINIT
        init_array_random (X, N);
#else
        init_array_sequence (X, N);
#endif
        
        cpu_stats_begin(stats);

        parallel_oddeven_sort (X, N) ;

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

    println_cpu_stats_report("odd-even parallel", average_report(experiments, NB_EXPERIMENTS));
  
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

    sequential_oddeven_sort (Y, N) ;
    parallel_oddeven_sort (Z, N) ;

    if (! are_vector_equals (Y, Z, N)) {
        fprintf(stderr, "ERROR: sorting with the sequential and the parallel algorithm does not give the same result\n") ;
        exit (-1) ;
    }


    free(X);
    free(Y);
    free(Z);
    
}
