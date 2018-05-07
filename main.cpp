#include <iostream>
#include "headers\XorList.h"
#include <list>
#include "headers\XorListIterator.h"
//#include "StackAllocator\headers\StackAlloc.h"

int random_checking();

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
	xl.erase(iter);

	std::cout << std::endl;
	XorListIterator<int, std::allocator<int> > test = xl.begin();
	std::cout << *test;
	test++;
	std::cout << *test;
/*	test++;
	std::cout << *test;*/

    random_checking();

	system("pause");
}

//for debuging or checking
int random_checking() {
	StackAlloc<int> sa;
    using xor_list = XorList<int, StackAlloc<int> >;
	XorList<int, StackAlloc<int> > xlist(sa);
	std::list<int> stdlist;

	srand(time(NULL));
	for (int i = 0; i < 10000; ++i) {
		/*		std::cout << "////////////" << std::endl << "MY: ";
		xlist.show();

		std::cout << std::endl << "STD: ";
		for (auto elem : stdlist) {
		std::cout << elem;
		}
		std::cout << std::endl;*/

		int choice = rand() % 6;

		switch (choice) {
			case 0:
			{
				int pushing = rand() % 10000;

                int where = rand() % 2;

                if (where == 0) {
                    xlist.push_front(pushing);
                    stdlist.push_front(pushing);
                } else {
                    xlist.push_back(pushing);
                    stdlist.push_back(pushing);
                }

				xlist.push_back(pushing);
				stdlist.push_back(pushing);

				break;
			}

			case 1:
			{
				int pushing = rand() % 10000;

				if (stdlist.empty()) {
					xlist.push_back(pushing);
					stdlist.push_back(pushing);
					continue;
				}

				int index = rand() % stdlist.size();

                auto xiter = xlist.begin();
				auto iter = stdlist.begin();
				for (int j = 0; j < index; ++j) {
					++iter;
                    ++xiter;
				}

				stdlist.insert(iter, pushing);
                xlist.insert_after(xiter, pushing);

				break;
			}

			case 2:
			{
				if (stdlist.size() == 0)
					continue;

                int index = rand() % stdlist.size();

                auto xiter = xlist.begin();
                auto iter = stdlist.begin();
                for (int j = 0; j < index; ++j) {
                    ++iter;
                    ++xiter;
                }

                xlist.erase(xiter);
				stdlist.erase(iter);

				break;
			}

			case 3:
			{
				if (stdlist.size() == 0)
					continue;

				int index = rand() % stdlist.size();
				int val = rand() % 10000;

                auto xiter = xlist.begin();
                auto iter = stdlist.begin();
                for (int j = 0; j < index; ++j) {
                    ++iter;
                    ++xiter;
                }

                *iter = val;
                *xiter = val;

				break;
			}

			case 4:
			{
				if (stdlist.size() == 0)
					continue;

                auto xiter = xlist.begin();
                auto iter = stdlist.begin();
                for (int j = 0; j < stdlist.size(); ++j) {
                    ++iter;
                    ++xiter;

                    if (*iter != *xiter)
                        std::cout << "Error: " << *iter << " from std != " << *xiter;
                }

				break;
			}

            case 5:{
                if (stdlist.size() == 0)
                    continue;

                int where = rand() % 2;

                if (where == 0) {
                    xlist.pop_front();
                    stdlist.pop_front();
                } else {
                    xlist.pop_back();
                    stdlist.pop_back();
                }

                break;
            }
		}
	}

	system("pause");
}
