#include "memallocator.h"
#include <stdlib.h>
#include <inttypes.h>

#define FULLSIZE 16
typedef struct {
	uint32_t* intStart;
	int size;
}handler_t;
handler_t start;


int meminit(void* pMemory, int size) {
	start.size = size;
	//Initial descriptor contains size, next = NULL, prev, isBusy = 0
	start.intStart = (uint32_t*)pMemory;
	*(start.intStart) = size; // size
	uint32_t* p = NULL;
	*(start.intStart + 1) = (uintptr_t)p; // next
	*(start.intStart + 2) = (uintptr_t)p; // prev
	*(start.intStart + 3) = 0; //0 - free
	return 1;
}
void* memalloc(int size) {
	void* pointer = NULL;
	uint32_t* intHead = start.intStart;
	while (!(*(intHead + 3) == 0 && *intHead >= size + FULLSIZE)) {
		if ((uint32_t*)(*(intHead + 1)) == NULL) { // NEXT == NULL means no free blocks next
			return NULL;
		}
		else {
			intHead = (uint32_t*)(*(intHead + 1));
		}
	}

	if ((*intHead - size - FULLSIZE) > FULLSIZE) { //if can divide block by 2
		uint32_t* rightHead = (uint32_t*)((char*)intHead + size + FULLSIZE); //pointer to added right block
		*rightHead = *intHead - size - FULLSIZE; //size of first block
		*(rightHead + 1) = (uintptr_t)(*(intHead + 1)); //next for right block = next for initital block
		*(rightHead + 2) = (uintptr_t)intHead;
		*(rightHead + 3) = 0;
		if ((uint32_t*)(*(intHead + 1)) != NULL) {
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)rightHead; //element that is after right block has prev pointer on right block
		}

		*intHead = size + FULLSIZE;
		*(intHead + 1) = (uintptr_t)rightHead;
		// prev doesnt change
		*(intHead + 3) = 1;
	}
	else { //if space only for 1 mem block. There may be 'hole'
		*intHead = size + FULLSIZE;
		*(intHead + 3) = 1;
	}
	pointer = (void*)(intHead + 4);

	return pointer;
}

void memfree(void* p) {
	uint32_t* intHead = (uint32_t*)p - 4;
	*(intHead + 3) = 0; 

	uint32_t* leftHead = (uint32_t*)*(intHead + 2);
	uint32_t* rightHead = (uint32_t*)*(intHead + 1);
	//Merge with left if free
	if (leftHead != NULL && *(leftHead + 3) == 0) {
		int size;

		if (rightHead != NULL) {
			*(rightHead + 2) = (uintptr_t)leftHead;
			*(leftHead + 1) = (uintptr_t)rightHead;
			size = abs((int)((char*)leftHead - (char*)rightHead));
		}
		else {
			size = abs((int)((char*)leftHead - (char*)intHead)) + *intHead;
			uint32_t* p = NULL;
			*(leftHead + 1) = (uintptr_t)p;
		}

		*leftHead = size;
		intHead = leftHead;
	}

	rightHead = (uint32_t*)*(intHead + 1);

	//Merge with right if free
	if (rightHead != NULL && *(rightHead + 3) == 0) {
		if ((uint32_t*)(*(rightHead + 1)) != NULL) {
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)intHead; //element that is after right block has prev pointer on right block	
		}
		*(intHead + 1) = (uintptr_t) * (rightHead + 1); //next of head is element that is after right block
		int size;
		if ((uint32_t) * (rightHead + 1) == NULL) {
			size = abs((int)((char*)intHead - (char*)rightHead)) + *rightHead;
		}
		else {
			size = abs((int)((char*)intHead - (char*)((uint32_t*)*(intHead + 1))));
		}

		*intHead = size;
	}

	//Remove right 'hole'
	if (rightHead != NULL) {
		int fullBlockSize = abs((int)((char*)intHead - (char*)rightHead));

		if (*intHead < fullBlockSize) {
			*intHead = fullBlockSize;
		}
	}
	else { 
		int distance = abs((int)((char*)start.intStart - (char*)intHead)) + *(intHead);
		if (distance < start.size) {
			*intHead += start.size - distance;
		}
	}
	
}

void memdone() {
	uint32_t* intHead = start.intStart;
	int s = 0;
	while (intHead != NULL) {
		if (*(intHead + 3) == 0) {
			s += *intHead;
		}
		intHead = (uint32_t*)*(intHead + 1);
	}
}
int memgetminimumsize() {
	return FULLSIZE;
}
int memgetblocksize() {
	return FULLSIZE;
}