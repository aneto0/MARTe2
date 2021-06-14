/**
 * @file BitRangeGTest.cpp
 * @brief Source file for class BitRangeGTest
 * @date 13/10/2018
 * @author Andre Neto
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
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CRCTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L0Types_CRCGTest,TestConstructor) {
    CRCTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L0Types_CRCGTest,TestComputeTable_0x3D65) {
    CRCTest test;
    ASSERT_TRUE(test.TestComputeTable_0x3D65());
}

TEST(BareMetal_L0Types_CRCGTest,TestComputeTable_0x3D65_inverted) {
    CRCTest test;
    ASSERT_TRUE(test.TestComputeTable_0x3D65_inverted());
}

TEST(BareMetal_L0Types_CRCGTest,TestComputeTable_0x3D65_initialCRC) {
    CRCTest test;
    ASSERT_TRUE(test.TestComputeTable_0x3D65_initialCRC());
}

TEST(BareMetal_L0Types_CRCGTest,TestComputeTable_0x3D65_inverted_initialCRC) {
    CRCTest test;
    ASSERT_TRUE(test.TestComputeTable_0x3D65_inverted_initialCRC());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_uint16_CRC_0) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_uint16_CRC_0());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_uint16_CRC_0_inverted) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_uint16_CRC_0_inverted());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_uint16_CRC_0_initialCRC) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_uint16_CRC_0_initialCRC());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_uint16_CRC_0_inverted_initialCRC) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_uint16_CRC_0_inverted_initialCRC());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_uint8_CRC_0) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_uint8_CRC_0());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_CRC8_CDMA2000) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_CRC8_CDMA2000());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_CRC8_CDMA2000_inverted) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_CRC8_CDMA2000_inverted());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_CRC32_MPEG2) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_CRC32_MPEG2());
}

TEST(BareMetal_L0Types_CRCGTest,TestCompute_CRC32_MPEG2_inverted) {
    CRCTest test;
    ASSERT_TRUE(test.TestCompute_CRC32_MPEG2_inverted());
}
