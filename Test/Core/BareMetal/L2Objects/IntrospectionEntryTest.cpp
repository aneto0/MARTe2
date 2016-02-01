/**
 * @file IntrospectionEntryTest.cpp
 * @brief Source file for class IntrospectionEntryTest
 * @date 20/01/2016
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
 * the class IntrospectionEntryTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IntrospectionEntryTest.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

bool IntrospectionEntryTest::TestConstructor(const char8* memberName,
                                             const char8 *type,
                                             const char8 *modifiers,
                                             const char8 *attributes,
                                             uint32 size,
                                             uint23 index) {

    IntrospectionEntry memberInfo(memberName, type, modifiers, attributes, size, index);

    if (StringHelper::Compare(memberInfo.GetMemberName(), memberName) != 0) {
        return false;
    }

    if (StringHelper::Compare(memberInfo.GetMemberTypeName(), type) != 0) {
        return false;
    }

    if (StringHelper::Compare(memberInfo.GetMemberModifiers(), modifiers) != 0) {
        return false;
    }

    if (StringHelper::Compare(memberInfo.GetMemberAttributes(), attributes) != 0) {
        return false;
    }

    if (memberInfo.GetMemberSize() != size) {
        return false;
    }
    if (memberInfo.GetMemberByteOffset() != index) {
        return false;
    }

    return true;
}

bool IntrospectionEntryTest::TestGetMemberName(const char8* memberName) {
    IntrospectionEntry memberInfo(memberName, "", "", "", 0, 0);
    return (memberName == NULL)?(memberInfo.GetMemberName()==NULL):(StringHelper::Compare(memberInfo.GetMemberName(), memberName) == 0);
}

bool IntrospectionEntryTest::TestGetMemberModifiers(const char8* modifiers) {
    IntrospectionEntry memberInfo("", "", modifiers, "", 0, 0);
    return (modifiers == NULL)?(memberInfo.GetMemberModifiers()==NULL):(StringHelper::Compare(memberInfo.GetMemberModifiers(), modifiers) == 0);
}

bool IntrospectionEntryTest::TestGetMemberSize(uint32 size) {
    IntrospectionEntry memberInfo("", "", "", "", size, 0);
    return memberInfo.GetMemberSize() == size;
}

bool IntrospectionEntryTest::TestGetMemberByteOffset(uint32 byteOffset) {
    IntrospectionEntry memberInfo("", "", "", "", 0, byteOffset);
    return memberInfo.GetMemberByteOffset() == byteOffset;
}

bool IntrospectionEntryTest::TestGetMemberTypeName(const char8* typeName) {
    IntrospectionEntry memberInfo("", typeName, "", "", 0, 0);
    return (typeName == NULL)?(memberInfo.GetMemberTypeName()==NULL):(StringHelper::Compare(memberInfo.GetMemberTypeName(), typeName) == 0);
}

bool IntrospectionEntryTest::TestGetMemberTypeDescriptor() {

    const TypeDescriptor typeDes[] = { CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, SignedInteger64Bit, UnsignedInteger8Bit,
            UnsignedInteger16Bit, UnsignedInteger32Bit, UnsignedInteger64Bit, Float32Bit, Float64Bit, Character8Bit, VoidType, InvalidType };

    const char8* typeNames[] = { "string", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "float32", "float64", "char8", "void",
            static_cast<const char8*>(NULL)};

    uint32 i = 0u;
    while (typeNames[i] != NULL) {
        IntrospectionEntry memberInfo("", typeNames[i], "", "",0, 0);
        if(memberInfo.GetMemberTypeDescriptor()!=typeDes[i]) {
            return false;
        }
        i++;
    }
    IntrospectionEntry memberInfo("", "Object", "", "", 0, 0);
    return memberInfo.GetMemberTypeDescriptor().isStructuredData;
}

bool IntrospectionEntryTest::TestGetMemberAttributes(const char8 * attributes) {
    IntrospectionEntry memberInfo("", "", "", attributes, 0, 0);
    return (attributes == NULL)?(memberInfo.GetMemberModifiers()==NULL):(StringHelper::Compare(memberInfo.GetMemberAttributes(), attributes) == 0);

}

bool IntrospectionEntryTest::TestIsConstant(const IntrospectionEntryTestTable *table) {

    uint32 i = 0u;
    while (table[i].modifiers != NULL) {
        IntrospectionEntry memberInfoConstant("","", table[i].modifiers,"", 0,0);
        if(memberInfoConstant.IsConstant(table[i].level)!=table[i].expected) {
            return false;
        }
        i++;
    }

    return true;
}

bool IntrospectionEntryTest::TestGetMemberPointerLevel(const IntrospectionEntryTestTable *table) {
    uint32 i = 0u;
    while (table[i].modifiers != NULL) {
        IntrospectionEntry memberInfoConstant("","", table[i].modifiers,"", 0,0);
        if(memberInfoConstant.GetMemberPointerLevel()!=table[i].level) {
            return false;
        }
        i++;
    }

    return true;
}
