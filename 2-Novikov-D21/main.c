#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "HamiltonianPath.h"
#define TEST_VERTEX_NUM 13
void CreateRndHamiltonPath() {
	//Using Dirac condition: if degree of every vertex >= n/2 then it's hamilton path
	srand(time(NULL));
	FILE* file = fopen("input.txt", "w");
	int** adjMatr = (int**)calloc(TEST_VERTEX_NUM, sizeof(int*));
	for (int i = 0; i < TEST_VERTEX_NUM; i++) {
		adjMatr[i] = (int*)calloc(TEST_VERTEX_NUM, sizeof(int));
		if (adjMatr[i] == NULL) {
			return NULL;
		}
	}
	fprintf(file, "%i\n", TEST_VERTEX_NUM);
	for (int i = 0; i < TEST_VERTEX_NUM; i++) {
		fprintf(file, "%i ", i + 1);
		for (int k = i + 1; k < TEST_VERTEX_NUM; k++) {
			if ((rand() % 2) == 1) {
				adjMatr[i][k] = 1;
				adjMatr[k][i] = 1;
			}
		}
		int n = 0;
		for (int j = 0; j < TEST_VERTEX_NUM; j++) {
			if (adjMatr[i][j] == 1) {
				n++;
			}
		}
		while (n < TEST_VERTEX_NUM / 2) {
			int rnd = rand() % TEST_VERTEX_NUM;
			if (adjMatr[i][rnd] == 0 && rnd != i) {
				adjMatr[i][rnd] = 1;
				adjMatr[rnd][i] = 1;
				n++;
			}
		}
		for (int j = i + 1; j < TEST_VERTEX_NUM; j++) {
			if (adjMatr[i][j] == 1) {
				fprintf(file, "%i ", j + 1);
			}

		}
		fprintf(file, "\n");
	}

	for (int i = 0; i < TEST_VERTEX_NUM; i++) {
		free(adjMatr[i]);
	}
	free(adjMatr);
	fclose(file);
}
int main() {
	CreateRndHamiltonPath();
	FindHamiltonianPath();
	return 0;
}
