/*
 *  StackHolderGTest.cpp
 *
 *  Created on: Mar 16, 2015
 */


#include <limits.h>
#include "gtest/gtest.h"
#include "StackHolderTest.h"

class StackHolderGTest: public ::testing::Test {
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

TEST_F(StackHolderGTest,TestAll) {
    StackHolderTest stackholdertest;
    ASSERT_TRUE(stackholdertest.TestAll());
}

TEST_F(StackHolderGTest,TestNULLConditions) {
    StackHolderTest stackholdertest;
    ASSERT_TRUE(stackholdertest.TestNULLConditions());
}
