#include "Stack.h"
void StackPush(stack_t* stack, int data) {
	stack->arr[stack->head] = data;
	stack->head++;
}
stack_t* StackInit(int size) {
	stack_t* stack = malloc(sizeof(stack_t));
	if (stack == NULL) {
		return NULL;
	}
	stack->arr = calloc(size, sizeof(int));
	if (stack->arr == NULL) {
		return NULL;
	}
	stack->head = 0;
	return stack;
}
void StackPop(stack_t* stack) {
	stack->head--;
	stack->arr[stack->head] = 0;
}