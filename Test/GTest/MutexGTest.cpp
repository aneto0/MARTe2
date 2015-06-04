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



TEST_F(MutexGTest,TestSyncTimeout) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncTimeout(2000, 10));
}

TEST_F(MutexGTest,TestSyncFast) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncFast(10));
}

TEST_F(MutexGTest,TestSyncTimeoutFast) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncTimeoutFast(2000, 10));
}

TEST_F(MutexGTest,FakeLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.FakeLock());
}

TEST_F(MutexGTest,DeadLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.DeadLock());
}

TEST_F(MutexGTest,KillWithLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.KillWithLock());
}

TEST_F(MutexGTest,TestSync) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestSync(10));
}
