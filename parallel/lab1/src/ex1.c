#include <stdio.h>
#include <time.h>
#include <omp.h>

#define SIZE 100000

int main() {
    int vector1[SIZE];
    int vector2[SIZE];
    int result[SIZE];

    // Initialize vectors
    for (int i = 0; i < SIZE; i++) {
        vector1[i] = i;
        vector2[i] = i * 2;
    }

    // Straightforward version
    clock_t start_time_straight = clock(); // Start timing

    // Perform vector summation
    for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < 1000; j++)
        result[i] = vector1[i] + vector2[i];
    }

    clock_t end_time_straight = clock(); // End timing

    // Print result
    printf("Straightforward result: ");
    for (int i = 0; i < 1; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    // Print execution time for straightforward version
    double time_taken_straight = ((double)(end_time_straight - start_time_straight)) / CLOCKS_PER_SEC;
    printf("Straightforward execution time: %f seconds\n", time_taken_straight);

    // OpenMP version
    int result_omp[SIZE]; // Redefine result array for OpenMP version

    clock_t start_time_omp = clock(); // Start timing

    // Perform vector summation in parallel
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < 1000; j++)
        result_omp[i] = vector1[i] + vector2[i];
    }

    clock_t end_time_omp = clock(); // End timing

    // Print result
    printf("OpenMP result: ");
    for (int i = 0; i < 1; i++) {
        printf("%d ", result_omp[i]);
    }
    printf("\n");

    // Print execution time for OpenMP version
    double time_taken_omp = ((double)(end_time_omp - start_time_omp)) / CLOCKS_PER_SEC;
    printf("OpenMP execution time: %f seconds\n", time_taken_omp);

    return 0;
}
