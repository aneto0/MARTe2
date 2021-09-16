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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "ShiftTest.h"
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 8;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -8;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftUint8) {
    ShiftTest myShift;
    uint8 number = 8;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftUint16) {
    ShiftTest myShift;
    uint16 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftInt16) {
    ShiftTest myShift;
    int16 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftUint32) {
    ShiftTest myShift;
    uint32 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftInt32) {
    ShiftTest myShift;
    int32 number = 12345;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftUint64) {
    ShiftTest myShift;
    uint64 number = 1234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftInt64) {
    ShiftTest myShift;
    int64 number = 1234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalRightSaveShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestLogicalRightSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestLogicalLeftSaveShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 15;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestLogicalLeftSaveShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -15;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestLogicalLeftSaveShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 23489123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestLogicalLeftSaveShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -23489123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalLeftSaveShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalLeftSaveShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalLeftSaveShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestLogicalLeftSaveShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestLogicalLeftSaveShift(number));
}


TEST(BareMetal_L0Types_ShiftGTest,TestMathematicRightSafeShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestMathematicRightSafeShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestMathematicRightSafeShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 123456;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestMathematicRightSafeShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -123456;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}


TEST(BareMetal_L0Types_ShiftGTest, TestMathematicRightSafeShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicRightSafeShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicRightSafeShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicRightSafeShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestMathematicRightSafeShift(number));
}


TEST(BareMetal_L0Types_ShiftGTest,TestMathematicLeftSafeShiftNegativeInt8) {
    ShiftTest myShift;
    int8 number = -56;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestMathematicLeftSafeShiftPositiveInt8) {
    ShiftTest myShift;
    int8 number = 56;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestMathematicLeftSafeShiftPositiveInt64) {
    ShiftTest myShift;
    int64 number = 52453456;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest,TestMathematicLeftSafeShiftNegativeInt64) {
    ShiftTest myShift;
    int64 number = -52453456;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicLeftSafeShiftDoubleInteger_Unsigned) {
    ShiftTest myShift;
    DoubleInteger<uint64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicLeftSafeShiftDoubleInteger_Signed) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 12345671234567;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicLeftSafeShiftPositive_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = 123123123;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}

TEST(BareMetal_L0Types_ShiftGTest, TestMathematicLeftSafeShiftNegative_DoubleInteger) {
    ShiftTest myShift;
    DoubleInteger<int64> number = -123123123;
    ASSERT_TRUE(myShift.TestMathematicLeftSaveShift(number));
}
