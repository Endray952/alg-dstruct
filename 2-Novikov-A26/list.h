#pragma once
typedef struct {
	int day;
	int month;
	int year;
} Date;
typedef struct Node_t {
	struct Node_t* next;
	struct Node_t* prev;
	int temperature;
	Date date;
} Node;
typedef struct List_t {
	Node* head;
} List;
List* listInitialize();
void addNodeBefore(List* list, Node* node_in_list, Node* node_adds_before);
void addNodeInEmptyList(List* list, Node* node);
void addNodeAfter(List* list, Node* node_in_list, Node* node_adds_after);
void addNodeSorting(List* list, Date date, int temperature);
void printList(List* list);
void printListLT0(List* list);
void checkTemperature(List* list, int temperature);