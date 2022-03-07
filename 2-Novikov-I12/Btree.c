#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "Btree.h"
B_node* root;
static B_node* __allocate_node() {
	B_node* node = malloc(sizeof(B_node));
	if (node != NULL) {
		for (int i = 0; i < MAX_CHILDREN; i++) {
			node->children[i] = NULL;
		}
		node->keys_num = 0;
	}
	return node;
}


static int __search_pos(int key, int* key_arr, int n) {
	int pos = 0;
	while (pos < n && key > key_arr[pos])
		pos++;
	return pos;

}


static KeyStatus __del(B_node* node, int key) {
	int pos, i, pivot, n;
	int* key_arr;
	KeyStatus value;
	B_node** p;
	B_node* lptr;
	B_node* rptr;
	if (node == NULL) {
		return SEARCH_FAIL;
	}
	n = node->keys_num;
	key_arr = node->keys;
	p = node->children;
	pos = __search_pos(key, key_arr, n);
	if (p[0] == NULL) {
		if (pos == n || key < key_arr[pos]) {
			return SEARCH_FAIL;
		}
		for (i = pos + 1; i < n; i++)
		{
			key_arr[i - 1] = key_arr[i];
			p[i] = p[i + 1];
		}
		return --node->keys_num >= (node == root ? 1 : MIN) ? SUCCESS : LESS_KEYS;
	}
	if (pos < n && key == key_arr[pos]) {
		B_node* qp = p[pos], * qp1;
		int nkey;
		while (1) {
			nkey = qp->keys_num;
			qp1 = qp->children[nkey];
			if (qp1 == NULL) {
				break;
			}	
			qp = qp1;
		}
		key_arr[pos] = qp->keys[nkey - 1];
		qp->keys[nkey - 1] = key;
	}
	value = __del(p[pos], key);
	if (value != LESS_KEYS) {
		return value;
	}
	if (pos > 0 && p[pos - 1]->keys_num > MIN) {
		pivot = pos - 1;
		lptr = p[pivot];
		rptr = p[pos];
		rptr->children[rptr->keys_num + 1] = rptr->children[rptr->keys_num];
		for (i = rptr->keys_num; i > 0; i--) {
			rptr->keys[i] = rptr->keys[i - 1];
			rptr->children[i] = rptr->children[i - 1];
		}
		rptr->keys_num++;
		rptr->keys[0] = key_arr[pivot];
		rptr->children[0] = lptr->children[lptr->keys_num];
		key_arr[pivot] = lptr->keys[--lptr->keys_num];
		return SUCCESS;
	}
	if (pos < n && p[pos + 1]->keys_num > MIN) {
		pivot = pos;
		lptr = p[pivot];
		rptr = p[pivot + 1];
		lptr->keys[lptr->keys_num] = key_arr[pivot];
		lptr->children[lptr->keys_num + 1] = rptr->children[0];
		key_arr[pivot] = rptr->keys[0];
		lptr->keys_num++;
		rptr->keys_num--;
		for (i = 0; i < rptr->keys_num; i++) {
			rptr->keys[i] = rptr->keys[i + 1];
			rptr->children[i] = rptr->children[i + 1];
		}
		rptr->children[rptr->keys_num] = rptr->children[rptr->keys_num + 1];
		return SUCCESS;
	}
	if (pos == n) {
		pivot = pos - 1;
	}
	else {
		pivot = pos;
	}
	lptr = p[pivot];
	rptr = p[pivot + 1];
	lptr->keys[lptr->keys_num] = key_arr[pivot];
	lptr->children[lptr->keys_num + 1] = rptr->children[0];
	for (i = 0; i < rptr->keys_num; i++) {
		lptr->keys[lptr->keys_num + 1 + i] = rptr->keys[i];
		lptr->children[lptr->keys_num + 2 + i] = rptr->children[i + 1];
	}
	lptr->keys_num = lptr->keys_num + rptr->keys_num + 1;
	free(rptr);
	for (i = pos + 1; i < n; i++) {
		key_arr[i - 1] = key_arr[i];
		p[i] = p[i + 1];
	}
	return --node->keys_num >= (node == root ? 1 : MIN) ? SUCCESS : LESS_KEYS;
}


