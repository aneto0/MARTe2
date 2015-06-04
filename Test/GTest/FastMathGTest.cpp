/*
 *  FastMathGTest.cpp
 *
 *  Created on: Feb 13, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "FastMathTest.h"

class FastMathGTest: public ::testing::Test {
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

TEST_F(FastMathGTest,FastMathTest) {
    FastMathTest fastMathTest;
    ASSERT_TRUE(fastMathTest.All());
}

TEST_F(FastMathGTest,CircleTest) {
    FastMathTest fastMathTest;
    ASSERT_TRUE(fastMathTest.CircleTest());
}
