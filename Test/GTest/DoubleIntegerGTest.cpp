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
   /*

    DoubleInteger< int32 > sbit64(0xf000000000000000);

    //!= operator
    if (sbit64 != 0xf000000000000000) {
    return false;
    }

    //Math shift with sign extension.
    if ((sbit64 >> 60) != -1) {
    return false;
    }

    //Copy bit a bit.
    DoubleInteger< uint32 > ubit64(0xf000000000000000);

    //Math shift without sign extension.
    if ((ubit64 >> 60) != 0xf) {
    return false;
    }

    sbit64 = 0xf;

    //left shift.
    if ((sbit64 << 4) != 0xf0) {
    return false;
    }

    //left shift.
    if ((sbit64 << 63) != 0x8000000000000000) {
    return false;
    }
    */
    DoubleIntegerTest<int32> numbertest;

    const testTableDI testTable[]={
            {(DoubleInteger<int32>)0xf000000000000000, 60, (DoubleInteger<int32>)-1},
            {(DoubleInteger<int32>)0, 0xff, (DoubleInteger<int32>)0}
    };

    ASSERT_TRUE(numbertest.TestShift(testTable));
}

TEST_F(DoubleIntegerGTest,TestLogicalOperators) {
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestLogicalOperators());
}

TEST_F(DoubleIntegerGTest,TestMathematicOperators) {
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestMathematicOperators());
}


