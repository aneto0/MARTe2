/*
 *   ComplexGTest.cpp
 *
 *  Created on: Mar 17, 2015
 */


#include <limits.h>
#include "gtest/gtest.h"
#include "ComplexTest.h"

class ComplexGTest: public ::testing::Test {
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

TEST_F(ComplexGTest,TestConstructors) {
    ComplexTest complextest;
    ASSERT_TRUE(complextest.TestConstructors());
}

TEST_F(ComplexGTest,TestAddAndDifferenceOperators) {
    ComplexTest complextest;
    ASSERT_TRUE(complextest.TestAddAndDifferenceOperators());
}

TEST_F(ComplexGTest,TestProductOperators) {
    ComplexTest complextest;
    ASSERT_TRUE(complextest.TestProductOperators());
}

TEST_F(ComplexGTest,TestNormAndDivision) {
    ComplexTest complextest;
    ASSERT_TRUE(complextest.TestNormAndDivision());
}

TEST_F(ComplexGTest,TestMathOperations) {
    ComplexTest complextest;
    ASSERT_TRUE(complextest.TestMathOperations());
}
