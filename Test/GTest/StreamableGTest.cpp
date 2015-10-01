/*
 *  StreamaStringGTest.cpp
 *
 *  Created on: May 19, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "StreamableTest.h"

class StreamableGTest: public ::testing::Test {
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


TEST_F(StreamableGTest,TestSeek) {
    StreamableTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek());
}


TEST_F(StreamableGTest,TestPrint) {
    StreamableTest stringtest;
    ASSERT_TRUE(stringtest.TestPrint());
}


TEST_F(StreamableGTest,TestToken) {
    StreamableTest stringtest;
    ASSERT_TRUE(stringtest.TestToken());
}
