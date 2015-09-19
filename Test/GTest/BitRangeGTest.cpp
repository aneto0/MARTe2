/**
 * @file BitRangeGTest.cpp
 * @brief Source file for class BitRangeGTest
 * @date 31/08/2015
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
 * the class BitRangeGTest (public, protected, and private). Be aware that some 
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
#include "BitRangeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BitRangeGTest,TestCopyOperatorUnion) {
    BitRangeTest<int64> bitrange;
    ASSERT_TRUE(bitrange.TestCopyOperatorUnion());
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned16in32) {
    BitRangeTest<uint32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeUnsignedUnsigned16in32) {
    BitRangeTest<uint32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedUnsigned16in32) {
    BitRangeTest<int32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeSignedUnsigned16in32) {
    BitRangeTest<int32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedSigned16in32) {
    BitRangeTest<int32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeSignedSigned16in32) {
    BitRangeTest<int32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnSignedSigned16in32) {
    BitRangeTest<uint32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeUnSignedSigned16in32) {
    BitRangeTest<uint32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

///////////////////////////////////////////////

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned8in16) {
    BitRangeTest<uint16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeUnsignedUnsigned8in16) {
    BitRangeTest<uint16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedUnsigned8in16) {
    BitRangeTest<int16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeSignedUnsigned8in16) {
    BitRangeTest<int16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedSigned8in16) {
    BitRangeTest<int16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeSignedSigned8in16) {
    BitRangeTest<int16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnSignedSigned8in16) {
    BitRangeTest<uint16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeUnSignedSigned8in16) {
    BitRangeTest<uint16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

//////////////////////////////////////////////////////

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned32in64) {
    BitRangeTest<uint64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeUnsignedUnsigned32in64) {
    BitRangeTest<uint64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedUnsigned32in64) {
    BitRangeTest<int64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeSignedUnsigned32in64) {
    BitRangeTest<int64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedSigned32in64) {
    BitRangeTest<int64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeSignedSigned32in64) {
    BitRangeTest<int64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnSignedSigned32in64) {
    BitRangeTest<uint64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMajorSizeUnSignedSigned32in64) {
    BitRangeTest<uint64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMajorSize(input));
}

/////////////////////////////////

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnsignedUnsignedSameSize32) {
    BitRangeTest<uint32> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedUnsignedSameSize32) {
    BitRangeTest<int32> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedSignedSameSize32) {
    BitRangeTest<int32> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnSignedSignedSameSize32) {
    BitRangeTest<uint32> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

///////////////////////////////////
TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnsignedUnsigned64in32) {
    BitRangeTest<uint32> bitrange;
    uint64 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedUnsigned64in32) {
    BitRangeTest<int32> bitrange;
    uint64 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeSignedSigned64in32) {
    BitRangeTest<int32> bitrange;
    int64 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

TEST(BitRangeGTest,TestBasicTypeCastMinorSizeUnSignedSigned64in32) {
    BitRangeTest<uint32> bitrange;
    int64 input = 0;
    ASSERT_TRUE(bitrange.TestBasicTypeCastMinorSize(input));
}

/////////////////////////////

TEST(BitRangeGTest,TestBitSize) {
    BitRangeTest<uint32> bitrange;
    ASSERT_TRUE(bitrange.TestBitSize());
}


TEST(BitRangeGTest,TestBitOffset) {
    BitRangeTest<uint32> bitrange;
    ASSERT_TRUE(bitrange.TestBitOffset());
}
