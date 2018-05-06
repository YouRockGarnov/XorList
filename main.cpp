#include <iostream>
#include "headers\XorList.h"
#include "headers\XorListIterator.h"
//#include "StackAllocator\headers\StackAlloc.h"

int main() {
	std::allocator<int> stdal;
	StackAlloc<int> sa;
	XorList<int, std::allocator<int> > xl(stdal);

	xl.push_back(5);
	xl.push_back(6);
	XorListIterator<int, std::allocator<int> > iter = xl.begin();
	std::cout << *iter;
	iter++;
	std::cout << *iter;

	int check = 42;

	iter--;
	std::cout << *iter;

	xl.insert_after(iter, std::move(check));

	XorListIterator<int, std::allocator<int> > test = xl.begin();
	std::cout << *test;
	test++;
	std::cout << *test;
	test++;
	std::cout << *test;


	system("pause");
}