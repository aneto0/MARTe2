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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "TypeCharacteristicsTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestIsFloatInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsFloatInt8());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestIsFloatFloat32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsFloatFloat32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestIsFloatFloat64) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsFloatFloat64());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestIsSignedInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsSignedInt8());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestIsSignedUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestIsSignedUInt32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMaxValueInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueInt8());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMaxValueInt8BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueInt8BZ2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMaxValueUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueUInt32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMaxValueUInt32BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMaxValueUInt32BZ2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMinValueInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueInt8());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMinValueInt8BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueInt8BZ2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMinValueUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueUInt32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestMinValueUInt32BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestMinValueUInt32BZ2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableBitSizeInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeInt8());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableBitSizeInt8BZ) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeInt8BZ());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableBitSizeUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeUInt32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableBitSizeUInt32BZ2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableBitSizeUInt32BZ2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableNegativeBitSizeInt8) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableNegativeBitSizeInt8());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableNegativeBitSizeInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableNegativeBitSizeInt32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableNegativeBitSizeUInt32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableNegativeBitSizeUInt32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestUsableNegativeBitSizeFloat32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestUsableNegativeBitSizeFloat32());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberNoChangesSameType) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<uint32, uint32>(10, true, 10)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberNoChangesDifferentType) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<uint32, int16>(10, true, 10)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberNoChangesFromFloat32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<float32, uint16>(10, true, 10)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberSaturateToMax) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<uint32, uint16>(MAX_UINT32, false, MAX_UINT16)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberSaturateToMin) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<int32, int16>(MIN_INT32, false, MIN_INT16)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberSaturateToUnsigned) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<int32, uint32>(-5, false, 0)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSafeNumber2NumberSaturateFromFloat32) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE((myTypeCharacteristicsTest.TestSafeNumber2Number<float32, uint64>(MAX_FLOAT32, false, MAX_UINT64)));
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSaturableIntegerNoChanges) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerNoChanges());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSaturableIntegerLargerThan) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLargerThan());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSaturableIntegerLargerThan2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLargerThan2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSaturableIntegerLessThan) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLessThan());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestSaturableIntegerLessThan2) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestSaturableIntegerLessThan2());
}

TEST(BareMetal_L0Types_TypeCharacteristicsGTest,TestEpsilon) {
    TypeCharacteristicsTest myTypeCharacteristicsTest;
    ASSERT_TRUE(myTypeCharacteristicsTest.TestEpsilon());
}
