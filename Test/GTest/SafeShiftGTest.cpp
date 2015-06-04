/*
 *  ShiftGTest.cpp
 *
 *  Created on: Feb 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "SafeShiftTest.h"

class SafeShiftGTest: public ::testing::Test {
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

TEST_F(SafeShiftGTest,TestLogicalRightShift) {
    SafeShiftTest myShift;
    ASSERT_TRUE(myShift.TestLogicalRightShift());
}

TEST_F(SafeShiftGTest,TestLogicalLeftShift) {
    SafeShiftTest myShift;
    ASSERT_TRUE(myShift.TestLogicalLeftShift());
}

TEST_F(SafeShiftGTest,TestMathematicRightShift) {
    SafeShiftTest myShift;
    ASSERT_TRUE(myShift.TestMathematicRightShift());
}

TEST_F(SafeShiftGTest,TestMathematicLeftShift) {
    SafeShiftTest myShift;
    ASSERT_TRUE(myShift.TestMathematicLeftShift());
}
