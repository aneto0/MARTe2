/*
 *  LoadableGTest.cpp
 *
 *  Created on: Mar 6, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "MemoryTest.h"

class MemoryGTest: public ::testing::Test {
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

TEST_F(MemoryGTest,TestMallocAndFree) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestMallocAndFree(100));
}

TEST_F(MemoryGTest,TestRealloc) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestRealloc(100, 10));
}

TEST_F(MemoryGTest,TestMemoryStringDup) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestMemoryStringDup("Hello"));
}

TEST_F(MemoryGTest,TestSharedMemory) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestSharedMemory());
}

TEST_F(MemoryGTest,TestCopyAndMove) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestCopyAndMove());
}

TEST_F(MemoryGTest,TestSetAndSearch) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestSetAndSearch());
}
