/**
 * @file TypeCharacteristicsGTest.cpp
 * @brief Source file for class TypeCharacteristicsGTest
 * @date 02/09/2015
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
 * the class TypeCharacteristicsGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "TypeCharacteristicsTest.h"
#include "CompilerTypes.h"
#include "BitRange.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//void printf(const char *s,...);

TEST(TypeCharacteristicsGTest,TestIsSignedInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsSignedInt8());
}

TEST(TypeCharacteristicsGTest,TestIsSignedUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsSignedUInt32());
}

TEST(TypeCharacteristicsGTest,TestMaxValueInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueInt8());
}

TEST(TypeCharacteristicsGTest,TestMaxValueInt8BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueInt8BZ2());
}

TEST(TypeCharacteristicsGTest,TestMaxValueUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueUInt32());
}

TEST(TypeCharacteristicsGTest,TestMaxValueUInt32BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueUInt32BZ2());
}

TEST(TypeCharacteristicsGTest,TestMinValueInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueInt8());
}

TEST(TypeCharacteristicsGTest,TestMinValueInt8BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueInt8BZ2());
}

TEST(TypeCharacteristicsGTest,TestMinValueUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueUInt32());
}

TEST(TypeCharacteristicsGTest,TestMinValueUInt32BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueUInt32BZ2());
}

TEST(TypeCharacteristicsGTest,TestUsableBitSizeInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeInt8());
}

TEST(TypeCharacteristicsGTest,TestUsableBitSizeInt8BZ) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeInt8BZ());
}

TEST(TypeCharacteristicsGTest,TestUsableBitSizeUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeUInt32());
}

TEST(TypeCharacteristicsGTest,TestUsableBitSizeUInt32BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeUInt32BZ2());
}

TEST(TypeCharacteristicsGTest,TestSaturableIntegerNoChanges) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerNoChanges());
}

TEST(TypeCharacteristicsGTest,TestSaturableIntegerLargerThan) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLargerThan());
}

TEST(TypeCharacteristicsGTest,TestSaturableIntegerLargerThan2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLargerThan2());
}

TEST(TypeCharacteristicsGTest,TestSaturableIntegerLessThan) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLessThan());
}

TEST(TypeCharacteristicsGTest,TestSaturableIntegerLessThan2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLessThan2());
}

#define TestSafeN2N(typeIn,typeOut,valueIn,expectedValueOut,testName)\
TEST(TypeCharacteristicsGTest,TestSafeNumber2Number ## testName) {\
    TypeCharacteristicsTest myTypeCharacteristicsTest;\
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<typeIn,typeOut>(valueIn,expectedValueOut)));\
}\



TestSafeN2N(MARTe::float32,MARTe::int20,1e6,524287,F32_I20_1M);
TestSafeN2N(MARTe::float32,MARTe::int21,1e6,1000000,F32_I21_1M);
TestSafeN2N(MARTe::float32,MARTe::uint19,1e6,524287,F32_U19_1M);
TestSafeN2N(MARTe::float32,MARTe::uint20,1e6,1000000,F32_U20_1M);
TestSafeN2N(MARTe::float32,MARTe::int20,-1e6,-524288,F32_I20_M1M);
TestSafeN2N(MARTe::float32,MARTe::int21,-1e6,-1000000,F32_I21_M1M);
TestSafeN2N(MARTe::uint30,MARTe::int30,1000000000,536870911,U30_I30_1G);
TestSafeN2N(MARTe::uint30,MARTe::int31,1000000000,1000000000,U30_I31_1G);
TestSafeN2N(MARTe::uint35,MARTe::int32,4000000000,2147483647,U35_I32_4G);
TestSafeN2N(MARTe::uint35,MARTe::uint32,4000000000,4000000000,U35_U32_4G);
TestSafeN2N(MARTe::int35 ,MARTe::uint32,4000000000,4000000000,I35_U32_4G);
TestSafeN2N(MARTe::int35 ,MARTe::float32,4000000128,4000000000,I35_F32_4G);
TestSafeN2N(MARTe::int35 ,MARTe::float64,4000000000,4000000000,I35_F64_4G);
	
