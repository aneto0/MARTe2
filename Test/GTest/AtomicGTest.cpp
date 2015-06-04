/*
 *  AtomicGTest.cpp
 *
 *  Created on: Feb 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "AtomicTest.h"

class AtomicGTest: public ::testing::Test {
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

TEST_F(AtomicGTest,Int32Test) {
    int32 testValue = 32;
    AtomicTest< int32 > int32Test(testValue);
    ASSERT_TRUE(int32Test.All());
}

TEST_F(AtomicGTest,Int16Test) {
    int16 testValue = 16;
    AtomicTest< int16 > int16Test(testValue);
    ASSERT_TRUE(int16Test.All());
}

TEST_F(AtomicGTest,Int8Test) {
    int8 testValue = 8;
    AtomicTest< int8 > int8Test(testValue);
    ASSERT_TRUE(int8Test.All());
}

TEST_F(AtomicGTest,ExchangeTest) {
    int32 testValue = 32;
    AtomicTest< int32 > exchangeTest(testValue);
    ASSERT_TRUE(exchangeTest.TestExchange());
}

TEST_F(AtomicGTest,AddTest) {
    int32 testValue = 32;
    AtomicTest< int32 > addTest(testValue);
    ASSERT_TRUE(addTest.TestAdd());
}

TEST_F(AtomicGTest,SubTest) {
    int32 testValue = 32;
    AtomicTest< int32 > subTest(testValue);
    ASSERT_TRUE(subTest.TestSub());
}
