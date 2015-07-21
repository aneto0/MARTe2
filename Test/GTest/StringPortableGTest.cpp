/*
 *  StringPortableGTest.cpp
 *
 *  Created on: Mar 27, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "StringPortableTest.h"

class StringPortableGTest: public ::testing::Test {
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

TEST_F(StringPortableGTest,TestLength) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestLength("Hello"));
}


TEST_F(StringPortableGTest,TestEqual) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestEqual("Hello", "Hello"));
}


TEST_F(StringPortableGTest,TestCopy) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopy("Hello"));
}


TEST_F(StringPortableGTest,TestAppend) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestAppend("Hello", "World"));
}

TEST_F(StringPortableGTest,TestSearch) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearch());
}

TEST_F(StringPortableGTest,TestToken) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestToken());
}


TEST_F(StringPortableGTest,TestNull) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestNull());
}
