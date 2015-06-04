/*
 *  QueueHolderGTest.cpp
 *
 *  Created on: Mar 16, 2015
 */


#include <limits.h>
#include "gtest/gtest.h"
#include "QueueHolderTest.h"

class QueueHolderGTest: public ::testing::Test {
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

TEST_F(QueueHolderGTest,TestAll) {
    QueueHolderTest queueholdertest;
    ASSERT_TRUE(queueholdertest.TestAll());
}

TEST_F(QueueHolderGTest,TestNULLConditions) {
    QueueHolderTest queueholdertest;
    ASSERT_TRUE(queueholdertest.TestNULLConditions());
}
