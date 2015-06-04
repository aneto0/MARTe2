/*
 *  AtomicGTest.cpp
 *
 *  Created on: Feb 26, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "HighResolutionTimerTest.h"

class HighResolutionTimerGTest: public ::testing::Test {
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

TEST_F(HighResolutionTimerGTest,TestFrequency) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestFrequency());
}

TEST_F(HighResolutionTimerGTest,TestCounter) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestCounter(5.0));
}
