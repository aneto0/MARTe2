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
/*lint -efile(766,BitBoolean.h) The header file TemplateParametersVerificator.h is used when the template is expanded (by the header that includes this header).*/
#include "CompilerTypes.h"
#include "TemplateParametersVerificator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *  @brief Boolean shifted type.
 *  @details This type could be used in an union obtaining the same effect of a one bit boolean in a structure.
 *  @tparam baseType the standard type which is used as a base for the new type
 *  @tparam bitOffset the actual bit offset of the new type
 *  @warning bitOffset must be less than the number of bits of baseType.
 */
/*lint -e{1721} operator= is not assignment operator. Justification: the input argument is bool because this type must be used as a boolean type.*/
/*lint -etemplate(948, 1790, 1942) No code is truly generated. This strategy is used to guarantee that certain rules
 * about the template are guaranteed at compilation time.
 * Operator will always evaluate to true if the template is correctly used, otherwise it will not compile (which is the objective)*/
template<typename baseType, uint8 bitOffset>
class BitBoolean: public TemplateParametersVerificator<((sizeof(baseType) * 8u) > bitOffset)> {

public:

    /**
     * @brief Copy operator.
     * @param[in] flag is the boolean value in input
     */
    void operator=(const bool flag);

    /**
     * @brief Returns the boolean value.
     * @return the boolean value.
     */
    inline operator bool() const;

    /**
     * @brief Returns number of bits.
     * @return the number of bits.
     */
    static inline baseType NumberOfBits();

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
     * The mask (only a bit shifted)
     */
    /*lint -e{845} The right argument to operator >> / << is certain to be 0. Justification: it depends by the template instance. */
    static const baseType mask = static_cast<baseType>(static_cast<baseType>(1) << bitOffset);

    /**
     * A mask with 0 in the specified bit and 1 in other number bits.
     */
    static const baseType notMask = static_cast<baseType>(~mask);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/*lint -e{1539} the attribute value is not initialized because this type can be used inside an union*/
template<typename baseType, uint8 bitOffset>
void BitBoolean<baseType, bitOffset>::operator=(const bool flag) {
    if (flag) {
        value |= mask;
    }
    else {
        value &= notMask;
    }
}

template<typename baseType, uint8 bitOffset>
BitBoolean<baseType, bitOffset>::operator bool() const {
    return ((value & mask) != 0u);
}

template<typename baseType, uint8 bitOffset>
baseType BitBoolean<baseType, bitOffset>::NumberOfBits() {
    return static_cast<baseType>(1);
}

template<typename baseType, uint8 bitOffset>
baseType BitBoolean<baseType, bitOffset>::BitOffset() {
    return bitOffset;
}

}

#endif /* BITBOOLEAN_H_ */

