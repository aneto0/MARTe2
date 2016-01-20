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
                                             bool isConstant,
                                             const char8 *modifiers,
                                             uint32 size,
                                             uint23 index) {

    IntrospectionEntry memberInfo(memberName, type, isConstant, modifiers, size, index);

    if (StringHelper::Compare(memberInfo.GetMemberName(), memberName) != 0) {
        return false;
    }
    if (StringHelper::Compare(memberInfo.GetMemberModifiers(), modifiers) != 0) {
        return false;
    }

    if (memberInfo.GetMemberSize() != size) {
        return false;
    }
    if (memberInfo.GetMemberByteOffset() != index) {
        return false;
    }

    return memberInfo.IsConstant() == isConstant;
}

bool IntrospectionEntryTest::TestGetMemberName(const char8* memberName) {
    IntrospectionEntry memberInfo(memberName, "", 0, "", 0, 0);
    return (memberName == NULL)?(memberInfo.GetMemberName()==NULL):(StringHelper::Compare(memberInfo.GetMemberName(), memberName) == 0);
}

bool IntrospectionEntryTest::TestGetMemberModifiers(const char8* modifiers) {
    IntrospectionEntry memberInfo("", "", 0, modifiers, 0, 0);
    return (modifiers == NULL)?(memberInfo.GetMemberModifiers()==NULL):(StringHelper::Compare(memberInfo.GetMemberModifiers(), modifiers) == 0);
}


bool IntrospectionEntryTest::TestGetMemberSize(uint32 size){
    IntrospectionEntry memberInfo("", "", 0, "", size, 0);
    return memberInfo.GetMemberSize()==size;
}


bool IntrospectionEntryTest::TestGetMemberByteOffset(uint32 byteOffset){
    IntrospectionEntry memberInfo("", "", 0, "", 0, byteOffset);
    return memberInfo.GetMemberByteOffset()==byteOffset;
}

bool IntrospectionEntryTest::TestGetMemberTypeDescriptor(){

    const TypeDescriptor typeDes[] = { CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, SignedInteger64Bit, UnsignedInteger8Bit,
            UnsignedInteger16Bit, UnsignedInteger32Bit, UnsignedInteger64Bit, Float32Bit, Float64Bit, Character8Bit, VoidType, InvalidType };

    const char8* typeNames[] = { "string", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "float32", "float64", "char8", "void",
            static_cast<const char8*>(NULL)};

    uint32 i=0u;
    while (typeNames[i]!=NULL){
        IntrospectionEntry memberInfo("", typeNames[i], 0, "", 0, 0);
        if(memberInfo.GetMemberTypeDescriptor()!=typeDes[i]){
            return false;
        }
        i++;
    }
    IntrospectionEntry memberInfo("", "Object", 0, "", 0, 0);
    return memberInfo.GetMemberTypeDescriptor().isStructuredData;
}


bool IntrospectionEntryTest::TestIsConstant(){
    IntrospectionEntry memberInfoConstant("", "", true, "", 0, 0);
    IntrospectionEntry memberInfoNoConstant("", "", false, "", 0, 0);
    return (memberInfoConstant.IsConstant()) && !(memberInfoNoConstant.IsConstant());
}
