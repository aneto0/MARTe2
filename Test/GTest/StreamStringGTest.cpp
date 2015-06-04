/*
 *  StreamaStringGTest.cpp
 *
 *  Created on: May 14, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "StreamStringTest.h"

class StreamStringGTest: public ::testing::Test {
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

TEST_F(StreamStringGTest,TestGetC) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetC("HelloWorld"));
}

TEST_F(StreamStringGTest,TestPutC) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestPutC("HelloWorld"));
}


TEST_F(StreamStringGTest,TestRead) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld"));
}

TEST_F(StreamStringGTest,TestWrite) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld"));
}


TEST_F(StreamStringGTest,TestSeek) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek("ThisIsTheStringToRead"));
}


TEST_F(StreamStringGTest,TestOperators) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestOperators("ThisIsTheStringToRead","ThisIsTheStringToWrite"));
}


TEST_F(StreamStringGTest,TestPrint) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestPrint());
}


TEST_F(StreamStringGTest,TestToken) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestToken());
}
