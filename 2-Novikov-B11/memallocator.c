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
	//Начальный дескриптр содержит размер, next = NULL и prev
	start.intStart = (uint32_t*)pMemory;
	*(start.intStart) = size;
	uint32_t* p = NULL;
	*(start.intStart + 1) = (uintptr_t)p;
	*(start.intStart + 2) = (uintptr_t)p;
	*(start.intStart + 3) = 0; //0 - свободно
	return 1;
}
void* memalloc(int size) {
	void* pointer = NULL;
	uint32_t* intHead = start.intStart;
	uint32_t* endHandler;
	int inEnd = 0;
	while (1) {
		if (*(intHead + 3) == 0 && *intHead > size + FULLSIZE) {
			if ((uint32_t*)(*(intHead + 1)) == NULL) { //Если вставляем в начало
				inEnd = 1;
				endHandler = intHead;
			}
			break;
		}

		if ((uint32_t*)(*(intHead + 1)) == NULL) { // NEXT == NULL только у дескриптора конца
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
		*nextHead = *intHead - size - FULLSIZE; // Перенес размер вправо
		*(nextHead + 1) = (uintptr_t)p; //next у конца == NULL
		*(nextHead + 2) = intHead; // предыдущий элесент у конца
		*(nextHead + 3) = 0;//Ячейка свободна

		*intHead = size + FULLSIZE;
		*(intHead + 1) = (uintptr_t)nextHead;
		//prev элемент остается таким же, как и у головы раньше
		*(intHead + 3) = 1; // ячейка занята
		pointer = (void*)(intHead + 4);
	}
	else {
		if ((*intHead - size - FULLSIZE) > FULLSIZE) { //блок посередине делим на 2 блока, тк достаточно места
			char* charHead = (char*)intHead;
			uint32_t* rightHead = (uint32_t*)(charHead + size + FULLSIZE); //Указывает на правый добавленный блок
			*rightHead = *intHead - size - FULLSIZE; //Размер правого блока
			*(rightHead + 1) = (uintptr_t)(*(intHead + 1)); //next для правого = next для начального
			*(rightHead + 2) = (uintptr_t)intHead;
			*(rightHead + 3) = 0;
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)rightHead; //У нас есть next элемент, поэтому ему задаем prev значение

			*intHead = size + FULLSIZE;
			*(intHead + 1) = (uintptr_t)rightHead;
			// prev не трогаем
			*(intHead + 3) = 1;
		}
		else { //Может появиться дырка
			*intHead = size + FULLSIZE;
			*(intHead + 3) = 1;
		}
		pointer = (void*)(intHead + 4);
	}
	return pointer;
}

void memfree(void* p) {
	uint32_t* intHead = (uint32_t*)p - 4;
	*(intHead + 3) = 0; // Ячейка свободна
	//Можно слить с левым блоком
	uint32_t* leftHead = (uint32_t*)*(intHead + 2);
	if (leftHead != NULL && *(leftHead + 3) == 0) {
		uint32_t* rightHead = (uint32_t*)*(intHead + 1); //У нас же всегла есть правый элемент, только у конечного дескриптора нету
		*(rightHead + 2) = (uintptr_t)leftHead;
		*(leftHead + 1) = (uintptr_t)rightHead;

		int size = abs((int)((char*)leftHead - (char*)rightHead));
		*leftHead = size;
		intHead = leftHead;
	}
	uint32_t* rightHead = (uint32_t*)*(intHead + 1);
	//uint32_t* rightPrev = (uint32_t*)*(rightHead + 2);
	//Можно слить с правым блоком
	if (*(rightHead + 3) == 0) {
		if ((uint32_t*)(*(rightHead + 1)) != NULL) {
			*((uint32_t*)(*(rightHead + 1)) + 2) = (uintptr_t)intHead; //Элемент после правого указывает теперь на левый как свой prev		
		}
		*(intHead + 1) = (uintptr_t)*(rightHead + 1);
		int size;
		if ((uint32_t) * (rightHead + 1) == NULL) {//Если у нас раастояние между блоком и конечным дескриптором
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
//	//Возвращаемый указатель указывает на начало свободного блока данных. Перед ним есть поля size и next.
//	void* p = charHead + SIZE;
//
//	//Запоминаем значения указателя на начальный дескриптор
//	uint32_t memsize = *intHead;
//	uint32_t* memP = (uint32_t*)*(intHead + 1); // в принципе у начального дескриптора поле next всегда NULL
//
//	//добавить поле с размером выделяемого блока в конце
//	*intHead = size + SIZE + 4;
//	*(intHead + 1) = NULL;
//	charHead += SIZE + size;
//	intHead = (uint32_t*)charHead;
//	*intHead = size + SIZE + 4;
//
//	//переместить указатель головы на нужное место
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
//	//утсановим отрицательный размер в дескрипотре освобождаемого блока
//	int size = *(pointer - 2);
//	int n1 = *(pointer - 2) = -size; // n1,n2 for test
//	pointer = (int*)(charPointer + size);
//	int n2 = *pointer = -size;
//	pointer = (int*)charPointer;
//
//	//Установим указатель next освободившейся ячейки туда, где раньше был head
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