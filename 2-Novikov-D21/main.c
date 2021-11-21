#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define TEST_VERTEX_NUM 30
void CreateRndHamiltonPath();
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
int** adjMatr;
int vertexNum = 0;
void ReadAdjacencyList(FILE* file) {
	vertexNum = 0;
	if (fscanf(file, "%i", &vertexNum) != 1) {
		return NULL;
	}
	adjMatr = (int**)calloc(vertexNum, sizeof(int*));
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
int checkMatr() {
	int endVertexNum = 0;
	for (int i = 0; i < vertexNum; i++){
		int intersecNum = 0;
		for (int k = 0; k < vertexNum; k++){
			if (adjMatr[i][k] == 1) {
				intersecNum++;
			}
		}
		if (intersecNum == 0) {
			return -1;
		}
		else if (intersecNum == 1) {
			endVertexNum++;
			if (endVertexNum > 2) {
				return -1;
			}
		}
	}
	return 1;
}
void HasHamiltonPath(int curVert) {
	visitedVertNum++;
	visited[curVert] = 1;
	StackPush(visitOrder, curVert);
	for (int i = 0; i < vertexNum; i++){
		if (adjMatr[curVert][i] == 1 && visited[i] == 0) {
			HasHamiltonPath(i);
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
	// next start vert
	if (visitedVertNum == 0 && curVert < vertexNum - 1) { 
		printf("%i ", curVert + 1);
		HasHamiltonPath(curVert + 1);
	}
}
void StartHanilton() {
	if (checkMatr() == -1) {
		visitedVertNum = -1;
		return;
	}
	HasHamiltonPath(0);
}
//int Hamiltonian_path()
//{
//	//int dp[vertexNum][(1 << vertexNum)];
//	int** dp = malloc(vertexNum * sizeof(int*));
//	for (int i = 0; i < vertexNum; i++){
//		dp[i] = malloc((1 << vertexNum) * sizeof(int));
//		//dp[i] = calloc(pow(2,15), sizeof(int));
//		for (int k = 0; k < (1 << vertexNum); k++)
//		{
//			dp[i][k] = 0;
//		}
//	}
//	// Initialize the table
//	//memset(dp, 0, sizeof(dp));
//
//	// Set all dp[i][(1 << i)] to
//	// true
//	for (int i = 0; i < vertexNum; i++)
//		dp[i][(1 << i)] = 1;
//
//	// Iterate over each subset
//	// of nodes
//	for (int i = 0; i < (1 << vertexNum); i++) {
//
//		for (int j = 0; j < vertexNum; j++) {
//
//			// If the jth nodes is included
//			// in the current subset
//			if (i & (1 << j)) {
//
//				// Find K, neighbour of j
//				// also present in the
//				// current subset
//				for (int k = 0; k < vertexNum; k++) {
//
//					if (i & (1 << k)
//						&& adjMatr[k][j]
//						&& j != k
//						&& dp[k][i ^ (1 << j)]) {
//
//						// Update dp[j][i]
//						// to true
//						dp[j][i] = 1;
//						break;
//					}
//				}
//			}
//		}
//	}
//
//	// Traverse the vertices
//	for (int i = 0; i < vertexNum; i++) {
//
//		// Hamiltonian Path exists
//		if (dp[i][(1 << vertexNum) - 1])
//			return 1;
//	}
//
//	// Otherwise, return false
//	return 0;
//}
int main() {
	//CreateRndHamiltonPath();
	FILE* input = fopen("input.txt", "r");
	ReadAdjacencyList(input);
	fclose(input);
	visited = calloc(vertexNum, sizeof(int));
	visitOrder = StackInit(vertexNum);

	for (int i = 0; i < vertexNum; i++){
		for (int k = 0; k < vertexNum; k++) {
			printf("%i ", adjMatr[i][k]);
		}
		puts("");
	}

	//method
	//HasHamiltonPath(0);
	StartHanilton();
	/*if (Hamiltonian_path()) {
		puts("yes");
	}
	else {
		puts("no");
	}*/

	FILE* output = fopen("output.txt", "w");
	puts("");
	if (visitedVertNum == 0) {
		printf("%i \n", 0);
		fprintf(output, "%i", 0);
	}
	else if (visitedVertNum == -1) {
		puts("lel");
	}
	else {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i ", visitOrder->arr[k] + 1);
			fprintf(output, "%i ", visitOrder->arr[k] + 1);
		}
	}
	fclose(output);
	return 0;
}
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
		for (int j = 0; j < TEST_VERTEX_NUM; j++){
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

	for (int i = 0; i < TEST_VERTEX_NUM; i++){
		free(adjMatr[i]);
	}
	free(adjMatr);
	fclose(file);
}