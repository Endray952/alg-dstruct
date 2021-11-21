#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "HamiltonianPath.h"
#define TEST_VERTEX_NUM 18
#define TEST 1
void Test() {
	//Create non Hamiltonian path
	srand(time(NULL));
	CreateRndHamiltonPath(TEST_VERTEX_NUM, TEST_VERTEX_NUM - 3);
	FILE* file = fopen("input.txt", "a+");
	fprintf(file, "%i %i %i\n", TEST_VERTEX_NUM - 2, TEST_VERTEX_NUM - 1, TEST_VERTEX_NUM);
	fprintf(file, "%i %i\n", TEST_VERTEX_NUM - 1, TEST_VERTEX_NUM);
	fprintf(file, "%i\n",TEST_VERTEX_NUM);
	fclose(file);
}
/**
 * CPU: Intel(R) Core(TM) i7-6700K CPU 4.10GHz
 * RAM: 32Gb DDR4 2133 MHz
 * SSD: SATA 3 read speed 545Mb/s Write Speed 465Mb/s
 *
 * Vertecies amount: 18
 *
 * Stress Test results worst case (non Hamiltonian path):
 *   Memory used: 7 Mb
 *   execution time: 464.9 seconds
 */
int main() {
	if (TEST) {
		Test();
	}
	FindHamiltonianPath(TEST);
	return 0;
}
