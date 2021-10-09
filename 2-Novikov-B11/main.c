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
	
	char* init = malloc(1000);
	meminit(init, 1000);
	int* p = (int*)memalloc(24);
	int* p1 = (int*)memalloc(100);
	int n1 = *(p-4);
	int n2 = *(p -1);
	int n3 = *(p1 - 4);
	int n4 = *(p1 - 1);
	int n5 = *(p1 + 25);
	int n6 = (int)((char*)p1 - (char*)p);
	memfree(p);
	n1 = *(p - 4);
	n2 = *(p - 1);
	memfree(p1);
	n1 = *(p - 4);
	n2 = *(p - 1);
	return 0;
}
