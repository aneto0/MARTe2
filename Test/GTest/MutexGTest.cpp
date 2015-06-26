/*
 *  MutexGTest.cpp
 *
 *  Created on: Feb 20, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "MutexTest.h"

class MutexGTest: public ::testing::Test {
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

TEST_F(MutexGTest,TestLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(500));
}

TEST_F(MutexGTest,TestUnLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock());
}

