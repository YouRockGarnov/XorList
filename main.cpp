#include <iostream>
#include "headers\XorList.h"
#include <list>
#include "headers\XorListIterator.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gtests.h"
//#include "StackAllocator\headers\StackAlloc.h"

bool DEBUG = false;

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	system("pause");
}
