#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define error_check if(error != NO_ERROR)  return NULL
#define queueInit _queueInit(); error_check
#define createList _createList(); error_check
#define listAdd(list, data) _listAdd(list, data); error_check
#define queuePush(queue, vertex) _queuePush(queue, vertex); error_check
#define queueGet(queue) _queueGet(queue); error_check
typedef enum {
	NO_ERROR,
	MALLOC_E,
	EMPTY_LIST,
	CREATE_LIST_E,
	CREATE_QUEUE_E,
	EMPTY_QUEUE_GET_E
}error_t;
error_t error = NO_ERROR;
void BFSexec(int** adjMatr, int vertexNum);
typedef struct node_t {
	struct node_t* next;
	int vertex;
}node_t;
typedef struct {
	struct node_t* head;
	struct node_t* tail;
}queue_t;
typedef struct {
	struct node_t* start;
}list_t;
queue_t* _queueInit() {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	if (queue == NULL) {
		error = CREATE_QUEUE_E;
		return NULL;
	}
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}
list_t* _createList() {
	list_t* list = malloc(sizeof(list_t));
	if (list == NULL) {
		error = CREATE_LIST_E;
		return NULL;
	}
	list->start = NULL;
	return list;
}
void _listAdd(list_t* list, int data) {
	node_t* newNode = malloc(sizeof(node_t));
	if (newNode == NULL) {
		error = MALLOC_E;
		return;
	}
	if (list->start == NULL) {
		list->start = newNode;
	}
	else {
		node_t* node;
		for (node = list->start; node->next != NULL; node = node->next);
		node->next = newNode;
	}
	newNode->vertex = data;
	newNode->next = NULL;
	
}
void _queuePush(queue_t* queue, int vertex) {
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	if (newNode == NULL) {
		error = MALLOC_E;
		return;
	}
	newNode->next = NULL;
	newNode->vertex = vertex;
	if (queue->head != NULL) {
		queue->head->next = newNode;
	}
	queue->head = newNode;
	if (queue->tail == NULL) {
		queue->tail = newNode;
	}
}
int _queueGet(queue_t* queue) {
	if (queue->tail == NULL) {
		error = EMPTY_QUEUE_GET_E;
		return;
	}
	int elem = queue->tail->vertex;
	if (queue->tail == queue->head) {
		queue->head = NULL;
	}
	node_t* tail = queue->tail;
	queue->tail = queue->tail->next;
	free(tail);
	return elem;
}
int queueIsEmpty(queue_t* queue) {
	if (queue->head == NULL && queue->tail == NULL) {
		return 1;
	}
	return 0;
}
int notContain(list_t* list, int elem) {
	for (node_t* node = list->start; node != NULL; node = node->next) {
		if (node->vertex == elem) {
			return 0;
		}
	}
	return 1;
}

int** ReadAdjacencyList(int* _vertexNum) {
	int vertexNum = 0;
	if (fscanf(stdin, "%i", &vertexNum) != 1) {
		error = EMPTY_LIST;
		return NULL;
	}
	int** adjMatr = (int**)calloc(vertexNum, sizeof(int*));
	if (adjMatr == NULL) {
		error = MALLOC_E;
		return NULL;
	}
	for (int i = 0; i < vertexNum; i++) {
		adjMatr[i] = (int*)calloc(vertexNum, sizeof(int));
		if (adjMatr[i] == NULL) {
			error = MALLOC_E;
			return NULL;
		}
	}
	char c;
	char str[10];
	int vertexWasRead = 0;
	int curVertex = 0;
	int newLineNum = 0;
	while (newLineNum <= vertexNum) {
		c = fgetc(stdin);
		if (c == '\n') {
			if (vertexWasRead != 0) { 
				int vertex = atoi(str);
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
				curVertex = atoi(str);
				memset(str, '\0', sizeof(str));
			}
			else  {
				int vertex = atoi(str);
				adjMatr[curVertex][vertex] = 1;
				adjMatr[vertex][curVertex] = 1;
				memset(str, '\0', sizeof(str));
			}
		}
	}
	*_vertexNum = vertexNum;
	return adjMatr;
}

void BFSexec(int** adjMatr, int vertexNum) {
	list_t* visitedArr = createList;
	queue_t* toExplore = queueInit;
	queuePush(toExplore, 0);
	listAdd(visitedArr, 0);
	while (!queueIsEmpty(toExplore))
	{
		int elem = queueGet(toExplore);				
		for (int i = 0; i < vertexNum; i++)
		{
			if (adjMatr[elem][i] == 1 && notContain(visitedArr, i))
			{
				queuePush(toExplore, i);
				listAdd(visitedArr, i);
			}
		}	
	}

	for (node_t* node = visitedArr->start; node != NULL; node = node->next) {
		fprintf(stdout, "%d ", node->vertex);
	}	
}

int main() {
	int* vertexNum = malloc(sizeof(int));
	if (vertexNum == NULL) {
		error = MALLOC_E;
		return;
	}
	int** adjMatr = ReadAdjacencyList(vertexNum);
	error_check;
	BFSexec(adjMatr, *vertexNum);
	error_check;
	return 0;
}
