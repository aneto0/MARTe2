/*
 *  LinkedListHolderGTest.cpp
 *
 *  Created on: Mar 11, 2015
 */


#include <limits.h>
#include "gtest/gtest.h"
#include "LinkedListHolderTest.h"

class LinkedListHolderGTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test
        // (right before the destructor).
    }
};

TEST_F(LinkedListHolderGTest,TestInsertAndSorting) {
    LinkedListHolderTest linkedlistholdertest;
    ASSERT_TRUE(linkedlistholdertest.TestInsertAndSorting());
}

TEST_F(LinkedListHolderGTest,TestAddRemoveAndSearch) {
    LinkedListHolderTest linkedlistholdertest;
    ASSERT_TRUE(linkedlistholdertest.TestAddRemoveAndSearch());
}

TEST_F(LinkedListHolderGTest,TestNULLConditions) {
    LinkedListHolderTest linkedlistholdertest;
    ASSERT_TRUE(linkedlistholdertest.TestNULLConditions());
}
