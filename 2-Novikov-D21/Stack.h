#pragma once
#include "stdlib.h"
typedef struct {
	int* arr;
	int head;
}stack_t;
void StackPush(stack_t* stack, int data);
stack_t* StackInit(int size);
void StackPop(stack_t* stack);
	