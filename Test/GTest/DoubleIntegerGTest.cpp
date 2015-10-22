/*
 *  DoubleIntegerGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "DoubleIntegerTest.h"

class DoubleIntegerGTest: public ::testing::Test {
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


TEST_F(DoubleIntegerGTest,TestRightShift_int64) {
    const testTableShiftDI<int64> testTable[]={
            testTableShiftDI<int64>(DoubleInteger<int64>(0xf000000000000000, 0), 124, -1),
            testTableShiftDI<int64>(DoubleInteger<int64>(0x8000000000000000, 0), 3, DoubleInteger<int64>(0xf000000000000000, 0)),
            testTableShiftDI<int64>(DoubleInteger<int64>(0x7000000000000000, 0), 62, DoubleInteger<int64>(1,0xc000000000000000)),
            testTableShiftDI<int64>(0, 0xff, 0)
    };
    DoubleIntegerTest<int64> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestLeftShift_uint64) {
    const testTableShiftDI<uint64> testTable[]={
            testTableShiftDI<uint64>(DoubleInteger<uint64>(0, 0x000000000000000f), 124, DoubleInteger<uint64>(0xf000000000000000,0)),
            testTableShiftDI<uint64>(DoubleInteger<uint64>(0x1800000000000000, 0), 4, DoubleInteger<uint64>(0x8000000000000000,0)),
            testTableShiftDI<uint64>(DoubleInteger<uint64>(0, 0x0000000000000007), 62, DoubleInteger<uint64>(1,0xc000000000000000)),
            testTableShiftDI<uint64>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint64> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}



TEST_F(DoubleIntegerGTest,TestRightShift_int32) {
    const testTableShiftDI<int32> testTable[]={
            testTableShiftDI<int32>(0xf000000000000000, 60, -1),
            testTableShiftDI<int32>(0x8000000000000000, 3, 0xf000000000000000),
            testTableShiftDI<int32>(0x7000000000000000, 30, 0x1c0000000),
            testTableShiftDI<int32>(0, 0xff, 0)
    };
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestLeftShift_int32) {
    const testTableShiftDI<int32> testTable[]={
            testTableShiftDI<int32>(0x000000000000000f, 60, 0xf000000000000000),
            testTableShiftDI<int32>(0x1800000000000000, 4, 0x8000000000000000),
            testTableShiftDI<int32>(0x0000000000000007, 30, 0x1c0000000),
            testTableShiftDI<int32>(0, 0xff, 0)
    };
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}

TEST_F(DoubleIntegerGTest,TestRightShift_uint32) {


    const testTableShiftDI<uint32> testTable[]={
            testTableShiftDI<uint32>(0xf000000000000000, 60, 0xf),
            testTableShiftDI<uint32>(0x8000000000000000, 3, 0x1000000000000000),
            testTableShiftDI<uint32>(0x7000000000000000, 30, 0x1c0000000),
            testTableShiftDI<uint32>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestLeftShift_uint32) {
    const testTableShiftDI<uint32> testTable[]={
            testTableShiftDI<uint32>(0x000000000000000f, 60, 0xf000000000000000),
            testTableShiftDI<uint32>(0x1800000000000000, 4, 0x8000000000000000),
            testTableShiftDI<uint32>(0x0000000000000007, 30, 0x1c0000000),
            testTableShiftDI<uint32>(0, 0xff, 0)
    };    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestRightShift_int16) {
    const testTableShiftDI<int16> testTable[]={
            testTableShiftDI<int16>(0xf0000000, 30, -1),
            testTableShiftDI<int16>(0x80000000, 3, 0xf0000000),
            testTableShiftDI<int16>(0x70000000, 14, 0x1c000),
            testTableShiftDI<int16>(0, 0xff, 0)
    };
    DoubleIntegerTest<int16> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestLeftShift_uint16) {
    const testTableShiftDI<uint16> testTable[]={
            testTableShiftDI<uint16>(0x0000000f, 28, 0xf0000000),
            testTableShiftDI<uint16>(0x18000000, 4, 0x80000000),
            testTableShiftDI<uint16>(0x00000007, 14, 0x1c000),
            testTableShiftDI<uint16>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint16> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestRightShift_int8) {
    const testTableShiftDI<int8> testTable[]={
            testTableShiftDI<int8>(0xf000, 12, -1),
            testTableShiftDI<int8>(0x8000, 3, 0xf000),
            testTableShiftDI<int8>(0x7000, 6, 0x1c0),
            testTableShiftDI<int8>(0, 0xff, 0)
    };
    DoubleIntegerTest<int8> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestLeftShift_uint8) {
    const testTableShiftDI<uint8> testTable[]={
            testTableShiftDI<uint8>(0x000f, 13, 0xe000),
            testTableShiftDI<uint8>(0x1800, 4, 0x8000),
            testTableShiftDI<uint8>(0x0007, 6, 0x1c0),
            testTableShiftDI<uint8>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint8> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}


TEST_F(DoubleIntegerGTest,TestAndOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xfffffff00fffffff),
            testTableLogicDI<int32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestOrOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0xffffffffffffffff),
            testTableLogicDI<int32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xffffffffffffffff),
            testTableLogicDI<int32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestInvertOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(0xffffffff0fffffff, 0, 0x00000000f0000000),
            testTableLogicDI<int32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestAndOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<uint32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xfffffff00fffffff),
            testTableLogicDI<uint32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestOrOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0xffffffffffffffff),
            testTableLogicDI<uint32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xffffffffffffffff),
            testTableLogicDI<uint32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestInvertOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<uint32>(0xffffffff0fffffff, 0, 0x00000000f0000000),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestIsMajorOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(1, 0, 0),
            testTableLogicDI<int32>(0xffffffffffffffff, 0xfffffffffffffffe, 0),
            testTableLogicDI<int32>(0x7fffffffffffffff, 0x7ffffffffffffffe, 0),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}

TEST_F(DoubleIntegerGTest,TestIsMinorOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 1),
            testTableLogicDI<int32>(0xffffffffffffffff, 0xfffffffffffffffe, 1),
            testTableLogicDI<int32>(0xfffffffffffffffe, 0xffffffffffffffffe, 0),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}






TEST_F(DoubleIntegerGTest,TestLogicalOperators) {
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestLogicalOperators());
}

TEST_F(DoubleIntegerGTest,TestMathematicOperators) {
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestMathematicOperators());
}


