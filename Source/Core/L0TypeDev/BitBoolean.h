/**
 * @file BitBoolean.h
 * @brief Header file for class BitBoolean
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

 * @details This header file contains the declaration of the class BitBoolean
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BITBOOLEAN_H_
#define BITBOOLEAN_H_

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
 *  @brief Boolean shifted type.
 *  @details This type could be used in an union obtaining the same effect of a one bit boolean in a structure.
 */
template<typename baseType, uint8 bitOffset>
class BitBoolean {

public:

    /**
     * @brief Copy operator.
     * @param[in] flag is the boolean value in input
     */
    void operator=(bool flag);

    /**
     * @brief Returns the boolean value.
     * @return the boolean value.
     */
    inline operator bool() const;
    /**
     * @brief Cast to AnyType.
     * @details Thanks to this operator this object can be treated as an AnyType object.
     * @return the AnyType associated to this object.
     */
    inline operator AnyType() const;

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
     * The mask (only a bit shifted)
     */
    static const baseType mask = (static_cast<baseType>(1) << bitOffset);

    /**
     * A mask with 0 in the specified bit and 1 in other number bits.
     */
    static const baseType notMask = ~mask;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType, uint8 bitOffset>
void BitBoolean<baseType, bitOffset>::operator=(bool flag) {
    if (flag) {
        value |= mask;
    }
    else {
        value &= notMask;
    }
}

template<typename baseType, uint8 bitOffset>
BitBoolean<baseType, bitOffset>::operator bool() const {
    return ((value & mask) == mask) && (mask != static_cast<baseType>(0));
}

template<typename baseType, uint8 bitOffset>
BitBoolean<baseType, bitOffset>::operator AnyType() const {
    const TypeDescriptor td = { false, false, { { UnsignedInteger, 1 } } };
    return AnyType(td, bitOffset, this);
}

template<typename baseType, uint8 bitOffset>
baseType BitBoolean<baseType, bitOffset>::BitSize() {
    return static_cast<baseType>(1);
}

template<typename baseType, uint8 bitOffset>
baseType BitBoolean<baseType, bitOffset>::BitOffset() {
    return bitOffset;
}

} // end TypeDefinition namespace
#endif /* BITBOOLEAN_H_ */

