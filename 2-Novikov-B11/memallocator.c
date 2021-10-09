#include "memallocator.h"
#include <stdlib.h>
//#include <cstdint>
#include <inttypes.h>

#define SIZE 12
#define FULLSIZE 16
typedef struct {
	char* charHead;
	uint32_t* intHead;
}handler_t;
handler_t head;


int meminit(void* pMemory, int size) {
	//��������� ���������� �����������. � ���������� ����������� ����� ���� size, next = NULL � prev = NULL. 
	head.charHead = (char*)pMemory;
	head.intHead = (uint32_t*)pMemory;
	int* signedHead;
	uint32_t* p = NULL;
	//� ������ ����� ����� ����������� ����, ��������� � �������
	*head.intHead = 0;
	head.intHead++;
	//��������� ���� � ���������
	signedHead = (int*)head.intHead;
	*signedHead = -size;
	*(head.intHead + 1) = (uintptr_t)p;
	*(head.intHead + 2) = (uintptr_t)p;
	//*(head.intHead + 3) = size; //� ��������� ����������� �� �����, �� �� ��� ��� ����
	int n = *((int*)head.intHead);
	return 1;
}
void* memalloc(int size) {
	int _size = size;
	int isEnd = 0;
	int* signedInt;
	char* charHead;
	uint32_t* intHead = head.intHead;
	while (1) {
		int n1 = abs(*((int*)intHead));
		if (abs(*((int*)intHead)) - FULLSIZE >= size) {
			if (intHead == head.intHead) {
				isEnd = 1; //�� ���� ��� ���� ����� ����������� ������ head
			}
			break;
		}
		if ((uint32_t*)(*(intHead + 1)) != NULL) {
			intHead = (uint32_t*)*(intHead + 1);
		}
		else {
			return NULL; //������ �� ����� �� �����, �� ������� ����� �� ���������
		}
	}

	//����������, ��� ��������� �� ���������� ��� �����������
	int oldSize = abs(*((int*)intHead)); //������ ������ � ������ ������ �� ����������
	uint32_t* oldNext = (uint32_t*)(*(intHead + 1));
	uint32_t* oldPrev = (uint32_t*)(*(intHead + 2));

	//��������� ������ �������
	uint32_t* nullP = NULL;
	if (oldNext != NULL && oldSize - _size - FULLSIZE < FULLSIZE) { //���� �� ��������� �� � �����, �� ������������� ��������� �����
		_size = oldSize - FULLSIZE;
	}
	*intHead = _size  + FULLSIZE;
	*(intHead + 1) = (uintptr_t)nullP;
	*(intHead + 2) = (uintptr_t)nullP;
	charHead = (char*)(intHead + 3);

	intHead = (uint32_t*)(charHead + _size);

	*intHead = _size + FULLSIZE;
	void* p = (void*)charHead;

	//���� � ��� ���������� ���� ������� ������, � ��� �������� ����� ����, �� ���������� ���������  �� ��������� ����� ������
	if (oldSize > FULLSIZE + _size) { // ���� ������ ������ ���������� � ����� � � ��������
		intHead++;
		signedInt = (int*)intHead;
		*signedInt = -(oldSize - (FULLSIZE + _size));
		int n10 = *((int*)intHead);
		//*(intHead) = oldSize - (FULLSIZE + _size);
		*(intHead + 1) = (uintptr_t)oldNext;
		*(intHead + 2) = (uintptr_t)oldPrev;
		if (oldPrev != NULL) {
			*(oldPrev - 2) = (uintptr_t)intHead;
		}
		if (isEnd == 0) { //���� ������� ���� � ��������
			charHead = (char*)(intHead + 3);
			intHead = (uint32_t*)(charHead + _size);
			signedInt = (int*)intHead;
			*signedInt = -(oldSize - (FULLSIZE + _size));
			//*intHead = oldSize - (FULLSIZE + _size);
			intHead = (uint32_t*)(charHead - FULLSIZE + 4); //��������� ������ ������ �� ������� �����
		}
	}
	else {
		if (oldPrev != NULL) {
			*(oldPrev + 1) = (uintptr_t)oldNext; //��������� ������� ��� ����������� - ��������� ��� �������
		}
	}
	
	if (isEnd == 1) {
		//int* signedInt = (int*)intHead;
		//*signedInt = -*signedInt;
		head.intHead = intHead;
		int mg = (int)(*((int*)intHead));
		head.charHead = (char*)head.intHead;
		
		
	}
	else {
		*(head.intHead + 1) = (uintptr_t)intHead;
	}
	return p;
}
void memfree(void* p) {
	char* charHead;
	uint32_t* intHead = (uint32_t*)((char*)p - SIZE);
	int* signedHead = (int*)intHead;
	int size = *signedHead;
	int inEnd = 1; //����, ���� ������������� ������ �� � ��� �� ���������
	
	//����������, ��� ��������� � ������, �� ����� �� ����������� �� �������������� �������	
	uint32_t* oldHead = head.intHead;

	//��������, ����� �� ����� �����
	if ( *(signedHead - 1) != 0 && *(signedHead - 1) < 0) {
		inEnd = 0;

		charHead = (char*)(intHead - 1); //���������� ����� �� ������ ������ �����
		int leftSize = -*((int*)(intHead - 1));//- �� ������ ������������� � ��������� �������
		charHead -= leftSize - 4; //���������� ����� �� ������ ������ �����
		intHead = (uint32_t*)charHead; //���������� ��� �� 3*4, ����� ��������� �� ����� ������		

		size += leftSize;
		signedHead = (int*)intHead;
	}
	else if (*((int*)((char*)(signedHead)+size)) < 0 ) {
		charHead = (char*)(signedHead) + size;
		int n2 = *((int*)charHead);
		inEnd = 0;
		uint32_t* initHead = (uint32_t*)signedHead;
		int n12 = *signedHead;
		signedHead = (int*)charHead; //��������� �� ������ ������� ��������
		//*((uint32_t*)(charHead - size + 4)) = (uintptr_t)(*(signedHead + 1)); //���������� ����� ��������� �� next

		//���������� � �������� next ��� ��������������� ����, ��� ���� �������� prev
		//uint32_t* tmp = (uint32_t*)((uintptr_t)*(signedHead + 1));
		if (((uint32_t*)((uintptr_t)*(signedHead + 1))) != NULL) {
			*((uint32_t*)((uintptr_t)*(signedHead + 1)) + 2) = (uintptr_t)(initHead);
		}
		
		if ((uint32_t*)((uintptr_t) *(signedHead + 2)) != NULL) { //���������� � ��������, �� ������� �������� prev � �������������� �����
			uint32_t* prevElem = (uint32_t*)((uintptr_t) *(signedHead + 2));
			*(prevElem + 1) = (uintptr_t)(initHead); // �������� ���� next
			*((uint32_t*)(charHead - size + 8)) = (uintptr_t)prevElem;
		}
		else {
			head.intHead = initHead;
		}
		size += abs(*signedHead);

	}


	//���������� ������� �������� size
	*signedHead = -size;
	charHead = (char*)signedHead;
	charHead += size - 4;
	signedHead = (int*)charHead;
	*signedHead = -size;
	

	if (inEnd == 1) {
		//� ������ ������ ������ ���� ���������� �������
		*(oldHead + 2) = (uintptr_t)(intHead);

		//������������ ������
		head.intHead = (uint32_t*)(intHead);

		//������ � ����� ������ ��������� �������
		*(head.intHead + 1) = (uintptr_t)(oldHead);
	}

	int n1 = *((uint32_t*)(*(head.intHead + 1)));
	

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