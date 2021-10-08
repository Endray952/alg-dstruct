#include "memallocator.h"
int main() {
	typedef struct header_ Header;

	void* memory_block = malloc(1000);
	meminit(memory_block, 1000);
	int* a = (int)memalloc(8);
	/*char* p = a;
	int n1 = *(a - 2);
	p += 8; a = (int*)p;
	int n2 = *a;
	int n3 = *(a + 1);*/
	int* b = (int)memalloc(24);
	memfree(a);
	int m1 = *(b - 2);
	int m2 = *(a - 2);
	int m3 = *(b + 7);
	int m4 = *(b + 8);
	/*int*b = (int)memalloc(24);
	char* p = a;
	int n1 = *(a - 2);
	p += 15; a = (int*)p;
	int n2 = *a;
	int n3 = *(a + 1);

	int n4 = *(a + 9);
	int n5 = *(a + 10);
	int* n6 = (int*)*(a + 11);*/
	return 0;
}