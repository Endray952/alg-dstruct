#include "list.h"
#include <stdio.h>
list_t* listInitialize() {
	list_t* list = (list_t*)malloc(sizeof(list_t));
	list->head = NULL;
	return list;
}
void AddNodeBefore(list_t* list, node_t* nodeInList, node_t* nodeAddsBefore) {
	if (nodeInList->prev == NULL) {
		nodeAddsBefore->next = nodeInList;
		nodeAddsBefore->prev = NULL;
		list->head = nodeAddsBefore;
		nodeInList->prev = nodeAddsBefore;
	}
	else {
		nodeInList->prev->next = nodeAddsBefore;
		nodeAddsBefore->prev = nodeInList->prev;
		nodeAddsBefore->next = nodeInList;
		nodeInList->prev = nodeAddsBefore;
	}
}
void AddNodeInEmptyList(list_t* list, node_t* node) {
	list->head = node;
	node->next = NULL;
	node->prev = NULL;
}
void AddNodeAfter(list_t* list, node_t* nodeInList, node_t* nodeAddsAfter) {
	nodeAddsAfter->next = nodeInList->next;
	if (nodeInList->next != NULL) {
		nodeInList->next->prev = nodeAddsAfter;
	}
	nodeAddsAfter->prev = nodeInList;
	nodeInList->next = nodeAddsAfter;
}
void AddNodeSorting(list_t* list, date_t* date, int temperature) {
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	newNode->date = *date;
	newNode->temperature = temperature;
	if (list->head == NULL) {
		AddNodeInEmptyList(list, newNode);
	}
	else {
		node_t* iterationNode = list->head;
		while (1) {
			if (newNode->temperature < iterationNode->temperature) {
				AddNodeBefore(list, iterationNode, newNode);
				return;
			}
			else if (newNode->temperature == iterationNode->temperature) {
				if ((newNode->date.year < iterationNode->date.year) ||
					(newNode->date.year == iterationNode->date.year && newNode->date.month < iterationNode->date.month) ||
					(newNode->date.year == iterationNode->date.year && newNode->date.month == iterationNode->date.month && newNode->date.day < iterationNode->date.day)) {
					AddNodeBefore(list, iterationNode, newNode);
				}
				else {
					AddNodeAfter(list, iterationNode, newNode);
				}
				return;
			}
			if (iterationNode->next != NULL) {
				iterationNode = iterationNode->next;
			}
			else {
				break;
			}
		}
		AddNodeAfter(list, iterationNode, newNode);
	}
}
void PrintList(list_t* list) {
	if (list->head != NULL) {
		node_t* node = list->head;
		while (node != NULL) {
			printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
			node = node->next;
		}
	}
	else {
		puts("Empty");
	}

}
void PrintListTempLower0(list_t* list) {
	if (list->head != NULL) {
		node_t* node = list->head;
		int noSuchElem = 0;
		while (node != NULL) {
			if (node->temperature < 0) {
				printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
				noSuchElem = 1;
			}
			node = node->next;
		}
		if (noSuchElem == 0) {
			puts("No such element in list");
		}
	}
	else {
		puts("Empty list");
	}
}
void CheckTemperature(list_t* list, int temperature) {
	if (list->head != NULL) {
		node_t* node = list->head;
		int noSuchElem = 0;
		while (node != NULL) {
			if (node->temperature == temperature) {
				printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
				noSuchElem = 1;
			}
			node = node->next;
		}
		if (noSuchElem == 0) {
			puts("No such element in list");
		}
	}
	else {
		puts("Empty list");
	}
}