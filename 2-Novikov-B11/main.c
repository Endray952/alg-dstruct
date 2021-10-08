#include "memallocator.h"
#include <stdlib.h>
void memallocTest(int size, int* _p) {
	int* p = _p;
	int _size_1 = *(p-3);
	int* next = *(p - 2);
	int* prev = *(p - 1);

	char* skip = (char*)p;
	p = (int*)(skip + size);

	int _size_2 = *p;
	int _size_end_1 = *(p+1);
	int* next_end = *(p + 2);
	int* prev_end = *(p + 3);
	//int _size_end_2 = *(p+4);
	if (next == NULL) {
		int b = 4;
	}
}
int main() {
	typedef struct header_ Header;

	void* memory_block = malloc(1000);
	meminit(memory_block, 1000);
	int* p = (int*)memalloc(20);
	int* p1 = (int*)memalloc(36);
	memallocTest(36, p1);
	memfree(p);
	int* p2 = (int*)memalloc(22);
	memallocTest(22, p2);
	memallocTest(36, p1);
	//memallocTest(8, p);
	
	/*char* p = a;
	int n1 = *(a - 2);
	p += 8; a = (int*)p;
	int n2 = *a;
	int n3 = *(a + 1);*/

	/*int* b = (int)memalloc(24);
	memfree(a);
	int m1 = *(b - 2);
	int m2 = *(a - 2);
	int m3 = *(b + 7);
	int m4 = *(b + 8);*/

	/*int*b = (int)memalloc(24);
	int* b2 = b;
	int b1 = *(b - 2);
	char* p = a;
	int n1 = *(a - 2);
	p += 8; a = (int*)p;
	int n2 = *a;
	int n3 = *(a + 1);

	int n4 = *(a + 9);
	int n5 = *(a + 10);
	int* n6 = (int*)*(a + 11);*/
	return 0;
}
