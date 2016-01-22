/**
 * @file IntrospectionEntry.cpp
 * @brief Source file for class IntrospectionEntry
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

    dimensionSize[0] = 1u;
    dimensionSize[1] = 1u;
    dimensionSize[2] = 1u;

    if (modifiers != NULL) {
        int32 i=StringHelper::Length(modifiers);
        uint32 nDim=0u;
        while(modifiers[i]!='\0' && (nDim<3u)) {
            if(modifiers[i]=='[') {
                for(uint32 k=0u; k<nDim; k++) {
                    uint32 prev=(nDim-k)-1u;
                    dimensionSize[0]=0u;
                    dimensionSize[nDim-k]=dimensionSize[prev];
                }
                i++;
                while(modifiers[i]==']') {
                    dimensionSize[0]*=10u;
                    dimensionSize[0]=modifiers[i]-'0';
                    i++;
                }
                nDim++;
            }
            i++;
        }
        numberOfDimensions=nDim;
    }
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
                REPORT_ERROR(ErrorManagement::FatalError, "GetMemberTypeDescriptor: No ClassProperties associated to the specified structured object");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "GetMemberTypeDescriptor: No structured object with the specified type found inside the ClassRegistryDatabase");
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

bool IntrospectionEntry::IsConstant(const uint32 ptrLevel) const {
    bool ret = isConstant;
    if (modifiers != NULL) {
        if (ptrLevel > 0u) {
            uint32 i = 0u;
            uint32 ptrCnt = 0u;
            while (modifiers[i] != '\0') {
                if (modifiers[i] == '*') {
                    ptrCnt++;
                }
                if (ptrCnt == ptrLevel) {
                    i++;
                    ret = (modifiers[i] == 'C');
                    break;
                }
                i++;
            }
        }
    }
    return ret;
}

uint32 IntrospectionEntry::GetMemberPointerLevel() const {
    uint32 ptrLevel = 0u;
    uint32 i = 0u;
    if (modifiers != NULL) {
        while (modifiers[i] != '\0') {
            if (modifiers[i] == '*') {
                ptrLevel++;
            }
            i++;
        }
    }
    return ptrLevel;
}

uint32 IntrospectionEntry::GetNumberOfElements(const uint32 dimension) const{
    return (dimension < 3u) ? dimensionSize[dimension] : 0u;
}

uint32 IntrospectionEntry::GetNumberOfDimensions() const{
    return numberOfDimensions;
}


}

