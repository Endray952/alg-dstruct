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
	//утсановка начального дескриптора. ” начального дескриптора будет поле size и поле next = NULL. 
	//
	head.charHead = (char*)pMemory;
	head.intHead = (uint32_t*)pMemory;
	uint32_t* p = NULL;
	//”становка полей с размерами
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
	//¬озвращаемый указатель указывает на начало свободного блока данных. ѕеред ним есть пол€ size и next.
	void* p = head.charHead + SIZE;

	//«апоминаем значени€ указател€ на начальный дескриптор
	uint32_t memsize = *head.intHead;
	uint32_t* memP = (uint32_t*)*(head.intHead + 1); // в принципе у начального дескриптора поле next всегда NULL

	//добавить поле с размером выдел€емого блока в конце
	*head.intHead = size + SIZE + 4;
	*(head.intHead + 1) = NULL;
	head.charHead += SIZE + size;
	head.intHead = (uint32_t*)head.charHead;
	*head.intHead = size + SIZE + 4;

	//переместить указатель головы на нужное место
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
	//утсановим отрицательный размер в дескрипотре освобождаемого блока
	int size = *(pointer - 2);
	int n1 = *(pointer - 2) = -size; // n1,n2 for test
	pointer = (int*)(charPointer + size);
	int n2 = *pointer = -size;
	pointer = (int*)charPointer;

	//”становим указатель next освободившейс€ €чейки туда, где раньше был head
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