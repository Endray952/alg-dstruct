#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
queue_t* queueInit() {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}
list_t* createList() {
	list_t* list = malloc(sizeof(list_t));
	list->start = NULL;
	return list;
}
void listAdd(list_t* list, int data) {
	node_t* newNode = malloc(sizeof(node_t));
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
//void queuePop(queue_t* queue) {
//	if (queue->tail == queue->head) {
//		queue->head = NULL;
//	}
//	node_t* tail = queue->tail;
//	queue->tail = queue->tail->next;
//	free(tail);
//}
int queueGet(queue_t* queue) {
	if (queue->tail == NULL) {
		return -1;
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


//int** ReadAdjacencyList(int* _vertexNum) {
//
//	//FILE* file = fopen("test.txt", "r");
//	int vertexNum = 0;
//	fscanf(stdin, "%i", &vertexNum);
//
//	int** adjMatr = (int**)calloc(vertexNum, sizeof(int*));
//	for (int i = 0; i < vertexNum; i++) {
//		adjMatr[i] = (int*)calloc(vertexNum, sizeof(int));
//	}
//
//	//printf("%i\n", vertexNum);
//
//	char c;
//	char str[10];
//	int it = 0;//step of reading numner in one line
//	int curVertex;
//	int newLineNum = 0;
//	//while (fscanf(stdin, "%c", &c) == 1) {
//	while (newLineNum <= vertexNum) {
//		c = fgetc(stdin);
//		if (c == '\n') {
//			if (it != 0) {
//				int vertex = atoi(str);
//				adjMatr[curVertex][vertex] = 1;
//				adjMatr[vertex][curVertex] = 1;
//			}
//			it = 0;
//			strcpy(str, "");
//			newLineNum++;
//		}
//		else if (isdigit(c)) {
//			char cToStr[2];
//			cToStr[0] = c;
//			cToStr[1] = '\0';
//			strcat(str, cToStr);
//		}
//		else {
//			if (it == 0) {
//				it++;
//				curVertex = atoi(str);
//				strcpy(str, "");
//			}
//			else {
//				int vertex = atoi(str);
//				adjMatr[curVertex][vertex] = 1;
//				adjMatr[vertex][curVertex] = 1;
//				strcpy(str, "");
//			}
//		}
//	}
//
//
//	*_vertexNum = vertexNum;
//	return(adjMatr);
//	//BFSexec(adjMatr, vertexNum);
//	
//	
//}
//int notContain(int* arr, int size, int elem) {
//	for (int i = 0; i < size; i++) {
//		if (arr[i] == elem) {
//			return 0;
//		}
//	}
//	return 1;
//}
#define TRUE 1
#define FALSE 0
char** grafMatrix = NULL;
int* way = NULL;
char* check = NULL;
int nPoints = 0;

void FillInMatrix(FILE* flow);

int SystemInit(FILE* flow)
{
	fscanf(flow, "%d", &nPoints);

	if (nPoints == 0)
	{
		return 1;
	}

	grafMatrix = (char**)calloc(nPoints, sizeof(char*));
	way = (int*)calloc(nPoints, sizeof(int));
	check = (char*)calloc(nPoints, sizeof(char));

	if ((grafMatrix == NULL) || (way == NULL) || (check == NULL))
	{
		return 2;
	}

	way[0] = 0;
	check[0] = TRUE;

	for (int i = 0; i < nPoints; i++)
	{
		grafMatrix[i] = (char*)calloc(nPoints, sizeof(char));

		if (grafMatrix[i] == NULL)
		{
			return 2;
		}
	}

	FillInMatrix(flow);

	return 0;
}

void FillInMatrix(FILE* flow)
{
	int count = -1;
	int curVert = -1;
	int i = 0;
	char number[6] = { 0 };

	while (count < nPoints)
	{
		char sym = fgetc(flow);

		if ((sym == ' ') || (sym == '\n'))
		{
			if (i != 0)
			{
				number[i] = '\0';

				if (curVert == -1)
				{
					curVert = atoi(number);
				}
				else
				{
					int vertex = atoi(number);
					grafMatrix[curVert][vertex] = TRUE;
					grafMatrix[vertex][curVert] = TRUE;
				}

			}

			if (sym == '\n')
			{
				count++;
				curVert = -1;
			}
			memset(number, '\0', sizeof(number));
			i = 0;
		}
		if ((sym != ' ') && (sym != '\n'))
		{
			number[i] = sym;
			i++;
		}
	}
	
}




int notContain(list_t* list, int elem) {
	for (node_t* node = list->start; node!= NULL ; node = node->next) {
		if (node->vertex == elem) {
			return 0;
		}
	}
	return 1;
}
void BFSexec(int** adjMatr, int vertexNum) {
	/*for (int i = 0; i < vertexNum; i++) {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i  ", adjMatr[i][k]);
		}puts("");
	}*/
	/*for (int i = 0; i < nPoints; i++) {
		for (int k = 0; k < nPoints; k++) {
			printf("%i  ", grafMatrix[i][k]);
		}puts("");
	}*/

	//int* visitedArr = (int*)malloc(vertexNum * sizeof(int));
	//int visitedPos = 0;
	list_t* visitedArr = createList();
	queue_t* toExplore = queueInit();
	queuePush(toExplore, 0);
	listAdd(visitedArr, 0);
	while (!queueIsEmpty(toExplore))
	{
		int elem = queueGet(toExplore);				
		for (int i = 0; i < vertexNum; ++i)
		{
			if (grafMatrix[elem][i] == 1 && notContain(visitedArr, i))
			{
				queuePush(toExplore, i);
				listAdd(visitedArr, i);
			}
		}	
	}



	/*visitedArr[0] = 0;
	visitedPos++;*/
	//for (int i = 0; i < vertexNum; i++){
	//	if (notContain(visitedArr, visitedPos, i)) {
	//		visitedArr[visitedPos] = i;
	//		visitedPos++;
	//	}
	//	for (int k = i; k < vertexNum; k++){
	//		if (adjMatr[i][k] == 1 && notContain(visitedArr, visitedPos, k)) {
	//			visitedArr[visitedPos] = k;
	//			visitedPos++;
	//		}
	//		//visitedArr[i]
	//	}

	//}
	//int deadNum = 0;
	//while (visitedPos < vertexNum)
	//{
	//	for (int i = 0; i < vertexNum; i++)
	//	{
	//		if (adjMatr[visitedArr[deadNum]][i] == 1)
	//		{
	//			if (notContain(visitedArr, visitedPos + 1, i));
	//			{
	//				//check[i] = TRUE;
	//				visitedArr[visitedPos] = i;
	//				visitedPos++;
	//			}
	//		}
	//	}
	//	deadNum++;
	//}



	//while (visitedPos < vertexNum) {
	//	if (visitedPos == 0) {
	//		queuePush(toExplore, 0);
	//		//visitedArr[visitedPos] = 0;
	//	}
	//	else {
	//		int i = 1;
	//		while (!notContain(visitedArr, visitedPos + 1, i)) {
	//			i++;
	//		}
	//		queuePush(toExplore, i);
	//		visitedArr[visitedPos] = i;
	//	}
	//	//visitedPos++;
	//	while (toExplore->head != NULL) {
	//		visitedArr[visitedPos] = toExplore->tail->vertex;
	//		visitedPos++;
	//		for (int intersecElem = 0; intersecElem < vertexNum; intersecElem++) {
	//			//int elem = adjMatr[toExplore->tail->vertex][intersecElem];
	//			//int curVert = toExplore->tail->vertex;
	//			if (adjMatr[toExplore->tail->vertex][intersecElem] == 1 && notContain(visitedArr, visitedPos, intersecElem)) {
	//				queuePush(toExplore, intersecElem);
	//				//int b = toExplore->head->vertex;
	//				/*visitedArr[visitedPos] = toExplore->head->vertex;
	//				visitedPos++;*/
	//			}
	//		}
	//		
	//		queuePop(toExplore);
	//	}
	//}

	for (node_t* node = visitedArr->start; node != NULL; node = node->next) {
		fprintf(stdout, "%d ", node->vertex);
	}

	/*for (int i = 0; i < vertexNum; i++) {
		fprintf(stdout,"%d ", visitedArr[i]);
	}*/
	
}
int main() {
	
	int* vertexNum = malloc(sizeof(int));
	//int** adjMatr = ReadAdjacencyList(vertexNum);
	/*for (int i = 0; i < vertexNum; i++) {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i  ", adjMatr[i][k]);
		}puts("");
	}*/
	//BFSexec(adjMatr, *vertexNum);
	SystemInit(stdin);
	BFSexec(grafMatrix, nPoints);
	return 0;
}
