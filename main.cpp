#include "headers\XorList.h"
//#include "StackAllocator\headers\StackAlloc.h"

int main() {
	StackAlloc<int> sa;
	XorList<int, StackAlloc<int> > xl(sa);

	xl.push_back(9);
	//xl.pop_back();
}