/*
 *  ThreadsDatabaseGTest.cpp
 *
 *  Created on: Feb 25, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "ThreadsDatabaseTest.h"

class ThreadsDatabaseGTest: public ::testing::Test {
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

TEST_F(ThreadsDatabaseGTest,TestGetInfoAndLock) {
    ThreadsDatabaseTest TDtest;
    ASSERT_TRUE(TDtest.TestGetInfoAndLock(10));
}

TEST_F(ThreadsDatabaseGTest,TestRemoveEntry) {
    ThreadsDatabaseTest TDtest;
    ASSERT_TRUE(TDtest.TestRemoveEntry(100));
}

TEST_F(ThreadsDatabaseGTest,TestGetId) {
    ThreadsDatabaseTest TDtest;
    ASSERT_TRUE(TDtest.TestGetId(10));
}

TEST_F(ThreadsDatabaseGTest,TestTimeoutLock) {
    ThreadsDatabaseTest TDtest;
    ASSERT_TRUE(TDtest.TestTimeoutLock(500));
}
