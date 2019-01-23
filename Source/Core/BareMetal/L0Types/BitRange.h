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
/*lint -efile(766,BitRange.h) The header file TemplateParametersVerificator.h is used when the template is expanded (by the header that includes this header).*/
#include "CompilerTypes.h"
#include "TypeCharacteristics.h"
#include "TemplateParametersVerificator.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *  @brief A number with configurable size and bit offset.
 *  @details Using these types into an union allows to have the same effect of a struct with bit fielded attributes.
 *  @tparam baseType the standard type which is used as a base for the novel type
 *  @tparam numberOfBits the actual bit size of the novel type
 *  @tparam bitOffset the actual bit offset of the novel type
 *  @remark numberOfBits + bitOffset must be minor than the bit size of baseType.
 */
/*lint -e{1721} operator= is not assignment operator. Justification: the input argument is a
 basic type because this type must be used as a binary number.*/
/*lint -etemplate(948, 1790, 1942, 685, 944, 845) No code is truly generated.
 * This strategy is used to guarantee that certain rules about the template are guaranteed at compilation time.
 * Operator will always evaluate to true if the template is correctly used, otherwise it will not compile (which the objective)*/
template<typename baseType, uint8 numberOfBits, uint8 bitOffset>
class BitRange: public TemplateParametersVerificator<((static_cast<uint16>(sizeof(baseType) * 8u)) >= (numberOfBits + bitOffset)) && (numberOfBits > 0u) && (bitOffset >= 0u)> {

public:

    /**
     * @brief Copy operator.
     * @details Since this type could be used in an union, this function change only
     * the range of bits specified leaving untouched the bits outside the range.
     * @details The input argument could be another type. If its value is greater than the maximum possible value
     * or lower than the minimum possible value, the value assigned will be saturated.
     * @tparam inputType the actual type for input
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
     * @brief Returns the number of bits.
     * @return the bit size.
     */
    static inline baseType GetNumberOfBits();

    /**
     * @brief Returns the bit offset.
     * @return the bit offset.
     */
    static inline baseType BitOffset();

private:

    /** attempt at stopping copy operator  misbehaviour */
    operator BitRange<baseType, numberOfBits, bitOffset>() const { return *this; }

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
    static const baseType mask = static_cast<baseType>((static_cast<baseType>(~static_cast<baseType>(0u)) >> (baseTypeBitSize - numberOfBits)) << bitOffset);

    /**
     * The mask covering with ones the space out of the bit range.
     */
    static const baseType notMask = static_cast<baseType>(~mask);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType, uint8 numberOfBits, uint8 bitOffset>
template<typename inputType>
void BitRange<baseType, numberOfBits, bitOffset>::operator=(inputType input) {

    baseType temporaryValue = SaturateInteger<baseType, inputType, numberOfBits>(input);

    // shifts the number
    temporaryValue <<= bitOffset;

    // masks the number
    temporaryValue &= mask;

    // clears the bit range in the value
    value &= notMask;

    // puts the value in the bit range
    value |= temporaryValue;
}

template<typename baseType, uint8 numberOfBits, uint8 bitOffset>
BitRange<baseType, numberOfBits, bitOffset>::operator baseType() const {

    baseType temporaryValue = value;

    temporaryValue &= mask;

    // to align sign bits
    temporaryValue <<= (baseTypeBitSize - bitOffset - numberOfBits);

    // this should sign extend
    temporaryValue >>= (baseTypeBitSize - numberOfBits);

    return temporaryValue;
}

template<typename baseType, uint8 numberOfBits, uint8 bitOffset>
baseType BitRange<baseType, numberOfBits, bitOffset>::GetNumberOfBits() {
    return numberOfBits;
}

template<typename baseType, uint8 numberOfBits, uint8 bitOffset>
baseType BitRange<baseType, numberOfBits, bitOffset>::BitOffset() {
    return bitOffset;
}

template <typename baseType, uint8 numberOfBits, uint8 bitOffset>
class TypeCharacteristics<BitRange<baseType, numberOfBits, bitOffset>>{
public:

	/**
	 *  @brief Returns true if the type is a float, false otherwise.
	 *  @tparam T a float/integer type
	 *  @return true if the type is a float, false otherwise.
	 */
	static bool IsFloat() {
	    return false;
	}
	/**
	 *  @brief Returns true if the integer type is signed, false otherwise.
	 *  @tparam T An integer type
	 *  @return true if the type is signed, false otherwise.
	 */
	static bool IsSigned() {
	    /*lint -e{948}   Operator '<' always evaluates to True\False. Justification: it depends by the template instance. */
	    return ((static_cast<baseType>(-1)) < 0);
	}

	/**
	 * @brief Returns the maximum possible value of the template integer/float type.
	 * @tparam T An integer/float type
	 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
	 */
	static const baseType MaxValue() {
		const baseType maxNum = (std::numeric_limits<baseType>::max()>>(sizeof(baseType)*8 - numberOfBits));
		return maxNum;
	}

	/**
	 * @brief Returns the minimum possible value of the template integer type.
	 * @tparam T An integer/float type
	 * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
	 */
	static const baseType MinValue() {
		const baseType minNum = (IsSigned())?(std::numeric_limits<baseType>::min()>>(sizeof(baseType)*8 - numberOfBits)):0;
		return minNum;
	}

	/**
	 * @brief Returns the type usable bit size.
	 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1). For floats it is the exponent size
	 * @tparam T An integer type
	 * @return the type usable bit size.
	 */
	static const uint8 UsableBitSize() {
		const uint8 nOfBits = (IsSigned())?numberOfBits-1:numberOfBits;
	    return nOfBits;
	}

	/**
	 * @brief Returns the type usable bit size in the negative range.
	 * @details For unsigned types the usable bit size is 0, for signed types is (sizeof(T)*8-1). For floats it is the exponent size
	 * @tparam T An integer type
	 * @return the type usable bit size.
	 */
	static const uint8 UsableNegativeBitSize() {
		const uint8 nOfBits = (IsSigned())?numberOfBits-1:0;
	    return nOfBits;
	}
};


}

#endif /* L0TYPEDEV_BITRANGE_H_ */
