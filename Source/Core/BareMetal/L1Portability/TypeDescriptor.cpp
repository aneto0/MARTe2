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
    combinedArrayType = SizedCArray;
    arraySize        = 1u;
    type             = Void;
}

TypeDescriptor::TypeDescriptor(const uint32 x) {
    all = x;
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,const ComplexSubType 	subType){
    dataIsConstant   = isConstantIn;
    type             = ComplexType;
    complexType      = subType;
    combinedArrayType = SizedCArray;
    arraySize        = 1u;
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,
                   const BasicType typeIn,
                   const uint32 numberOfBitsIn,
                   const uint8  bitsOffsetIn  ){

    dataIsConstant   = isConstantIn;
    type             = typeIn;
    isStructuredData = false;

	BasicObjectSize bos;
    if (typeIn == Void){
    	bos = SizeUnknown;
    } else {
    	bos = BasicObjectSizeFromBits(numberOfBitsIn);
    }
	objectSize  = bos;
    type        = typeIn;
    arrayProperty = SizedCArray;
    arraySize     = 1u;


    bool isBitType        = ((objectSize == SizeBits) || (bitsOffsetIn != 0u));

    if (isBitType) {
        if ((typeIn != SignedInteger) &&  (typeIn != UnsignedInteger)){
            type     = Void;
        }
        if (numberOfBitsIn <= 65535u){
            this->numberOfBits = numberOfBitsIn;
        } else {
            type    = Void;
        }
        if (numberOfBitsIn <= 63u){
            this->bitOffset = bitsOffsetIn;
        } else {
            type 	= Void;
        }
    }
}


TypeDescriptor::TypeDescriptor(    const bool isConstantIn,
                                   const BasicType typeIn,
                                   const BasicObjectSize objectSizeIn,
                                   const CombinedArrayType arrayTypeIn,
                                   const uint32 arraySizeIn
               ){
    dataIsConstant       = isConstantIn;
    type             = typeIn;
    isStructuredData = false;
    objectSize       = objectSizeIn;
    if (objectSizeIn == SizeBits) type = Void;

    combinedArrayType = arrayTypeIn;

    switch (arrayTypeIn){

    case SizedCArray:
    case ConstStaticZeroTermArray:
    case StaticZeroTermArray:{
    	uint32 max = 1 << arraySize.GetNumberOfBits();

    	if (arraySizeIn >= (max - 1)){
    		arraySize = 0;  /// large array
    	} else {
    		arraySize = arraySizeIn;
    	}
    } break;
    default:{};

    };
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,const uint32  &structuredDataIdCodeIn) {
    isStructuredData = true;
    dataIsConstant   = isConstantIn;
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

const TypeDescriptor Character8Bit(false, Char, 8u);
const TypeDescriptor Float32Bit(false, Float, 32u);
const TypeDescriptor Float64Bit(false, Float, 64u);
const TypeDescriptor Float128Bit(false, Float, 128u);
const TypeDescriptor VoidType(false, Void, 0u);
const TypeDescriptor SignedInteger8Bit(false, SignedInteger, 8u);
const TypeDescriptor UnsignedInteger8Bit(false, UnsignedInteger, 8u);
const TypeDescriptor SignedInteger16Bit(false, SignedInteger, 16u);
const TypeDescriptor UnsignedInteger16Bit(false, UnsignedInteger, 16u);
const TypeDescriptor SignedInteger32Bit(false, SignedInteger, 32u);
const TypeDescriptor UnsignedInteger32Bit(false, UnsignedInteger, 32u);
const TypeDescriptor SignedInteger64Bit(false, SignedInteger, 64u);
const TypeDescriptor UnsignedInteger64Bit(false, UnsignedInteger, 64u);
const TypeDescriptor ConstCharString(true, Char, Size8bit,ZeroTermArray,  1);
const TypeDescriptor CharString(false,  Char, Size8bit,ZeroTermArray,  1);
const TypeDescriptor DynamicCharString(false,  Char, Size8bit,DynamicZeroTermArray, 1);
const TypeDescriptor StaticCharString(false,  Char, Size8bit,StaticZeroTermArray, 1);
const TypeDescriptor StructuredDataInterfaceType(false, StructuredDataInterface);
const TypeDescriptor VoidPointer(false, Void, Size8bit,PointerArray,0);
const TypeDescriptor ConstVoidPointer(false, Void, Size8bit,ConstPointerArray,0);
const TypeDescriptor ArrayLayerType(false, DelegatedType, Size8bit,SizedCArray,0);


}
