/*
 *  SleepGTest.cpp
 *
 *  Created on: Feb 13, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "SleepTest.h"

class SleepGTest: public ::testing::Test {
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

TEST_F(SleepGTest,SleepTest) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.All());
}

