/**
 * @file BitRange.h
 * @brief Header file for class BitRange
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

 * @details This header file contains the declaration of the class BitRange
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPEDEV_BITRANGE_H_
#define L0TYPEDEV_BITRANGE_H_

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
 *  @brief A number with a settable size and a settable bit shift.
 *  @details Using these types into an union allows to have the same effect of a struct with bit fielded attributes.
 */
template<typename baseType, uint8 bitSize, uint8 bitOffset>
class BitRange {

public:

    /**
     * @brief Copy operator.
     * @details Since this type could be used in an union, this function change only
     * the range of bits specified leaving untouched the bits outside the range.\n
     * The \a input argument could be another type. If its value is greater than the maximum possible value
     * or lower than the minimum possible value, the value assigned will be saturated.
     * @param[in] input is the desired number value.
     */
    template<typename inputType> void operator=(inputType input);
    /**
     * @brief Cast to the type of the value attribute.
     * @details Thanks to this operator this object can be treated as a number.
     * @return the number value.
     */
    inline operator baseType() const;


    /**
     * @brief Returns the bit size.
     * @return the bit size.
     */
    static inline baseType BitSize();
    /**
     * @brief Returns the bit offset.
     * @return the bit offset.
     */
    static inline baseType BitOffset();

private:
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
    static const baseType minValue = (isSigned ? (static_cast<baseType>(-1) << (bitSize - 1)) : static_cast<baseType>(0));

    /**
     * The maximum possible value.
     */
    static const baseType maxValue = (
            isSigned ?
                    ((static_cast<baseType>(1) << (bitSize - 1)) - static_cast<baseType>(1)) :
                    (static_cast<baseType>(-1) >> ((sizeof(baseType) * 8) - bitSize)));

    /**
     * The mask covering with ones the specified bit range.
     */
    static const baseType mask = static_cast<baseType>((~static_cast<baseType>(0u) >> (baseTypeBitSize - bitSize)) << bitOffset);

    /**
     * The mask covering with ones the space out of the bit range.
     */
    static const baseType notMask = ~mask;

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType, uint8 bitSize, uint8 bitOffset>
template<typename inputType>
void BitRange<baseType, bitSize, bitOffset>::operator=(inputType input) {

    baseType temporaryValue = 0;
    const bool isInputSigned = TypeCharacteristics<inputType>::IsSigned();
    const uint8 inputBitSize = static_cast<uint8>(sizeof(inputType) * 8);

    if ((!isInputSigned) && (isSigned)) {
        // in this case checks only who is the greater (input is always >0)
        if (input > static_cast<inputType>(maxValue)) {
            temporaryValue = maxValue;
        }
        else {
            temporaryValue = static_cast<baseType>(input);
        }
    }
    else if ((isInputSigned) && (!isSigned)) {
        // in this case if input is negative saturates this to zero
        if (input < static_cast<inputType>(0)) {
            //0
            temporaryValue = minValue;
        }
        else {
            //if the input is >0 saturates in case.
            if (static_cast<baseType>(input) > maxValue) {
                temporaryValue = maxValue;
            }
            else {
                temporaryValue = static_cast<baseType>(input);
            }
        }
    }
    else {

        // choose the correct cast otherwise a positive number
        // could become negative because clipped by the cast
        if (inputBitSize <= baseTypeBitSize) {
            // the types are both signed or unsigned
            if (static_cast<baseType>(input) > maxValue) {
                temporaryValue = maxValue;
            }

            else if (static_cast<baseType>(input) < minValue) {
                temporaryValue = minValue;
            }
            else {
                temporaryValue = static_cast<baseType>(input);
            }
        }
        else {
            // the types are both signed or unsigned
            if (input > static_cast<inputType>(maxValue)) {
                temporaryValue = maxValue;
            }

            else if (input < static_cast<inputType>(minValue)) {
                temporaryValue = minValue;
            }
            else {
                temporaryValue = static_cast<baseType>(input);
            }

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

template<typename baseType, uint8 bitSize, uint8 bitOffset>
BitRange<baseType, bitSize, bitOffset>::operator baseType() const {

    baseType temporaryValue = value;

    temporaryValue &= mask;

    // to align sign bits
    temporaryValue <<= (baseTypeBitSize - bitOffset - bitSize);

    // this should sign extend
    temporaryValue >>= (baseTypeBitSize - bitSize);

    return temporaryValue;
}



template<typename baseType, uint8 bitSize, uint8 bitOffset>
baseType BitRange<baseType, bitSize, bitOffset>::BitSize() {
    return bitSize;
}
template<typename baseType, uint8 bitSize, uint8 bitOffset>
baseType BitRange<baseType, bitSize, bitOffset>::BitOffset() {
    return bitOffset;
}

}


#endif /* L0TYPEDEV_BITRANGE_H_ */

