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





TEST(EndianityGTest,ToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.ToLittleEndian());
}

TEST(EndianityGTest,FromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.FromLittleEndian());
}


TEST(EndianityGTest,MemCopyToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyFromLittleEndian());
}



TEST(EndianityGTest,ToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyToFromEndian());
}
/////////////////////////////////////////////////////////////////

TEST(EndianityGTest,ToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.ToLittleEndian());
}

TEST(EndianityGTest,FromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.FromLittleEndian());
}

TEST(EndianityGTest,MemCopyToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyFromLittleEndian());
}



TEST(EndianityGTest,ToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyToFromEndian());
}

/////////////////////////////////////////////////////////////////

TEST(EndianityGTest,ToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.ToLittleEndian());
}

TEST(EndianityGTest,FromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.FromLittleEndian());
}

TEST(EndianityGTest,MemCopyToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyFromLittleEndian());
}


TEST(EndianityGTest,ToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyToFromEndian());
}

//////////////////////////////////////////////////////////////
TEST(EndianityGTest,ToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.ToLittleEndian());
}

TEST(EndianityGTest,FromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.FromLittleEndian());
}

TEST(EndianityGTest,MemCopyToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyFromLittleEndian());
}

TEST(EndianityGTest,ToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyToFromEndian());
}

/////////////////////////////////////////////////////////////////////////


TEST(EndianityGTest,ToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.ToLittleEndian());
}


TEST(EndianityGTest,FromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.FromBigEndian());
}


TEST(EndianityGTest,FromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.FromLittleEndian());
}


TEST(EndianityGTest,MemCopyToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyFromLittleEndian());
}


TEST(EndianityGTest,ToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyToFromEndian());
}

//////////////////////////////////////////////////////////////
TEST(EndianityGTest,ToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.ToLittleEndian());
}


TEST(EndianityGTest,FromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.FromLittleEndian());
}

TEST(EndianityGTest,MemCopyToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyFromLittleEndian());
}


TEST(EndianityGTest,ToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyToFromEndian());
}


////////////////////////////////////////////////////////



TEST(EndianityGTest,ToBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.ToLittleEndian());
}


TEST(EndianityGTest,FromBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.FromLittleEndian());
}

TEST(EndianityGTest,MemCopyToBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyFromLittleEndian());
}


TEST(EndianityGTest,ToFromEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyToFromEndian());
}


/////////////////////////////////////////////////////////////////////////




TEST(EndianityGTest,ToBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.ToBigEndian());
}

TEST(EndianityGTest,ToLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.ToLittleEndian());
}


TEST(EndianityGTest,FromBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.FromBigEndian());
}

TEST(EndianityGTest,FromLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.FromLittleEndian());
}

TEST(EndianityGTest,MemCopyToBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyToBigEndian());
}

TEST(EndianityGTest,MemCopyToLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyToLittleEndian());
}

TEST(EndianityGTest,MemCopyFromBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyFromBigEndian());
}

TEST(EndianityGTest,MemCopyFromLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyFromLittleEndian());
}


TEST(EndianityGTest,ToFromEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.ToFromEndian());
}


TEST(EndianityGTest,MemCopyToFromEndianDouble) {
    EndianityTest<double> floatTest(0.123456789);
    ASSERT_TRUE(floatTest.MemCopyToFromEndian());
}

