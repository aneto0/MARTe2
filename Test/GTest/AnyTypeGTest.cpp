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

//TODO Move to Object tests
#if 0
TEST(AnyTypeGTest, TestAnyType_Object) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_Object());
}

TEST(AnyTypeGTest, TestAnyType_ConstObject) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstObject());
}

TEST(AnyTypeGTest, TestCreateFromOtherType) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestCreateFromOtherType());
}

TEST(AnyTypeGTest, TestCreateFromOtherConstType) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestCreateFromOtherConstType());
}
#endif

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<uint8>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<uint16>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<uint32>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<uint64>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<int8>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<int16>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<int32>());
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitBoolean<int64>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<uint8>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<uint16>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<uint32>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<uint64>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<int8>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<int16>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<int32>());
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_BitRange<int64>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<uint8>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<uint16>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<uint32>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<uint64>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<int8>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<int16>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<int32>());
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_FractionalInteger<int64>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerUInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<uint8>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerUInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<uint16>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerUInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<uint32>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerUInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<uint64>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerInt8) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<int8>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerInt16) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<int16>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerInt32) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<int32>());
}

TEST(AnyTypeGTest, TestAnyType_ConstFractionalIntegerInt64) {
    AnyTypeTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestAnyType_ConstFractionalInteger<int64>());
}

