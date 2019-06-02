/**
 * @file ShiftGTest.cpp
 * @brief Source file for class ShiftGTest
 * @date 19/10/2015
 * @author Llorenç Capellà
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class ShiftGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

#include "TestSupport.h"
#include "ShiftTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(ShiftGTest, TestLogicalRightSaveShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 8;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -8;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftUint8) {
    ShiftTest myShift;
    uint8 number = 8;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftUint16) {
    ShiftTest myShift;
    uint16 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftInt16) {
    ShiftTest myShift;
    int16 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftUint32) {
    ShiftTest myShift;
    uint32 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftInt32) {
    ShiftTest myShift;
    int32 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftUint64) {
    ShiftTest myShift;
    uint64 number = 1234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftInt64) {
    ShiftTest myShift;
    int64 number = 1234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest, TestLogicalRightSaveShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(ShiftGTest,TestLogicalLeftSaveShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 15;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest,TestLogicalLeftSaveShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -15;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest,TestLogicalLeftSaveShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 23489123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest,TestLogicalLeftSaveShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -23489123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest, TestLogicalLeftSaveShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest, TestLogicalLeftSaveShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest, TestLogicalLeftSaveShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(ShiftGTest, TestLogicalLeftSaveShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}


TEST(ShiftGTest,TestMathematicRightSafeShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(ShiftGTest,TestMathematicRightSafeShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(ShiftGTest,TestMathematicRightSafeShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 123456;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(ShiftGTest,TestMathematicRightSafeShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -123456;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}


TEST(ShiftGTest, TestMathematicRightSafeShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(ShiftGTest, TestMathematicRightSafeShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(ShiftGTest, TestMathematicRightSafeShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(ShiftGTest, TestMathematicRightSafeShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}


TEST(ShiftGTest,TestMathematicLeftSafeShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -56;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest,TestMathematicLeftSafeShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 56;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest,TestMathematicLeftSafeShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 52453456;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest,TestMathematicLeftSafeShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -52453456;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest, TestMathematicLeftSafeShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest, TestMathematicLeftSafeShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest, TestMathematicLeftSafeShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(ShiftGTest, TestMathematicLeftSafeShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}
