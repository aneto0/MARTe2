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

#include "TestSupport.h"
#include "AnyTypeTest.h"
#include "TestObjectHelper1.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(AnyTypeGTest, TestAnyType_Void) {
    ASSERT_TRUE(AnyTypeTest::TestAnyType_Void());
}

TEST(AnyTypeGTest, TestAnyType_AnyTypeVoid) {
    ASSERT_TRUE(AnyTypeTest::TestAnyType_AnyTypeVoid());
}

TEST(AnyTypeGTest, TestAnyType_Int8) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<int8>(1,SignedInteger8Bit));
}

TEST(AnyTypeGTest, TestAnyType_UInt8) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<uint8>(1U,SignedInteger8Bit));
}

TEST(AnyTypeGTest, TestAnyType_ConstInt8) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const int8>(1,TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger) | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt8) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const uint8>(1,TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_Int16) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<int16>(1,SignedInteger16Bit));
}

TEST(AnyTypeGTest, TestAnyType_UInt16) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<uint16>(1U,SignedInteger16Bit));
}

TEST(AnyTypeGTest, TestAnyType_ConstInt16) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const int16>(1,TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger) | TDRANGE(basicTypeSize,Size16bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt16) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const uint16>(1,TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(basicTypeSize,Size16bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_Int32) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<int32>(1,SignedInteger32Bit));
}

TEST(AnyTypeGTest, TestAnyType_UInt32) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<uint32>(1U,SignedInteger32Bit));
}

TEST(AnyTypeGTest, TestAnyType_ConstInt32) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const int32>(1,TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger) | TDRANGE(basicTypeSize,Size32bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt32) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const uint32>(1,TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(basicTypeSize,Size32bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_Int64) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<int64>(1,SignedInteger64Bit));
}

TEST(AnyTypeGTest, TestAnyType_UInt64) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<uint64>(1U,SignedInteger64Bit));
}

TEST(AnyTypeGTest, TestAnyType_ConstInt64) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const int64>(1,TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger) | TDRANGE(basicTypeSize,Size64bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_ConstUInt64) {
	ASSERT_TRUE(AnyTypeTest::ATVerify<const uint64>(1,TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(basicTypeSize,Size64bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_Float32) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<float32>(1.1F,Float32Bit));
}

TEST(AnyTypeGTest, TestAnyType_ConstFloat32) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<const float32>(1.1F,TypeDescriptor(TDRANGE(fullType,TDF_Float) | TDRANGE(basicTypeSize,Size32bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_Float64) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<float64>(1.1F,Float64Bit));
}

TEST(AnyTypeGTest, TestAnyType_ConstFloat64) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<const float64>(1.1F,TypeDescriptor(TDRANGE(fullType,TDF_Float) | TDRANGE(basicTypeSize,Size64bit) | TDRANGE(dataIsConstant,1))));
}

TEST(AnyTypeGTest, TestAnyType_ConstPointerToConts) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<const void* const>(NULL,TDRANGE(fullType,TDF_Void) | TDRANGE(basicTypeSize,SizeUnknown) | TDRANGE(dataIsConstant,1),"p"));
}

TEST(AnyTypeGTest, TestAnyType_ConstPointer) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<void* const>(NULL,TDRANGE(fullType,TDF_Void) | TDRANGE(basicTypeSize,SizeUnknown) | TDRANGE(dataIsConstant,1),"P"));
}

TEST(AnyTypeGTest, TestAnyType_ConstCharPointerToConst) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<const char8* const>(NULL,TDRANGE(fullType,TDF_Char) | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(dataIsConstant,1),"p"));
}

TEST(AnyTypeGTest, TestAnyType_Char8) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<char8>(NULL,TDRANGE(fullType,TDF_Char) | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(dataIsConstant,1)));
}

TEST(AnyTypeGTest, TestAnyType_ConstChar8) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<const char8>(NULL,TDRANGE(fullType,TDF_Char) | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(dataIsConstant,1)));
}

TEST(AnyTypeGTest, TestAnyType_Object) {
    ASSERT_TRUE(AnyTypeTest::ATVerify<TestObjectHelper1 *>(NULL,ClassRegistryItem::Instance<TestObjectHelper1>()->GetTypeDescriptor(),"P"));
}

TEST(AnyTypeGTest, TestAnyType_ConstObject) {
	TypeDescriptor td = ClassRegistryItem::Instance<TestObjectHelper1>()->GetTypeDescriptor();
	td.dataIsConstant = true;
    ASSERT_TRUE(AnyTypeTest::ATVerify<TestObjectHelper1 * const >(NULL,td,"P"));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt8) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<uint8,4>*>(NULL, BitSetBoolean(uint8,4),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt16) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<uint16,9>*>(NULL, BitSetBoolean(uint16,9),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt32) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<uint32,18>*>(NULL, BitSetBoolean(uint32,18),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanUInt64) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<uint64,34>*>(NULL, BitSetBoolean(uint64,34),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt8) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<int8,4>*>(NULL, SignedBitSet(int8,1,4),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt16) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<int16,9>*>(NULL, SignedBitSet(int16,1,9),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt32) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<int32,18>*>(NULL, SignedBitSet(int32,1,18),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitBooleanInt64) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitBoolean<int64,34>*>(NULL, SignedBitSet(int64,1,34),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt8) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<uint8,4,2>*>(NULL, UnsignedBitSet(uint8,4,2),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt16) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<uint16,4,10>*>(NULL, UnsignedBitSet(uint16,4,10),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt32) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<uint32,18,10>*>(NULL, UnsignedBitSet(uint32,18,10),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeUInt64) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<uint64,18,40>*>(NULL, UnsignedBitSet(uint64,18,40),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt8) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<int8,4,2>*>(NULL, SignedBitSet(int8,4,2),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt16) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<int16,4,10>*>(NULL, SignedBitSet(int16,4,10),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt32) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<int32,18,10>*>(NULL, SignedBitSet(int32,18,10),"P")));
}

TEST(AnyTypeGTest, TestAnyType_BitRangeInt64) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<BitRange<int64,18,40>*>(NULL, SignedBitSet(int64,18,40),"P")));
}


TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt8) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<uint4*>(NULL, UnsignedBitSet(uint8,4,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt16) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<uint12*>(NULL, UnsignedBitSet(uint16,12,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt32) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<uint24*>(NULL, UnsignedBitSet(uint32,24,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerUInt64) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<uint48*>(NULL, UnsignedBitSet(uint64,48,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt8) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<int4*>(NULL, SignedBitSet(int8,4,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt16) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<int12*>(NULL, SignedBitSet(int16,12,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt32) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<int24*>(NULL, SignedBitSet(int32,24,0),"P")));
}

TEST(AnyTypeGTest, TestAnyType_FractionalIntegerInt64) {
    ASSERT_TRUE((AnyTypeTest::ATVerify<int48*>(NULL, SignedBitSet(int64,48,0),"P")));
}

TEST(AnyTypeGTest, TestSetDataPointer) {
    ASSERT_TRUE(AnyTypeTest::TestSetDataPointer());
}

TEST(AnyTypeGTest, TestGetDataPointer) {
    ASSERT_TRUE(AnyTypeTest::TestGetDataPointer());
}

TEST(AnyTypeGTest, TestGetTypeDescriptor) {
    ASSERT_TRUE(AnyTypeTest::TestGetTypeDescriptor());
}

