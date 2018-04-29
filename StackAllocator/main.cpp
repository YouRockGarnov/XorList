/*#include <iostream>
#include <list>
#include <vector>
#include "headers\StackAlloc.h"

void Check_StackAlloc() {
	StackAlloc<int> sa;
	ptr<int> p = sa.allocate(1);
	ptr<int> p2 = sa.allocate(1);

	*p2 = 12;
	*p = 9;
	std::cout << *p << *p2;

	system("pause");
}

void Check_BufferChain() {
	BufferChain bc;

	bc.add_block();
	ptr<int> p;
	for (size_t i = 0; i < 1024; i++) {
		p = bc.get_ptr_for_type<int>(1);
	}


	*p = 9;
	std::cout << *p;

	system("pause");
}

void CheckSTDList(){
	std::vector<int, StackAlloc<int> > li;
	li.push_back(9);
	li.push_back(10);

    for (auto& a : li) {
        std::cout << a << ' ';
    }

	system("pause");
}

int main() {

	CheckSTDList();

	return 0;
}*/