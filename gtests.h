#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "headers\XorList.h"
#include <list>
#include "headers\XorListIterator.h"

TEST(XorListTest, Constructors) {
	using alloc = StackAlloc<int>;

	alloc  sa;
	using xor_list = XorList<int, alloc  >;
	XorList<int, alloc  > xlist(500, 1, sa);

	auto iter = xlist.begin();
	for (size_t i = 0; i < xlist.size(); i++) {
		ASSERT_EQ(*iter, 1);
		iter++;
	}

	XorList<int, alloc  > xlist2(std::move(xlist));
	iter = xlist.begin();
	auto iter2 = xlist.begin();
	for (size_t i = 0; i < xlist.size(); i++) {
		ASSERT_EQ(*iter, *iter2);
		iter++;
		iter2++;
	}

	XorList<int, alloc> xlist3 = xlist2;

	iter2 = xlist.begin();
	auto iter3 = xlist.begin();
	for (size_t i = 0; i < xlist.size(); i++) {
		ASSERT_EQ(*iter2, *iter3);
		iter2++;
		iter3++;
	}
}

TEST(XorListTest, ManualTest) {
	std::allocator<int> stdal;
	StackAlloc<int> sa;
	XorList<int, std::allocator<int> > xl(stdal);

	xl.push_back(1);
	XorListIterator<int, std::allocator<int> > titer = xl.begin();
	xl.insert_after(titer, 2);

	titer = xl.begin();
	titer++;
	xl.erase(titer);

	xl.push_back(3);

	auto xiter = xl.begin();
	for (int j = 0; j < xl.size(); ++j) {
		std::cout << *xiter << " ";
		++xiter;
	} std::cout << '\n';


	//////////////////////
	titer = xl.begin();

	xl.push_back(1);
	xl.insert_after(titer, 2);

	titer = xl.begin();
	titer++;
	xl.insert_after(titer, 3);

	xl.push_front(5);
	xl.push_back(6);

	titer = xl.begin();
	titer++;
	xl.insert_after(titer, 7);

	xiter = xl.begin();
	for (int j = 0; j < xl.size(); ++j) {
		std::cout << *xiter << " ";
		++xiter;
	} std::cout << '\n';


	titer = xl.begin();
	++titer;
	++titer;
	xl.erase(titer);

	titer = xl.begin();
	++titer;
	++titer;

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
}

TEST(XorListTest, random_check) {
	bool DEBUG = false;

	using alloc = StackAlloc<int>;

	alloc  sa;
	using xor_list = XorList<int, alloc  >;
	XorList<int, alloc  > xlist(sa);
	std::list<int> stdlist;

	srand(time(NULL));
	for (int i = 0; i < 10000; ++i) {

		auto iter = stdlist.begin();
		auto xiter = xlist.begin();
		for (int j = 0; j < xlist.size(); ++j) {
			ASSERT_EQ(*iter, *xiter);
			++xiter;
			++iter;
		}

		int choice = rand() % 6;

		switch (choice) {
			case 0:
			{
				int pushing = rand() % 10000;

				int where = rand() % 2;
				if (DEBUG)
					std::cout << "push " << pushing;

				if (where == 0) {
					xlist.push_front(pushing);
					stdlist.push_front(pushing);

					if (DEBUG)
						std::cout << " front\n";
				} else {
					xlist.push_back(pushing);
					stdlist.push_back(pushing);

					if (DEBUG)
						std::cout << " back\n";
				}

				break;
			}

			case 1:
			{
				int pushing = rand() % 10000;

				if (stdlist.empty()) {
					xlist.push_back(pushing);
					stdlist.push_back(pushing);

					if (DEBUG) {
						std::cout << "push " << pushing;
						std::cout << " back\n";
					}
					continue;
				}

				int index = rand() % stdlist.size();

				auto xiter = xlist.begin();
				auto iter = stdlist.begin();
				for (int j = 0; j < index; ++j) {
					++iter;
					++xiter;
				}

				if (DEBUG)
					std::cout << "insert " << pushing << " after " << index << "\n";

				stdlist.insert(iter, pushing);
				xlist.insert_before(xiter, pushing);

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

				if (DEBUG)
					std::cout << "erase " << index << "\n";

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

				if (DEBUG)
					std::cout << "set " << val << " from " << index << "\n";

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
					if (*iter != *xiter)
						//std::cout << "Error: " << *iter << " from std != " << *xiter;

						++iter;
					++xiter;
				}

				break;
			}

			case 5:
			{
				if (stdlist.size() == 0)
					continue;

				int where = rand() % 2;

				if (DEBUG)
					std::cout << "pop ";

				if (where == 0) {
					if (DEBUG)
						std::cout << "front\n";

					xlist.pop_front();
					stdlist.pop_front();
				} else {
					if (DEBUG)
						std::cout << "back\n";

					xlist.pop_back();
					stdlist.pop_back();
				}

				break;
			}
		}
	}
}
