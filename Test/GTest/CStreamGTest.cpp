/*
 *  CStreamGTest.cpp
 *
 *  Created on: Mar 10, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "CStreamTest.h"

class CStreamGTest: public ::testing::Test {
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



TEST_F(CStreamGTest,TestCPut) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestCPut('H'));
}

TEST_F(CStreamGTest,TestCGet) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestCGet('W'));
}



TEST_F(CStreamGTest,TestCRead) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestCRead("Hello"));
}

TEST_F(CStreamGTest,TestCWrite) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestCWrite("World"));
}

TEST_F(CStreamGTest,TestPrintInt32) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestPrintInt32("-1234","4294966062","FFFFFB2E","37777775456",-1234));
}

TEST_F(CStreamGTest,TestPrintInt64) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestPrintInt64("1099500000000","FFFF4E9300","17777723511400",(int64)1099500000000));
}


TEST_F(CStreamGTest,TestPrintDouble) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestPrintDouble());
}

TEST_F(CStreamGTest,TestPrintString) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestPrintString("Hello World"));
}

TEST_F(CStreamGTest,TestCPrintf) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TestCPrintf());
}


TEST_F(CStreamGTest,TokenTest) {
    CStreamTest cstreamtest;
    ASSERT_TRUE(cstreamtest.TokenTest());
}


