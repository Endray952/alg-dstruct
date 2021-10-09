#include "memallocator.h"
#include <stdlib.h>

void test2MallocandFreeSecond() {
	int* p = (int*)memalloc(24);
	int* p1 = (int*)memalloc(100);
	memfree(p1);
	int n1 = *(p - 4);
	int n2 = *(p - 1);
	int n3 = *(p + 6);
	int n4 = *((int*)*(p - 3));
	int* n5 = (int*)*(p - 2);

}
void test3MallocandFreeMiddle() {
	int* p = (int*)memalloc(24);
	int* p1 = (int*)memalloc(100);
	int* p2 = (int*)memalloc(200);
	memfree(p1);
	int n1 = *(p1 - 4);
	int n2 = *(p1 - 1);
	int n3 = *(p1 + 25);

	int n4 = *((int*)*(p1 - 3));
	int n5 = *((int*)*(p1 - 2));

	int n6 = *((int*)*(p2 - 3));

	int* p3 = (int*)memalloc(50);
	int n7 = *((int*)*(p - 3));
	int n8 = *((int*)*(p3 - 2));
	int n9 = *((int*)*(p3 - 3));
	int n10 = *((int*)*(p2 - 2));
	int n11 = *((int*)*(p2 - 2) + 3);
}
void testSIzes() {
	//int  memsize = memgetminimumsize() + memgetblocksize() + 2;
	int  memsize = 30;
	void *ptr = malloc(memsize);
	meminit(ptr, memsize);
	void* p1 = memalloc(1); // Success!
	int n1 = *((int*)p1-4);
}
int main() {
	//testSIzes();
	char* init = malloc(1000);
	meminit(init, 16 * 2 + 29);
	int* p2 = memalloc(1);
	int* p3 = memalloc(27);
	/*int* p1 = memalloc(100);
	int* p2 = memalloc(200);
	int* p3 = memalloc(90);
	memfree(p1);
	p1 = memalloc(90);
	memfree(p2);*/
	//test3MallocandFreeMiddle();
	//test2MallocandFreeSecond();
	return 0;
}
