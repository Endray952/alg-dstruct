#include "memallocator.h"
#include <stdlib.h>
//#include <cstdint>
#include <inttypes.h>

#define SIZE 12
#define FULLSIZE 16
typedef struct {
	char* charHead;
	uint32_t* intStart;
}handler_t;
handler_t start;



int meminit(void* pMemory, int size) {
	//��������� ��������� �������� ������, next = NULL � prev
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
	int inEnd = 0;
	while (1) {
		if (*(intHead + 3) == 0 && *intHead > size + FULLSIZE) {
			if ((uint32_t*)(*(intHead + 1)) == NULL) { //���� ��������� � ������
				if (*intHead < size + 2 * FULLSIZE) return NULL;
				inEnd = 1;				
			}
			break;
		}

		if ((uint32_t*)(*(intHead + 1)) == NULL) { // NEXT == NULL ������ � ����������� �����
			return NULL;
		}
		else {
			intHead = (uint32_t*)(*(intHead + 1));
		}
	}

	if (inEnd == 1) {
		uint32_t* p = NULL;
		char* charHead = (char*)intHead;
		uint32_t* nextHead = (uint32_t*)(charHead + size + FULLSIZE);
		*nextHead = *intHead - size - FULLSIZE; // ������� ������ ������
		*(nextHead + 1) = (uintptr_t)p; //next � ����� == NULL
		*(nextHead + 2) = (uintptr_t)intHead; // ���������� ������� � �����
		*(nextHead + 3) = 0;//������ ��������

		*intHead = size + FULLSIZE;
		*(intHead + 1) = (uintptr_t)nextHead;
		//prev ������� �������� ����� ��, ��� � � ������ ������
		*(intHead + 3) = 1; // ������ ������
		pointer = (void*)(intHead + 4);
	}
	else {
		if ((*intHead - size - FULLSIZE) > FULLSIZE) { //���� ���������� ����� �� 2 �����, �� ���������� �����
			char* charHead = (char*)intHead;
			uint32_t* rightHead = (uint32_t*)(charHead + size + FULLSIZE); //��������� �� ������ ����������� ����
			*rightHead = *intHead - size - FULLSIZE; //������ ������� �����
			*(rightHead + 1) = (uintptr_t)(*(intHead + 1)); //next ��� ������� = next ��� ����������
			*(rightHead + 2) = (uintptr_t)intHead;
			*(rightHead + 3) = 0;
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)rightHead; //� ��� ���� next �������, ������� ��� ������ prev ��������

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
	}
	return pointer;
}

void memfree(void* p) {
	uint32_t* intHead = (uint32_t*)p - 4;
	*(intHead + 3) = 0; // ������ ��������
	//����� ����� � ����� ������
	uint32_t* leftHead = (uint32_t*)*(intHead + 2);
	if (leftHead != NULL && *(leftHead + 3) == 0) {
		uint32_t* rightHead = (uint32_t*)*(intHead + 1); //� ��� �� ������ ���� ������ �������, ������ � ��������� ����������� ����
		*(rightHead + 2) = (uintptr_t)leftHead;
		*(leftHead + 1) = (uintptr_t)rightHead;

		int size = abs((int)((char*)leftHead - (char*)rightHead));
		*leftHead = size;
		intHead = leftHead;
	}

	uint32_t* rightHead = (uint32_t*)*(intHead + 1);

	//����� ����� � ������ ������
	if (*(rightHead + 3) == 0) {
		if ((uint32_t*)(*(rightHead + 1)) != NULL) {
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)intHead; //������� ����� ������� ��������� ������ �� ����� ��� ���� prev		
		}
		*(intHead + 1) = (uintptr_t)*(rightHead + 1);
		int size;
		if ((uint32_t) * (rightHead + 1) == NULL) {//���� � ��� ���������� ����� ������ � �������� ������������
			size = abs((int)((char*)intHead - (char*)rightHead)) + *rightHead;
		}
		else {
			size = abs((int)((char*)intHead - (char*)((uint32_t*)*(intHead + 1))));
		}
		 
		int b = size;
		*intHead = size;
	}

	//������ ����� ������
	if ((uint32_t*) * (rightHead + 1) != NULL) {
		int fullBlockSize = abs((int)((char*)intHead - (char*)((uint32_t*)*(intHead + 1))));
		
		if (*intHead < fullBlockSize) {
			*intHead = fullBlockSize;
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

	return 2*FULLSIZE;
}
int memgetblocksize() {
	return FULLSIZE;
}