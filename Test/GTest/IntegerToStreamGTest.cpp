/*
 *  IntegerToStreamGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "IntegerToStreamTest.h"

class IntegerToStreamGTest: public ::testing::Test {
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



TEST_F(IntegerToStreamGTest,TestDecimalMagnitude) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestDecimalMagnitude());
}

TEST_F(IntegerToStreamGTest,TestHexadecimalMagnitude) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestHexadecimalMagnitude());
}

TEST_F(IntegerToStreamGTest,TestOctalMagnitude) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestOctalMagnitude());
}

TEST_F(IntegerToStreamGTest,TestBinaryMagnitude) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestBinaryMagnitude());
}


TEST_F(IntegerToStreamGTest,TestDecimalStream) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestDecimalStream());
}


TEST_F(IntegerToStreamGTest,TestHexadecimalStream) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestHexadecimalStream());
}


TEST_F(IntegerToStreamGTest,TestOctalStream) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestOctalStream());
}


TEST_F(IntegerToStreamGTest,TestBinaryStream) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestBinaryStream());
}

TEST_F(IntegerToStreamGTest,TestIntegerToStream) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestIntegerToStream());
}

TEST_F(IntegerToStreamGTest,TestBitSetToStream) {
    IntegerToStreamTest numbertest;
    ASSERT_TRUE(numbertest.TestBitSetToStream());
}
