/*
 *  StringPortableGTest.cpp
 *
 *  Created on: Mar 27, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "StringPortableTest.h"


TEST(StringPortableGTest,TestLength) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestLength("Hello"));
}

TEST(StringPortableGTest,TestCompare) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCompare("Hello", "Hello"));
}

TEST(StringPortableGTest,TestCompareN) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCompareN());
}

TEST(StringPortableGTest,TestCopy) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopy("Hello"));
}

TEST(StringPortableGTest,TestCopyN) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopyN());
}

TEST(StringPortableGTest,TestConcatenateNoResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNoResult());
}
TEST(StringPortableGTest,TestConcatenateNNoResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNNoResult());
}


TEST(StringPortableGTest,TestConcatenateWithResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateWithResult());
}
TEST(StringPortableGTest,TestConcatenateNWithResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNWithResult());
}




TEST(StringPortableGTest,TestSearchChars) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchChars());
}

TEST(StringPortableGTest,TestSearchChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchChar());
}

TEST(StringPortableGTest,TestSearchLastChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchLastChar());
}
TEST(StringPortableGTest,TestSearchString) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchString());
}

TEST(StringPortableGTest,TestSearchIndex) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchIndex());
}


TEST(StringPortableGTest,TestTokenByChars) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByChars());
}

TEST(StringPortableGTest,TestTokenByString) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByString());
}

TEST(StringPortableGTest,TestSubstr) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSubstr());
}

TEST(StringPortableGTest,TestSetChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSetChar());
}
