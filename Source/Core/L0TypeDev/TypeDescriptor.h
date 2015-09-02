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
#include "BasicType.h"
#include "BitRange.h"
#include "BitBoolean.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

/**
 * @brief A structure Used to describe the type pointed to by a pointer.
 * @details Depending on the first bit isStructuredData it may contain a code identifying a structure
 * or the remaining bit can be used to identify a specific basic type.\n
 * Basic types are integers 8-64 bit, floats, doubles, char pointers and void pointers.
 * Cannot use BitField.h as TypeDescriptor is required by it
 */
class TypeDescriptor {
public:

    /**
     * A structure for type informations storing.
     */
    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {
        /**
         if true then the data is a structure or class and its definition
         has to be found in the ObjectRegistryDatabase
         */
        BitBoolean<uint16, 0u> isStructuredData;

        /**
         the data is constant - cannot be written to
         */
        BitBoolean<uint16, 1u> isConstant;

        /**
         the actual type of data
         */
        BitRange<uint16, 4u, 2u> type;

        /**
         the size in bytes or bits
         */
        BitRange<uint16, 10u, 6u> size;

        /**
         * A code related univocally to a record in the ObjectRegistryDatabase
         */
        BitRange<uint16, 14u, 2u> structuredDataIdCode;

        /**
         * Fills all the memory area.
         */
        BitRange<uint16, 16u, 0u> all;
    };

    /**
     * @brief Constructor by 16 bit integer.
     * @param[in] x contains the type informations which must be stored into this memory area.
     */
    TypeDescriptor(const uint16 x = 0u) {
        all = x;
    }

    /**
     * @brief Basic Type constructor.
     * @param[in] isConstantIn in specifies if the type is constant.
     * @param[in] typeIn is the type.
     * @param[in] sizeIn is the size.
     */
    TypeDescriptor(const bool isConstantIn,
                   const uint16 typeIn,
                   const uint16 sizeIn) {
        isStructuredData = false;
        isConstant = isConstantIn;
        type = typeIn;
        size = sizeIn;
    }

    /**
     * @brief Structured objects constructor.
     * @param[in] isConstantIn in specifies if the object is constant.
     * @param[in] typeIn is the type.
     * @param[in] sizeIn is the size.
     */
    TypeDescriptor(const bool isConstantIn,
                   const uint16 structuredDataIdCodeIn) {

        isStructuredData = true;
        isConstant = isConstantIn;
        structuredDataIdCode = structuredDataIdCodeIn;
    }

    /**
     * @brief Equal operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    bool operator==(const TypeDescriptor &typeDescriptor) const {
        return (structuredDataIdCode == typeDescriptor.structuredDataIdCode);
    }

};

/** Float descriptor. */
static const TypeDescriptor Float32Bit(false, Float, 32u);

/** Double descriptor. */
static const TypeDescriptor Float64Bit(false, Float, 64u);

/** 128 bit Float descriptor */
static const TypeDescriptor Float128Bit(false, Float, 128u);

/** Void descriptor */
static const TypeDescriptor VoidType(false, SignedInteger, 0u);

/** Int8 descriptor */
static const TypeDescriptor SignedInteger8Bit(false, SignedInteger, 8u);

/** Uint8 descriptor */
static const TypeDescriptor UnsignedInteger8Bit(false, UnsignedInteger, 8u);

/** Int16 descriptor */
static const TypeDescriptor SignedInteger16Bit(false, SignedInteger, 16u);

/** Uint16 descriptor */
static const TypeDescriptor UnsignedInteger16Bit(false, UnsignedInteger, 16u);

/** Int32 descriptor */
static const TypeDescriptor SignedInteger32Bit(false, SignedInteger, 32u);

/** Uint32 descriptor */
static const TypeDescriptor UnsignedInteger32Bit(false, UnsignedInteger, 32u);

/** Int64 descriptor */
static const TypeDescriptor SignedInteger64Bit(false, SignedInteger, 64u);

/** Uint64 descriptor */
static const TypeDescriptor UnsignedInteger64Bit(false, UnsignedInteger, 64u);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPEDESCRIPTOR_H_ */

