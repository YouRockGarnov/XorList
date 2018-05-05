#include <iostream>
#include "headers\XorList.h"
#include "headers\XorListIterator.h"
//#include "StackAllocator\headers\StackAlloc.h"

int main() {
	StackAlloc<int> sa;
	XorList<int, StackAlloc<int> > xl(sa);

	xl.push_back(5);
	xl.push_back(6);
	XorListIterator<int, StackAlloc<int> > iter = xl.begin();
	std::cout << *iter;
	iter++;
	std::cout << *iter;
	iter--;
	std::cout << *iter;

	system("pause");
}