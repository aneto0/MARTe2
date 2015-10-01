/*
 *  IntegerToStreamGTest.cpp
 *
 *  Created on: May 4, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "BitSetToIntegerTest.h"

class BitSetToIntegerGTest: public ::testing::Test {
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



TEST_F(BitSetToIntegerGTest,TestBitSetToBitSet) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet());
}

TEST_F(BitSetToIntegerGTest,TestBitSetToInteger) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToInteger());
}

TEST_F(BitSetToIntegerGTest,TestIntegerToBitSet) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestIntegerToBitSet());
}


