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
#include "TypeCharacteristics.h"
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
    static const baseType minValue = (isSigned ? (static_cast<baseType>(-1) << (bitSize - 1)) : static_cast<baseType>(0));

    /**
     * The maximum value of the integer.
     */
    static const baseType maxValue =
            (isSigned ?
                    ((static_cast<baseType>(1) << (bitSize - 1)) - static_cast<baseType>(1)) : (static_cast<baseType>(-1) >> ((sizeof(baseType) * 8) - bitSize)));

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

    // saturation to max
    if (input > static_cast<inputType>(0)) {
        // cast to the type which has the max usable size
        if (TypeCharacteristics<inputType>::UsableBitSize() > TypeCharacteristics<baseType>::UsableBitSize()) {
            if (input > static_cast<inputType>(maxValue)) {
                input = maxValue;
            }
        }
        else {
            // input<0 and basetype unsigned bug
            if (static_cast<baseType>(input) > maxValue) {
                input = maxValue;
            }
        }
    }

    //saturation to min
    else {
        // check min
        if (!isSigned) {
            input = 0;
        }
        else {

            // only consider signed against signed for minimum
            // unsigned have 0 as minimum which is greater than the minimum of all fractional signed
            if (TypeCharacteristics<inputType>::IsSigned()) {
                if (TypeCharacteristics<inputType>::UsableBitSize() > TypeCharacteristics<baseType>::UsableBitSize()) {
                    if (input < static_cast<inputType>(minValue)) {
                        input = minValue;
                    }
                }
                else {

                    // input>0 and basetype signed bug
                    if (static_cast<baseType>(input) < minValue) {
                        input = minValue;
                    }
                }
            }
        }
    }

    value = static_cast<baseType>(input);

    /*  baseType temporaryValue = 0;
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
     */
    /*
     const bool isInputSigned = (static_cast<inputType>(-1)<0);

     const uint8 inputBitSize = static_cast<uint8>(sizeof(inputType) * 8);
     const uint8 baseTypeBitSize = static_cast<uint8>(sizeof(baseType) * 8);

     if ((!isInputSigned) && (isSigned)) {
     // in this case checks only who is the greater (input is always >0)
     if (input > static_cast<inputType>(maxValue)) {
     value = maxValue;
     }
     else {
     value = static_cast<baseType>(input);
     }
     }
     else if ((isInputSigned) && (!isSigned)) {
     // in this case if input is negative saturates this to zero
     if (input < static_cast<inputType>(0)) {
     //0
     value = minValue;
     }
     else {
     //if the input is >0 saturates in case.
     if (static_cast<baseType>(input) > maxValue) {
     value = maxValue;
     }
     else {
     value = static_cast<baseType>(input);
     }
     }
     }
     else {

     // choose the correct cast otherwise a positive number
     // could become negative because clipped by the cast
     if (inputBitSize <= baseTypeBitSize) {
     // the types are both signed or unsigned
     if (static_cast<baseType>(input) > maxValue) {
     value = maxValue;
     }

     else if (static_cast<baseType>(input) < minValue) {
     value = minValue;
     }
     else {
     value = static_cast<baseType>(input);
     }
     }
     else {
     // the types are both signed or unsigned
     if (input > static_cast<inputType>(maxValue)) {
     value = maxValue;
     }

     else if (input < static_cast<inputType>(minValue)) {
     value = minValue;
     }
     else {
     value = static_cast<baseType>(input);
     }

     }
     }*/

}

