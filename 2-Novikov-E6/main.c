#include <stdio.h>
#include <stdlib.h>
#define RECURSION_ERROR 1
#define ORIENT_LEFT -1
#define ORIENT_RIGHT 1
char* file_content;
int pos_in_file_content;
int max_name_size = 0;
int recurs_err = !RECURSION_ERROR;
int nodes = 0;
typedef struct btree_t {
	struct btree_t* left;
	struct btree_t* right;
	char* name;
}btree_t;
typedef struct {
	char* str;
	btree_t* tree;
}tree_print_t;
btree_t* RecursiveTreeParser() {
	if (file_content[pos_in_file_content] == '\'') {
		pos_in_file_content += 2;
		if (file_content[pos_in_file_content] == '\0') {
			return NULL;
		}
		char c = file_content[pos_in_file_content++];
		for (; c == '(' || c == ')'; pos_in_file_content++) {
			c = file_content[pos_in_file_content];
		}
		pos_in_file_content--;
		return NULL;
	}
	char c = 0;
	int size_of_name = 0;
	btree_t* tree = malloc(sizeof(btree_t));
	if (tree == NULL) {
		recurs_err = RECURSION_ERROR;
		return NULL;
	}
	for (int i = 0; c != '('; i++) {
		c = file_content[pos_in_file_content + i];
		size_of_name++;
	}
	size_of_name--;
	tree->name = malloc((size_of_name + 1) * sizeof(char));
	if (tree->name == NULL) {
		recurs_err = RECURSION_ERROR;
		return NULL;
	}
	int i = 0;
	for (; i < size_of_name; i++) {
		tree->name[i] = file_content[pos_in_file_content];
		pos_in_file_content++;
	}
	tree->name[i] = '\0';
	pos_in_file_content++;
	btree_t* left = RecursiveTreeParser();
	if (recurs_err) {
		return NULL;
	}
	btree_t* right = RecursiveTreeParser();
	if (recurs_err) {
		return NULL;
	}
	tree->left = left;
	tree->right = right;
	return tree;
}
btree_t* ReadBinaryTree() {
	FILE* bt_file = fopen("Binary tree.txt", "r");
	if (bt_file == NULL) {
		printf("Error in opening file");
		return NULL;
	}
	fseek(bt_file, 0, SEEK_END);
	int size = ftell(bt_file);
	rewind(bt_file);
	if (size < 1) {
		printf("Error: empty file");
		return NULL;
	}
	file_content = malloc((size + 2) * sizeof(char));
	fscanf(bt_file, "%s", file_content);
	puts(file_content);
	fclose(bt_file);
	pos_in_file_content = 0;
	btree_t* btree = RecursiveTreeParser();
	free(file_content);
	return btree;
}
tree_print_t* tree_print;
int elem = 0;
void PrintTreeRecursive(btree_t* root, int space, int len, int orient){
	space += max_name_size;
	int size = strlen(root->name);
	if (root->right != NULL) {
		PrintTreeRecursive(root->right, space, size, ORIENT_RIGHT);
	}
	tree_print[elem].str = calloc((space + 1), sizeof(char));
	tree_print[elem].tree = root;
	for (int i = 0; i < space - 2 * max_name_size + len; i++) {
		tree_print[elem].str[i] = ' ';
	}	
	if (space - 2 * max_name_size + len > 0) {
		if (orient == ORIENT_RIGHT) {
			tree_print[elem].str[space - 2 * max_name_size + len] = '/';
		}
		else if (orient == ORIENT_LEFT) {
			tree_print[elem].str[space - 2 * max_name_size + len] = '\\';
		}
		for (int i = space - 2 * max_name_size + len + 1; i < space - max_name_size; i++) {
			tree_print[elem].str[i] = '-';
		}
	}
	for (int i = 0; i < size; i++){
		tree_print[elem].str[space - max_name_size + i] = root->name[i];
	}
	elem++;
	if (root->left != NULL) {
		PrintTreeRecursive(root->left, space, size, ORIENT_LEFT);
	}
}
void FindMax(btree_t* tree) {
	if (tree != NULL) {
		int len = strlen(tree->name);
		if (max_name_size < len) {
			max_name_size = len;
		}
		FindMax(tree->left);
		FindMax(tree->right);
		nodes++;
	}
}
void FindVertLines(btree_t* tree) {
	if (tree != NULL) {
		int right_ind = -1;
		int left_ind = -1;
		int own_ind;
		for (int i = 0; i < nodes; i++) {
			if (tree == tree_print[i].tree) {
				own_ind = i;
				break;
			}
		}
		for (int i = 0; i < nodes; i++) {
			if (tree->right == tree_print[i].tree) {
				right_ind = i;
			}
			else if (tree->left == tree_print[i].tree) {
				left_ind = i;
			}
			else if (right_ind > 0 && left_ind > 0) {
				break;
			}
		}
		if (right_ind >= 0) {
			int len = strlen(tree_print[own_ind].str);
			for (int i = right_ind + 1; i < own_ind; i++) {
				tree_print[i].str[len] = '|';
			}
		}
		if (left_ind >= 0) {
			int len = strlen(tree_print[own_ind].str);
			for (int i = own_ind + 1; i < left_ind; i++) {
				tree_print[i].str[len] = '|';
			}
		}
		FindVertLines(tree->right);
		FindVertLines(tree->left);
	}
}
void printTree(btree_t* tree) {
	FindMax(tree);
	max_name_size++;
	tree_print = malloc(nodes * sizeof(tree_print_t));
	PrintTreeRecursive(tree, 0, 0, 0);
	FindVertLines(tree);
	for (int i = 0; i < nodes; i++) {
		puts(tree_print[i].str);
	}
}
int main() {
	btree_t* tree = ReadBinaryTree();
	if (tree == NULL) {
		return -1;
	}
	printTree(tree);
	return 0;
}