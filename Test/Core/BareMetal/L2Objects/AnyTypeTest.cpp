/**
 * @file AnyTypeTest.cpp
 * @brief Source file for class AnyTypeTest
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

 * @details This source file contains the definition of all the methods for
 * the class AnyTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AnyTypeTest.h"
#include "ClassRegistryItemT.h"
#include "TestObjectHelper1.h"
#include "StringHelper.h"
#include "IntrospectionT.h"
#include <typeinfo>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct TestATStructure {
    uint32 member1;
};

DECLARE_CLASS_MEMBER(TestATStructure, member1, uint32, "", "");

static const IntrospectionEntry *fields[] = { &TestATStructure_member1_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestATStructure, fields);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

AnyTypeTest::AnyTypeTest() :
        targetVariable(1),
        constantPtrToConstant(&targetVariable),
        constPtr(NULL) {
            retVal = false;
            defaultBitAddress = 1;
            defaultDataDescription.isConstant = true;
            defaultDataDescription.isStructuredData = false;
            defaultDataDescription.numberOfBits = 5;
            defaultDataDescription.type = Float;
        }

bool AnyTypeTest::TestAnyType_Void() {
    AnyType anytype;

    retVal = (anytype.GetDataPointer() == NULL);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 0);

    return retVal;
}

bool AnyTypeTest::TestAnyType_AnyTypeVoid() {
    AnyType anytype;
    AnyType anytype1(anytype);

    retVal = (anytype1.GetDataPointer() == NULL);
    retVal &= (anytype1.GetBitAddress() == 0);
    td = anytype1.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 0);

    return retVal;
}

bool AnyTypeTest::TestAnyType_TypeDescriptor_BitAddress_ConstPointerToConst() {
    AnyType anytype(defaultDataDescription, defaultBitAddress, constantPtrToConstant);

    retVal = (anytype.GetDataPointer() == &targetVariable);
    retVal &= (anytype.GetBitAddress() == 1);
    td = anytype.GetTypeDescriptor();
    retVal &= (td == defaultDataDescription);

    return retVal;
}

bool AnyTypeTest::TestAnyType_TypeDescriptor_BitAddress_ConstPointer() {
    AnyType anytype(defaultDataDescription, defaultBitAddress, constPtr);

    retVal = (anytype.GetDataPointer() == NULL);
    retVal &= (anytype.GetBitAddress() == 1);
    td = anytype.GetTypeDescriptor();
    retVal &= (td == defaultDataDescription);

    return retVal;
}

bool AnyTypeTest::TestIsVoid() {
    AnyType anytype;
    AnyType anytype2(defaultDataDescription, defaultBitAddress, constPtr);

    retVal = anytype.IsVoid();
    retVal &= !anytype2.IsVoid();

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int8() {
    int8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt8() {
    uint8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt8() {
    const int8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt8() {
    const uint8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int16() {
    int16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt16() {
    uint16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt16() {
    const int16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt16() {
    const uint16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int32() {
    int32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt32() {
    uint32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt32() {
    const int32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt32() {
    const uint32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int64() {
    int64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt64() {
    uint64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt64() {
    const int64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == SignedInteger);
    retVal &= (td.numberOfBits == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt64() {
    const uint64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Float32() {
    float32 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == Float);
    retVal &= (td.numberOfBits == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstFloat32() {
    const float32 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == Float);
    retVal &= (td.numberOfBits == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Float64() {
    float64 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == Float);
    retVal &= (td.numberOfBits == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstFloat64() {
    const float64 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == Float);
    retVal &= (td.numberOfBits == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstPointerToConts() {
    AnyType anytype(constantPtrToConstant);

    retVal = (anytype.GetDataPointer() == constantPtrToConstant);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == Pointer);
    retVal &= (td.numberOfBits == sizeof(void*) * 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstPointer() {
    AnyType anytype(constPtr);

    retVal = (anytype.GetDataPointer() == constPtr);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == Pointer);
    retVal &= (td.numberOfBits == sizeof(void*) * 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstCharPointerToConst() {
    const char8 *const charPtr = "hello";
    AnyType anytype(charPtr);

    retVal = (anytype.GetDataPointer() == charPtr);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == BT_CCString);
    retVal &= (td.numberOfBits == sizeof(const char*) * 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Char8() {
    char8 character = 'h';
    AnyType anytype(character);

    retVal = (((char8*) anytype.GetDataPointer())[0] == character);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == CArray);
    retVal &= (td.numberOfBits == 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstChar8() {
    const char8 character = 'h';
    AnyType anytype(character);

    retVal = (((char8*) anytype.GetDataPointer())[0] == character);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == CArray);
    retVal &= (td.numberOfBits == 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Object() {
    TestObjectHelper1 obj;
    AnyType anytype(obj);
    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->Find("TestObjectHelper1");

    retVal = (classItem != NULL);
    if (retVal) {
        retVal &= (anytype.GetBitAddress() == 0);
        retVal &= (anytype.GetDataPointer() == &obj);

        td = anytype.GetTypeDescriptor();
        retVal &= (td.isStructuredData == true);
        retVal &= (td.isConstant == false);

        uint32 structuredDataIdCode = td.structuredDataIdCode;
        uint32 classUniqueId = classItem->GetClassProperties()->GetUniqueIdentifier();
        retVal &= (structuredDataIdCode == classUniqueId);
    }
    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstObject() {
    const TestObjectHelper1 obj;
    AnyType anytype(obj);
    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->Find("TestObjectHelper1");

    retVal = (classItem != NULL);
    if (retVal) {
        retVal &= (anytype.GetBitAddress() == 0);
        retVal &= (anytype.GetDataPointer() == &obj);

        td = anytype.GetTypeDescriptor();
        retVal &= (td.isStructuredData == true);
        retVal &= (td.isConstant == true);

        uint32 structuredDataIdCode = td.structuredDataIdCode;
        uint32 classUniqueId = classItem->GetClassProperties()->GetUniqueIdentifier();
        retVal &= (structuredDataIdCode == classUniqueId);
    }
    return retVal;
}

bool AnyTypeTest::TestCreateFromOtherType() {
    TestObjectHelper1 obj;
    AnyType anytype;
    AnyType::CreateFromOtherType(anytype, obj);
    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(TestObjectHelper1).name());

    retVal = (classItem != NULL);
    if (retVal) {
        retVal &= (anytype.GetBitAddress() == 0);
        retVal &= (anytype.GetDataPointer() == &obj);

        td = anytype.GetTypeDescriptor();
        retVal &= (td.isStructuredData == true);
        retVal &= (td.isConstant == false);

        uint32 structuredDataIdCode = td.structuredDataIdCode;
        uint32 classUniqueId = classItem->GetClassProperties()->GetUniqueIdentifier();
        retVal &= (structuredDataIdCode == classUniqueId);
    }
    return retVal;
}

bool AnyTypeTest::TestCreateFromOtherConstType() {
    const TestObjectHelper1 obj;
    AnyType anytype;
    AnyType::CreateFromOtherType(anytype, obj);
    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(TestObjectHelper1).name());

    retVal = (classItem != NULL);
    if (retVal) {
        retVal &= (anytype.GetBitAddress() == 0);
        retVal &= (anytype.GetDataPointer() == &obj);

        td = anytype.GetTypeDescriptor();
        retVal &= (td.isStructuredData == true);
        retVal &= (td.isConstant == true);

        uint32 structuredDataIdCode = td.structuredDataIdCode;
        uint32 classUniqueId = classItem->GetClassProperties()->GetUniqueIdentifier();
        retVal &= (structuredDataIdCode == classUniqueId);
    }
    return retVal;
}

bool AnyTypeTest::TestSetDataPointer() {
    int32 x = 1;
    void *ptr = (void*) &x;
    AnyType at;
    at.SetDataPointer(ptr);
    return at.GetDataPointer() == ptr;
}

bool AnyTypeTest::TestGetDataPointer() {
    return TestSetDataPointer();
}

bool AnyTypeTest::TestGetTypeDescriptor() {
    TypeDescriptor tdes = UnsignedInteger64Bit;
    AnyType at(tdes, 0, (void*) NULL);
    return at.GetTypeDescriptor() == tdes;
}

bool AnyTypeTest::TestSetTypeDescriptor() {
    bool ok = true;
    AnyType at;
    uint32 numberOfTypes = 11u;
    TypeDescriptor types[] = { SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, SignedInteger64Bit, UnsignedInteger8Bit, UnsignedInteger16Bit,
            UnsignedInteger32Bit, UnsignedInteger64Bit, Float32Bit, Float64Bit, InvalidType };
    for (uint32 typeIdx = 0u; (typeIdx < numberOfTypes) && ok; typeIdx++) {
        at.SetTypeDescriptor(types[typeIdx]);
        ok = (at.GetTypeDescriptor() == types[typeIdx]);
    }
    return ok;
}

bool AnyTypeTest::TestGetBitAddress() {
    uint32 bitAddr = 10;
    AnyType at(InvalidType, bitAddr, (void*) NULL);
    return at.GetBitAddress() == bitAddr;
}

bool AnyTypeTest::TestSetNumberOfDimensions() {
    AnyType at;
    at.SetNumberOfDimensions(10);
    return at.GetNumberOfDimensions() == 10;
}

bool AnyTypeTest::TestGetNumberOfDimensions() {
    return TestSetNumberOfDimensions();
}

bool AnyTypeTest::TestSetNumberOfElements() {
    AnyType at;
    at.SetNumberOfElements(0, 1);
    at.SetNumberOfElements(1, 2);
    at.SetNumberOfElements(2, 3);

    retVal = (at.GetNumberOfElements(0) == 1);
    retVal &= (at.GetNumberOfElements(1) == 2);
    retVal &= (at.GetNumberOfElements(2) == 3);
    return retVal;
}

bool AnyTypeTest::TestGetNumberOfElements() {
    return TestSetNumberOfElements();
}

bool AnyTypeTest::TestSetStaticDeclared() {
    AnyType at;
    if (!at.IsStaticDeclared()) {
        return false;
    }
    at.SetStaticDeclared(false);
    if (at.IsStaticDeclared()) {
        return false;
    }
    at.SetStaticDeclared(true);
    return at.IsStaticDeclared();
}

bool AnyTypeTest::TestIsStaticDeclared() {
    return TestSetStaticDeclared();
}

bool AnyTypeTest::TestGetBitSize() {

    for (uint32 i = 0u; i < 63; i++) {
        uint64 x = i;
        AnyType at(UnsignedInteger64Bit, i, &x);
        if (at.GetBitSize() != (64 + i)) {
            return false;
        }
    }

    return true;
}

bool AnyTypeTest::TestGetByteSize() {

    bool ret = true;
    for (uint32 i = 0u; (i < 63) && ret; i++) {
        uint64 x = i;
        AnyType at(UnsignedInteger64Bit, i, &x);
        ret = (at.GetByteSize() == ((64 + i + 7) / 8));
    }

    if (ret) {
        TestObjectHelper1 obj;
        AnyType anytype(obj);
        ret = (anytype.GetByteSize() == sizeof(TestObjectHelper1));
    }

    return ret;
}

bool AnyTypeTest::TestGetDataSize() {
    TypeDescriptor type = UnsignedInteger64Bit;
    uint32 numberOfDimensions = 3u;
    uint32 numberOfElements[] = { 1u, 2u, 3u };
    uint32 totalSize = (64u / 8u) * numberOfElements[0u] * numberOfElements[1u] * numberOfElements[2u];

    AnyType at;
    at.SetTypeDescriptor(type);
    at.SetNumberOfDimensions(numberOfDimensions);
    at.SetNumberOfElements(0u, numberOfElements[0u]);
    at.SetNumberOfElements(1u, numberOfElements[1u]);
    at.SetNumberOfElements(2u, numberOfElements[2u]);

    return at.GetDataSize() == totalSize;
}

#include "stdio.h"
bool AnyTypeTest::TestPositionOperator_MatrixStructuredStaticDeclared() {

    const uint32 nRows = 2;
    const uint32 nCols = 2;

    TestATStructure test[nRows][nCols];

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            test[i][j].member1 = (i + j);
        }
    }

    TypeDescriptor tdes(false, ClassRegistryDatabase::Instance()->Find("TestATStructure")->GetClassProperties()->GetUniqueIdentifier());

    AnyType at(tdes, 0, test);
    at.SetNumberOfDimensions(2);
    at.SetNumberOfElements(0, nRows);
    at.SetNumberOfElements(1, nCols);

    for (uint32 i = 0; i < nRows; i++) {
        AnyType vector = at[i];
        for (uint32 j = 0; j < nCols; j++) {
            AnyType element = at[i][j];
            TestATStructure* vectorDataPtr = (TestATStructure*)vector.GetDataPointer();
            if(vectorDataPtr != NULL) {
                if(vectorDataPtr[j].member1 != (i + j)) {
                    return false;
                }
            }

            TestATStructure* elementDataPtr = (TestATStructure*)element.GetDataPointer();
            if(elementDataPtr != NULL) {
                if(elementDataPtr->member1 != (i + j)) {
                    return false;
                }
            }

            // if ((*((TestATStructure*) (vector.GetDataPointer()) + j)).member1 != (i + j)) {
            //     return false;
            // }
            // if ((*(TestATStructure*) (element.GetDataPointer())).member1 != (i + j)) {
            //     return false;
            // }
        }
    }
    return true;
}

bool AnyTypeTest::TestPositionOperator_MatrixStructuredHeapDeclared() {

    const uint32 nRows = 2;
    const uint32 nCols = 2;

    TestATStructure **test = new TestATStructure*[nRows];

    for (uint32 i = 0; i < nRows; i++) {
        test[i] = new TestATStructure[nCols];
        for (uint32 j = 0; j < nCols; j++) {
            test[i][j].member1 = (i + j);
        }
    }

    TypeDescriptor tdes(false, ClassRegistryDatabase::Instance()->Find("TestATStructure")->GetClassProperties()->GetUniqueIdentifier());

    AnyType at(tdes, 0, test);
    at.SetNumberOfDimensions(2);
    at.SetNumberOfElements(0, nRows);
    at.SetNumberOfElements(1, nCols);
    at.SetStaticDeclared(false);
    bool ret = true;

    for (uint32 i = 0; i < nRows; i++) {
        AnyType vector = at[i];
        for (uint32 j = 0; j < nCols; j++) {
            AnyType element = at[i][j];

            TestATStructure* vectorDataPtr = (TestATStructure*)vector.GetDataPointer();
            if(vectorDataPtr != NULL) {
                if(vectorDataPtr[j].member1 != (i + j)) {
                    return false;
                }
            }

            TestATStructure* elementDataPtr = (TestATStructure*)element.GetDataPointer();
            if(elementDataPtr != NULL) {
                if(elementDataPtr->member1 != (i + j)) {
                    return false;
                }
            }

            // if ((*((TestATStructure*) (vector.GetDataPointer()) + j)).member1 != (i + j)) {
            //     ret = false;
            // }
            // if ((*(TestATStructure*) (element.GetDataPointer())).member1 != (i + j)) {
            //     ret = false;
            // }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        delete[] test[i];
    }
    delete[] test;

    return ret;
}

bool AnyTypeTest::TestPositionOperator_MatrixBasicStaticDeclared() {

    const uint32 nRows = 2;
    const uint32 nCols = 2;

    uint32 test[nRows][nCols];
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            test[i][j] = (i + j);
        }
    }

    AnyType at(test);

    for (uint32 i = 0; i < nRows; i++) {
        AnyType vector = at[i];
        for (uint32 j = 0; j < nCols; j++) {
            AnyType element = at[i][j];

            if (*((uint32*) (vector.GetDataPointer()) + j) != (i + j)) {
                return false;
            }
            if (*(uint32*) (element.GetDataPointer()) != (i + j)) {
                return false;
            }
        }
    }
    return true;

}

bool AnyTypeTest::TestPositionOperator_MatrixBasicHeapDeclared() {
    const uint32 nRows = 2;
    const uint32 nCols = 2;

    uint32 **test = (uint32**) HeapManager::Malloc(sizeof(uint32*) * nRows);
    for (uint32 i = 0; i < nRows; i++) {
        test[i] = new uint32[nCols];
        for (uint32 j = 0; j < nCols; j++) {
            test[i][j] = (i + j);
        }
    }

    AnyType at(UnsignedInteger32Bit, 0u, test);
    at.SetNumberOfDimensions(2);
    at.SetNumberOfElements(0, nRows);
    at.SetNumberOfElements(1, nCols);
    at.SetStaticDeclared(false);

    bool ret = true;
    for (uint32 i = 0; i < nRows; i++) {
        AnyType vector = at[i];
        for (uint32 j = 0; j < nCols; j++) {
            AnyType element = at[i][j];

            if (*((uint32*) (vector.GetDataPointer()) + j) != (i + j)) {
                ret = false;
            }
            if (*(uint32*) (element.GetDataPointer()) != (i + j)) {
                ret = false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        delete[] test[i];
    }
    HeapManager::Free((void*&) test);

    return ret;
}

bool AnyTypeTest::TestPositionOperator_VectorStructured() {

    const uint32 nElements = 32;

    TestATStructure test[nElements];
    for (uint32 i = 0; i < nElements; i++) {
        test[i].member1 = i;
    }

    TypeDescriptor tdes(false, ClassRegistryDatabase::Instance()->Find("TestATStructure")->GetClassProperties()->GetUniqueIdentifier());

    AnyType at(tdes, 0, test);
    at.SetNumberOfDimensions(1);
    at.SetNumberOfElements(0, nElements);

    for (uint32 i = 0; i < nElements; i++) {
        AnyType element = at[i];

        if ((*(TestATStructure*) (element.GetDataPointer())).member1 != (i)) {
            return false;
        }
    }
    return true;
}

bool AnyTypeTest::TestPositionOperator_VectorBasic() {
    const uint32 nElements = 32;

    uint32 test[nElements];
    for (uint32 i = 0; i < nElements; i++) {
        test[i] = i;
    }
    AnyType at(test);

    for (uint32 i = 0; i < nElements; i++) {
        AnyType element = at[i];

        if (*(uint32*) (element.GetDataPointer()) != (i)) {
            return false;
        }
    }
    return true;
}

bool AnyTypeTest::TestPositionOperator_VectorCString() {

    const uint32 nElements = 3;

    const char8 *test[] = { "Hello", "Ciao", "Hola" };
    AnyType at(test);

    for (uint32 i = 0; i < nElements; i++) {
        AnyType element = at[i];

        if (StringHelper::Compare((const char8*) element.GetDataPointer(), test[i]) != 0) {
            return false;
        }
    }
    return true;
}

bool AnyTypeTest::TestPositionOperator_VectorPointer() {
    const uint32 nElements = 3;

    const char8 *var[] = { "Hello", "Ciao", "Hola" };
    void *test[] = { &var[0], &var[1], &var[2] };
    AnyType at(test);

    for (uint32 i = 0; i < nElements; i++) {
        AnyType element = at[i];

        if (element.GetDataPointer() != test[i]) {
            return false;
        }
    }
    return true;
}
