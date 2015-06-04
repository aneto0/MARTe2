/*
 *  LoadableGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "LoadableLibraryTest.h"

class LoadableLibraryGTest: public ::testing::Test {
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



TEST_F(LoadableLibraryGTest,TestOpenCloseLibrary) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestOpenCloseLibrary("libm.so"));
}

TEST_F(LoadableLibraryGTest,TestLoadFunction) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestLoadFunction("libm.so", "cos"));
}


