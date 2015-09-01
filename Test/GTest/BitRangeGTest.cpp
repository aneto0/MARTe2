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

TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnsignedUnsigned16in32) {
    BitRangeTest<uint32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnsignedUnsigned16in32) {
    BitRangeTest<uint32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedUnsigned16in32) {
    BitRangeTest<int32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedUnsigned16in32) {
    BitRangeTest<int32> bitrange;
    uint16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}


TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedSigned16in32) {
    BitRangeTest<int32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedSigned16in32) {
    BitRangeTest<int32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}


TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnSignedSigned16in32) {
    BitRangeTest<uint32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnSignedSigned16in32) {
    BitRangeTest<uint32> bitrange;
    int16 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}



///////////////////////////////////////////////

TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnsignedUnsigned8in16) {
    BitRangeTest<uint16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnsignedUnsigned8in16) {
    BitRangeTest<uint16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedUnsigned8in16) {
    BitRangeTest<int16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedUnsigned8in16) {
    BitRangeTest<int16> bitrange;
    uint8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}


TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedSigned8in16) {
    BitRangeTest<int16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedSigned8in16) {
    BitRangeTest<int16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnSignedSigned8in16) {
    BitRangeTest<uint16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnSignedSigned8in16) {
    BitRangeTest<uint16> bitrange;
    int8 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

//////////////////////////////////////////////////////


TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnsignedUnsigned32in64) {
    BitRangeTest<uint64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnsignedUnsigned32in64) {
    BitRangeTest<uint64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedUnsigned32in64) {
    BitRangeTest<int64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedUnsigned32in64) {
    BitRangeTest<int64> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}


TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedSigned32in64) {
    BitRangeTest<int64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedSigned32in64) {
    BitRangeTest<int64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnSignedSigned32in64) {
    BitRangeTest<uint64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnSignedSigned32in64) {
    BitRangeTest<uint64> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}




/////////////////////////////////

TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnsignedUnsignedSameSize32) {
    BitRangeTest<uint32> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnsignedUnsignedSameSize32) {
    BitRangeTest<uint32> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedUnsignedSameSize32) {
    BitRangeTest<int32> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedUnsignedSameSize32) {
    BitRangeTest<int32> bitrange;
    uint32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}


TEST(BitRangeGTest,TestCopyOperatorMinorSizeSignedSignedSameSize32) {
    BitRangeTest<int32> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeSignedSignedSameSize32) {
    BitRangeTest<int32> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}


TEST(BitRangeGTest,TestCopyOperatorMinorSizeUnSignedSignedSameSize32) {
    BitRangeTest<uint32> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMinorSize(input));
}

TEST(BitRangeGTest,TestCopyOperatorMajorSizeUnSignedSignedSameSize32) {
    BitRangeTest<uint32> bitrange;
    int32 input = 0;
    ASSERT_TRUE(bitrange.TestCopyOperatorMajorSize(input));
}



