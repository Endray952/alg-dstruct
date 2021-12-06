#include <stdio.h>
#include <stdlib.h>
#define RECURSION_ERROR 1
char* file_content;
int pos_in_file_content;
int max_name_size = 0;
int recurs_err = !RECURSION_ERROR;
typedef struct btree_t {
	struct btree_t* left;
	struct btree_t* right;
	char* name;
}btree_t;


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


void printTreeRecursive(btree_t* root, int space, int len)
{
	space += max_name_size;
	int size = strlen(root->name);
	if (root->right != NULL) {
		printTreeRecursive(root->right, space, size);
	}
	printf("\n");
	/*for (int i = 0; i < space - max_name_size; i++) {
		printf(" ");
	}*/
	for (int i = 0; i < space - 2 * max_name_size + len; i++) {
		printf(" ");
	}	
	if (space - 2 * max_name_size + len > 0) {
		for (int i = space - 2 * max_name_size + len; i < space - max_name_size; i++) {
			printf("-");
		}
	}
	
	printf("%s\n", root->name);
	
	if (root->left != NULL) {
		printTreeRecursive(root->left, space, size);
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
	}

}
void printTree(btree_t* tree)
{
	FindMax(tree);
	max_name_size++;
	printTreeRecursive(tree, 0, 0);

}


int main() {
	btree_t* tree = ReadBinaryTree();
	if (tree == NULL) {
		return -1;
	}
	printTree(tree);
	return 0;
}