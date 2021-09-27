#pragma once
typedef struct {
	int day;
	int month;
	int year;
} date_t;
typedef struct node_t {
	struct node_t* next;
	struct node_t* prev;
	int temperature;
	date_t date;
} node_t;
typedef struct {
	node_t* head;
} list_t;
list_t* ListInitialize();
void AddNodeBefore(list_t* list, node_t* node_in_list, node_t* node_adds_before);
void AddNodeInEmptyList(list_t* list, node_t* node);
void AddNodeAfter(list_t* list, node_t* node_in_list, node_t* node_adds_after);
int AddNodeSorting(list_t* list, date_t* date, int temperature);
void PrintListTempLower0(list_t* list);
void CheckTemperature(list_t* list, int temperature);