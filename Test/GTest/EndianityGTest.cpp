/**
 * @file EndianityGTest.cpp
 * @brief Source file for class EndianityGTest
 * @date 25/giu/2015
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
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "EndianityTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/





TEST(EndianityGTest,TestToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestToLittleEndian());
}

TEST(EndianityGTest,TestFromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestFromBigEndian());
}

TEST(EndianityGTest,TestFromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestFromLittleEndian());
}


TEST(EndianityGTest,TestMemCopyToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyFromLittleEndian());
}



TEST(EndianityGTest,TestToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.TestMemCopyToFromEndian());
}
/////////////////////////////////////////////////////////////////

TEST(EndianityGTest,TestToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestToLittleEndian());
}

TEST(EndianityGTest,TestFromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestFromBigEndian());
}

TEST(EndianityGTest,TestFromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestFromLittleEndian());
}

TEST(EndianityGTest,TestMemCopyToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyFromLittleEndian());
}



TEST(EndianityGTest,TestToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.TestMemCopyToFromEndian());
}

/////////////////////////////////////////////////////////////////

TEST(EndianityGTest,TestToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestToLittleEndian());
}

TEST(EndianityGTest,TestFromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestFromBigEndian());
}

TEST(EndianityGTest,TestFromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestFromLittleEndian());
}

TEST(EndianityGTest,TestMemCopyToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyFromLittleEndian());
}


TEST(EndianityGTest,TestToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.TestMemCopyToFromEndian());
}

//////////////////////////////////////////////////////////////
TEST(EndianityGTest,TestToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestToLittleEndian());
}

TEST(EndianityGTest,TestFromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestFromBigEndian());
}

TEST(EndianityGTest,TestFromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestFromLittleEndian());
}

TEST(EndianityGTest,TestMemCopyToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyFromLittleEndian());
}

TEST(EndianityGTest,TestToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.TestMemCopyToFromEndian());
}

/////////////////////////////////////////////////////////////////////////


TEST(EndianityGTest,TestToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestToLittleEndian());
}


TEST(EndianityGTest,TestFromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestFromBigEndian());
}


TEST(EndianityGTest,TestFromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestFromLittleEndian());
}


TEST(EndianityGTest,TestMemCopyToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyFromLittleEndian());
}


TEST(EndianityGTest,TestToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.TestMemCopyToFromEndian());
}

//////////////////////////////////////////////////////////////
TEST(EndianityGTest,TestToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestToLittleEndian());
}


TEST(EndianityGTest,TestFromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestFromBigEndian());
}

TEST(EndianityGTest,FromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestFromLittleEndian());
}

TEST(EndianityGTest,TestMemCopyToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyFromLittleEndian());
}


TEST(EndianityGTest,TestToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.TestMemCopyToFromEndian());
}


////////////////////////////////////////////////////////



TEST(EndianityGTest,TestToBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestToLittleEndian());
}


TEST(EndianityGTest,TestFromBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestFromBigEndian());
}

TEST(EndianityGTest,TestFromLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestFromLittleEndian());
}

TEST(EndianityGTest,TestMemCopyToBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestMemCopyFromLittleEndian());
}


TEST(EndianityGTest,TestToFromEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestToFromEndian());
}


TEST(EndianityGTest,TestMemCopyToFromEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.TestMemCopyToFromEndian());
}


/////////////////////////////////////////////////////////////////////////




TEST(EndianityGTest,TestToBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestToBigEndian());
}

TEST(EndianityGTest,TestToLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestToLittleEndian());
}


TEST(EndianityGTest,TestFromBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestFromBigEndian());
}

TEST(EndianityGTest,TestFromLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestFromLittleEndian());
}

TEST(EndianityGTest,TestMemCopyToBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestMemCopyToBigEndian());
}

TEST(EndianityGTest,TestMemCopyToLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestMemCopyToLittleEndian());
}

TEST(EndianityGTest,TestMemCopyFromBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestMemCopyFromBigEndian());
}

TEST(EndianityGTest,TestMemCopyFromLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestMemCopyFromLittleEndian());
}


TEST(EndianityGTest,TestToFromEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.TestToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndianDouble) {
    EndianityTest<double> floatTest(0.123456789);
    ASSERT_TRUE(floatTest.TestMemCopyToFromEndian());
}

