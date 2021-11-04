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


void ReadAdjacencyList() {

	//FILE* file = fopen("test.txt", "r");
	int vertexNum = 0;
	fscanf(stdin, "%i", &vertexNum);

	int** adjMatr = (int**)calloc(vertexNum, sizeof(int*));
	for (int i = 0; i < vertexNum; i++) {
		adjMatr[i] = (int*)calloc(vertexNum, sizeof(int));
	}

	//printf("%i\n", vertexNum);

	char c;
	char str[10];
	int it = 0;//step of reading numner in one line
	int curVertex;
	int newLineNum = 0;
	//while (fscanf(stdin, "%c", &c) == 1) {
	while (newLineNum <= vertexNum) {
		c = fgetc(stdin);
		if (c == '\n') {
			if (it != 0) {
				int vertex = atoi(str);
				adjMatr[curVertex][vertex] = 1;
				adjMatr[vertex][curVertex] = 1;
			}
			it = 0;
			strcpy(str, "");
			newLineNum++;
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
	BFSexec(adjMatr, vertexNum);
	
	
}
int notContain(int* arr, int size, int elem) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == elem) {
			return 0;
		}
	}
	return 1;
}
void BFSexec(int** adjMatr, int vertexNum) {
	for (int i = 0; i < vertexNum; i++) {
		for (int k = 0; k < vertexNum; k++) {
			printf("%i  ", adjMatr[i][k]);
		}puts("");
	}


	int* visitedArr = (int*)malloc(vertexNum * sizeof(int));
	int visitedPos = 0;
	queue_t* toExplore = queueInit();
	queuePush(toExplore, 0);
	while (toExplore->head != NULL)
	{
		int el = toExplore->tail->vertex;
		queuePop(toExplore);
		if (notContain(visitedArr, visitedPos + 1, el)) {
			visitedArr[visitedPos] = el;
			visitedPos++;
			//printf("%i ", visitedArr[visitedPos]);
		}
		
		for (int i = 0; i < vertexNum; ++i)
		{
			if (adjMatr[el][i] == 1 && notContain(visitedArr, visitedPos + 1, i))
			{

				queuePush(toExplore, i);
			}
		}
		//visitedPos++;
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



	for (int i = 0; i < vertexNum; i++) {
		fprintf(stdout,"%d ", visitedArr[i]);
	}
	
}
int main() {

	ReadAdjacencyList();
	
	return 0;
}
