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
	//утсановка начального дескриптора. У начального дескриптора будет поле size, next = NULL и prev = NULL. 
	head.charHead = (char*)pMemory;
	head.intHead = (uint32_t*)pMemory;
	uint32_t* p = NULL;
	//Установка поля с размерами
	*head.intHead = size;
	*(head.intHead + 1) = (uintptr_t)p;
	*(head.intHead + 2) = (uintptr_t)p;
	//*(head.intHead + 3) = size; //В начальном дескрипторе не нужен, но хз как без него
	return 1;
}
void* memalloc(int size) {
	int isEnd = 0;
	char* charHead;
	uint32_t* intHead = head.intHead;
	while (1) {
		if (*intHead - FULLSIZE >= size) {
			if (intHead == head.intHead) {
				isEnd = 1; //То есть нам надо будем передвигать голову head
			}
			break;
		}
		if ((uint32_t*)*(intHead + 1) != NULL) {
			intHead = (uint32_t*)*(intHead + 1);
		}
		else {
			return NULL; //Значит мы дошли до канца, но нужного блока не оказалось
		}
	}

	//запоминаем, что находится по доступному нам дескриптору
	int oldSize = *(intHead); //размер ячейки С УЧЕТОМ ПАМЯТИ НА ДЕСКРИПТОР
	uint32_t* oldNext = (uint32_t*)*(intHead + 1);
	uint32_t* oldPrev = (uint32_t*)*(intHead + 2);

	//Заполняем ячейку данными
	uint32_t* nullP = NULL;
	*intHead = size  + FULLSIZE;
	*(intHead + 1) = (uintptr_t)nullP;
	*(intHead + 2) = (uintptr_t)nullP;
	charHead = (char*)(intHead + 3);
	intHead = (uint32_t*)(charHead + size);
	*intHead = size + FULLSIZE;
	void* p = (void*)charHead;

	//Если у нас посередине была большая ячейка, в нее записали малый блок, то предвинуть указатель  на свободную часть справа
	if (oldSize > FULLSIZE + size) { // Сюда входит случай добавления в начало и в середину
		intHead++;
		*(intHead) = oldSize - (FULLSIZE + size);
		*(intHead + 1) = (uintptr_t)oldNext;
		*(intHead + 2) = (uintptr_t)oldPrev;
		if (oldPrev != NULL) {
			*(oldPrev - 2) = (uintptr_t)intHead;
		}
		if (isEnd == 0) { //Если разбили блок в середине
			charHead = (char*)(intHead + 3);
			intHead = (uint32_t*)(charHead + size);
			*intHead = oldSize - (FULLSIZE + size);
			intHead = (uint32_t*)(charHead - FULLSIZE + 4); //Указатель должен сидеть на размере блока
		}
	}
	else {
		if (oldPrev != NULL) {
			*(oldPrev + 1) = (uintptr_t)oldNext; //Следующий элемент для пердыдущего - следующий для бывшего
		}
	}
	
	if (isEnd == 1) {
		head.intHead = (uint32_t*)intHead;
		head.charHead = (char*)head.intHead;
	}
	else {
		*(head.intHead + 1) = (uintptr_t)intHead;
	}
	return p;
}
void memfree(void* p) {
	//запоминаем, что находится в голове
	int oldSize = *(head.intHead); 
	uint32_t* oldHead = head.intHead;

	//У старой головы теперь есть предыдущий элемент
	*(oldHead + 2) = (uintptr_t)((char*)p - SIZE);

	//Переставляем голову
	head.intHead = (uint32_t*)((char*)p - SIZE);
	
	//Ставим у новой головы следующий элемент
	*(head.intHead + 1) = (uintptr_t)(oldHead);


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