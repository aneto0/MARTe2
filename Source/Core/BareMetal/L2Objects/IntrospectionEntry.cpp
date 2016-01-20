/**
 * @file IntrospectionEntry.cpp
 * @brief Source file for class IntrospectionEntry
 * @date 20/gen/2016
 * @author pc
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
 * the class IntrospectionEntry (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IntrospectionEntry.h"
#include "ClassRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

IntrospectionEntry::IntrospectionEntry(const char8 * const memberNameIn,
                                       const char8* const typeNameIn,
                                       const bool isConstantIn,
                                       const char8* const modifiersIn,
                                       const uint32 sizeIn,
                                       const uint32 byteOffsetIn) {
    memberName = memberNameIn;
    modifiers = modifiersIn;
    size = sizeIn;
    byteOffset = byteOffsetIn;
    typeName = typeNameIn;
    isConstant = isConstantIn;
}

const char8* IntrospectionEntry::GetMemberName() const {
    return memberName;
}

TypeDescriptor IntrospectionEntry::GetMemberTypeDescriptor() const {
    TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);
    // Not a basic type !
    if (typeDes == InvalidType) {
        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
        if (item != NULL) {
            const ClassProperties *properties=item->GetClassProperties();
            if(properties!=NULL) {
                typeDes=TypeDescriptor(isConstant,properties->GetUniqueIdentifier());
                typeDes.isConstant=isConstant;
            }
            else {
                // TODO
            }
        }
        else {
            // TODO
        }
    }
    else {
        typeDes.isConstant=isConstant;
    }
    return typeDes;
}

uint32 IntrospectionEntry::GetMemberSize() const {
    return size;
}

const char8 * IntrospectionEntry::GetMemberModifiers() const {
    return modifiers;
}

uint32 IntrospectionEntry::GetMemberByteOffset() const {
    return byteOffset;
}

bool IntrospectionEntry::IsConstant() const {
    return isConstant;
}
}

