/*
 *  AtomicGTest.cpp
 *
 *  Created on: Feb 27, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "BasicConsoleTest.h"

class BasicConsoleGTest: public ::testing::Test {
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

TEST_F(BasicConsoleGTest,TestOpen) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpen());
}

TEST_F(BasicConsoleGTest,TestWrite) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWrite("Hello\nWorld\n", 3));
}

//This tests need user intervent. Do not uncomment for automatic tests.

 TEST_F(BasicConsoleGTest,TestRead) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestRead("Hello\n", 6));
 }

 TEST_F(BasicConsoleGTest,TestPaging) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestPaging(10,5,5));
 }
/*
 TEST_F(BasicConsoleGTest,TestPerfChar) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestPerfChar());
 }*/

 TEST_F(BasicConsoleGTest,TestNotImplemented) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestNotImplemented());
 }
