#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "HamiltonianPath.h"
#define CREATE_RND_PATH 0

int main() {
	if (CREATE_RND_PATH) {
		CreateRndHamiltonPath();
	}
	FindHamiltonianPath();
	
	return 0;
}
