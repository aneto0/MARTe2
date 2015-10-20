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



TEST_F(BitSetToIntegerGTest,TestBitSetToBitSet_32) {
    BitSetToIntegerTest<uint32> bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet());
}

TEST_F(BitSetToIntegerGTest,TestBitSetToBitSet_16) {
    BitSetToIntegerTest<uint32> bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet());
}

TEST_F(BitSetToIntegerGTest,TestBitSetToBitSet_8) {
    BitSetToIntegerTest<uint8> bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet());
}

TEST_F(BitSetToIntegerGTest,TestBitSetToSignedInteger) {
    BitSetToIntegerTest<uint32> bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToSignedInteger());
}
/*
TEST_F(BitSetToIntegerGTest,TestBitSetToUnsignedInteger) {
    BitSetToIntegerTest<uint32> bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToUnsignedInteger());
}
*/
TEST_F(BitSetToIntegerGTest,TestIntegerToBitSet) {
    BitSetToIntegerTest<uint32> bitsettest;
    ASSERT_TRUE(bitsettest.TestIntegerToBitSet());
}


