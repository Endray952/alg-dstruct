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
	//��������� ��������� �������� size, next = NULL, prev, isBusy = 0
	start.intStart = (uint32_t*)pMemory;
	*(start.intStart) = size;
	uint32_t* p = NULL;
	*(start.intStart + 1) = (uintptr_t)p;
	*(start.intStart + 2) = (uintptr_t)p;
	*(start.intStart + 3) = 0; //0 - ��������
	return 1;
}
void* memalloc(int size) {
	void* pointer = NULL;
	uint32_t* intHead = start.intStart;
	while (1) {
		if (*(intHead + 3) == 0 && *intHead >= size + FULLSIZE) {
			break;
		}

		if ((uint32_t*)(*(intHead + 1)) == NULL) { // NEXT == NULL ������ � ����������� �����
			return NULL;
		}
		else {
			intHead = (uint32_t*)(*(intHead + 1));
		}
	}


	if ((*intHead - size - FULLSIZE) > FULLSIZE) { //���� ���������� ����� �� 2 �����, �� ���������� �����
		//char* charHead = (char*)intHead;
		uint32_t* rightHead = (uint32_t*)((char*)intHead + size + FULLSIZE); //��������� �� ������ ����������� ����
		*rightHead = *intHead - size - FULLSIZE; //������ ������� �����
		*(rightHead + 1) = (uintptr_t)(*(intHead + 1)); //next ��� ������� = next ��� ����������
		*(rightHead + 2) = (uintptr_t)intHead;
		*(rightHead + 3) = 0;
		if ((uint32_t*)(*(intHead + 1)) != NULL) {
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)rightHead; //� ��� ���� next �������, ������� ��� ������ prev ��������
		}

		*intHead = size + FULLSIZE;
		*(intHead + 1) = (uintptr_t)rightHead;
		// prev �� �������
		*(intHead + 3) = 1;
	}
	else { //����� ��������� �����
		*intHead = size + FULLSIZE;
		*(intHead + 3) = 1;
	}
	pointer = (void*)(intHead + 4);

	return pointer;
}

void memfree(void* p) {
	uint32_t* intHead = (uint32_t*)p - 4;
	*(intHead + 3) = 0; // ������ ��������

	uint32_t* leftHead = (uint32_t*)*(intHead + 2);
	uint32_t* rightHead = (uint32_t*)*(intHead + 1);
	//����� ����� � ����� ������
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

	//����� ����� � ������ ������
	if (rightHead != NULL && *(rightHead + 3) == 0) {
		if ((uint32_t*)(*(rightHead + 1)) != NULL) {
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)intHead; //������� ����� ������� ��������� ������ �� ����� ��� ���� prev		
		}
		*(intHead + 1) = (uintptr_t) * (rightHead + 1);
		int size;
		if ((uint32_t) * (rightHead + 1) == NULL) {//���� � ��� ���������� ����� ������ � �������� ������������
			size = abs((int)((char*)intHead - (char*)rightHead)) + *rightHead;
		}
		else {
			size = abs((int)((char*)intHead - (char*)((uint32_t*)*(intHead + 1))));
		}

		*intHead = size;
	}

	//������ ����� ������
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
	//������ ����� �����
	/*if ((uint32_t*)*(intHead + 2) != NULL) {
		leftHead = (uint32_t*)*(intHead + 2);
		int fullLeftBlockSize = abs((int)((char*)leftHead - (char*)intHead));
		if (*leftHead < fullLeftBlockSize) {
			int initialSize = (int)*intHead;
			char* charHead = (char*)((char*)intHead - abs((int)*leftHead - fullLeftBlockSize));
			intHead = (uint32_t*)charHead;
			*(rightHead + 2) = (uintptr_t)intHead;
			*(leftHead + 1) = (uintptr_t)intHead;
			*intHead = initialSize + abs((int)*leftHead - fullLeftBlockSize);
			*(intHead + 1) = (uintptr_t)rightHead;
			*(intHead + 2) = (uintptr_t)leftHead;
			*(intHead + 3) = 0;
		}
	}*/


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