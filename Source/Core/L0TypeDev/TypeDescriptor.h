/**
 * @file TypeDescriptor.h
 * @brief Header file for class TypeDescriptor
 * @date 26/08/2015
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

 * @details This header file contains the declaration of the class TypeDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPEDESCRIPTOR_H_
#define TYPEDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

/**
 *  The basic types that can be used.
 */
typedef uint32 BasicType;

/**
 * The type is signed.
 */
const BasicType SignedInteger = 0u;

/**
 * The type is unsigned.
 */
const BasicType UnsignedInteger = 1u;

/**
 * The type is a float number.
 */
const BasicType Float = 2u;

/**
 * The type is a pointer.
 */
const BasicType Pointer = 3u;

/**
 * The type is a C string
 * (const char *)
 */
const BasicType CCString = 8u;

/**
 * The type is a pointer to a C string allocated on heap.
 * (char **)
 */
const BasicType PCString = 9u;

/**
 * The type is an array of bytes with the size
 * specified in the size field.
 */
const BasicType CArray = 10u;

/**
 * The type is a StreamString class.
 */
const BasicType StreamString = 11u;

/**
 * The type is a StreamInterface class.
 */
const BasicType StreamInterface = 12u;

/**
 * @brief A structure Used to describe the type pointed to by a pointer.
 * @details Depending on the first bit isStructuredData it may contain a code identifying a structure
 * or the remaining bit can be used to identify a specific basic type.\n
 * Basic types are ints 8-64 bit, floats, doubles, char pointers and void pointers.
 */
struct TypeDescriptor {

    /**
     if true then the data is a structure or class and its definition
     has to be found in the ObjectRegistryDatabase
     */
    bool isStructuredData :1;

    /**
     the data is constant - cannot be written to
     */
    bool isConstant :1;

    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {  // 14 bit unnamed union

        /**
         * A struct used to to describe basic types
         */
        struct {

            /**
             the actual type of data
             */
            BasicType type :4;

            /**
             the size in bytes or bits
             */
            uint32 size :10;

        } typeInfo;

        /**
         * A code related univocally to a record in the ObjectRegistryDatabase
         */
        uint32 structuredDataIdCode :14;

    };

    /**
     * @brief Equal operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    bool operator==(const TypeDescriptor &typeDescriptor) const {

        return (isStructuredData == 1u) ?
                (structuredDataIdCode == typeDescriptor.structuredDataIdCode) :
                ((typeInfo.type == typeDescriptor.typeInfo.type) && (typeInfo.size == typeDescriptor.typeInfo.size));

    }
};

/** Float descriptor. */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor Float32Bit = { false, false, { { Float, 32u } } };

/** Double descriptor. */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor Float64Bit = { false, false, { { Float, 64u } } };

/** 128 bit Float descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor Float128Bit = { false, false, { { Float, 128u } } };

/** Void descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor VoidType = { false, false, { { SignedInteger, 0u } } };

/** Int8 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor SignedInteger8Bit = { false, false, { { SignedInteger, 8u } } };

/** Uint8 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor UnsignedInteger8Bit = { false, false, { { UnsignedInteger, 8u } } };

/** Int16 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor SignedInteger16Bit = { false, false, { { SignedInteger, 16u } } };

/** Uint16 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor UnsignedInteger16Bit = { false, false, { { UnsignedInteger, 16u } } };

/** Int32 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor SignedInteger32Bit = { false, false, { { SignedInteger, 32u } } };

/** Uint32 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor UnsignedInteger32Bit = { false, false, { { UnsignedInteger, 32u } } };

/** Int64 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor SignedInteger64Bit = { false, false, { { SignedInteger, 64u } } };

/** Uint64 descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor UnsignedInteger64Bit = { false, false, { { UnsignedInteger, 64u } } };

/** Pointer descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor VoidPointer = { false, false, { { Pointer, sizeof(void *) * 8u } } };

/** CCString descriptor */
/*lint -e{9119} Implicit conversion of integer to a smaller type justified for number which require less than 14 bits.*/
/*lint -e{708} Union initialization justified since the standard initializes the first member.*/
const TypeDescriptor ConstCString = { true, false, { { CCString, 0u } } };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPEDESCRIPTOR_H_ */

