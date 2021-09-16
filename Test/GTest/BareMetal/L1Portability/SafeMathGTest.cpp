/**
 * @file SafeMathGTest.cpp
 * @brief Source file for class SafeMathGTest
 * @date 17/04/2020
 * @author Dídac Magriñá
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
 * the class SafeMathGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "SafeMathTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitUint8) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<uint8>(0));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitUint16) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<uint16>(0));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitUint32) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<uint32>(0));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitUint64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<uint64>(0));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitInt8) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<int8>(0x80));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitInt16) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<int16>(0x8000));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitInt32) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<int32>(0x80000000));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitInt64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<int64>(0x8000000000000000));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitFloat32) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<float32>(0));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSignBitFloat64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSignBit<float64>(0));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAllOnesInt8) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAllOnes<int8>(0xFF));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAllOnesInt16) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAllOnes<int16>(0xFFFF));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAllOnesInt32) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAllOnes<int32>(0xFFFFFFFF));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAllOnesInt64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAllOnes<int64>(0xFFFFFFFFFFFFFFFF));
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionUint8_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionNoError<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionUint8_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionOverflow<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt8_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionNoError<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt8_NnoError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionNegativeNoError<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt8_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionOverflow<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt8_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionUnderflow<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionUint64_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionNoError<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionUint64_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionOverflow<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt64_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionNoError<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt64_NnoError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionNegativeNoError<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt64_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionOverflow<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestAdditionInt64_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestAdditionUnderflow<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionUint8_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionNoError<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionUint8_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionUnderflow<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt8_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionNoError<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt8_NnoError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionNegativeNoError<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt8_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionOverflow<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt8_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionUnderflow<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionUint64_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionNoError<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionUint64_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionUnderflow<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt64_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionNoError<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt64_NnoError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionNegativeNoError<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt64_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionOverflow<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestSubtractionInt64_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestSubtractionUnderflow<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationUint8_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationNoError<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationUint8_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationOverflow<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt8_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationNoError<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt8_NnoError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationNegativeNoError<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt8_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationOverflow<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt8_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationUnderflow<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationUint64_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationNoError<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationUint64_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationOverflow<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt64_noError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationNoError<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt64_NnoError) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationNegativeNoError<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt64_overflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationOverflow<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestMultiplicationInt64_underflow) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestMultiplicationUnderflow<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualUint8) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqual<uint8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualInt8) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqual<int8>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualUint64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqual<uint64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualInt64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqual<int64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualFloat32) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqual<float32>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualFloat64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqual<float64>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualEpsilonFloat32) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqualEpsilon<float32>());
}

TEST(BareMetal_L1Portability_SafeMathGTest,TestIsEqualEpsilonFloat64) {
    SafeMathTest mathTest;
    ASSERT_TRUE(mathTest.TestIsEqualEpsilon<float64>());
}
