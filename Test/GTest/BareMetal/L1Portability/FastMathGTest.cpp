/**
 * @file FastMathGTest.cpp
 * @brief Source file for class FastMathGTest
 * @date 26/06/2015
 * @author Giuseppe Ferr�
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
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "FastMathTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FastMathGTest,TestFloatToInt32) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.TestFloatToInt32(2.0f, 2));
}

TEST(FastMathGTest,TestSin) {
    FastMathTest MathTest;
    float expected = 0.0f;
    ASSERT_TRUE(MathTest.TestSin(0.0f, expected));
}

TEST(FastMathGTest,TestCos) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.TestCos(0.0f, 1.0f));
}

TEST(FastMathGTest,All) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.All());
}

TEST(FastMathGTest,CircleTest) {
    FastMathTest MathTest;
    ASSERT_TRUE(MathTest.CircleTest());
}

TEST(FastMathGTest,TestSquareRootUint8_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(0U,0U));
}

TEST(FastMathGTest,TestSquareRootUint8_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(4U,2U));
}

TEST(FastMathGTest,TestSquareRootUint8_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(255U,15U));
}

TEST(FastMathGTest,TestSquareRootUint16_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint16(0U,0U));
}

TEST(FastMathGTest,TestSquareRootUint16_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint8(4U,2U));
}

TEST(FastMathGTest,TestSquareRootUint16_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint16(65535U,255U));
}

TEST(FastMathGTest,TestSquareRootUint32_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint32(0LU,0LU));
}

TEST(FastMathGTest,TestSquareRootUint32_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint32(4LU,2LU));
}

TEST(FastMathGTest,TestSquareRootUint32_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint32(4294967295LU,65535LU));
}

TEST(FastMathGTest,TestSquareRootUint64_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint64(0LLU,0LLU));
}

TEST(FastMathGTest,TestSquareRootUint64_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint64(4LLU,2LLU));
}

TEST(FastMathGTest,TestSquareRootUint64_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootUint64(18446744073709551615LLU,4294967295LLU));
}

/* INT POSITIVE*/

TEST(FastMathGTest,TestSquareRootInt8_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(0,0));
}

TEST(FastMathGTest,TestSquareRootInt8_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(4,2));
}

TEST(FastMathGTest,TestSquareRootInt8_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(127,11));
}

TEST(FastMathGTest,TestSquareRootInt16_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(0,0));
}

TEST(FastMathGTest,TestSquareRootInt16_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(4,2));
}

TEST(FastMathGTest,TestSquareRootInt16_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(32767,181));
}

TEST(FastMathGTest,TestSquareRootInt32_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(0L,0L));
}

TEST(FastMathGTest,TestSquareRootInt32_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(4L,2L));
}

TEST(FastMathGTest,TestSquareRootInt32_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(2147483647L,46340L));
}

TEST(FastMathGTest,TestSquareRootInt64_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(0LL,0LL));
}

TEST(FastMathGTest,TestSquareRootInt64_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(4LL,2LL));
}

TEST(FastMathGTest,TestSquareRootInt64_max) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(9223372036854775807LL,3037000499LL));
}

/* INT Negative*/

TEST(FastMathGTest,TestSquareRootInt8_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(-0,0));
}

TEST(FastMathGTest,TestSquareRootInt8_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(-4,2));
}

TEST(FastMathGTest,TestSquareRootInt8_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt8(-127,11));
}

TEST(FastMathGTest,TestSquareRootInt16_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(-0,0));
}

TEST(FastMathGTest,TestSquareRootInt16_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(-4,2));
}

TEST(FastMathGTest,TestSquareRootInt16_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt16(-32767,181));
}

TEST(FastMathGTest,TestSquareRootInt32_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(-0L,0L));
}

TEST(FastMathGTest,TestSquareRootInt32_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(-4L,2L));
}

TEST(FastMathGTest,TestSquareRootInt32_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(-2147483647L,46340L));
}

TEST(FastMathGTest,TestSquareRootInt64_N0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(-0LL,0LL));
}
/* Problems with this test */
TEST(FastMathGTest,TestSquareRootInt64_N4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(-4LL,2LL));
}

TEST(FastMathGTest,TestSquareRootInt64_Nmax) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt64(-9223372036854775807LL,3037000499LL));
}

/* FLOAT POSITIVE*/

TEST(FastMathGTest,TestSquareRootFloat32_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(0.0,0.0));
}

TEST(FastMathGTest,TestSquareRootFloat32_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootInt32(4.0,2.0));
}

TEST(FastMathGTest,TestSquareRootFloat64_0) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootFloat64(0.0,0.0));
}

TEST(FastMathGTest,TestSquareRootFloat64_4) {
    FastMathTest test;
    ASSERT_TRUE(test.TestSquareRootFloat64(4.0,2.0));
}

