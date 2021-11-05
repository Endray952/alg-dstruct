#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define error_check if(error != NO_E)  return
#define queueInit _queueInit(); error_check
#define createList _createList(); error_check
#define listAdd(list, data) _listAdd(list, data); error_check
#define queuePush(queue, vertex) _queuePush(queue, vertex); error_check
#define queueGet(queue) _queueGet(queue); error_check
#define STRESS_TEST 0
#define TEST_VERTEX_NUM 4000
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
    elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart; 

typedef enum {
	NO_E,
	MALLOC_E,
	EMPTY_LIST,
	CREATE_LIST_E,
	CREATE_QUEUE_E,
	EMPTY_QUEUE_GET_E
}error_t;
error_t error = NO_E;
void BFSexec(int** adjMatr, int vertexNum);
void createTestGraph();
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

int** ReadAdjacencyList(int* _vertexNum, FILE* file) {
	int vertexNum = 0;
	if (fscanf(file, "%i", &vertexNum) != 1) {
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
		c = fgetc(file);
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
	TIMER_INIT
	TIMER_START
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
	TIMER_STOP
	if (STRESS_TEST) {
		//fprintf(stdout, "%lf ", elapsedTime);
	}
	else {
		for (node_t* node = visitedArr->start; node != NULL; node = node->next) {
			fprintf(stdout, "%d ", node->vertex);
		}
	}
}
/**
 * CPU: Intel(R) Core(TM) i7-6700K CPU 4.10GHz
 * RAM: 32Gb DDR4 2133 MHz 
 * SSD: SATA 3 read speed 545Mb/s Write Speed 465Mb/s
 *
 * Vertecies amount: 4000
 *
 * Stress Test results:
 *     Memory used: 64.3 MB
 *
 *	   BFS execution time: 82.95 seconds
 */
int main() {
	if (!STRESS_TEST) {
		int* vertexNum = malloc(sizeof(int));
		if (vertexNum == NULL) {
			error = MALLOC_E;
			return;
		}
		int** adjMatr = ReadAdjacencyList(vertexNum, stdin);
		error_check;
		BFSexec(adjMatr, *vertexNum);
		error_check;
	}
	else {
		createTestGraph();
		int* vertexNum = malloc(sizeof(int));
		if (vertexNum == NULL) {
			error = MALLOC_E;
			return;
		}
		FILE* file = fopen("test.txt", "r");
		int** adjMatr = ReadAdjacencyList(vertexNum, file);
		error_check;
		BFSexec(adjMatr, *vertexNum);
		error_check;
		fclose(file);
	}
	return 0;
}
void createTestGraph() {
	srand(time(NULL));
	FILE* file = fopen("test.txt", "w");
	fprintf(file, "%i\n", TEST_VERTEX_NUM);
	for (int i = 0; i < TEST_VERTEX_NUM; i++) {
		fprintf(file, "%i ", i);
		for (int k = i + 1; k < TEST_VERTEX_NUM; k++) {
			if ((rand() % 2) == 1) {
				fprintf(file, "%i ", k);
			}

		}
		fprintf(file, "\n");
	}
	fclose(file);
}