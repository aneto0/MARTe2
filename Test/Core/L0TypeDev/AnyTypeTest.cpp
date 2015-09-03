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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace TypeDefinition;

AnyTypeTest::AnyTypeTest():dummy(1),constantPtrToConstant(&dummy), constPtr(NULL) {
    retVal = false;
    defaultBitAddress = 1;
    defaultDataDescription.isConstant = true;
    defaultDataDescription.isStructuredData = false;
    defaultDataDescription.size = 5;
    defaultDataDescription.type = Float;
}

bool AnyTypeTest::TestAnyType_Void(){
    AnyType anytype;

    retVal = (anytype.GetDataPointer() == NULL);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 0);
    retVal &= (td.size == 0);

    return retVal;
}

bool AnyTypeTest::TestAnyType_AnyTypeVoid(){
    AnyType anytype;
    AnyType anytype1(anytype);

    retVal = (anytype1.GetDataPointer() == NULL);
    retVal &= (anytype1.GetBitAddress() == 0);
    td = anytype1.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 0);
    retVal &= (td.size == 0);

    return retVal;
}

bool AnyTypeTest::TestAnyType_TypeDescriptor_BitAddress_ConstPointerToConst(){
    AnyType anytype(defaultDataDescription,  defaultBitAddress, constantPtrToConstant);

    retVal = (anytype.GetDataPointer() == &dummy);
    retVal &= (anytype.GetBitAddress() == 1);
    td = anytype.GetTypeDescriptor();
    retVal &= (td == defaultDataDescription);

    return retVal;
}

bool AnyTypeTest::TestAnyType_TypeDescriptor_BitAddress_ConstPointer(){
    AnyType anytype(defaultDataDescription,  defaultBitAddress, constPtr);

    retVal = (anytype.GetDataPointer() == NULL);
    retVal &= (anytype.GetBitAddress() == 1);
    td = anytype.GetTypeDescriptor();
    retVal &= (td == defaultDataDescription);

    return retVal;
}

bool AnyTypeTest::TestIsVoid(){
    AnyType anytype;
    AnyType anytype2(defaultDataDescription,  defaultBitAddress, constPtr);

    retVal = anytype.IsVoid();
    retVal &= !anytype2.IsVoid();

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int8(){
    int8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 0);
    retVal &= (td.size == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt8(){
    uint8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 1);
    retVal &= (td.size == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt8(){
    const int8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 0);
    retVal &= (td.size == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt8(){
    const uint8 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 1);
    retVal &= (td.size == 8);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int16(){
    int16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 0);
    retVal &= (td.size == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt16(){
    uint16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 1);
    retVal &= (td.size == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt16(){
    const int16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 0);
    retVal &= (td.size == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt16(){
    const uint16 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 1);
    retVal &= (td.size == 16);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int32(){
    int32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 0);
    retVal &= (td.size == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt32(){
    uint32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 1);
    retVal &= (td.size == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt32(){
    const int32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 0);
    retVal &= (td.size == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt32(){
    const uint32 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 1);
    retVal &= (td.size == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Int64(){
    int64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 0);
    retVal &= (td.size == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_UInt64(){
    uint64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 1);
    retVal &= (td.size == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstInt64(){
    const int64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 0);
    retVal &= (td.size == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstUInt64(){
    const uint64 a = 1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 1);
    retVal &= (td.size == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Float32(){
    float32 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 2);
    retVal &= (td.size == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstFloat32(){
    const float32 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 2);
    retVal &= (td.size == 32);

    return retVal;
}

bool AnyTypeTest::TestAnyType_Float64(){
    float64 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 2);
    retVal &= (td.size == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstFloat64(){
    const float64 a = 1.1;
    AnyType anytype(a);

    retVal = (anytype.GetDataPointer() == &a);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 2);
    retVal &= (td.size == 64);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstPointerToConts(){
    AnyType anytype(constantPtrToConstant);

    retVal = (anytype.GetDataPointer() == constantPtrToConstant);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 3);
    retVal &= (td.size == sizeof(void *) * 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstPointer(){
    AnyType anytype(constPtr);

    retVal = (anytype.GetDataPointer() == constPtr);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == 3);
    retVal &= (td.size == sizeof(void *) * 8u);

    return retVal;
}

bool AnyTypeTest::TestAnyType_ConstCharPointerToConst(){
    const char8 *const charPtr = "hello";
    AnyType anytype(charPtr);

    retVal = (anytype.GetDataPointer() == charPtr);
    retVal &= (anytype.GetBitAddress() == 0);
    td = anytype.GetTypeDescriptor();
    retVal &= (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == 8);
    retVal &= (td.size == sizeof(const char *) * 8u);

    return retVal;
}



