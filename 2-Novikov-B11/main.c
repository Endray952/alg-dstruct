#include "memallocator.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
//#define POOL_SIZE 10000
#define MAX_ALLOC_SIZE 100
#define TEST_ITERATIONS 1000

/** Use to init the clock */
#define TIMER_INIT \
    LARGE_INTEGER frequency; \
    LARGE_INTEGER t1,t2; \
    double elapsedTime; \
    QueryPerformanceFrequency(&frequency);


/** Use to start the performance timer */
#define TIMER_START QueryPerformanceCounter(&t1);

/** Use to stop the performance timer */
#define TIMER_STOP \
    QueryPerformanceCounter(&t2); \
    elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart; \
    


int main() {
	FILE* file = fopen("memalloc_memfree.txt", "w");
	void* pool = malloc((MAX_ALLOC_SIZE + 16) * TEST_ITERATIONS);
	meminit(pool, (MAX_ALLOC_SIZE + 16) * TEST_ITERATIONS);
	double* results = (double*)malloc(TEST_ITERATIONS* sizeof(double));
	int* pointers = (int*)malloc(TEST_ITERATIONS * sizeof(int));
	long int start;
	TIMER_INIT
	for (int testNum = 0; testNum < TEST_ITERATIONS; testNum++) {
		TIMER_START
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			pointers[iterNum] = (int*)memalloc((int)rand() % MAX_ALLOC_SIZE);
		}
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			memfree(pointers[iterNum]);
		}
		TIMER_STOP
			results[testNum] = elapsedTime;
	}
	for (int i = 0; i < TEST_ITERATIONS; i++) {
		fprintf(file, "%lf;", results[i]);
	}
	fclose(file);

	file = fopen("malloc_free.txt", "w");
	for (int testNum = 0; testNum < TEST_ITERATIONS; testNum++) {
		TIMER_START
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			pointers[iterNum] = (int*)malloc((int)rand() % MAX_ALLOC_SIZE);
		}
		for (int iterNum = 0; iterNum < testNum; iterNum++) {
			free(pointers[iterNum]);
		}
		TIMER_STOP
			results[testNum] = elapsedTime;
	}
	for (int i = 0; i < TEST_ITERATIONS; i++) {
		fprintf(file, "%lf;", results[i]);
	}
	fclose(file);

	free(results);
	free(pointers);
	free(pool);

	return 0;
}
