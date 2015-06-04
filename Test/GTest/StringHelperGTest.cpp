/*
 *  StringHelperGTest.cpp
 *
 *  Created on: Mar 30, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "StringHelperTest.h"

class StringHelperGTest: public ::testing::Test {
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

TEST_F(StringHelperGTest,TestLength) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestLength("Hello"));
}


TEST_F(StringHelperGTest,TestCompare) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompare("Hello", "Hello"));
}


TEST_F(StringHelperGTest,TestCopy) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopy("Hello"));
}


TEST_F(StringHelperGTest,TestConcatenate) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenate());
}

TEST_F(StringHelperGTest,TestSearch) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearch());
}
/*
TEST_F(StringHelperGTest,TestToken) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestToken());
}*/
