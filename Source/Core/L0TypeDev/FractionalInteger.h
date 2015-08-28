/**
 * @file FractionalInteger.h
 * @brief Header file for class FractionalInteger
 * @date 28/08/2015
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

 * @details This header file contains the declaration of the class FractionalInteger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FRACTIONALINTEGER_H_
#define FRACTIONALINTEGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "TypeDescriptor.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

/**
 * @brief An integer with the desired size in bits.
 */
template<typename baseType, uint8 bitSize>
class FractionalInteger {

public:

    /**
     * @brief Returns the size of the number as number of bits.
     * @return the number bit size.
     */
    static inline int BitSize();

    /**
     * @brief Returns the maximum possible value for this integer.
     * @return the maximum possible value for this integer.
     */
    static inline int MaxValue();

    /**
     * @brief Returns the minimum possible value for this integer.
     * @return the minimum possible value for this integer.
     */
    static inline int MinValue();

    /**
     * @brief Default constructor.
     * @details Sets the integer value to zero.
     */
    FractionalInteger();

    /**
     * @brief Constructor by value.
     * @details The argument \a input could be another integer type.
     * If the input value is greater than the maximum allowed value, the value is set to the maximum value.
     * If the input value is lower than the minimum allowed value, the value is set to the minimum value.
     * @param[in] is the desired integer value.
     */
    template<typename inputType> FractionalInteger(inputType input);

    /**
     * @brief Cast to integer.
     * @details Thanks to this operator this object can be treated as an integer.
     * @return the value of this integer.
     */
    inline operator baseType() const;

    /**
     * @brief Cast to AnyType.
     * @details Thanks to this operator this object can be treated as an AnyType object.
     * @return the AnyType associated to this object.
     */
    inline operator AnyType() const;

private:

    /**
     * The value of the integer.
     */
    baseType value;

    /**
     * A flag which is true if the integer is signed, false otherwise.
     */
    static const bool isSigned = ((static_cast<baseType>(-1)) < 0);

    /**
     * The minimum value of the integer.
     */
    static const baseType minValue = (isSigned ? (-1 << (bitSize - 1)) : 0);

    /**
     * The maximum value of the integer.
     */
    static const baseType maxValue = (isSigned ? ((0x1 << (bitSize - 1)) - 1) : (static_cast<baseType>(-1) >> ((sizeof(baseType) * 8) - bitSize)));

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType, uint8 bitSize>
int FractionalInteger<baseType, bitSize>::BitSize() {
    return bitSize;
}

template<typename baseType, uint8 bitSize>
int FractionalInteger<baseType, bitSize>::MaxValue() {
    return maxValue;
}

template<typename baseType, uint8 bitSize>
int FractionalInteger<baseType, bitSize>::MinValue() {
    return minValue;
}

template<typename baseType, uint8 bitSize>
FractionalInteger<baseType, bitSize>::FractionalInteger() {
    value = 0;
}

template<typename baseType, uint8 bitSize>
template<typename inputType>
FractionalInteger<baseType, bitSize>::FractionalInteger(inputType input) {
    baseType temporaryValue = 0;
    // saturation in case of input value out of range
    if (input >= static_cast<inputType>(maxValue)) {
        value = maxValue;
        // ERROR LOGGING
    }
    else {
        if (input <= static_cast<inputType>(minValue)) {
            value = minValue;
            // ERROR LOGGING
        }
        else {
            value = input;
        }
    }
}

template<typename baseType, uint8 bitSize>
FractionalInteger<baseType, bitSize>::operator baseType() const {
    return value;
}

template<typename baseType, uint8 bitSize>
FractionalInteger<baseType, bitSize>::operator AnyType() const {
    BasicType bt = UnsignedInteger;
    if (TypeCharacteristics<baseType>::IsSigned()) {
        bt = SignedInteger;
    }
    const TypeDescriptor td = { false, false, { { bt, bitSize } } };
    return AnyType(td, 0, this);
}

}

