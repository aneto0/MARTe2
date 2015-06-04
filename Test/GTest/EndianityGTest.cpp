/*
 *  EndianityGTest.cpp
 *
 *  Created on: Feb 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "EndianityTest.h"

class EndianityGTest: public ::testing::Test {
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

TEST_F(EndianityGTest,Uint16Test) {
    EndianityTest< uint16 > uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.All());
}

TEST_F(EndianityGTest,Int16Test) {
    EndianityTest< int16 > int16Test(0xAABB);
    ASSERT_TRUE(int16Test.All());
}

TEST_F(EndianityGTest,Uint32Test) {
    EndianityTest< uint32 > uint32Test(0xFFEEDDCC);
    ASSERT_TRUE(uint32Test.All());
}

TEST_F(EndianityGTest,Int32Test) {
    EndianityTest< int32 > int32Test(0xAABBCCDD);
    ASSERT_TRUE(int32Test.All());
}

TEST_F(EndianityGTest,Uint64Test) {
    EndianityTest< uint64 > uint64Test(0xFFEEDDCCBBAA1122);
    ASSERT_TRUE(uint64Test.All());
}

TEST_F(EndianityGTest,Int64Test) {
    EndianityTest< int64 > int64Test(0xAABBCCDDEEFF3344);
    ASSERT_TRUE(int64Test.All());
}

TEST_F(EndianityGTest,FloatTest) {
    EndianityTest< float > floatTest(0.12345f);
    ASSERT_TRUE(floatTest.All());
}

TEST_F(EndianityGTest,DoubleTest) {
    EndianityTest< double > doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.All());
}

