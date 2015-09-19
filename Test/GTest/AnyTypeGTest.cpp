/**
 * @file AnyTypeGTest.cpp
 * @brief Header file for class AnyTypeGTest
 * @date 31/08/2015
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

 * @details This header file contains the declaration of the class AnyTypeGTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_GTEST_ANYTYPEGTEST_CPP_
#define TEST_GTEST_ANYTYPEGTEST_CPP_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "AnyTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(AnyTypeGTest, TestAnyType_Void) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Void());
}

TEST(AnyTypeGTest, TestAnyType_AnyTypeVoid) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_AnyTypeVoid());
}

TEST(AnyTypeGTest, TestAnyType_TypeDescriptor_BitAddress_ConstPointerToConst) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_TypeDescriptor_BitAddress_ConstPointerToConst());
}

TEST(AnyTypeGTest, TestAnyType_TypeDescriptor_BitAddress_ConstPointer) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_TypeDescriptor_BitAddress_ConstPointer());
}

TEST(AnyTypeGTest, TestIsVoid) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestIsVoid());
}

TEST(AnyTypeGTest, TestAnyType_Int8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Int8());
}

TEST(AnyTypeGTest, TestAnyType_UInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_UInt8());
}

TEST(AnyTypeGTest, TestAnyType_ConstInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstInt8());
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstUInt8());
}

TEST(AnyTypeGTest, TestAnyType_Int16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Int16());
}

TEST(AnyTypeGTest, TestAnyType_UInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_UInt16());
}

TEST(AnyTypeGTest, TestAnyType_ConstInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstInt16());
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstUInt16());
}

TEST(AnyTypeGTest, TestAnyType_Int32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Int32());
}

TEST(AnyTypeGTest, TestAnyType_UInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_UInt32());
}

TEST(AnyTypeGTest, TestAnyType_ConstInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstInt32());
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstUInt32());
}

TEST(AnyTypeGTest, TestAnyType_Int64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Int64());
}

TEST(AnyTypeGTest, TestAnyType_UInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_UInt64());
}

TEST(AnyTypeGTest, TestAnyType_ConstInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstInt64());
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstUInt64());
}

TEST(AnyTypeGTest, TestAnyType_Float32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Float32());
}

TEST(AnyTypeGTest, TestAnyType_ConstFloat32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFloat32());
}

TEST(AnyTypeGTest, TestAnyType_Float64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Float64());
}

TEST(AnyTypeGTest, TestAnyType_ConstFloat64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFloat64());
}

TEST(AnyTypeGTest, TestAnyType_ConstPointerToConts) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstPointerToConts());
}

TEST(AnyTypeGTest, TestAnyType_ConstPointer) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstPointer());
}

TEST(AnyTypeGTest, TestAnyType_ConstCharPointerToConst) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstCharPointerToConst());
}

TEST(AnyTypeGTest,TestAnyTypeCast) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyTypeCastFromBitBoolean<int32>());
}

TEST(AnyTypeGTest,TestAnyTypeCastNonConstSigned) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyTypeCastNonConstFromFractionalInteger<int32>());
}

TEST(AnyTypeGTest,TestAnyTypeCastNonConstUnsigned) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyTypeCastNonConstFromFractionalInteger<uint32>());
}

TEST(AnyTypeGTest,TestAnyTypeCastConstSigned) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyTypeCastConstFromFractionalInteger<int32>());
}

TEST(AnyTypeGTest,TestAnyTypeCastConstUnigned) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyTypeCastConstFromFractionalInteger<uint32>());
}

#endif /* TEST_GTEST_ANYTYPEGTEST_CPP_ */
	
