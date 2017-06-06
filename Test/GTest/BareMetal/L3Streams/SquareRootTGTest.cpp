/**
 * @file SquareRootTGTest.cpp
 * @brief Source file for class SquareRootTGTest
 * @date 20/04/2017
 * @author Andre' Torres
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
 * the class SquareRootTGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include "SquareRootTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
using namespace FastMath;

/* UINT */

TEST(SquareRootTGTest,TestUint8_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint8(0U,0U));
}

TEST(SquareRootTGTest,TestUint8_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint8(4U,2U));
}

TEST(SquareRootTGTest,TestUint8_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint8(255U,15U));
}

TEST(SquareRootTGTest,TestUint16_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint16(0U,0U));
}

TEST(SquareRootTGTest,TestUint16_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint8(4U,2U));
}

TEST(SquareRootTGTest,TestUint16_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint16(65535U,255U));
}

TEST(SquareRootTGTest,TestUint32_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint32(0LU,0LU));
}

TEST(SquareRootTGTest,TestUint32_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint32(4LU,2LU));
}

TEST(SquareRootTGTest,TestUint32_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint32(4294967295LU,65535LU));
}

TEST(SquareRootTGTest,TestUint64_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint64(0LLU,0LLU));
}

TEST(SquareRootTGTest,TestUint64_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint64(4LLU,2LLU));
}

TEST(SquareRootTGTest,TestUint64_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestUint64(18446744073709551615LLU,4294967295LLU));
}

/* INT POSITIVE*/

TEST(SquareRootTGTest,TestInt8_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(0,0));
}

TEST(SquareRootTGTest,TestInt8_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(4,2));
}

TEST(SquareRootTGTest,TestInt8_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(127,11));
}

TEST(SquareRootTGTest,TestInt16_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt16(0,0));
}

TEST(SquareRootTGTest,TestInt16_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(4,2));
}

TEST(SquareRootTGTest,TestInt16_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt16(32767,181));
}

TEST(SquareRootTGTest,TestInt32_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(0L,0L));
}

TEST(SquareRootTGTest,TestInt32_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(4L,2L));
}

TEST(SquareRootTGTest,TestInt32_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(2147483647L,46340L));
}

TEST(SquareRootTGTest,TestInt64_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt64(0LL,0LL));
}

TEST(SquareRootTGTest,TestInt64_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt64(4LL,2LL));
}

TEST(SquareRootTGTest,TestInt64_max) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt64(9223372036854775807LL,3037000499LL));
}

/* INT Negative*/

TEST(SquareRootTGTest,TestInt8_N0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(-0,0));
}

TEST(SquareRootTGTest,TestInt8_N4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(-4,2));
}

TEST(SquareRootTGTest,TestInt8_Nmax) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt8(-127,11));
}

TEST(SquareRootTGTest,TestInt16_N0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt16(-0,0));
}

TEST(SquareRootTGTest,TestInt16_N4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt16(-4,2));
}

TEST(SquareRootTGTest,TestInt16_Nmax) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt16(-32767,181));
}

TEST(SquareRootTGTest,TestInt32_N0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(-0L,0L));
}

TEST(SquareRootTGTest,TestInt32_N4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(-4L,2L));
}

TEST(SquareRootTGTest,TestInt32_Nmax) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(-2147483647L,46340L));
}

TEST(SquareRootTGTest,TestInt64_N0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt64(-0LL,0LL));
}
/* Problems with this test */
TEST(SquareRootTGTest,TestInt64_N4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt64(-4LL,2LL));
}

TEST(SquareRootTGTest,TestInt64_Nmax) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt64(-9223372036854775807LL,3037000499LL));
}

/* FLOAT POSITIVE*/

TEST(SquareRootTGTest,TestFloat32_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(0.0,0.0));
}

TEST(SquareRootTGTest,TestFloat32_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestInt32(4.0,2.0));
}

TEST(SquareRootTGTest,TestFloat64_0) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestFloat64(0.0,0.0));
}

TEST(SquareRootTGTest,TestFloat64_4) {
    SquareRootTTest squareRerootTTest;
    ASSERT_TRUE(squareRerootTTest.TestFloat64(4.0,2.0));
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
