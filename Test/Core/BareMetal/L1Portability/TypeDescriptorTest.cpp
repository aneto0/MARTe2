/**
 * @file TypeDescriptorTest.cpp
 * @brief Source file for class TypeDescriptorTest
 * @date 03/09/2015
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
 * the class TypeDescriptorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptorTest.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static const TypeDescriptor typeDes[] = { CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, SignedInteger64Bit, UnsignedInteger8Bit, UnsignedInteger16Bit, UnsignedInteger32Bit, UnsignedInteger64Bit, BooleanType, Float32Bit, Float64Bit, Character8Bit, VoidType, InvalidType };
static const char8* typeNames[] = { "string", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "bool", "float32", "float64", "char8", "void", static_cast<const char8*>(NULL)};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool TypeDescriptorTest::TestConstructorUint(bool isObject,
                                             bool isConst,
                                             uint16 size,
                                             BasicType type) {

    uint16 initNumber = (isObject) | (isConst << 1) | (type << 2) | (size << 6);
    TypeDescriptor testTD = initNumber;

    if (testTD.isStructuredData != isObject) {
        return false;
    }

    if (testTD.isConstant != isConst) {
        return false;
    }

    if (testTD.numberOfBits != size) {
        return false;
    }

    if (testTD.type != type) {
        return false;
    }

    return true;
}

bool TypeDescriptorTest::TestConstructorBasicType(bool isConst,
                                                  uint16 size,
                                                  BasicType type) {

    TypeDescriptor testTD(isConst, type, size);

    if (testTD.isStructuredData != false) {
        return false;
    }

    if (testTD.isConstant != isConst) {
        return false;
    }

    if (testTD.numberOfBits != size) {
        return false;
    }

    if (testTD.type != type) {
        return false;
    }

    return true;

}

bool TypeDescriptorTest::TestConstructorObject(bool isConst,
                                               uint16 objCode) {

    TypeDescriptor testTD(isConst, objCode);

    if (testTD.isStructuredData != true) {
        return false;
    }

    if (testTD.isConstant != isConst) {
        return false;
    }

    if (testTD.structuredDataIdCode != objCode) {
        return false;
    }

    return true;
}

bool TypeDescriptorTest::TestIsEqualOperator(uint16 size,
                                             BasicType type) {

    bool isConst = true;
    TypeDescriptor byBasicType1(isConst, type, size);
    isConst = false;
    TypeDescriptor byBasicType2(isConst, type, size);

    if (!(byBasicType1 == byBasicType2)) {
        return false;
    }

    uint16 code = (type) | (size << 4);
    isConst = true;
    TypeDescriptor byObj1(isConst, code);
    isConst = false;
    TypeDescriptor byObj2(isConst, code);

    if (!(byObj1 == byObj2)) {
        return false;
    }

    return !(byBasicType1 == byObj1);
}

bool TypeDescriptorTest::TestIsEqualOperatorFail() {

    TypeDescriptor test1(0x10);

    TypeDescriptor test2(0x20);

    return !(test1 == test2);

}

bool TypeDescriptorTest::TestFieldSaturation() {

    uint16 size = 1 << 10;
    uint16 type = 1 << 4;

    bool isConst = true;

    TypeDescriptor testTD1(isConst, type, size);
    if (testTD1.all != 0xfffe) {
        return false;
    }

    type = 0;
    TypeDescriptor testTD2(isConst, type, size);
    if (testTD2.all != 0xffc2) {
        return false;
    }

    uint16 code = (size << 4) | (type);
    TypeDescriptor testTD3(isConst, code);
    return testTD3.all != 0xfffe;
}

bool TypeDescriptorTest::TestGetTypeDescriptorFromTypeName() {

    uint32 i = 0u;
    while (typeNames[i] != NULL) {
        if(TypeDescriptor::GetTypeDescriptorFromTypeName(typeNames[i])!=typeDes[i]) {
            return false;
        }
        i++;
    }
    return true;
}

bool TypeDescriptorTest::TestGetTypeNameFromTypeDescriptor() {
    uint32 i = 0u;
    while (typeNames[i] != NULL) {
        if(StringHelper::Compare(TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDes[i]),typeNames[i])!=0) {
            return false;
        }
        i++;
    }
    return true;
}

bool TypeDescriptorTest::TestGetTypeDescriptorFromStaticTable() {
    uint32 i = 0u;
    while (typeDes[i] != InvalidType) {
        if (TypeDescriptor::GetTypeDescriptorFromStaticTable(i) != typeDes[i]) {
            return false;
        }
        i++;
    }

    return true;
}

bool TypeDescriptorTest::TestGetTypeNameFromStaticTable() {
    uint32 i = 0u;
    while (typeDes[i] != InvalidType) {
        if (StringHelper::Compare(TypeDescriptor::GetTypeNameFromStaticTable(i), typeNames[i]) != 0) {
            return false;
        }
        i++;
    }

    return true;
}
