/*
 *   BStringGTest.cpp
 *
 *  Created on: Mar 10, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "BStringTest.h"

class BStringGTest: public ::testing::Test {
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

TEST_F(BStringGTest,TestFSReadAndFSWrite) {
    BStringTest bstringtest;
    ASSERT_TRUE(bstringtest.TestFSReadAndFSWrite());
}

TEST_F(BStringGTest,TestStringOperators) {
    BStringTest bstringtest;
    ASSERT_TRUE(bstringtest.TestStringOperators("Hello","World"));
}

TEST_F(BStringGTest,TestCharOperators) {
    BStringTest bstringtest;
    ASSERT_TRUE(bstringtest.TestCharOperators('H','W'));
}

