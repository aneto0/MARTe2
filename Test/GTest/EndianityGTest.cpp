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

class EndianityGTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test
        // (right before the destructor).
    }
};





TEST_F(EndianityGTest,ToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.ToLittleEndian());
}

TEST_F(EndianityGTest,FromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.FromLittleEndian());
}


TEST_F(EndianityGTest,MemCopyToBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyFromLittleEndian());
}



TEST_F(EndianityGTest,ToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndianU16) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.MemCopyToFromEndian());
}
/////////////////////////////////////////////////////////////////

TEST_F(EndianityGTest,ToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.ToLittleEndian());
}

TEST_F(EndianityGTest,FromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.FromLittleEndian());
}

TEST_F(EndianityGTest,MemCopyToBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyFromLittleEndian());
}



TEST_F(EndianityGTest,ToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndian16) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.MemCopyToFromEndian());
}

/////////////////////////////////////////////////////////////////

TEST_F(EndianityGTest,ToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.ToLittleEndian());
}

TEST_F(EndianityGTest,FromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.FromLittleEndian());
}

TEST_F(EndianityGTest,MemCopyToBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyFromLittleEndian());
}


TEST_F(EndianityGTest,ToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndianU32) {
    EndianityTest<uint32> uint32Test(0xAABB);
    ASSERT_TRUE(uint32Test.MemCopyToFromEndian());
}

//////////////////////////////////////////////////////////////
TEST_F(EndianityGTest,ToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.ToLittleEndian());
}

TEST_F(EndianityGTest,FromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.FromLittleEndian());
}

TEST_F(EndianityGTest,MemCopyToBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyFromLittleEndian());
}

TEST_F(EndianityGTest,ToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndian32) {
    EndianityTest<int32> int32Test(0xAABB);
    ASSERT_TRUE(int32Test.MemCopyToFromEndian());
}

/////////////////////////////////////////////////////////////////////////


TEST_F(EndianityGTest,ToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.ToLittleEndian());
}


TEST_F(EndianityGTest,FromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.FromBigEndian());
}


TEST_F(EndianityGTest,FromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.FromLittleEndian());
}


TEST_F(EndianityGTest,MemCopyToBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyFromLittleEndian());
}


TEST_F(EndianityGTest,ToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndianU64) {
    EndianityTest<uint64> uint64Test(0xAABB);
    ASSERT_TRUE(uint64Test.MemCopyToFromEndian());
}

//////////////////////////////////////////////////////////////
TEST_F(EndianityGTest,ToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.ToLittleEndian());
}


TEST_F(EndianityGTest,FromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.FromLittleEndian());
}

TEST_F(EndianityGTest,MemCopyToBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyFromLittleEndian());
}


TEST_F(EndianityGTest,ToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndian64) {
    EndianityTest<int64> int64Test(0xAABB);
    ASSERT_TRUE(int64Test.MemCopyToFromEndian());
}


////////////////////////////////////////////////////////



TEST_F(EndianityGTest,ToBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.ToLittleEndian());
}


TEST_F(EndianityGTest,FromBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.FromLittleEndian());
}

TEST_F(EndianityGTest,MemCopyToBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyFromLittleEndian());
}


TEST_F(EndianityGTest,ToFromEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndianFloat) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.MemCopyToFromEndian());
}


/////////////////////////////////////////////////////////////////////////




TEST_F(EndianityGTest,ToBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.ToBigEndian());
}

TEST_F(EndianityGTest,ToLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.ToLittleEndian());
}


TEST_F(EndianityGTest,FromBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.FromBigEndian());
}

TEST_F(EndianityGTest,FromLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.FromLittleEndian());
}

TEST_F(EndianityGTest,MemCopyToBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyToBigEndian());
}

TEST_F(EndianityGTest,MemCopyToLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyToLittleEndian());
}

TEST_F(EndianityGTest,MemCopyFromBigEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyFromBigEndian());
}

TEST_F(EndianityGTest,MemCopyFromLittleEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.MemCopyFromLittleEndian());
}


TEST_F(EndianityGTest,ToFromEndianDouble) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.ToFromEndian());
}


TEST_F(EndianityGTest,MemCopyToFromEndianDouble) {
    EndianityTest<double> floatTest(0.123456789);
    ASSERT_TRUE(floatTest.MemCopyToFromEndian());
}

