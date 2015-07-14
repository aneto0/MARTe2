/*
 *  MutexGTest.cpp
 *
 *  Created on: Feb 20, 2015
 */
#include <FastPollingMutexSemTest.h>
#include <limits.h>
#include "gtest/gtest.h"

TEST(FastPollingMutexSemGTest,TestConstructor) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestConstructor());
}


TEST(FastPollingMutexSemGTest,TestCreateNoLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false));
}

TEST(FastPollingMutexSemGTest,TestCreateLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST(FastPollingMutexSemGTest,TestFastLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(50, TTInfiniteWait));
}

TEST(FastPollingMutexSemGTest,TestFastLockWithFiniteTimeout) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(50, 10000000));
}

TEST(FastPollingMutexSemGTest,TestFastUnLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(50, TTInfiniteWait));
}

TEST(FastPollingMutexSemGTest,TestFastUnLockWithFiniteTimeout) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(50, 10000000));
}

TEST(FastPollingMutexSemGTest,TestFastLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLock(500, 1));
}

TEST(FastPollingMutexSemGTest,TestFastUnLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLock(500, 1));
}

TEST(FastPollingMutexSemGTest,TestFastLockErrorCode) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockErrorCode());
}

TEST(FastPollingMutexSemGTest,TestFastTryLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLock(500));
}

TEST(FastPollingMutexSemGTest,TestLocked) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLocked());
}

TEST(FastPollingMutexSemGTest,TestRecursive) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive());
}

