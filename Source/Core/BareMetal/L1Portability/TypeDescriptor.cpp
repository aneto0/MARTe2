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
#include "ClassRegistryIndex.h"


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
    return all == typeDescriptor.all;
}

bool TypeDescriptor::operator!=(const TypeDescriptor &typeDescriptor) const {
    return all == typeDescriptor.all;
}

//#include <stdio.h>

uint32 TypeDescriptor::Size()const{
	uint32 size = 0;
	if (isStructuredData){
		uint32 size = 0;
		ClassRegistryIndex *cri = ClassRegistryIndex::Instance();
		if (cri != NULL_PTR(ClassRegistryIndex *)){
			ClassRegistryBrief *crb = (*cri)[structuredDataIdCode];
			if (crb != NULL_PTR(ClassRegistryBrief *)){
				size = crb->sizeOfClass;
			}
		}
        return size;
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
//			uint32 baseSize = 1u;
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

/**
 * initialised to NULL
 */
static CCString (*IdToString)(TypeDescriptor) = NULL;

/**
 * @brief gets name of class from structuredDataIdCode
 */
bool TypeDescriptor::SetNameOfClassFromIdFunction(CCString (*fun)(TypeDescriptor)){
	IdToString = fun;
	return true;
}

/**
 * @brief gets name of class from structuredDataIdCode
 */
CCString TypeDescriptor::GetNameOfClassFromStructureId() const{
	CCString ret = "?";
	if (IdToString != NULL){
		ret = IdToString(*this);
	}
	return ret;
}

#define TYPENAME_CORE()             \
       	string.AppendN(constString);\
       	string.AppendN(typeName);   \
       	string.AppendNum(bits);

#define TEMPLATED_TYPENAME_CORE(className)       \
       	string.AppendN(#className "<");        \
       	string.AppendN(constString);			 \
       	string.AppendN(typeName);   			 \
       	string.AppendNum(bits);                  \
       	string.Append('>');


bool TypeDescriptor::ToString(DynamicCString &string) const{
	bool ret= true;

    const char8 *constString2= "const ";
    const char8 *constString= "";
    if (dataIsConstant){
    	constString = constString2;
    }

    if (isStructuredData){
    	string.AppendN(constString);
    	CCString className = GetNameOfClassFromStructureId();

    	if (className.GetSize()==0){
        	string.AppendN("unknown_struct_code(");
        	uint32 idCode = structuredDataIdCode;
        	string.AppendNum(idCode);
        	string.Append(')');
    	} else {
           	string.AppendN(className);
    	}
    }
    else // !isStructuredData
   	if (IsComplexType()){  //Stream,StructuredData,..
   		string.AppendN(constString);
   		string.AppendN(BasicTypeName(type,complexType));
   	}
   	else { //!IsComplexType()
   		CCString typeName = BasicTypeName(type,0);
   		if (IsBitType()){  // uint5 bitranges
   			string.AppendN(constString);
   			string.AppendN(typeName);
   			uint32 numberOfBitsR = (int)numberOfBits;
   			uint32 bitOffsetR = (int)bitOffset;
   			string.AppendNum(numberOfBitsR);
   			if (bitOffsetR > 0){
   				string.Append(':');
   				string.AppendNum(bitOffsetR);
   			}
   		}
   		else {// not bit type
			uint32 bits = BitsFromBasicObjectSize((int)objectSize);
	    	uint32 size = arraySize;
   			if (arrayProperty == SizedCArray_AP){   // []
   				TYPENAME_CORE()
   			    if (size == 0){
   					string.AppendN("[>2M]");
   			    }
   			    else // not infinite array
   			    if (size > 1){
   			    	string.AppendN("[");
   			    	string.AppendNum(size);
   			    	string.Append(']');
   			    }
   			} else // not sized array
   			if ((arrayProperty == StaticZeroTermArray_AP) ||
   				(arrayProperty == ConstStaticZeroTermArray_AP)){

   				if (arrayProperty == ConstStaticZeroTermArray_AP){
   		       		string.AppendN(constString2);
   		       	}
   		       	// support of StaticCString<size>
   		       	if (type == Char){
   		       		string.AppendN("StaticCString<");
   		       	} else {
   		       		// TODO
   		       		// generic type
   		       		// not generated by AnyType/VariableDescription
   		       		// uses modifiers instead
   		       		string.AppendN("StaticZeroTerminatedArray<");
   	   				TYPENAME_CORE()
   		       		string.Append(',');
   		       	}
	           	if (arraySize == 0){
	              	string.AppendN(">2M>");
	           	} else {
	               	string.AppendNum(size);
		            string.Append('>');
		        }
   			}   // not static zero term array
   			else
   	        { //other
   	        	uint32 cType = combinedArrayType;
   	        	switch(cType){
   	            case ConstZeroTermArray:
   	            case ZeroTermArray:
   	            case DynamicZeroTermArray:
   	            case ConstDynamicZeroTermArray:
   					{
   	            	if ((cType == ConstZeroTermArray) || (cType == ConstDynamicZeroTermArray)){
   	            		string.AppendN(constString2);
   	            	}
   	            	if ((cType == DynamicZeroTermArray) || (cType == ConstDynamicZeroTermArray)){
   	            		string.AppendN("Dynamic");
   	            	}

   	            	// support of CString<size>, DynamicCString
   	            	if ((type == Char) && (!dataIsConstant)) {
   	            		string.AppendN("CString");
   	            	} else
   	                if ((type == Char) && (dataIsConstant)) {
   	                	string.AppendN("CCString");
   	                } else {
   	                	TEMPLATED_TYPENAME_CORE(ZeroTerminatedArray)
   	                }
   	            }break;
   	            case ConstArray1D:
   	            case Array1D:{
   	            	if (cType == ConstArray1D) {
   	            		string.AppendN(constString2);
   	            	}
                	TEMPLATED_TYPENAME_CORE(Vector)
   	            }break;
   	            case ConstArray2D:
   	            case Array2D:{
   	            	if (cType == ConstArray2D) {
   	            		string.AppendN(constString2);
   	            	}
                	TEMPLATED_TYPENAME_CORE(Matrix)
   	            }break;
   	            case ConstPointerArray:
   	            case PointerArray:{
   	   				TYPENAME_CORE()
   	            	string.Append('*');
   	            	if (cType == ConstPointerArray) {
   	            		string.AppendN(constString2);
   	            	}
   	            }break;
   	            default:
   	            case ArrayUnknown:{
   	   				TYPENAME_CORE()
   	            	string.AppendN(" ?");
   	            }break;
   	            }
   	        } //	 other
   		}
   	}
	return ret;
}


}
