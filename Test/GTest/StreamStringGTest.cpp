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


TEST_F(StreamStringGTest,TestRead_minor_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld",5));
}

TEST_F(StreamStringGTest,TestRead_equal_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld",10));
}

TEST_F(StreamStringGTest,TestRead_bigger_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld",20));
}


TEST_F(StreamStringGTest,TestWrite_minor_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld",5));
}

TEST_F(StreamStringGTest,TestWrite_equal_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld",10));
}

TEST_F(StreamStringGTest,TestWrite_bigger_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld",20));
}

TEST_F(StreamStringGTest,TestUnbufferedWrite) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedWrite());
}

TEST_F(StreamStringGTest,TestUnbufferedRead) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedRead());
}
TEST_F(StreamStringGTest,TestUnbufferedSize) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedSize());
}
TEST_F(StreamStringGTest,TestUnbufferedSeek) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedSeek());
}
TEST_F(StreamStringGTest,TestUnbufferedRelativeSeek) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedRelativeSeek());
}
TEST_F(StreamStringGTest,TestUnbufferedPosition) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedPosition());
}
TEST_F(StreamStringGTest,TestUnbufferedSetSize) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestUnbufferedSetSize());
}

TEST_F(StreamStringGTest, TestSeek_in_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 16, true));
}

TEST_F(StreamStringGTest, TestSeek_out_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 33, false));
}






/*
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
*/
