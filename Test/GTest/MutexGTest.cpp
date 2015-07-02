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

TEST_F(MutexGTest,TestCreateNoLockNoRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false, false));
}

TEST_F(MutexGTest,TestCreateNoLockRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false, true));
}

TEST_F(MutexGTest,TestCreateLockNoRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true, false));
}

TEST_F(MutexGTest,TestCreateLockRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true, true));
}

TEST_F(MutexGTest,TestCloseNoLockNoRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(false, false));
}

TEST_F(MutexGTest,TestCloseNoLockRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(false, true));
}

TEST_F(MutexGTest,TestCloseLockNoRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(true, false));
}

TEST_F(MutexGTest,TestCloseLockRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(true, true));
}

TEST_F(MutexGTest,TestLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(50, TTInfiniteWait));
}

TEST_F(MutexGTest,TestLockWithFiniteTimeout) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(50, 10000000));
}

TEST_F(MutexGTest,TestUnLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, TTInfiniteWait));
}

TEST_F(MutexGTest,TestUnLockWithFiniteTimeout) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, 10000000));
}

TEST_F(MutexGTest,TestLockWithSmallFiniteTimeoutToFail) {
    MutexTest mutextest;
    ASSERT_FALSE(mutextest.TestLock(500, 1));
}

TEST_F(MutexGTest,TestUnLockWithSmallFiniteTimeoutToFail) {
    MutexTest mutextest;
    ASSERT_FALSE(mutextest.TestUnLock(500, 1));
}

TEST_F(MutexGTest,TestFastLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(50, TTInfiniteWait));
}

TEST_F(MutexGTest,TestFastLockWithFiniteTimeout) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(50, 10000000));
}

TEST_F(MutexGTest,TestFastUnLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(50, TTInfiniteWait));
}

TEST_F(MutexGTest,TestFastUnLockWithFiniteTimeout) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(50, 10000000));
}

TEST_F(MutexGTest,TestFastLockWithSmallFiniteTimeoutToFail) {
    MutexTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLock(500, 1));
}

TEST_F(MutexGTest,TestFastUnLockWithSmallFiniteTimeoutToFail) {
    MutexTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLock(500, 1));
}

TEST_F(MutexGTest,TestLockErrorCode) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestLockErrorCode());
}

TEST_F(MutexGTest,TestFastLockErrorCode) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockErrorCode());
}

TEST_F(MutexGTest,TestFastTryLock) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLock(500));
}

TEST_F(MutexGTest,TestIsRecursive) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestIsRecursive());
}

TEST_F(MutexGTest,TestRecursiveOn) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(true));
}

TEST_F(MutexGTest,TestRecursiveOff) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(false));
}

TEST_F(MutexGTest,TestCopyConstructor) {
    MutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCopyConstructor());
}
