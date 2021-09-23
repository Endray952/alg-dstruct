#include "list.h"
#include <stdio.h>
List* listInitialize() {
	List* list = malloc(sizeof(List));
	list->head = NULL;
	return list;
}
void addNodeBefore(List* list, Node* node_in_list, Node* node_adds_before) {
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
void addNodeInEmptyList(List* list, Node* node) {
	list->head = node;
	node->next = NULL;
	node->prev = NULL;
}
void addNodeAfter(List* list, Node* node_in_list, Node* node_adds_after) {
	node_adds_after->next = node_in_list->next;
	if (node_in_list->next != NULL) {
		node_in_list->next->prev = node_adds_after;
	}
	node_adds_after->prev = node_in_list;
	node_in_list->next = node_adds_after;
}
void addNodeSorting(List* list, Date* date, int temperature) {
	Node* node = malloc(sizeof(Node));
	node->date = *date;
	node->temperature = temperature;
	if (list->head == NULL) {
		addNodeInEmptyList(list, node);
	}
	else {
		Node* tmp_node = list->head;
		while (1) {
			if (node->temperature < tmp_node->temperature) {
				addNodeBefore(list, tmp_node, node);
				return;
			}
			else if (node->temperature == tmp_node->temperature) {
				if ((node->date.year < tmp_node->date.year) ||
					(node->date.year == tmp_node->date.year && node->date.month < tmp_node->date.month) ||
					(node->date.year == tmp_node->date.year && node->date.month == tmp_node->date.month && node->date.day < tmp_node->date.day)) {
					addNodeBefore(list, tmp_node, node);
				}
				else {
					addNodeAfter(list, tmp_node, node);
				}
				return;
			}
			if (tmp_node->next != NULL) {
				tmp_node = tmp_node->next;
			}
			else {
				break;
			}
		}
		addNodeAfter(list, tmp_node, node);
	}
}
void printList(List* list) {
	if (list->head != NULL) {
		Node* node = list->head;
		while (node != NULL) {
			printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
			node = node->next;
		}
	}
	else {
		puts("Empty");
	}

}
void printListLT0(List* list) {
	if (list->head != NULL) {
		Node* node = list->head;
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
void checkTemperature(List* list, int temperature) {
	if (list->head != NULL) {
		Node* node = list->head;
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