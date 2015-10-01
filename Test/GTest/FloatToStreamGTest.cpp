/*
 *  FloatToStreamGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "FloatToStreamTest.h"

class FloatToStreamGTest: public ::testing::Test {
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

TEST_F(FloatToStreamGTest,TestFixedPoint) {
    FloatToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestFixedPoint());
}

TEST_F(FloatToStreamGTest,TestFixedRelativePoint) {
    FloatToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestFixedRelativePoint());
}

TEST_F(FloatToStreamGTest,TestEngeneering) {
    FloatToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestEngeneering());
}


TEST_F(FloatToStreamGTest,TestSmart) {
    FloatToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestSmart());
}

TEST_F(FloatToStreamGTest,TestExponential) {
    FloatToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestExponential());
}

TEST_F(FloatToStreamGTest,TestCompact) {
    FloatToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestCompact());
}


