/**
 * @file EndianityGTest.cpp
 * @brief Source file for class EndianityGTest
 * @date 25/06/2015
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
 * the class EndianityGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "EndianityTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,FromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToFromEndianFloat) {
    EndianityTest<float32> float32Test(0.12345f);
    ASSERT_TRUE(float32Test.TestMemCopyToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToBigEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToLittleEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromBigEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestFromLittleEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToBigEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestMemCopyToBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyToLittleEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestMemCopyToLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromBigEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestMemCopyFromBigEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestMemCopyFromLittleEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestMemCopyFromLittleEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,TestToFromEndianDouble) {
    EndianityTest<float64> float64Test(0.123456789);
    ASSERT_TRUE(float64Test.TestToFromEndian());
}

TEST(BareMetal_L1Portability_EndianityGTest,MemCopyToFromEndianDouble) {
    EndianityTest<float64> float32Test(0.123456789);
    ASSERT_TRUE(float32Test.TestMemCopyToFromEndian());
}
