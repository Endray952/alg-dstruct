#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
	int* arr;
	int head;
}stack_t;
void StackPush(stack_t* stack, int data) {
	stack->arr[stack->head] = data;
	stack->head++;
}
stack_t* StackInit(int size) {
	stack_t* stack = malloc(sizeof(stack_t));
	stack->arr = calloc(size, sizeof(int));
	stack->head = 0;
	return stack;
}
void StackPop(stack_t* stack) {
	stack->head--;
	stack->arr[stack->head] = 0;
}
int* visited;
stack_t* visitOrder;
int visitedVertNum = 0;
//int** adjMatr;
//int vertexNum;
int** ReadAdjacencyList(int* _vertexNum, FILE* file) {
	int vertexNum = 0;
	if (fscanf(file, "%i", &vertexNum) != 1) {
		return NULL;
	}
	int** adjMatr = (int**)calloc(vertexNum, sizeof(int*));
	if (adjMatr == NULL) {
		return NULL;
	}
	for (int i = 0; i < vertexNum; i++) {
		adjMatr[i] = (int*)calloc(vertexNum, sizeof(int));
		if (adjMatr[i] == NULL) {
			return NULL;
		}
	}
	char c;
	char str[10];
	int vertexWasRead = 0;
	int curVertex = 0;
	int newLineNum = 0;
	while (newLineNum <= vertexNum) {
		c = fgetc(file);
		if (c == '\n') {
			if (vertexWasRead != 0) {
				int vertex = atoi(str) - 1;
				adjMatr[curVertex][vertex] = 1;
				adjMatr[vertex][curVertex] = 1;
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
			if (vertexWasRead == 0) {
				vertexWasRead = 1;
				curVertex = atoi(str) - 1;
				memset(str, '\0', sizeof(str));
			}
			else {
				int vertex = atoi(str) - 1;
				adjMatr[curVertex][vertex] = 1;
				adjMatr[vertex][curVertex] = 1;
				memset(str, '\0', sizeof(str));
			}
		}
	}
	*_vertexNum = vertexNum;
	return adjMatr;
}
void HasGamiltonPath(int** adjMatr, int vertexNum, int curVert) {
//void HasGamiltonPath(int curVert) {
	visitedVertNum++;
	visited[curVert] = 1;
	StackPush(visitOrder, curVert);
	for (int i = 0; i < vertexNum; i++){
		if (adjMatr[curVert][i] == 1 && visited[i] == 0) {
			HasGamiltonPath(adjMatr, vertexNum, i);
			//HasGamiltonPath(i);
			if (visitedVertNum == vertexNum) {
				return;
			}
		}
	}
	if ( visitedVertNum != vertexNum) {
		visitedVertNum--;
		visited[curVert] = 0;
		StackPop(visitOrder);
	}
	if (visitedVertNum == 0) { // next start vert
		HasGamiltonPath(adjMatr, vertexNum, curVert + 1);
		//HasGamiltonPath(curVert + 1);
	}
}
int main() {
	FILE* file = fopen("input.txt", "r");
	int* vertexNum = malloc(sizeof(int));
	if (vertexNum == NULL) {
		return 1;
	}
	int** adjMatr = ReadAdjacencyList(vertexNum, file);
	visited = calloc(vertexNum, sizeof(int));
	visitOrder = StackInit(vertexNum);

	for (int i = 0; i < *vertexNum; i++){
		for (int k = 0; k < *vertexNum; k++) {
			printf("%i ", adjMatr[i][k]);
		}
		puts("");
	}

	//method
	HasGamiltonPath(adjMatr, *vertexNum, 0);
	//HasGamiltonPath(0);
	printf("%i \n", visitedVertNum);
	for (int k = 0; k < *vertexNum; k++) {
		printf("%i ", visitOrder->arr[k] + 1);
	}
	puts("");
	fclose(file);
	
	
	return 0;
}