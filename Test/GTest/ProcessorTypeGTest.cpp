/*
 *  ProcessorGTest.cpp
 *
 *  Created on: Feb 13, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "ProcessorTypeTest.h"

class ProcessorTypeGTest: public ::testing::Test {
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

TEST_F(ProcessorTypeGTest,TestAssignmentOperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestAssignmentOperator());
}

TEST_F(ProcessorTypeGTest,TestOROperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestOROperator());
}

TEST_F(ProcessorTypeGTest,TestEqualityOperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestEqualityOperator());
}

TEST_F(ProcessorTypeGTest,TestInequalityOperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestInequalityOperator());
}

TEST_F(ProcessorTypeGTest,TestGetSetDefaultCPUs) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestGetSetDefaultCPUs());
}

TEST_F(ProcessorTypeGTest,TestConstructors) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestConstructors());
}
