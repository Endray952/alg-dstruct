#include "stdio.h"
#include "list.h"
//typedef struct {
//	int day;
//	int month;
//	int year;
//} Date;
//typedef struct Node_t {
//	struct Node_t* next;
//	struct Node_t* prev;
//	int temperature;
//	Date date;
//} Node;
//typedef struct List_t {
//	Node* head;
//} List;
//
//List* listInitialize() {
//	List* list = malloc(sizeof(List));
//	list->head = NULL;
//	return list;
//}
//void addNode(List* list, Date date, int temperature) {
//	Node* node = malloc(sizeof(Node));
//	node->date = date;
//	node->temperature = temperature;
//	if (list->head == NULL) {
//		list->head = node;
//		node->next = NULL;
//		node->prev = NULL;
//	}
//	else {
//		Node* tmp_node = list->head;
//		while(1) {
//			if (node->temperature < tmp_node->temperature) {
//				if (tmp_node->prev == NULL) {
//					node->next = tmp_node;
//					node->prev = NULL;
//					list->head = node;
//				}
//				else {
//					tmp_node->prev->next = node;
//					node->prev = tmp_node->prev;
//					node->next = tmp_node;
//					tmp_node->prev = node;
//				}
//				return;
//			}
//			else if (node->temperature == tmp_node->temperature) {
//				if ((node->date.year < tmp_node->date.year) ||
//					(node->date.year == tmp_node->date.year && node->date.month < tmp_node->date.month) ||
//					(node->date.year == tmp_node->date.year && node->date.month == tmp_node->date.month && node->date.day < tmp_node->date.day)) {			
//					if (tmp_node->prev == NULL) {
//						node->next = tmp_node;
//						node->prev = NULL;
//						list->head = node;
//					}
//					else {
//						tmp_node->prev->next = node;
//						node->prev = tmp_node->prev;
//						node->next = tmp_node;
//						tmp_node->prev = node;
//					}
//				}
//				else {
//					node->next = tmp_node->next;
//					if (tmp_node->next != NULL) {
//						tmp_node->next->prev = node;
//					}
//					node->prev = tmp_node;
//					tmp_node->next = node;
//					
//				}
//				return;
//			}
//			if (tmp_node->next != NULL) {
//				tmp_node = tmp_node->next;
//			}
//			else {
//				break;
//			}
//		} 
//		tmp_node->next = node;
//		node->next = NULL;
//		node->prev = tmp_node;
//	}
//}
//void printList(List* list) {
//	if (list->head != NULL) {
//		Node* node = list->head;
//		while (node != NULL) {
//			printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
//			node = node->next;
//		}
//	}
//	else {
//		puts("Empty");
//	}
//
//}
//void printListLT0(List* list) {
//	if (list->head != NULL) {
//		Node* node = list->head;
//		while (node != NULL) {
//			if (node->temperature < 0) {
//				printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
//			}
//			node = node->next;
//		}
//	}
//	else {
//		puts("Empty");
//	}
//}
void readFile(List* list) {
	FILE* file = fopen("data.txt", "r");
	Date date;
	int temperature ;
	while (fscanf(file, "%i.%i.%i;%i", &date.day, &date.month, &date.year, &temperature ) == 4) {
		addNodeSorting(list, date, temperature);
	}

	fclose(file);
}

//void checkTemperature(List* list, int temperature) {
//	if (list->head != NULL) {
//		Node* node = list->head;
//		while (node != NULL) {
//			if (node->temperature == temperature) {
//				printf("%02i.%02i.%i\t%iC\n", node->date.day, node->date.month, node->date.year, node->temperature);
//			}
//			node = node->next;
//		}
//	}
//	else {
//		puts("Empty list");
//	}
//}

int main() {
	List* list_of_temperatures = listInitialize();
	readFile(list_of_temperatures);
	printList(list_of_temperatures);
	puts("");
	printListLT0(list_of_temperatures);
	puts("");
	int find_temperature;
	scanf("%i", &find_temperature);
	checkTemperature(list_of_temperatures, find_temperature);
	return 0;
}