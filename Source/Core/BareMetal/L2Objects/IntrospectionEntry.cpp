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
 *
 * @details This source file contains the definition of all the methods for
 * the class IntrospectionEntry (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassProperties.h"
#include "IntrospectionEntry.h"
#include "ClassRegistryDatabase.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

IntrospectionEntry::IntrospectionEntry() {

    memberName = NULL_PTR(const char8*);
    typeName = NULL_PTR(const char8*);
    modifiers = NULL_PTR(const char8*);
    attributes = NULL_PTR(const char8*);
    size = 0u;
    byteOffset = 0u;
    numberOfDimensions = 1u;
    dimensionSize[0] = 1u;
    dimensionSize[1] = 1u;
    dimensionSize[2] = 1u;
}

IntrospectionEntry::IntrospectionEntry(const char8 * const memberNameIn,
                                       const char8* const typeNameIn,
                                       const char8* const modifiersIn,
                                       const char8* const attributesIn,
                                       const uint32 sizeIn,
                                       const uint32 byteOffsetIn) {
    memberName = memberNameIn;
    modifiers = modifiersIn;
    attributes = attributesIn;
    size = sizeIn;
    byteOffset = byteOffsetIn;
    typeName = typeNameIn;

    dimensionSize[0] = 1u;
    dimensionSize[1] = 1u;
    dimensionSize[2] = 1u;

    numberOfDimensions = 0u;

    if (modifiers != NULL) {
        uint32 i = 0u;
        uint8 nDim = 0u;
        uint32 lastDim = 0u;
        while ((modifiers[i] != '\0') && (nDim < 3u)) {
            if (modifiers[i] == '[') {
                lastDim = 0u;
                i++;
                while ((modifiers[i] != ']') && (modifiers[i] != '\0')) {
                    lastDim *= 10u;
                    int32 zero = static_cast<int32>('0');
                    int32 digit = (static_cast<int32>(modifiers[i]) - zero);
                    if ((digit >= 0) && ((digit <= 9))) {
                        lastDim += static_cast<uint32>(digit);
                    }
                    i++;
                }
                dimensionSize[nDim] = lastDim; 
                nDim++;
            }
            i++;
        }
        numberOfDimensions = nDim;
    }

}

const char8* IntrospectionEntry::GetMemberName() const {
    return memberName;
}

const char8* IntrospectionEntry::GetMemberTypeName() const {
    return typeName;
}

TypeDescriptor IntrospectionEntry::GetMemberTypeDescriptor() const {
    TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);

    bool isConstant = false;
    if (modifiers != NULL) {
        isConstant = (modifiers[0] == 'C');
    }

    // Not a basic type !
    if (typeDes == InvalidType) {
        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
        if (item != NULL) {
            const ClassProperties *properties = item->GetClassProperties();
            if (properties != NULL) {
                typeDes = TypeDescriptor(isConstant, properties->GetUniqueIdentifier());
                typeDes.isConstant = isConstant;
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "GetMemberTypeDescriptor: No ClassProperties associated to the specified structured object");
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,
                         "GetMemberTypeDescriptor: No structured object with the specified type found inside the ClassRegistryDatabase");
        }
    }
    else {
        typeDes.isConstant = isConstant;
    }

    return typeDes;
}

uint32 IntrospectionEntry::GetMemberSize() const {
    return size;
}

const char8 * IntrospectionEntry::GetMemberModifiers() const {
    return modifiers;
}

const char8 * IntrospectionEntry::GetMemberAttributes() const {
    return attributes;
}

uint32 IntrospectionEntry::GetMemberByteOffset() const {
    return byteOffset;
}

bool IntrospectionEntry::IsConstant(const uint32 ptrLevel) const {
    bool ret = false;
    if (modifiers != NULL) {
        ret = (modifiers[0] == 'C');
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

uint32 IntrospectionEntry::GetNumberOfElements(const uint32 dimension) const {
    return (dimension < 3u) ? dimensionSize[dimension] : dimensionSize[2];
}

uint8 IntrospectionEntry::GetNumberOfDimensions() const {
    return numberOfDimensions;
}

}

