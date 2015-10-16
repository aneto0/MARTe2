/*
 *  BufferedStreamGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include "BufferedStreamTest.h"
#include <limits.h>
#include "gtest/gtest.h"

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

TEST_F(BufferedStreamGTest,TestDefaultConstructor) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestDefaultConstructor());
}

TEST_F(BufferedStreamGTest,TestAnyType) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestAnyType());
}

TEST_F(BufferedStreamGTest,TestGetTimeout) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetTimeout());
}

TEST_F(BufferedStreamGTest,TestSetTimeout) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetTimeout());
}

TEST_F(BufferedStreamGTest,TestGetToken_TableTerminators_BufferSize_1) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(1, &TokenTestTableTerminators[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableTerminators_BufferSize_2) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(2, &TokenTestTableTerminators[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableTerminators_BufferSize_8) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(8, &TokenTestTableTerminators[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableTerminators_BufferSize_32) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(32, &TokenTestTableTerminators[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableSkipCharacters_BufferSize_1) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(1, &TokenTestTableSkipCharacters[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableSkipCharacters_BufferSize_2) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(2, &TokenTestTableSkipCharacters[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableSkipCharacters_BufferSize_8) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(8, &TokenTestTableSkipCharacters[0]));
}

TEST_F(BufferedStreamGTest,TestGetToken_TableSkipCharacters_BufferSize_32) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(32, &TokenTestTableSkipCharacters[0]));
}

TEST_F(BufferedStreamGTest,TestSkipTokens_BufferSize_1) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(1, &SkipTokensTestTable[0]));
}

TEST_F(BufferedStreamGTest,TestSkipTokens_BufferSize_2) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(2, &SkipTokensTestTable[0]));
}

TEST_F(BufferedStreamGTest,TestSkipTokens_BufferSize_8) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(8, &SkipTokensTestTable[0]));
}

TEST_F(BufferedStreamGTest,TestSkipTokens_BufferSize_32) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(32, &SkipTokensTestTable[0]));
}

TEST_F(BufferedStreamGTest,TestGetLine_NoSkipCharacter_BufferSize_1) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(1, false));
}

TEST_F(BufferedStreamGTest,TestGetLine_NoSkipCharacter_BufferSize_2) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(2, false));
}

TEST_F(BufferedStreamGTest,TestGetLine_NoSkipCharacter_BufferSize_8) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(8, false));
}

TEST_F(BufferedStreamGTest,TestGetLine_NoSkipCharacter_BufferSize_32) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(32, false));
}

TEST_F(BufferedStreamGTest,TestGetLine_SkipCharacter_BufferSize_1) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(1, true));
}

TEST_F(BufferedStreamGTest,TestGetLine_SkipCharacter_BufferSize_2) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(2, true));
}

TEST_F(BufferedStreamGTest,TestGetLine_SkipCharacter_BufferSize_8) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(8, true));
}

TEST_F(BufferedStreamGTest,TestGetLine_SkipCharacter_BufferSize_32) {
    BufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(32, true));
}

/*TEST_F(BufferedStreamGTest,TestGetCAndPutC) {
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
}*/






