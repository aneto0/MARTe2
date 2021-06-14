/**
 * @file FractionalIntegerGTest.cpp
 * @brief Source file for class FractionalIntegerGTest
 * @date 01/09/2015
 * @author Giuseppe Ferrò
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
 * the class FractionalIntegerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "FractionalIntegerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned16in32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    uint16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeUnsignedUnsigned16in32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    uint16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedUnsigned16in32) {
    FractionalIntegerTest<int32> fractionalInteger;
    uint16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeSignedUnsigned16in32) {
    FractionalIntegerTest<int32> fractionalInteger;
    uint16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedSigned16in32) {
    FractionalIntegerTest<int32> fractionalInteger;
    int16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeSignedSigned16in32) {
    FractionalIntegerTest<int32> fractionalInteger;
    int16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnSignedSigned16in32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    int16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeUnSignedSigned16in32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    int16 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

///////////////////////////////////////////////

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned8in16) {
    FractionalIntegerTest<uint16> fractionalInteger;
    uint8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeUnsignedUnsigned8in16) {
    FractionalIntegerTest<uint16> fractionalInteger;
    uint8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedUnsigned8in16) {
    FractionalIntegerTest<int16> fractionalInteger;
    uint8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeSignedUnsigned8in16) {
    FractionalIntegerTest<int16> fractionalInteger;
    uint8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedSigned8in16) {
    FractionalIntegerTest<int16> fractionalInteger;
    int8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeSignedSigned8in16) {
    FractionalIntegerTest<int16> fractionalInteger;
    int8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnSignedSigned8in16) {
    FractionalIntegerTest<uint16> fractionalInteger;
    int8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeUnSignedSigned8in16) {
    FractionalIntegerTest<uint16> fractionalInteger;
    int8 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

//////////////////////////////////////////////////////

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned32in64) {
    FractionalIntegerTest<uint64> fractionalInteger;
    uint32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeUnsignedUnsigned32in64) {
    FractionalIntegerTest<uint64> fractionalInteger;
    uint32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedUnsigned32in64) {
    FractionalIntegerTest<int64> fractionalInteger;
    uint32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeSignedUnsigned32in64) {
    FractionalIntegerTest<int64> fractionalInteger;
    uint32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedSigned32in64) {
    FractionalIntegerTest<int64> fractionalInteger;
    int32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeSignedSigned32in64) {
    FractionalIntegerTest<int64> fractionalInteger;
    int32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnSignedSigned32in64) {
    FractionalIntegerTest<uint64> fractionalInteger;
    int32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMajorSizeUnSignedSigned32in64) {
    FractionalIntegerTest<uint64> fractionalInteger;
    int32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMajorSize(input));
}

/////////////////////////////////

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnsignedUnsignedSameSize32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    uint32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedUnsignedSameSize32) {
    FractionalIntegerTest<int32> fractionalInteger;
    uint32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedSignedSameSize32) {
    FractionalIntegerTest<int32> fractionalInteger;
    int32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnSignedSignedSameSize32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    int32 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

///////////////////////////////////
TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned64in32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    uint64 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedUnsigned64in32) {
    FractionalIntegerTest<int32> fractionalInteger;
    uint64 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeSignedSigned64in32) {
    FractionalIntegerTest<int32> fractionalInteger;
    int64 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBasicTypeCastMinorSizeUnSignedSigned64in32) {
    FractionalIntegerTest<uint32> fractionalInteger;
    int64 input = 0;
    ASSERT_TRUE(fractionalInteger.TestBasicTypeCastMinorSize(input));
}

/////////////////////////////
TEST(BareMetal_L0Types_FractionalIntegerGTest,TestBitSize) {
    FractionalIntegerTest<uint32> fractionalInteger;
    ASSERT_TRUE(fractionalInteger.TestGetNumberOfBits());
}
