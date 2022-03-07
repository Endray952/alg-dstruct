#pragma once
#define T 2 
#define MIN (T - 1)
#define MAX (2 * T - 1) 
#define MAX_CHILDREN (2 * T)

typedef enum KeyStatus {
	SEARCH_FAIL, SUCCESS, LESS_KEYS,
} KeyStatus;

typedef struct B_node {
	struct B_node* children[MAX_CHILDREN];
	int keys[MAX];
	int keys_num;
}B_node;

void DeleteNode(int key);
void InsertNode(int data);
int Search(int data);
void PrintTree(int y);