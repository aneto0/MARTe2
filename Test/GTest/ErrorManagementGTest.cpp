/*
 *  ErrorManagementGTest.cpp
 *
 *  Created on: May 19, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "ErrorManagementTest.h"

class ErrorManagementGTest: public ::testing::Test {
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



TEST_F(ErrorManagementGTest,TestReportError) {
    ErrorManagementTest errortest;
    ASSERT_TRUE(errortest.TestReportError(Information,"Information","This is an error","FileError", 1, "TestReportError",10));
}


