/*
 * StaticListTemplateGTest.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: shareuser
 */

#include <limits.h>

#include "StaticListTemplateTest.h"
#include "gtest/gtest.h"

class StaticListTemplateGTest: public ::testing::Test {
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

TEST_F(StaticListTemplateGTest, ListAddTestInt32) {
    StaticListTemplateTest listAddTestInt32;
    ASSERT_TRUE(listAddTestInt32.ListAddTestInt32());
}

TEST_F(StaticListTemplateGTest, ListAddTestInt64) {
    StaticListTemplateTest listAddTestInt64;
    ASSERT_TRUE(listAddTestInt64.ListAddTestInt64());
}

TEST_F(StaticListTemplateGTest, ListExtractTestInt64) {
    StaticListTemplateTest listExtractTestInt64;
    ASSERT_TRUE(listExtractTestInt64.ListExtractTestInt64());
}

TEST_F(StaticListTemplateGTest, ListPeekTest) {
    StaticListTemplateTest listPeekTest;
    ASSERT_TRUE(listPeekTest.ListPeekTest());
}

TEST_F(StaticListTemplateGTest, ListDeleteTest) {
    StaticListTemplateTest listDeleteTest;
    ASSERT_TRUE(listDeleteTest.ListDeleteTest());
}

TEST_F(StaticListTemplateGTest, ListFindTest) {
    StaticListTemplateTest listFindTest;
    ASSERT_TRUE(listFindTest.ListFindTest());
}

TEST_F(StaticListTemplateGTest, ListInsertTest) {
    StaticListTemplateTest listInsertTest;
    ASSERT_TRUE(listInsertTest.ListInsertTest());
}