template<typename baseType, uint8 bitSize>
FractionalInteger<baseType, bitSize>::operator baseType() const {
    return value;
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

typedef TypeDefinition::FractionalInteger<uint64, 33u> uint33;
typedef TypeDefinition::FractionalInteger<uint64, 34u> uint34;
typedef TypeDefinition::FractionalInteger<uint64, 35u> uint35;
typedef TypeDefinition::FractionalInteger<uint64, 36u> uint36;
typedef TypeDefinition::FractionalInteger<uint64, 37u> uint37;
typedef TypeDefinition::FractionalInteger<uint64, 38u> uint38;
typedef TypeDefinition::FractionalInteger<uint64, 39u> uint39;
typedef TypeDefinition::FractionalInteger<uint64, 40u> uint40;
typedef TypeDefinition::FractionalInteger<uint64, 41u> uint41;
typedef TypeDefinition::FractionalInteger<uint64, 42u> uint42;
typedef TypeDefinition::FractionalInteger<uint64, 43u> uint43;
typedef TypeDefinition::FractionalInteger<uint64, 44u> uint44;
typedef TypeDefinition::FractionalInteger<uint64, 45u> uint45;
typedef TypeDefinition::FractionalInteger<uint64, 46u> uint46;
typedef TypeDefinition::FractionalInteger<uint64, 47u> uint47;
typedef TypeDefinition::FractionalInteger<uint64, 48u> uint48;
typedef TypeDefinition::FractionalInteger<uint64, 49u> uint49;
typedef TypeDefinition::FractionalInteger<uint64, 50u> uint50;
typedef TypeDefinition::FractionalInteger<uint64, 51u> uint51;
typedef TypeDefinition::FractionalInteger<uint64, 52u> uint52;
typedef TypeDefinition::FractionalInteger<uint64, 53u> uint53;
typedef TypeDefinition::FractionalInteger<uint64, 54u> uint54;
typedef TypeDefinition::FractionalInteger<uint64, 55u> uint55;
typedef TypeDefinition::FractionalInteger<uint64, 56u> uint56;
typedef TypeDefinition::FractionalInteger<uint64, 57u> uint57;
typedef TypeDefinition::FractionalInteger<uint64, 58u> uint58;
typedef TypeDefinition::FractionalInteger<uint64, 59u> uint59;
typedef TypeDefinition::FractionalInteger<uint64, 60u> uint60;
typedef TypeDefinition::FractionalInteger<uint64, 61u> uint61;
typedef TypeDefinition::FractionalInteger<uint64, 62u> uint62;
typedef TypeDefinition::FractionalInteger<uint64, 63u> uint63;

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

typedef TypeDefinition::FractionalInteger<int64, 33u> int33;
typedef TypeDefinition::FractionalInteger<int64, 34u> int34;
typedef TypeDefinition::FractionalInteger<int64, 35u> int35;
typedef TypeDefinition::FractionalInteger<int64, 36u> int36;
typedef TypeDefinition::FractionalInteger<int64, 37u> int37;
typedef TypeDefinition::FractionalInteger<int64, 38u> int38;
typedef TypeDefinition::FractionalInteger<int64, 39u> int39;
typedef TypeDefinition::FractionalInteger<int64, 40u> int40;
typedef TypeDefinition::FractionalInteger<int64, 41u> int41;
typedef TypeDefinition::FractionalInteger<int64, 42u> int42;
typedef TypeDefinition::FractionalInteger<int64, 43u> int43;
typedef TypeDefinition::FractionalInteger<int64, 44u> int44;
typedef TypeDefinition::FractionalInteger<int64, 45u> int45;
typedef TypeDefinition::FractionalInteger<int64, 46u> int46;
typedef TypeDefinition::FractionalInteger<int64, 47u> int47;
typedef TypeDefinition::FractionalInteger<int64, 48u> int48;
typedef TypeDefinition::FractionalInteger<int64, 49u> int49;
typedef TypeDefinition::FractionalInteger<int64, 50u> int50;
typedef TypeDefinition::FractionalInteger<int64, 51u> int51;
typedef TypeDefinition::FractionalInteger<int64, 52u> int52;
typedef TypeDefinition::FractionalInteger<int64, 53u> int53;
typedef TypeDefinition::FractionalInteger<int64, 54u> int54;
typedef TypeDefinition::FractionalInteger<int64, 55u> int55;
typedef TypeDefinition::FractionalInteger<int64, 56u> int56;
typedef TypeDefinition::FractionalInteger<int64, 57u> int57;
typedef TypeDefinition::FractionalInteger<int64, 58u> int58;
typedef TypeDefinition::FractionalInteger<int64, 59u> int59;
typedef TypeDefinition::FractionalInteger<int64, 60u> int60;
typedef TypeDefinition::FractionalInteger<int64, 61u> int61;
typedef TypeDefinition::FractionalInteger<int64, 62u> int62;
typedef TypeDefinition::FractionalInteger<int64, 63u> int63;

#endif /* FRACTIONALINTEGER_H_ */

