/**
 * @file EndianityGTest.cpp
 * @brief Source file for class EndianityGTest
 * @date 25/giu/2015
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

TEST_F(EndianityGTest,Uint16Test) {
    EndianityTest<uint16> uint16Test(0xAABB);
    ASSERT_TRUE(uint16Test.All());
}

TEST_F(EndianityGTest,Int16Test) {
    EndianityTest<int16> int16Test(0xAABB);
    ASSERT_TRUE(int16Test.All());
}

TEST_F(EndianityGTest,Uint32Test) {
    EndianityTest<uint32> uint32Test(0xFFEEDDCC);
    ASSERT_TRUE(uint32Test.All());
}

TEST_F(EndianityGTest,Int32Test) {
    EndianityTest<int32> int32Test(0xAABBCCDD);
    ASSERT_TRUE(int32Test.All());
}

TEST_F(EndianityGTest,Uint64Test) {
    EndianityTest<uint64> uint64Test(0xFFEEDDCCBBAA1122);
    ASSERT_TRUE(uint64Test.All());
}

TEST_F(EndianityGTest,Int64Test) {
    EndianityTest<int64> int64Test(0xAABBCCDDEEFF3344);
    ASSERT_TRUE(int64Test.All());
}

TEST_F(EndianityGTest,FloatTest) {
    EndianityTest<float> floatTest(0.12345f);
    ASSERT_TRUE(floatTest.All());
}

TEST_F(EndianityGTest,DoubleTest) {
    EndianityTest<double> doubleTest(0.123456789);
    ASSERT_TRUE(doubleTest.All());
}

