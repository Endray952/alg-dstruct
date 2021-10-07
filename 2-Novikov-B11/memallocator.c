#include "memallocator.h"
#include <stdlib.h>

//typedef struct {
//	int memLeft;
//	//поле под указатель
//}mem_descriptor_head_t;

//typedef struct mem_descriptor_t {
//	struct mem_descriptor_t* next;
//	struct mem_descriptor_t* prev;
//	int size;
//}mem_descriptor_t;

//mem_descriptor_t head;
typedef struct {
	int* pointer;
	int size;
	//поле под указатель
}mem_descriptor_head_t;
//mem_descriptor_head_t head;
char* head;


int meminit(void* pMemory, int size) {
	head = pMemory;
	int* intHead = head;
	int* p = NULL;
	*intHead = size;
	*(intHead + 1) = p;
	//head = intHead;
	return 1;
}
void* memalloc(int size) {
	void* p = head + 8;
	int* intHead = head;

	int memsize = *intHead;
	int* memP = *(intHead + 1);

	head += 8 + size;
	intHead = head;
	*intHead = (memsize - (8 + size));
	*(intHead + 1) = memP;

	//int a = *head;
	return p;
}