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
typedef uint8 BasicType;

/**
 * @brief The type is a signed integer.
 */
const BasicType SignedInteger = 0u;

/**
 * @brief The type is an unsigned integer.
 */
const BasicType UnsignedInteger = 1u;

/**
 * @brief The type is a float number.
 */
const BasicType Float = 2u;

/**
 * @brief The type is a char .
 */
const BasicType Char = 3u;

/**
 * @brief The type is a pointer. A void*. No information on the type pointed to
 */
//const BasicType Pointer = 4u;  // replaced by the BasicArrayType::BasicArrayType

/**
 * @brief The type is a signed bit range .
 */
const BasicType SignedBitInteger = 5u;

/**
 * @brief The type is an unsigned bit range.
 */
const BasicType UnsignedBitInteger = 6u;

/**
 * @brief The type is a StreamString class.
 */
const BasicType SString = 7u;

/**
 * @brief The type is a StreamInterface class.
 */
const BasicType Stream = 8u;

/**
 * @brief  The StructuredDataI type
 */
const BasicType StructuredDataInterface = 13u;

/**
 * @brief The type is unspecified -dereferencing a void *
 */
const BasicType Void = 14u;

/**
 * @brief The type is not valid
 */
const BasicType Invalid = 15u;




/**
 * @brief Definition of BasicArrayType
 */
typedef uint8 BasicArrayType;


/**
 * @brief The type is invalid or unKnown
 */
const BasicArrayType ArrayUnknown = 0u;

/**
 * @brief The type is a scalar (1) - vector : Vector<T>(0) or T[N] (N)
 */
const BasicArrayType Array1D = 1u;

/**
 * @brief The type is a matrix - type Matrix<T>
 */
const BasicArrayType Array2D = 2u;

/**
 * @brief The type is a matrix - more than 1 dimensions - the missing one are listed elsewhere
 */
const BasicArrayType ArrayLarge = 3u;

/**
 * @brief The type is a ZeroTerminatedArray of unknown size
 */
const BasicArrayType ZeroTermArray = 4u;

/**
 * @brief The type is a zero term vector of reallocable size
 */
const BasicArrayType DynamicZeroTermArray = 5u;

/**
 * @brief The type is a zero term vector of fixed size non-reallocable
 */
const BasicArrayType StaticZeroTermArray = 6u;

/**
 * @brief The type is a pointer to the specified type
 */
const BasicArrayType PointerArray = 7u;

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
 * @brief The type has size 128bit
 */
const BasicObjectSize Size512bit = 7u;

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
DLL_API CCString BasicTypeName(uint32 bt);


}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTYPE_H_ */
