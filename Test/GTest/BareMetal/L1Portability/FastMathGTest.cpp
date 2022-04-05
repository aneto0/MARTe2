/**
 * @file FastMathGTest.cpp
 * @brief Source file for class FastMathGTest
 * @date 26/06/2015
 * @author Giuseppe Ferro
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
 * the class FastMathGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "FastMathTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_FastMathGTest,TestFloatToInt32) {
    FastMathTest mathTest;
    ASSERT_TRUE(mathTest.TestFloatToInt32(2.0f, 2));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSin) {
    FastMathTest mathTest;
    float expected = 0.0f;
    ASSERT_TRUE(mathTest.TestSin(0.0f, expected));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCos) {
    FastMathTest mathTest;
    ASSERT_TRUE(mathTest.TestCos(0.0f, 1.0f));
}

TEST(BareMetal_L1Portability_FastMathGTest,All) {
    FastMathTest mathTest;
    ASSERT_TRUE(mathTest.All());
}

TEST(BareMetal_L1Portability_FastMathGTest,CircleTest) {
    FastMathTest mathTest;
    ASSERT_TRUE(mathTest.CircleTest());
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint8_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(0U,0U));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint8_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(4U,2U));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint8_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(255U,15U));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint16_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint16(0U,0U));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint16_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(4U,2U));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint16_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint16(65535U,255U));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint32_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint32(0LU,0LU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint32_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint32(4LU,2LU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint32_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint32(4294967295LU,65535LU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint64_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint64(0LLU,0LLU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint64_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint64(4LLU,2LLU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootUint64_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint64(18446744073709551615LLU,4294967295LLU));
}

/* INT POSITIVE*/

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt8_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(0,0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt8_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(4,2));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt8_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(127,11));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt16_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(0,0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt16_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(4,2));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt16_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(32767,181));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt32_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(0L,0L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt32_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(4L,2L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt32_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(2147483647L,46340L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt64_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(0LL,0LL));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt64_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(4LL,2LL));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt64_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(9223372036854775807LL,3037000499LL));
}

/* INT Negative*/

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt8_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(-0,0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt8_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(-4,2));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt8_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(-127,11));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt16_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(-0,0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt16_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(-4,2));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt16_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(-32767,181));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt32_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(-0L,0L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt32_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(-4L,2L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt32_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(-2147483647L,46340L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt64_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(-0LL,0LL));
}
/* Problems with this test */
TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt64_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(-4LL,2LL));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootInt64_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(-9223372036854775807LL,3037000499LL));
}

/* FLOAT POSITIVE*/

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootFloat32_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(0.0,0.0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootFloat32_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(4.0,2.0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootFloat64_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootFloat64(0.0,0.0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestSquareRootFloat64_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootFloat64(4.0,2.0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint8_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint8>(4,5,0,20));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint8_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint8>(255,255,254,1));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint16_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint16>(4,5,0,20));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint16_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint16>(65535,65535,65534,1));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint32_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint32>(4LU,5LU,0LU,20LU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint32_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint32>(4294967295LU,4294967295LU,4294967294LU,1LU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint64_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint64>(4LLU,5LLU,0LLU,20LLU));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyUint64_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<uint64>(18446744073709551615LLU,18446744073709551615LLU,18446744073709551614LLU,1LLU));
}


TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt8_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int8>(4,5,0,20));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt8_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int8>(127,127,63,1));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt16_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int16>(4,5,0,20));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt16_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int16>(32767,32767,16383,1));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt32_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int32>(4L,5L,0L,20L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt32_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int32>(2147483647L,2147483647L,1073741823L,1L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt64_4_5) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int64>(4LL,5LL,0LL,20LL));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt64_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int64>(9223372036854775807LL,9223372036854775807LL,4611686018427387903LL,1LL));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt8_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int8>(-128,-128,64,0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt16_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int16>(-32768,-32768,16384,0));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt32_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int32>(-2147483648L,-2147483648L,1073741824L,0L));
}

TEST(BareMetal_L1Portability_FastMathGTest,TestCompleteMultiplyInt64_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestCompleteMultiply<int64>(-9223372036854775807LL,-9223372036854775807LL,4611686018427387903LL,1LL));
}
