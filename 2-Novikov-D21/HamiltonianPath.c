#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "Stack.h"
#define NO_HAMILTONIAN 0
int* visited;
stack_t* visitOrder;
int visitedVertNum = 0;
int** adjMatr;
int vertexNum = 0;
void* ReadAdjacencyList(FILE* file) {
	vertexNum = 0;
	if (fscanf(file, "%i", &vertexNum) != 1) {
		fprintf(stderr, "%s", "file is empty");
		return NULL;
	}
	if (vertexNum <= 1 || vertexNum >= 1000) {
		fprintf(stderr, "%s", "invalid vertex number");
		return NULL;
	}
	adjMatr = (int**)calloc(vertexNum, sizeof(int*));
	if (adjMatr == NULL) {
		fprintf(stderr, "%s", "malloc error");
		return NULL;
	}
	for (int i = 0; i < vertexNum; i++) {
		adjMatr[i] = (int*)calloc(vertexNum, sizeof(int));
		if (adjMatr[i] == NULL) {
			fprintf(stderr, "%s", "malloc error");
			return NULL;
		}
	}
	char c;
	char str[10];
	int vertexWasRead = 0;
	int curVertex = 0;
	int newLineNum = 0;
	int wasSpace = 0;
	while (newLineNum <= vertexNum) {
		c = fgetc(file);
		if (c == '\n') {
			wasSpace = 0;
			if (vertexWasRead != 0) {
				if (str[0] != '\0') {
					int vertex = atoi(str) - 1;
					adjMatr[curVertex][vertex] = 1;
					adjMatr[vertex][curVertex] = 1;
				}
			}
			vertexWasRead = 0;
			memset(str, '\0', sizeof(str));
			newLineNum++;
		}
		else if (isdigit(c)) {
			char cToStr[2];
			cToStr[0] = c;
			cToStr[1] = '\0';
			strcat(str, cToStr);
		}
		else if (c == ' ') {
			wasSpace = 1;
			if (vertexWasRead == 0) {
				vertexWasRead = 1;
				curVertex = atoi(str) - 1;
				memset(str, '\0', sizeof(str));
			}
			else {
				if (str[0] != '\0') {
					int vertex = atoi(str) - 1;
					adjMatr[curVertex][vertex] = 1;
					adjMatr[vertex][curVertex] = 1;
					memset(str, '\0', sizeof(str));
				}
			}
		}

	}
}
int CheckAdjMatr() {
	int endVertexNum = 0;
	for (int i = 0; i < vertexNum; i++) {
		int intersecNum = 0;
		for (int k = 0; k < vertexNum; k++) {
			if (adjMatr[i][k] == 1) {
				intersecNum++;
			}
		}
		if (intersecNum == 0) {
			return NO_HAMILTONIAN;
		}
		else if (intersecNum == 1) {
			endVertexNum++;
			if (endVertexNum > 2) {
				return NO_HAMILTONIAN;
			}
		}
	}
	return 1;
}
void HamiltonianPath(int curVert) {
	visitedVertNum++;
	visited[curVert] = 1;
	StackPush(visitOrder, curVert);
	for (int i = 0; i < vertexNum; i++) {
		if (adjMatr[curVert][i] == 1 && visited[i] == 0) {
			HamiltonianPath(i);
			if (visitedVertNum == vertexNum) {
				return;
			}
		}
	}
	if (visitedVertNum != vertexNum) {
		visitedVertNum--;
		visited[curVert] = 0;
		StackPop(visitOrder);
	}
	// next start vert
	if (visitedVertNum == 0 && curVert < vertexNum - 1) {
		HamiltonianPath(curVert + 1);
	}
}
void FindHamiltonianPath() {
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) {
		fprintf(stderr, "%s", "open file error");
		return;
	}
	if (ReadAdjacencyList(input) == NULL) {
		return;
	};
	fclose(input);
	visited = calloc(vertexNum, sizeof(int));
	visitOrder = StackInit(vertexNum);
	if (visitOrder == NULL) {
		fprintf(stderr, "%s", "stack initialization error");
		return;
	}

	//Check if graph is connected or graph has 3 or more vertexes
	//that connect with only one vertex, so there can't be Hamiltonian path
	if (CheckAdjMatr() == NO_HAMILTONIAN) {
		printf("%i", 0);
		return;
	}

	//Start algorithm
	HamiltonianPath(0);

	FILE* output = fopen("output.txt", "w");
	if (output == NULL) {
		fprintf(stderr, "%s", "open file error");
		return;
	}
	if (visitedVertNum == 0) {
		printf("%i\n", 0);
		fprintf(output, "%i", 0);
	}
	else {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i ", visitOrder->arr[k] + 1);
			fprintf(output, "%i ", visitOrder->arr[k] + 1);
		}
	}
	fclose(output);
}