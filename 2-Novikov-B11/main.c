#include "memallocator.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//#define POOL_SIZE 10000
#define MAX_ALLOC_SIZE 100
#define TEST_ITERATIONS 1000

int main() {
	FILE* file = fopen("memalloc_memfree.txt", "w");
	void* pool = malloc((MAX_ALLOC_SIZE + 16) * TEST_ITERATIONS);
	meminit(pool, (MAX_ALLOC_SIZE + 16) * TEST_ITERATIONS);
	double* results = (double*)malloc(TEST_ITERATIONS* sizeof(double));
	int* pointers = (int*)malloc(TEST_ITERATIONS * sizeof(int));
	long int start;
	for (int testNum = 0; testNum < TEST_ITERATIONS; testNum++) {
		start = clock();
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			pointers[iterNum] = (int*)memalloc((int)rand() % MAX_ALLOC_SIZE);
		}
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			memfree(pointers[iterNum]);
		}
		results[testNum] = ((double)clock() - start)/CLOCKS_PER_SEC;
	}
	for (int i = 0; i < TEST_ITERATIONS; i++) {
		fprintf(file, "%lf;", results[i]);
	}
	fclose(file);

	file = fopen("malloc_free.txt", "w");
	for (int testNum = 0; testNum < TEST_ITERATIONS; testNum++) {
		start = clock();
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			pointers[iterNum] = (int*)malloc((int)rand() % MAX_ALLOC_SIZE);
		}
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			free(pointers[iterNum]);
		}
		results[testNum] = ((double)clock() - start) / CLOCKS_PER_SEC;
	}
	for (int i = 0; i < TEST_ITERATIONS; i++) {
		fprintf(file, "%lf;", results[i]);
	}
	fclose(file);

	free(results);
	free(pointers);

	return 0;
}