typedef TypeDefinition::FractionalInteger<uint8, 1u> uint1;
typedef TypeDefinition::FractionalInteger<uint8, 2u> uint2;
typedef TypeDefinition::FractionalInteger<uint8, 3u> uint3;
typedef TypeDefinition::FractionalInteger<uint8, 4u> uint4;
typedef TypeDefinition::FractionalInteger<uint8, 5u> uint5;
typedef TypeDefinition::FractionalInteger<uint8, 6u> uint6;
typedef TypeDefinition::FractionalInteger<uint8, 7u> uint7;
typedef TypeDefinition::FractionalInteger<uint16, 9u> uint9;
typedef TypeDefinition::FractionalInteger<uint16, 10u> uint10;
typedef TypeDefinition::FractionalInteger<uint16, 11u> uint11;
typedef TypeDefinition::FractionalInteger<uint16, 12u> uint12;
typedef TypeDefinition::FractionalInteger<uint16, 13u> uint13;
typedef TypeDefinition::FractionalInteger<uint16, 14u> uint14;
typedef TypeDefinition::FractionalInteger<uint16, 15u> uint15;
typedef TypeDefinition::FractionalInteger<uint32, 17u> uint17;
typedef TypeDefinition::FractionalInteger<uint32, 18u> uint18;
typedef TypeDefinition::FractionalInteger<uint32, 19u> uint19;
typedef TypeDefinition::FractionalInteger<uint32, 20u> uint20;
typedef TypeDefinition::FractionalInteger<uint32, 21u> uint21;
typedef TypeDefinition::FractionalInteger<uint32, 22u> uint22;
typedef TypeDefinition::FractionalInteger<uint32, 23u> uint23;
typedef TypeDefinition::FractionalInteger<uint32, 24u> uint24;
typedef TypeDefinition::FractionalInteger<uint32, 25u> uint25;
typedef TypeDefinition::FractionalInteger<uint32, 26u> uint26;
typedef TypeDefinition::FractionalInteger<uint32, 27u> uint27;
typedef TypeDefinition::FractionalInteger<uint32, 28u> uint28;
typedef TypeDefinition::FractionalInteger<uint32, 29u> uint29;
typedef TypeDefinition::FractionalInteger<uint32, 30u> uint30;
typedef TypeDefinition::FractionalInteger<uint32, 31u> uint31;

typedef TypeDefinition::FractionalInteger<int8, 2u> int2;
typedef TypeDefinition::FractionalInteger<int8, 3u> int3;
typedef TypeDefinition::FractionalInteger<int8, 4u> int4;
typedef TypeDefinition::FractionalInteger<int8, 5u> int5;
typedef TypeDefinition::FractionalInteger<int8, 6u> int6;
typedef TypeDefinition::FractionalInteger<int8, 7u> int7;
typedef TypeDefinition::FractionalInteger<int16, 9u> int9;
typedef TypeDefinition::FractionalInteger<int16, 10u> int10;
typedef TypeDefinition::FractionalInteger<int16, 11u> int11;
typedef TypeDefinition::FractionalInteger<int16, 12u> int12;
typedef TypeDefinition::FractionalInteger<int16, 13u> int13;
typedef TypeDefinition::FractionalInteger<int16, 14u> int14;
typedef TypeDefinition::FractionalInteger<int16, 15u> int15;
typedef TypeDefinition::FractionalInteger<int32, 17u> int17;
typedef TypeDefinition::FractionalInteger<int32, 18u> int18;
typedef TypeDefinition::FractionalInteger<int32, 19u> int19;
typedef TypeDefinition::FractionalInteger<int32, 20u> int20;
typedef TypeDefinition::FractionalInteger<int32, 21u> int21;
typedef TypeDefinition::FractionalInteger<int32, 22u> int22;
typedef TypeDefinition::FractionalInteger<int32, 23u> int23;
typedef TypeDefinition::FractionalInteger<int32, 24u> int24;
typedef TypeDefinition::FractionalInteger<int32, 25u> int25;
typedef TypeDefinition::FractionalInteger<int32, 26u> int26;
typedef TypeDefinition::FractionalInteger<int32, 27u> int27;
typedef TypeDefinition::FractionalInteger<int32, 28u> int28;
typedef TypeDefinition::FractionalInteger<int32, 29u> int29;
typedef TypeDefinition::FractionalInteger<int32, 30u> int30;
typedef TypeDefinition::FractionalInteger<int32, 31u> int31;

#endif /* FRACTIONALINTEGER_H_ */

