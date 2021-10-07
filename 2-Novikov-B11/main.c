#include "memallocator.h"
int main() {
	typedef struct header_ Header;
	Header* lel;
	int n = sizeof(lel);
	void* memory_block = malloc(1000);
	meminit(memory_block, 1000);
	int* a = (int)memalloc(22);
	char* p = a;
	p += 22;
	a = p;
	int b = *a;
	return 0;
}