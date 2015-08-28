/**
 * @file BitField.h
 * @brief Header file for class BitField
 * @date 26/08/2015
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

 * @details This header file contains the declaration of the class BitField
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BIT_FIELD_H_
#define BIT_FIELD_H_

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
 * @brief Provides information about the template type in input.
 */
template<typename T>
class TypeCharacteristics {
public:

    /**
     *  @brief Returns true if the type is signed, false otherwise.
     *  @return true if the type is signed, false otherwise.
     */
    static inline bool IsSigned() {
        return ((static_cast<T>(-1)) < 0);
    }

    /**
     * @brief Returns the maximum possible value of the template type.
     * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
     */
    static inline const T MaxValue() {
        if (IsSigned()) {
            // 0x7fff..f if signed
            T temp = 1 << (sizeof(T) * 8 - 1);
            return ~temp;
        }
        else {
            // 0xffff...f if unsigned
            T temp = 0;
            return ~temp;
        }
    }

    /**
     * @brief Returns the minimum possible value of the template type.
     * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
     */
    static inline const T MinValue() {
        if (IsSigned()) {
            // 0x80...0 if signed
            T temp = 1 << (sizeof(T) * 8 - 1);
            return temp;
        }
        else {
            // 0 if unsigned
            T temp = 0;
            return temp;
        }
    }
};

/**
 * @brief An integer with the desired size in bits.
 */
template<typename baseType, int bitSize>
class FractionalInteger {

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

public:

    /**
     * @brief Returns the size of the number as number of bits.
     * @return the number bit size.
     */
    static inline int BitSize() {
        return bitSize;
    }

    /**
     * @brief Returns the maximum possible value for this integer.
     * @return the maximum possible value for this integer.
     */
    static inline int MaxValue() {
        return maxValue;
    }

    /**
     * @brief Returns the minimum possible value for this integer.
     * @return the minimum possible value for this integer.
     */
    static inline int MinValue() {
        return minValue;
    }

    /**
     * @brief Default constructor.
     * @details Sets the integer value to zero.
     */
    FractionalInteger() {
        value = 0;
    }

    /**
     * @brief Constructor by value.
     * @details The argument \a input could be another integer type.
     * If the input value is greater than the maximum allowed value, the value is set to the maximum value.
     * If the input value is lower than the minimum allowed value, the value is set to the minimum value.
     * @param[in] is the desired integer value.
     */
    template<typename inputType> FractionalInteger(inputType input) {
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

    /**
     * @brief Cast to integer.
     * @details Thanks to this operator this object can be treated as an integer.
     * @return the value of this integer.
     */
    inline operator baseType() const {
        return value;
    }

    /**
     * @brief Cast to AnyType.
     * @details Thanks to this operator this object can be treated as an AnyType object.
     * @return the AnyType associated to this object.
     */
    inline operator AnyType() const {
        BasicType bt = UnsignedInteger;
        if (TypeCharacteristics<baseType>::IsSigned()) {
            bt = SignedInteger;
        }
        const TypeDescriptor td = { false, false, { { bt, bitSize } } };
        return AnyType(td, 0, this);
    }

};

/**
 *  @brief A number with a settable size and a settable bit shift.
 *  @details Using these types into an union allows to have the same effect of a struct with bit fielded attributes.
 */
template<typename baseType, int bitSize, int bitOffset>
class BitRange {

    /**
     * The number value.
     */
    baseType value;

    /**
     * The number size.
     */
    static const baseType baseTypeBitSize = (sizeof(baseType) * 8);

    /**
     * A flag true if the number is signed, false otherwise.
     */
    static const bool isSigned = ((static_cast<baseType>(-1)) < 0);

    /**
     * The minimum possible value.
     */
    static const baseType minValue = (isSigned ? (-1 << (bitSize - 1)) : 0);

    /**
     * The maximum possible value.
     */
    static const baseType maxValue = (isSigned ? ((0x1 << (bitSize - 1)) - 1) : (static_cast<baseType>(-1) >> ((sizeof(baseType) * 8) - bitSize)));

    /**
     * The mask covering with ones the specified bit range.
     */
    static const baseType mask = static_cast<baseType>((~0u >> (baseTypeBitSize - bitSize)) << bitOffset);

    /**
     * The mask covering with ones the space out of the bit range.
     */
    static const baseType notMask = ~mask;

public:

    /**
     * @brief Copy operator.
     * @details Since this type could be used in an union, this function change only
     * the range of bits specified leaving untouched the bits outside the range.\n
     * The \a input argument could be another type. If its value is greater than the maximum possible value
     * or lower than the minimum possible value, the value assigned will be saturated.
     * @param[in] input is the desired number value.
     */
    template<typename inputType> void operator=(inputType input) {
        baseType temporaryValue = 0;

        // saturates the input value if its out of allowed range
        if (input >= static_cast<inputType>(maxValue)) {
            temporaryValue = maxValue;
            // ERROR LOGGING
        }
        else {
            if (input <= static_cast<inputType>(minValue)) {
                temporaryValue = minValue;
            }
            else {
                temporaryValue = input;
            }
        }
        // shifts the number
        temporaryValue <<= bitOffset;

        // masks the number
        temporaryValue &= mask;

        // clears the bit range in the value
        value &= notMask;

        // puts the value in the bit range
        value |= temporaryValue;
    }


