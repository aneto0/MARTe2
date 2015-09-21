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

#include "CompilerTypes.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *  @brief A number with configurable size and bit offset.
 *  @details Using these types into an union allows to have the same effect of a struct with bit fielded attributes.
 *  @warning bitSize + bitOffset must be minor than the bit size of baseType.
 */
/*lint -e{1721} operator= is not assignment operator. Justification: the input argument is a
 basic type because this type must be used as a binary number.*/
template<typename baseType, uint8 bitSize, uint8 bitOffset>
class BitRange {

public:

    /**
     * @brief Copy operator.
     * @details Since this type could be used in an union, this function change only
     * the range of bits specified leaving untouched the bits outside the range.
     * @details The input argument could be another type. If its value is greater than the maximum possible value
     * or lower than the minimum possible value, the value assigned will be saturated.
     * @param[in] input is the desired number value.
     */
    template<typename inputType>
    void operator=(inputType input);

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
    static const uint8 baseTypeBitSize = static_cast<uint8>(sizeof(baseType) * 8u);

    /**
     * The mask covering with ones the specified bit range.
     */
    /*lint -e{845} The right argument to operator >> / << is certain to be 0. Justification: it depends by the template instance. */
    static const baseType mask = static_cast<baseType>((static_cast<baseType>(~static_cast<baseType>(0u)) >> (baseTypeBitSize - bitSize)) << bitOffset);

    /**
     * The mask covering with ones the space out of the bit range.
     */
    static const baseType notMask = static_cast<baseType>(~mask);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType, uint8 bitSize, uint8 bitOffset>
template<typename inputType>
void BitRange<baseType, bitSize, bitOffset>::operator=(inputType input) {

    baseType temporaryValue = SaturateInteger<baseType, inputType, bitSize>(input);

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
