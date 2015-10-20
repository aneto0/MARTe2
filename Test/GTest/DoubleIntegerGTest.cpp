/*
 *  DoubleIntegerGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "DoubleIntegerTest.h"

class DoubleIntegerGTest: public ::testing::Test {
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



TEST_F(DoubleIntegerGTest,TestShift) {
    DoubleIntegerTest numbertest;
    ASSERT_TRUE(numbertest.TestShift());
}

TEST_F(DoubleIntegerGTest,TestLogicalOperators) {
    DoubleIntegerTest numbertest;
    ASSERT_TRUE(numbertest.TestLogicalOperators());
}

TEST_F(DoubleIntegerGTest,TestMathematicOperators) {
    DoubleIntegerTest numbertest;
    ASSERT_TRUE(numbertest.TestMathematicOperators());
}


