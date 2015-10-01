/*
 *  BufferedStreamGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "BufferedStreamTest.h"

class BufferedStreamGTest: public ::testing::Test {
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

TEST_F(BufferedStreamGTest,TestGetC) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetC("HelloWorld"));
}

TEST_F(BufferedStreamGTest,TestPutC) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestPutC("HelloWorld"));
}


TEST_F(BufferedStreamGTest,TestGetCAndPutC) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetCAndPutC("HelloWorld"));
}


TEST_F(BufferedStreamGTest,TestRead) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead("HelloWorld"));
}

TEST_F(BufferedStreamGTest,TestWrite) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite("HelloWorld"));
}


TEST_F(BufferedStreamGTest,TestReadAndWrite) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestReadAndWrite("ThisIsTheStringToRead","ThisIsTheStringToWrite"));
}


TEST_F(BufferedStreamGTest,TestSeek) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSeek("ThisIsTheStringToRead","ThisIsTheStringToWrite"));
}


TEST_F(BufferedStreamGTest,TestSwitch) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSwitch("ThisIsTheStringToRead","ThisIsTheStringToWrite"));
}

TEST_F(BufferedStreamGTest,TestPrint) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestPrint());
}

TEST_F(BufferedStreamGTest,TestToken) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestToken());
}






