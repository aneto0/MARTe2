#include <limits.h>

#include "StaticQueueHolderTest.h"
#include "gtest/gtest.h"

class StaticQueueHolderGTest: public ::testing::Test {
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

TEST_F(StaticQueueHolderGTest, QueueAddTest) {
    StaticQueueHolderTest queueAddTest;
    ASSERT_TRUE(queueAddTest.QueueAddTest());
}

TEST_F(StaticQueueHolderGTest, QueueExtractTest) {
    StaticQueueHolderTest queueExtractTest;
    ASSERT_TRUE(queueExtractTest.QueueExtractTest());
}

TEST_F(StaticQueueHolderGTest, QueQuePeekTest) {
    StaticQueueHolderTest queuePeekTest;
    ASSERT_TRUE(queuePeekTest.QueuePeekTest());
}

