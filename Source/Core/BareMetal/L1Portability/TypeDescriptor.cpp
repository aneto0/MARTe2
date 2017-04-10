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
#include "Vector.h"
#include "Matrix.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


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
    dataIsConstant   = isConstantIn;
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

//#include <stdio.h>

uint32 TypeDescriptor::Size(){
	uint32 size = 0;
	if (isStructuredData){
		return 0; // not supported yet
	} else {
	    if (IsBitType()){
			uint32 totalBitSpan = this->numberOfBits + this->bitOffset;
			uint32 totalByteSpan = ( totalBitSpan +7)/8;
			uint32 actualSize = 1;
			while (actualSize < totalByteSpan){
				actualSize = actualSize * 2;
			}
			size = actualSize;
//printf (" %i %i ", totalBitSpan, size);
		} else {
			uint32 multiplier = 1u;
			uint32 baseSize = 1u;
			if (arrayProperty == SizedCArray_AP){
				multiplier = arraySize;
				switch(type){
				case  SignedInteger:
				case  UnsignedInteger:
				case  Float:
				case  Char:{
					size = arraySize * (BitsFromBasicObjectSize(objectSize) / 8);
				} break;
				default: {
					size = 0;
				}
				}
			} else {
				if (arrayProperty == UnSizedA_AP){
					switch (arrayType){
					case PointerArray_BT:
					case ZeroTermArray_BT:
 					case DynamicZeroTermArray_BT:{
						size = sizeof (void *);
					} break;
					case Array1D_BT:{
						size = sizeof (Vector<char8>);
					} break;
					case Array2D_BT:{
						size = sizeof (Matrix<char8>);
					} break;
					default:{
						size = 0;
					}
					}
				} else {
					// StaticZeroTerm is a pointer
					size = sizeof (void *);
				}
			}

		}
	}
	return size;
}



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
const TypeDescriptor DelegatedType(false,Delegated,SizeUnknown,SizedCArray,1);


}
