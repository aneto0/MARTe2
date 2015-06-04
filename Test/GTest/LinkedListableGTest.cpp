/*
 *  LinkedListableGTest.cpp
 *
 *  Created on: Mar 10, 2015
 */


#include <limits.h>
#include "gtest/gtest.h"
#include "LinkedListableTest.h"

class LinkedListableGTest: public ::testing::Test {
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

TEST_F(LinkedListableGTest,TestInsertAndSorting) {
    LinkedListableTest linkedlistabletest;
    ASSERT_TRUE(linkedlistabletest.TestInsertAndSorting());
}

TEST_F(LinkedListableGTest,TestAddAndRemove) {
    LinkedListableTest linkedlistabletest;
    ASSERT_TRUE(linkedlistabletest.TestAddAndRemove());
}
