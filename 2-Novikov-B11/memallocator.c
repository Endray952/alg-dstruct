#include "memallocator.h"
#include <stdlib.h>
//#include <cstdint>
#include <inttypes.h>

#define SIZE 8
typedef struct {
	char* charHead;
	uint32_t* intHead;
	int size;
	uint32_t* next;
	uint32_t* prev;
}handler_t;
handler_t head;


int meminit(void* pMemory, int size) {
	//��������� ���������� �����������. � ���������� ����������� ����� ���� size � ���� next = NULL. 
	//
	head.charHead = (char*)pMemory;
	head.intHead = (uint32_t*)pMemory;
	uint32_t* p = NULL;
	//��������� ����� � ���������
	*head.intHead = size;
	*(head.intHead + 1) = (uintptr_t)p;
	return 1;
}
void* memalloc(int size) {
	/*do {
		if (*head.intHead - 12 >= size) {
			break;
		}
	} while()*/
	//������������ ��������� ��������� �� ������ ���������� ����� ������. ����� ��� ���� ���� size � next.
	void* p = head.charHead + SIZE;

	//���������� �������� ��������� �� ��������� ����������
	uint32_t memsize = *head.intHead;
	uint32_t* memP = (uint32_t*)*(head.intHead + 1); // � �������� � ���������� ����������� ���� next ������ NULL

	//�������� ���� � �������� ����������� ����� � �����
	*head.intHead = size + SIZE + 4;
	*(head.intHead + 1) = NULL;
	head.charHead += SIZE + size;
	head.intHead = (uint32_t*)head.charHead;
	*head.intHead = size + SIZE + 4;

	//����������� ��������� ������ �� ������ �����
	head.charHead += 4;
	head.intHead = (uint32_t*)head.charHead;
	*head.intHead = (memsize - (SIZE + size + 4));
	*(head.intHead + 1) = (uintptr_t)memP;
	head.next = memP;
	//int a = *head;
	return p;
}
void memfree(void* p) {
	int* pointer = (int*)p;
	char* charPointer = (char*)p;
	//��������� ������������� ������ � ����������� �������������� �����
	int size = *(pointer - 2);
	int n1 = *(pointer - 2) = -size; // n1,n2 for test
	pointer = (int*)(charPointer + size);
	int n2 = *pointer = -size;
	pointer = (int*)charPointer;

	//��������� ��������� next �������������� ������ ����, ��� ������ ��� head
	uint32_t* uPointer = (uint32_t*)pointer;

	*(uPointer - 1) = (uintptr_t)head.intHead;
	int n3 = *head.intHead; //for test
	int n4 = (int)*((uint32_t*)(uPointer - 1));
	head.intHead = uPointer - 2;
	head.charHead = (char*)head.intHead;
	int n5 = *head.intHead;
	int n6 = *((uint32_t*)*(head.intHead + 1));
}
void memdone() {

}
int memgetminimumsize() {

	return 8;
}
int memgetblocksize() {
	return 8;
}