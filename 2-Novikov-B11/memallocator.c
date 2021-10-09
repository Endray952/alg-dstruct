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
	uint32_t* endHandler;
	int inEnd = 0;
	while (1) {
		if (*(intHead + 3) == 0 && *intHead > size + FULLSIZE) {
			if ((uint32_t*)(*(intHead + 1)) == NULL) { //���� ��������� � ������
				inEnd = 1;
				endHandler = intHead;
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
		*(nextHead + 2) = intHead; // ���������� ������� � �����
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
	//uint32_t* rightPrev = (uint32_t*)*(rightHead + 2);
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
	
	
}



//void* memalloc_1(int size) {
//	uint32_t* intHead = head.intHead;
//	char* charHead = head.charHead;
//	while (1){
//		if (*intHead - 12 >= size) {
//			break;
//		}
//		if ((uint32_t*)*(intHead + 1) != NULL) {
//			intHead = (uint32_t*)*(intHead + 1);
//			charHead = (char*)intHead;
//		}
//		else {
//			return NULL;
//		}
//		
//	} 
//	//������������ ��������� ��������� �� ������ ���������� ����� ������. ����� ��� ���� ���� size � next.
//	void* p = charHead + SIZE;
//
//	//���������� �������� ��������� �� ��������� ����������
//	uint32_t memsize = *intHead;
//	uint32_t* memP = (uint32_t*)*(intHead + 1); // � �������� � ���������� ����������� ���� next ������ NULL
//
//	//�������� ���� � �������� ����������� ����� � �����
//	*intHead = size + SIZE + 4;
//	*(intHead + 1) = NULL;
//	charHead += SIZE + size;
//	intHead = (uint32_t*)charHead;
//	*intHead = size + SIZE + 4;
//
//	//����������� ��������� ������ �� ������ �����
//	charHead += 4;
//	intHead = (uint32_t*)charHead;
//	*intHead = (memsize - (SIZE + size + 4));
//	*(intHead + 1) = (uintptr_t)memP;
//	//head.next = memP;
//	//int a = *head;
//	if ((uint32_t*)*(head.intHead + 1) == NULL) {
//		head.intHead = intHead;
//		head.charHead = charHead;
//	}
//	return p;
//}
//void memfree_1(void* p) {
//	int* pointer = (int*)p;
//	char* charPointer = (char*)p;
//	//��������� ������������� ������ � ����������� �������������� �����
//	int size = *(pointer - 2);
//	int n1 = *(pointer - 2) = -size; // n1,n2 for test
//	pointer = (int*)(charPointer + size);
//	int n2 = *pointer = -size;
//	pointer = (int*)charPointer;
//
//	//��������� ��������� next �������������� ������ ����, ��� ������ ��� head
//	uint32_t* uPointer = (uint32_t*)pointer;
//
//	*(uPointer - 1) = (uintptr_t)head.intHead;
//	int n3 = *head.intHead; //for test
//	int n4 = (int)*((uint32_t*)(uPointer - 1));
//	head.intHead = uPointer - 2;
//	head.charHead = (char*)head.intHead;
//	int n5 = *head.intHead;
//	int n6 = *((uint32_t*)*(head.intHead + 1));
//}
void memdone() {

}
int memgetminimumsize() {

	return 2*FULLSIZE;
}
int memgetblocksize() {
	return FULLSIZE;
}