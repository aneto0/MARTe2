/*
 *  MutexGTest.cpp
 *
 *  Created on: Feb 20, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "FastPollingMutexTest.h"

TEST(FastPollingMutexGTest,TestConstructor) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestConstructor());
}


TEST(FastPollingMutexGTest,TestCreateNoLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false));
}

TEST(FastPollingMutexGTest,TestCreateLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST(FastPollingMutexGTest,TestFastLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(50, TTInfiniteWait));
}

TEST(FastPollingMutexGTest,TestFastLockWithFiniteTimeout) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(50, 10000000));
}

TEST(FastPollingMutexGTest,TestFastUnLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(50, TTInfiniteWait));
}

TEST(FastPollingMutexGTest,TestFastUnLockWithFiniteTimeout) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(50, 10000000));
}

TEST(FastPollingMutexGTest,TestFastLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLock(500, 1));
}

TEST(FastPollingMutexGTest,TestFastUnLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLock(500, 1));
}

TEST(FastPollingMutexGTest,TestFastLockErrorCode) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockErrorCode());
}

TEST(FastPollingMutexGTest,TestFastTryLock) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLock(500));
}

TEST(FastPollingMutexGTest,TestLocked) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestLocked());
}

TEST(FastPollingMutexGTest,TestRecursive) {
    FastPollingMutexTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive());
}

