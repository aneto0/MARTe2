/**
 * @file TypeDescriptor.cpp
 * @brief Source file for class TypeDescriptor
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
 * the class TypeDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Type cast information
 */
/*lint -e9150 [MISRA C++ Rule 11-0-1]. Justification: a structure with no functions is used to describe this type. */
struct TypeCastInfo {
    /**
     * Type description
     */
    TypeDescriptor typeDes;
    /**
     * Name of the cast
     */
    const char8 *castName;
};

static const TypeCastInfo basicTypeInfo[] = {
        { CharString, "string" },
        { SignedInteger8Bit, "int8" },
        { SignedInteger16Bit, "int16" },
        { SignedInteger32Bit,"int32" },
        { SignedInteger64Bit, "int64" },
        { UnsignedInteger8Bit, "uint8" },
        { UnsignedInteger16Bit, "uint16" },
        { UnsignedInteger32Bit, "uint32" },
        { UnsignedInteger64Bit, "uint64" },
        { BooleanType, "bool" },
        { Float32Bit, "float32" },
        { Float64Bit, "float64" },
        { Character8Bit, "char8" },
        { VoidType, "void"},
        { InvalidType, static_cast<const char8*>(NULL)}
};
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TypeDescriptor::TypeDescriptor(const uint16 x) {
    all = x;
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,
                               const uint16 typeIn,
                               const uint16 numberOfBitsIn) {
    isStructuredData = false;
    isConstant = isConstantIn;
    type = typeIn;
    numberOfBits = numberOfBitsIn;
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,
                               const ClassUID &structuredDataIdCodeIn) {
    isStructuredData = true;
    isConstant = isConstantIn;
    structuredDataIdCode = structuredDataIdCodeIn;
}

bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
    bool ret = ((all | (0x0002u)) == (typeDescriptor.all | (0x0002u)));
    return ret;
}

bool TypeDescriptor::operator!=(const TypeDescriptor &typeDescriptor) const {
    bool ret = ((all | (0x0002u)) != (typeDescriptor.all | (0x0002u)));
    return ret;
}

bool TypeDescriptor::IsNumericType() const {

    bool isFloat           = (type == Float);
    bool isSignedInteger   = (type == SignedInteger);
    bool isUnsignedInteger = (type == UnsignedInteger);
    bool isBoolean         = (type == BT_Boolean);
    
    bool isNumeric = (isFloat || isSignedInteger || isUnsignedInteger);
    
    bool hasNonZeroSize = (numberOfBits > 0u);
    
    return ( (!isStructuredData) && (hasNonZeroSize) && (isNumeric) && (!isBoolean));
}

TypeDescriptor TypeDescriptor::GetTypeDescriptorFromTypeName(const char8 * const typeName){
    uint32 typeIndex = 0u;
    while (basicTypeInfo[typeIndex].castName != NULL) {
        if (StringHelper::Compare(typeName, basicTypeInfo[typeIndex].castName) == 0) {
            break;
        }
        else {
            typeIndex++;
        }
    }

    return basicTypeInfo[typeIndex].typeDes;
}


const char8 *TypeDescriptor::GetTypeNameFromTypeDescriptor(const TypeDescriptor &typeDescriptor){
    uint32 typeIndex = 0u;
    while (basicTypeInfo[typeIndex].typeDes != InvalidType) {
        if (basicTypeInfo[typeIndex].typeDes == typeDescriptor) {
            break;
        }
        else {
            typeIndex++;
        }
    }

    return basicTypeInfo[typeIndex].castName;
}

TypeDescriptor TypeDescriptor::GetTypeDescriptorFromStaticTable(const uint32 index){
    return basicTypeInfo[index].typeDes;
}

const char8 *TypeDescriptor::GetTypeNameFromStaticTable(const uint32 index){
    return basicTypeInfo[index].castName;
}


}
