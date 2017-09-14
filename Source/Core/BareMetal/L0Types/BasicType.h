/**
 * @file BasicType.h
 * @brief Header file for class BasicType
 * @date 28/08/2015
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class BasicType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTYPE_H_
#define BASICTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Definition of BasicType as a 4 bit integer.
 */
// not usable in switch sections
//typedef uint4 BasicType;

// used to encode types in the TypeDescriptor
typedef uint8 BasicType;


/**
 * @brief The type is one of the known complex types OBject,Stream,StructuredData.
 */
const BasicType ComplexType = 0u;

/**
 * @brief The type is a signed integer.
 */
const BasicType SignedInteger = 1u;

/**
 * @brief The type is an unsigned integer.
 */
const BasicType UnsignedInteger = 2u;

/**
 * @brief The type is a float number.
 */
const BasicType Float = 3u;

/**
 * @brief The type is a char .
 */
const BasicType Char = 4u;

/**
 * @brief The type is defined elsewhere
 */
const BasicType Delegated = 6u;

/**
 * @brief The type is unspecified -dereferencing a void *
 */
const BasicType Void = 7u;


/*
 *
 *   sub-types for the complex type
 *
 */

// used to encode types in the TypeDescriptor
typedef uint8 ComplexSubType;

/**
 * @brief The type is a StreamString class.
 */
const ComplexSubType  SString = 1u;

/**
 * @brief The type is a StreamInterface class.
 */
const ComplexSubType  Stream = 2u;

/**
 * @brief  The StructuredDataI type
 */
const ComplexSubType  StructuredDataInterface = 3u;


/**
 * @brief The type is not valid
 */
//const BasicType Invalid = 15u;


/**
 * @brief Definition of BasicArrayType
 */
//typedef uint8 SizedArrayType;

/**
 * @brief The type is a []
 */
//const SizedArrayType CSArray = 0u;

/**
 * @brief The type is a []
 */
//const SizedArrayType StaticZeroTermSArray  = 1u;

/**
 * @brief Definition of BasicArrayPropertyType
 */
typedef uint8 BasicArrayPropertyType;

/**
 * @brief The type is a Vector<T>(0)
 */
const BasicArrayPropertyType      UnSizedA_AP    = 0u;
const BasicArrayPropertyType ConstUnSizedA_AP    = 4u;
const BasicArrayPropertyType      SizedCArray_AP = 2u;
const BasicArrayPropertyType      StaticZeroTermArray_AP   = 3u;
const BasicArrayPropertyType ConstStaticZeroTermArray_AP   = 7u;  // ZTA const


/**
 * @brief Definition of BasicArrayType
 */
typedef uint8 BasicArrayType;

/**
 * @brief The type is invalid or unKnown
 */
const BasicArrayType ArrayUnknown_BT = 0u;

/**
 * @brief The type is a Vector<T>(0)
 */
const BasicArrayType Array1D_BT = 1u;

/**
 * @brief The type is a Matrix<T>
 */
const BasicArrayType Array2D_BT = 2u;

/**
 * @brief The type is a pointer to the specified type
 */
const BasicArrayType PointerArray_BT = 3u;

/**
 * @brief The type is a ZeroTerminatedArray of unknown size
 */
const BasicArrayType ZeroTermArray_BT = 4u;

/**
 * @brief The type is a zero term vector of reallocable size
 */
const BasicArrayType DynamicZeroTermArray_BT = 5u;

/**
 * Used to specify combinations of ArrayType and ArrayProperty
 */
typedef uint8 CombinedArrayType;

/**
 * @brief
 */
const CombinedArrayType SizedCArray = SizedCArray_AP ;

/**
 * @brief The type is a zero term vector of fixed size non-reallocable
 */
const CombinedArrayType StaticZeroTermArray = StaticZeroTermArray_AP ;

/**
 * @brief The type is a zero term vector of fixed size non-reallocable
 */
const CombinedArrayType ConstStaticZeroTermArray = ConstStaticZeroTermArray_AP ;

/**
 * @brief this mask allows testing and setting for const types
 */
const CombinedArrayType ConstArrayMask = ConstUnSizedA_AP;

/**
 * CombinedArrayType of ArrayUnknown
 */
const CombinedArrayType ArrayUnknown = (ArrayUnknown_BT << 3);

/**
 * @brief The type is a Vector<T>(0)
 */
const CombinedArrayType Array1D = (Array1D_BT << 3);

/**
 * @brief The type is a Vector<T>(0)
 */
const CombinedArrayType ConstArray1D = Array1D + ConstUnSizedA_AP;

/**
 * @brief The type is a Matrix<T>
 */
const CombinedArrayType Array2D = (Array2D_BT << 3);

/**
 * @brief The type is a Matrix<T>
 */
const CombinedArrayType ConstArray2D = Array2D + ConstUnSizedA_AP ;

/**
 * @brief The type is a pointer to the specified type
 */
const CombinedArrayType PointerArray = (PointerArray_BT << 3);

/**
 * @brief The type is a pointer to the specified type
 */
const CombinedArrayType ConstPointerArray = PointerArray + ConstUnSizedA_AP ;

/**
 * @brief The type is a ZeroTerminatedArray of unknown size
 */
const CombinedArrayType ZeroTermArray = (ZeroTermArray_BT << 3);

/**
 * @brief The type is a ZeroTerminatedArray of unknown size
 */
const CombinedArrayType ConstZeroTermArray = ZeroTermArray + ConstUnSizedA_AP ;

/**
 * @brief The type is a zero term vector of reallocable size
 */
const CombinedArrayType DynamicZeroTermArray = (DynamicZeroTermArray_BT << 3);

/**
 * @brief The type is a zero term vector of reallocable size
 */
const CombinedArrayType ConstDynamicZeroTermArray = DynamicZeroTermArray + ConstUnSizedA_AP;


/**
 * @brief Definition of BasicArrayType
 */
typedef uint8 BasicObjectSize;

/**
 * @brief The type has an unknown size
 */
const BasicObjectSize SizeUnknown = 0u;

/**
 * @brief The type has size 8bit
 */
const BasicObjectSize Size8bit = 1u;

/**
 * @brief The type has size 16bit
 */
const BasicObjectSize Size16bit = 2u;

/**
 * @brief The type has size 32bit
 */
const BasicObjectSize Size32bit = 3u;

/**
 * @brief The type has size 64bit
 */
const BasicObjectSize Size64bit = 4u;

/**
 * @brief The type has size 128bit
 */
const BasicObjectSize Size128bit = 5u;

/**
 * @brief The type has size 256bit
 */
const BasicObjectSize Size256bit = 6u;

/**
 * @brief The type has bit size
 */
const BasicObjectSize SizeBits = 7u;

/**
 * TODO
 * @return SizeUnknown if the number of bits is not exactly 2^N with N in the range [3 9]
 */
DLL_API BasicObjectSize BasicObjectSizeFromBits(uint32 bits);

/**
 * TODO
 * @return 0 if bos = SizeUnknown otherwise the size in bits
 */
DLL_API uint32 BitsFromBasicObjectSize(BasicObjectSize bos);

/**
 * TODO
 * @return a string for the basicType
 */
class CCString;
DLL_API CCString BasicTypeName(BasicType bt,ComplexSubType cs);



}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTYPE_H_ */
