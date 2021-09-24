#include "list.h"
#include <stdio.h>
list_t* listInitialize() {
	list_t* list = malloc(sizeof(list_t));
	list->head = NULL;
	return list;
}
void AddNodeBefore(list_t* list, node_t* node_in_list, node_t* node_adds_before) {
	if (node_in_list->prev == NULL) {
		node_adds_before->next = node_in_list;
		node_adds_before->prev = NULL;
		list->head = node_adds_before;
	}
	else {
		node_in_list->prev->next = node_adds_before;
		node_adds_before->prev = node_in_list->prev;
		node_adds_before->next = node_in_list;
		node_in_list->prev = node_adds_before;
	}
}
void AddNodeInEmptyList(list_t* list, node_t* node) {
	list->head = node;
	node->next = NULL;
	node->prev = NULL;
}
void AddNodeAfter(list_t* list, node_t* node_in_list, node_t* node_adds_after) {
	node_adds_after->next = node_in_list->next;
	if (node_in_list->next != NULL) {
		node_in_list->next->prev = node_adds_after;
	}
	node_adds_after->prev = node_in_list;
	node_in_list->next = node_adds_after;
}
void AddNodeSorting(list_t* list, date_t* date, int temperature) {
	node_t* newNode = malloc(sizeof(node_t));
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