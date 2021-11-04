#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define HandleError(error) _handle_error(error); return 0
typedef enum {
	NO_ERROR,
	MALLOC_ERROR,
	SCANF_ERROR,
	EMPTY_LIST
}error_t;
error_t BFSexec(int** adjMatr, int vertexNum);

typedef struct node_t {
	struct node_t* next;
	int vertex;
}node_t;
typedef struct {
	struct node_t* head;
	struct node_t* tail;
}queue_t;
queue_t* queueInit() {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}
void queuePush(queue_t* queue, int vertex) {
	node_t* node = (node_t*)malloc(sizeof(node_t));
	node->next = NULL;
	node->vertex = vertex;
	if (queue->head != NULL) {
		queue->head->next = node;
	}
	queue->head = node;
	if (queue->tail == NULL) {
		queue->tail = node;
	}
}
void queuePop(queue_t* queue) {
	if (queue->tail == queue->head) {
		queue->head = NULL;
	}
	node_t* tail = queue->tail;
	queue->tail = queue->tail->next;
	free(tail);
}
void _handle_error(error_t error) {
	if (error == SCANF_ERROR) {
		puts("SCANF_ERROR");
	}
	else if (error == EMPTY_LIST) {
		puts("EMPTY_LIST");
	}
}

error_t ReadAdjacencyList() {

	//FILE* file = fopen("test.txt", "r");
	int vertexNum = 0;
	if (fscanf(stdin, "%i", &vertexNum) == 0) {
		return SCANF_ERROR;
	}
	else if (vertexNum == 0) {
		return EMPTY_LIST;
	}
	int** adjMatr = (int**)calloc(vertexNum, sizeof(int*));
	for (int i = 0; i < vertexNum; i++) {
		adjMatr[i] = (int*)calloc(vertexNum, sizeof(int));
	}

	//printf("%i\n", vertexNum);

	char c;
	char str[10];
	int it = 0;//step of reading numner in one line
	int curVertex;
	int count = 0;
	//while (fscanf(stdin, "%c", &c) == 1) {
	while (count < vertexNum) {
		c = fgetc(stdin);
		if (c == '\n') {
			if (it != 0) {
				int vertex = atoi(str);
				adjMatr[curVertex][vertex] = 1;
				adjMatr[vertex][curVertex] = 1;
			}
			it = 0;
			strcpy(str, "");
			count++;
		}
		else if (isdigit(c)) {
			char cToStr[2];
			cToStr[0] = c;
			cToStr[1] = '\0';
			strcat(str, cToStr);
		}
		else {
			if (it == 0) {
				it++;
				curVertex = atoi(str);
				strcpy(str, "");
			}
			else {
				int vertex = atoi(str);
				adjMatr[curVertex][vertex] = 1;
				adjMatr[vertex][curVertex] = 1;
				strcpy(str, "");
			}
		}
	}
	/*for (int i = 0; i < vertexNum; i++) {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i  ", adjMatr[i][k]);
		}puts("");
	}*/
	BFSexec(adjMatr, vertexNum);
	//printf("%i", vertexNum);
	return NO_ERROR;
}
int notContain(int* arr, int size, int elem) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == elem) {
			return 0;
		}
	}
	return 1;
}
error_t BFSexec(int** adjMatr, int vertexNum) {
	/*for (int i = 0; i < vertexNum; i++) {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i  ", adjMatr[i][k]);
		}puts("");
	}*/
	int* visitedArr = (int*)malloc(vertexNum * sizeof(int));
	int visitedPos = 0;
	queue_t* toExplore = queueInit();
	//visitedArr[visitedPos] = 0;
	while (visitedPos < vertexNum) {
		if (visitedPos == 0) {
			queuePush(toExplore, 0);
			visitedArr[visitedPos] = 0;
		}
		else {
			int i = 1;
			while (!notContain(visitedArr, visitedPos + 1, i)) {
				i++;
			}
			queuePush(toExplore, i);
			visitedArr[visitedPos] = i;
		}
		visitedPos++;
		while (toExplore->head != NULL) {
			for (int intersecElem = 0; intersecElem < vertexNum; intersecElem++) {
				int elem = adjMatr[toExplore->tail->vertex][intersecElem];
				int curVert = toExplore->tail->vertex;
				if (adjMatr[toExplore->tail->vertex][intersecElem] == 1 && notContain(visitedArr, visitedPos + 1, intersecElem)) {
					queuePush(toExplore, intersecElem);
					int b = toExplore->head->vertex;
					visitedArr[visitedPos] = toExplore->head->vertex;
					visitedPos++;
				}
			}
			/*for (int i = 0; i < visitedPos; i++) {
				printf("%i ", visitedArr[i]);
			}*/
			/*visitedArr[visitedPos] = toExplore->tail->vertex;
			visitedPos++;*/
			queuePop(toExplore);
		}
	}



	for (int i = 0; i < visitedPos; i++) {
		fprintf(stdout,"%i", visitedArr[i]);
		if (i != visitedPos - 1) {
			fprintf(stdout, " ");
		}
	}
	return NO_ERROR;
}
int main() {

	error_t res = ReadAdjacencyList();
	/*if (res != NO_ERROR) {
		HandleError(res);
	}*/
	
	return 0;
}