void DeleteNode(int key) {
	B_node* uproot;
	KeyStatus value;
	value = __del(root, key);
	if (value == LESS_KEYS) {
		uproot = root;
		root = root->children[0];
		free(uproot);
	}
}


static void __split(B_node* root_node, int index) {
	B_node* right_child = __allocate_node();
	B_node* left_child = root_node->children[index];
	right_child->keys_num = MIN;
	for (int i = 0; i < MIN; i++) {
		right_child->keys[i] = left_child->keys[i + T];
	}
	if (left_child->children[0] != NULL) {
		for (int i = 0; i < T; i++) {
			right_child->children[i] = left_child->children[i + T];
		}
	}
	left_child->keys_num = MIN;
	for (int i = root_node->keys_num; i >= index; i--) {
		root_node->children[i + 1] = root_node->children[i];
	}
	root_node->children[index + 1] = right_child;
	for (int i = root_node->keys_num - 1; i >= index; i--) {
		root_node->keys[i + 1] = root_node->keys[i];
	}
	root_node->keys[index] = left_child->keys[T - 1];
	root_node->keys_num++;
}


static void __ins_not_full(B_node* node, int data) {
	int len = node->keys_num - 1;
	if (node->children[0] == NULL) {
		while (len >= 0 && data < node->keys[len]) {
			node->keys[len + 1] = node->keys[len];
			len--;
		}
		node->keys[len + 1] = data;
		node->keys_num++;
	}
	else {
		while (len >= 0 && data < node->keys[len]) {
			len--;
		}
		len++;
		if (node->children[len]->keys_num == MAX) {
			__split(node, len);
			if (data > node->keys[len]) {
				len++;
			}
		}
		__ins_not_full(node->children[len], data);
	}

}


void InsertNode(int data) {
	if (Search(data)) {
		return;
	}
	if (root == NULL) {
		root = __allocate_node();
		root->keys[0] = data;
		root->keys_num = 1;
	}
	else if (root->keys_num == MAX) {
		B_node* old_root = root;
		B_node* new_root_node = __allocate_node();
		root = new_root_node;
		new_root_node->children[0] = old_root;
		__split(new_root_node, 0);
		__ins_not_full(new_root_node, data);
	}
	else {
		__ins_not_full(root, data);
	}

}


int Search(int data) {
	B_node* cur = root;
	int child_index = 0;
	while (cur != NULL) {
		for (int i = 0; i < cur->keys_num; i++) {
			if (data == cur->keys[i]) {
				return 1;
			}
			if (data < cur->keys[i]) {
				child_index = i;
				break;
			}
			else {
				child_index = i + 1;
			}

		}
		cur = cur->children[child_index];
	}
	return 0;
}


static void __set_pos(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


static int __calc_width(B_node* node) {
	int width = 0;
	for (int i = 0; i < node->keys_num; i++) {
		int x = node->keys[i];
		while (x != 0) {
			x /= 10;
			width++;
		}
	}
	return width;
}


int x = 1;
static void __print_tree_recurs(B_node* node, int y, int width) {
	if (node == NULL) {
		return;
	}
	__print_tree_recurs(node->children[0], y + 2, width);
	__set_pos(x, y);
	printf("( ");
	for (int i = 0; i < node->keys_num; i++) {
		printf("%d ", node->keys[i]);
	}
	printf(")");
	for (int i = 1; i <= node->keys_num; i++) {
		x += width + 4;
		__print_tree_recurs(node->children[i], y + 2, width);
	}

}


void PrintTree(int y) {
	x = 1;
	int width = __calc_width(root);
	__print_tree_recurs(root, y, width);
}