#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "Btree.h"


int main() {
	char command;
	int value;
	while (scanf("%c", &command) >= 1) {
		if (command != 'p' && command != 'q') {
			scanf("%d", &value);
		}
		switch (command) {
		case 'a':
			InsertNode(value);
			break;
		case 'r':
			DeleteNode(value);
			break;
		case 'f':
			if (Search(value))
				puts("yes");
			else
				puts("no");
			break;
		case 'p':
			system("cls");
			PrintTree(0);
			break;
		case 'q':
			return 0;
			break;
		}
	}
	return 0;
}