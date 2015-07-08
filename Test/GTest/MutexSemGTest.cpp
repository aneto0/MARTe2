/*
 *  MutexSemGTest.cpp
 *
 *  Created on: Feb 20, 2015
 */
#include <limits.h>
#include <MutexSemTest.h>
#include "gtest/gtest.h"

class MutexSemGTest: public ::testing::Test {
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

TEST_F(MutexSemGTest,TestCreateNoRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate( false));
}

TEST_F(MutexSemGTest,TestCreateRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST_F(MutexSemGTest,TestCloseNoRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(false));
}

TEST_F(MutexSemGTest,TestCloseRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(true));
}

TEST_F(MutexSemGTest,TestLock) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(50, TTInfiniteWait));
}

TEST_F(MutexSemGTest,TestLockWithFiniteTimeout) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(1, 10000000));
}

TEST_F(MutexSemGTest,TestUnLock) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, TTInfiniteWait));
}

TEST_F(MutexSemGTest,TestUnLockWithFiniteTimeout) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, 10000000));
}

TEST_F(MutexSemGTest,TestLockWithSmallFiniteTimeoutToFail) {
    MutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestLock(500, 1));
}

TEST_F(MutexSemGTest,TestUnLockWithSmallFiniteTimeoutToFail) {
    MutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestUnLock(500, 1));
}

TEST_F(MutexSemGTest,TestLockErrorCode) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLockErrorCode());
}

TEST_F(MutexSemGTest,TestIsRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestIsRecursive());
}

TEST_F(MutexSemGTest,TestRecursiveOn) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(true));
}

TEST_F(MutexSemGTest,TestRecursiveOff) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(false));
}

TEST_F(MutexSemGTest,TestCopyConstructor) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCopyConstructor());
}
