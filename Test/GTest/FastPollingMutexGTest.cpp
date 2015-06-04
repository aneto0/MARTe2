/*
 *  MutexGTest.cpp
 *
 *  Created on: Mar 9, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "FastPollingMutexTest.h"

class FastPollingMutexGTest: public ::testing::Test {
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



TEST_F(FastPollingMutexGTest,TestSyncTimeout) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncTimeout(2000, 20));
}


TEST_F(FastPollingMutexGTest,FakeLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.FakeLock());
}

TEST_F(FastPollingMutexGTest,DeadLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.DeadLock());
}

TEST_F(FastPollingMutexGTest,KillWithLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.KillWithLock());
}

TEST_F(FastPollingMutexGTest,TestSync) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestSync(10));
}