    /**
     * @brief baseType cast.
     * @details This operator allows to treat this object as a baseType object.\n
     * The number will be
     * @
     */
    inline operator baseType() const {

        baseType temporaryValue = value;

        temporaryValue &= mask;

        // to align sign bits
        temporaryValue <<= (baseTypeBitSize - bitOffset - bitSize);

        // this should sign extend
        temporaryValue >>= (baseTypeBitSize - bitSize);

        return temporaryValue;
    }

    inline operator AnyType() const {
        BasicType bt = UnsignedInteger;
        if (TypeCharacteristics<baseType>::IsSigned()) {
            bt = SignedInteger;
        }
        const TypeDescriptor td = { false, false, { { bt, bitSize } } };
        return AnyType(td, bitOffset, this);
    }

    static inline baseType BitSize() {
        return bitSize;
    }

    static inline baseType BitOffset() {
        return bitOffset;
    }

};

}

typedef TypeDefinition::FractionalInteger<uint8, 1> uint1;
typedef TypeDefinition::FractionalInteger<uint8, 2> uint2;
typedef TypeDefinition::FractionalInteger<uint8, 3> uint3;
typedef TypeDefinition::FractionalInteger<uint8, 4> uint4;
typedef TypeDefinition::FractionalInteger<uint8, 5> uint5;
typedef TypeDefinition::FractionalInteger<uint8, 6> uint6;
typedef TypeDefinition::FractionalInteger<uint8, 7> uint7;
typedef TypeDefinition::FractionalInteger<uint16, 9> uint9;
typedef TypeDefinition::FractionalInteger<uint16, 10> uint10;
typedef TypeDefinition::FractionalInteger<uint16, 11> uint11;
typedef TypeDefinition::FractionalInteger<uint16, 12> uint12;
typedef TypeDefinition::FractionalInteger<uint16, 13> uint13;
typedef TypeDefinition::FractionalInteger<uint16, 14> uint14;
typedef TypeDefinition::FractionalInteger<uint16, 15> uint15;
typedef TypeDefinition::FractionalInteger<uint32, 17> uint17;
typedef TypeDefinition::FractionalInteger<uint32, 18> uint18;
typedef TypeDefinition::FractionalInteger<uint32, 19> uint19;
typedef TypeDefinition::FractionalInteger<uint32, 20> uint20;
typedef TypeDefinition::FractionalInteger<uint32, 21> uint21;
typedef TypeDefinition::FractionalInteger<uint32, 22> uint22;
typedef TypeDefinition::FractionalInteger<uint32, 23> uint23;
typedef TypeDefinition::FractionalInteger<uint32, 24> uint24;
typedef TypeDefinition::FractionalInteger<uint32, 25> uint25;
typedef TypeDefinition::FractionalInteger<uint32, 26> uint26;
typedef TypeDefinition::FractionalInteger<uint32, 27> uint27;
typedef TypeDefinition::FractionalInteger<uint32, 28> uint28;
typedef TypeDefinition::FractionalInteger<uint32, 29> uint29;
typedef TypeDefinition::FractionalInteger<uint32, 30> uint30;
typedef TypeDefinition::FractionalInteger<uint32, 31> uint31;

typedef TypeDefinition::FractionalInteger<int8, 2> int2;
typedef TypeDefinition::FractionalInteger<int8, 3> int3;
typedef TypeDefinition::FractionalInteger<int8, 4> int4;
typedef TypeDefinition::FractionalInteger<int8, 5> int5;
typedef TypeDefinition::FractionalInteger<int8, 6> int6;
typedef TypeDefinition::FractionalInteger<int8, 7> int7;
typedef TypeDefinition::FractionalInteger<int16, 9> int9;
typedef TypeDefinition::FractionalInteger<int16, 10> int10;
typedef TypeDefinition::FractionalInteger<int16, 11> int11;
typedef TypeDefinition::FractionalInteger<int16, 12> int12;
typedef TypeDefinition::FractionalInteger<int16, 13> int13;
typedef TypeDefinition::FractionalInteger<int16, 14> int14;
typedef TypeDefinition::FractionalInteger<int16, 15> int15;
typedef TypeDefinition::FractionalInteger<int32, 17> int17;
typedef TypeDefinition::FractionalInteger<int32, 18> int18;
typedef TypeDefinition::FractionalInteger<int32, 19> int19;
typedef TypeDefinition::FractionalInteger<int32, 20> int20;
typedef TypeDefinition::FractionalInteger<int32, 21> int21;
typedef TypeDefinition::FractionalInteger<int32, 22> int22;
typedef TypeDefinition::FractionalInteger<int32, 23> int23;
typedef TypeDefinition::FractionalInteger<int32, 24> int24;
typedef TypeDefinition::FractionalInteger<int32, 25> int25;
typedef TypeDefinition::FractionalInteger<int32, 26> int26;
typedef TypeDefinition::FractionalInteger<int32, 27> int27;
typedef TypeDefinition::FractionalInteger<int32, 28> int28;
typedef TypeDefinition::FractionalInteger<int32, 29> int29;
typedef TypeDefinition::FractionalInteger<int32, 30> int30;
typedef TypeDefinition::FractionalInteger<int32, 31> int31;

typedef union {
    TypeDefinition::BitRange<uint32, 4, 0> a;
    TypeDefinition::BitRange<uint32, 4, 4> b;
    TypeDefinition::BitRange<uint32, 10, 8> c;
    TypeDefinition::BitRange<uint32, 14, 18> d;

} myBitStruct;

const myBitStruct *p;

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BITFIELD_H_ */

