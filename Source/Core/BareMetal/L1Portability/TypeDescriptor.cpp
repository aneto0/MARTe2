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

#include <TypeDescriptor.h>
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

#if 0
/*lint -e9150 [MISRA C++ Rule 11-0-1]. Justification: a structure with no functions is used to describe this type. */
struct TypeCastInfo {
    TypeDescriptor typeDes;
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
        { Float32Bit, "float32" },
        { Float64Bit, "float64" },
        { Character8Bit, "char8" },
        { VoidType, "void"},
        { InvalidType, static_cast<const char8*>(NULL)}
};
#endif
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TypeDescriptor::TypeDescriptor() {
    all = 0;
}

TypeDescriptor::TypeDescriptor(const uint32 x) {
    all = x;
}


static BasicObjectSize GetRegularType(uint32 bits){
    BasicObjectSize  bos = SizeUnknown;
    if (bits > 32){
        if (bits > 128){
            if (bits == 256) bos = Size256bit;
            if (bits == 512) bos = Size512bit;
        } else {
            if (bits == 128) bos = Size128bit;
            if (bits == 64)  bos = Size64bit;
        }
    } else {
        if (bits == 32)      bos = Size32bit;
        if (bits == 16)      bos = Size16bit;
        if (bits == 8)       bos = Size8bit;
    }

    return bos;
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,
                   const BasicType typeIn,
                   const uint32 numberOfBitsIn,
                   const uint8  bitsOffsetIn){

    isConstant       = isConstantIn;
    type             = typeIn;
    isStructuredData = false;

    BasicObjectSize bos = GetRegularType(numberOfBitsIn);

    bool isBitType        = ((bos == SizeUnknown) || (bitsOffsetIn != 0u));

    if (isBitType) {
        if (typeIn == UnsignedInteger) {
            type = UnsignedBitInteger;
        } else
        if (typeIn == UnsignedInteger) {
            type = SignedBitInteger;
        } else {
            type = Invalid;
        }
        if (numberOfBitsIn <= 65535u){
            this->numberOfBits = numberOfBitsIn;
        } else {
            type = Invalid;
        }
        if (numberOfBitsIn <= 63u){
            this->bitOffset = bitsOffsetIn;
        } else {
            type = Invalid;
        }
    } else {
        objectSize  = bos;
        arrayType = Array1D;
        arraySize = 1u;
    }
}

TypeDescriptor::TypeDescriptor(    const bool isConstantIn,
                                   const BasicType typeIn,
                                   const BasicObjectSize objectSizeIn,
                                   const BasicArrayType arrayTypeIn,
                                   const uint32 numberOfColumnsIn,
                                   const uint32 numberOfRowsIn
               ){
    isConstant       = isConstantIn;
    type             = typeIn;
    isStructuredData = false;
    objectSize       = objectSizeIn;
    arrayType        = arrayTypeIn;


    switch (arrayTypeIn){
    case StaticZeroTermArray:{
        if (numberOfColumnsIn > 0xFFFFFu){
            arrayType            = ZeroTermArray;
        } else {
            arraySize            = numberOfColumnsIn;
        }
    case Array1D:{
        if (numberOfColumnsIn > 0xFFFFFu){
            arrayType            = ArrayLarge;
            arraySize            = 0;
        } else {
            arraySize            = numberOfColumnsIn;
        }
    }break;
    case Array2D:{
        if (numberOfRowsIn > 0x3FFu){
            arrayType            = ArrayLarge;
            if (numberOfRowsIn > 0x7FFFFu){
                arraySize            = 0;
            } else {
                arraySize            = numberOfRowsIn;
            }
        } else {
            if (numberOfColumnsIn > 0x3FFu){
                arrayType            = ArrayLarge;
                arraySize            = numberOfRowsIn;
            } else {
                numberOfColumns      = numberOfColumnsIn;
                numberOfRows         = numberOfRowsIn;
            }
        }
    }break;
    case ArrayLarge:{
        if (numberOfRowsIn > 0x7FFFFu){
            arraySize            = 0;
        } else {
            arraySize            = numberOfRowsIn;
        }
    }break;
    default:{

    }

    }

}


TypeDescriptor::TypeDescriptor(const bool isConstantIn,
                               const uint32  &structuredDataIdCodeIn) {
    isStructuredData = true;
    isConstant       = isConstantIn;
    structuredDataIdCode = structuredDataIdCodeIn;
}

bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
//    bool ret = ((all | (0x0002u)) == (typeDescriptor.all | (0x0002u)));
//    return ret;
    return all == typeDescriptor.all;
}

bool TypeDescriptor::operator!=(const TypeDescriptor &typeDescriptor) const {
//    bool ret = ((all | (0x0002u)) != (typeDescriptor.all | (0x0002u)));
//    return ret;
    return all == typeDescriptor.all;
}

/*
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
*/

}
