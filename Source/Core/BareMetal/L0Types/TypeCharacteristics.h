/**
 * @file TypeCharacteristics.h
 * @brief Header file for module TypeCharacteristics
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

 * @details This header file contains the declaration of the module TypeCharacteristics
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECHARACTERISTICS_H_
#define TYPECHARACTERISTICS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include <limits>
#include <float.h>

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/***
 * Defined in WinDef.h
 * Conflicts with the methods min() and max() of <limits>
 */
#undef max
#undef min

/**
 * @brief Remaps and extends <limits>
 */
template<typename T>
class  TypeCharacteristics {
public:
/**
 *  @brief Returns true if the type is a float, false otherwise.
 *  @tparam T a float/integer type
 *  @return true if the type is a float, false otherwise.
 */
static bool IsFloat() {
    /*lint -e{???}   Operator '!=' always evaluates to True\False. Justification: it depends by the template instance. */
    return ((static_cast<T>(0.1F)) != static_cast<T>(0));
}
/**
 *  @brief Returns true if the integer type is signed, false otherwise.
 *  @tparam T An integer type
 *  @return true if the type is signed, false otherwise.
 */
static bool IsSigned() {
    /*lint -e{948}   Operator '<' always evaluates to True\False. Justification: it depends by the template instance. */
    return IsFloat() || ((static_cast<T>(-1)) < 0);
}

/**
 * @brief Returns the maximum possible value of the template integer/float type.
 * @tparam T An integer/float type
 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
 */
static const T MaxValue() {
	return std::numeric_limits<T>::max();
}

/**
 * @brief Returns the minimum possible value of the template integer type.
 * @tparam T An integer/float type
 * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
 */
static const T MinValue() {
	T ret = std::numeric_limits<T>::min();
	if (IsFloat()){
		ret = -std::numeric_limits<T>::max();
	}
	return ret;
}

/**
 * @brief Returns the type usable bit size.
 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1). For floats it is the exponent size
 * @tparam T An integer type
 * @return the type usable bit size.
 */
static const uint16 UsableBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint16 nOfBits = (IsFloat()) ? ((sizeof(T)==8)? (DBL_MAX_EXP) : (FLT_MAX_EXP)) :
    		((IsSigned()) ? static_cast<uint8>(sizeof(T) * 8u - 1u) : static_cast<uint8>(sizeof(T) * 8u));
    return nOfBits;
}

/**
 * @brief Returns the type usable bit size in the negative range.
 * @details For unsigned types the usable bit size is 0, for signed types is (sizeof(T)*8-1). For floats it is the exponent size
 * @tparam T An integer type
 * @return the type usable bit size.
 */
static const uint16 UsableNegativeBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint16 nOfBits = (IsFloat()) ? ((sizeof(T)==8)? (DBL_MAX_EXP) : (FLT_MAX_EXP)) :
    		((IsSigned()) ? static_cast<uint8>(sizeof(T) * 8u - 1u) : 0);
    return nOfBits;
}

};

#if 0
/**
 * @brief Remaps and extends <limits>
 */
namespace TypeCharacteristics {

/**
 *  @brief Returns true if the type is a float, false otherwise.
 *  @tparam T a float/integer type
 *  @return true if the type is a float, false otherwise.
 */
template<typename T>
bool IsFloat() {
    /*lint -e{???}   Operator '!=' always evaluates to True\False. Justification: it depends by the template instance. */
    return ((static_cast<T>(0.1F)) != static_cast<T>(0));
}

/**
 *  @brief Returns true if the integer type is signed, false otherwise.
 *  @tparam T An integer type
 *  @return true if the type is signed, false otherwise.
 */
template<typename T>
bool IsSigned() {
    /*lint -e{948}   Operator '<' always evaluates to True\False. Justification: it depends by the template instance. */
    return IsFloat<T>() || ((static_cast<T>(-1)) < 0);
}

/**
 * @brief Returns the maximum possible value of the template integer/float type.
 * @tparam T An integer/float type
 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
 */
template<typename T>
const T MaxValue() {
	return std::numeric_limits<T>::max();
#if 0
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const T maxValue = (IsSigned<T>()) ? static_cast<T>((static_cast<T>(1) << (sizeof(T) * 8u - 1u)) - static_cast<T>(1)) : static_cast<T>(-1);
    return maxValue;
#endif
}

/**
 * @brief Returns the minimum possible value of the template integer type.
 * @tparam T An integer/float type
 * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
 */
template<typename T>
const T MinValue() {
	return std::numeric_limits<T>::min();

#if 0
	/*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const T minValue = (IsSigned<T>()) ? static_cast<T>(1 << (sizeof(T) * 8u - 1u)) : static_cast<T>(0);
    return minValue;
#endif
}


/**
 * @brief Returns the type usable bit size.
 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1). For floats it is the exponent size
 * @tparam T An integer type
 * @return the type usable bit size.
 */
template<typename T>
const uint8 UsableBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint8 nOfBits = (IsFloat<T>()) ? ((sizeof(T)==8)? (DBL_MAX_EXP) : (FLT_MAX_EXP)) :
    		((IsSigned<T>()) ? static_cast<uint8>(sizeof(T) * 8u - 1u) : static_cast<uint8>(sizeof(T) * 8u));
    return nOfBits;
}

/**
 * @brief Returns the type usable bit size in the negative range.
 * @details For unsigned types the usable bit size is 0, for signed types is (sizeof(T)*8-1). For floats it is the exponent size
 * @tparam T An integer type
 * @return the type usable bit size.
 */
template<typename T>
const uint8 UsableNegativeBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint8 nOfBits = (IsFloat<T>()) ? ((sizeof(T)==8)? (DBL_MAX_EXP) : (FLT_MAX_EXP)) :
    		((IsSigned<T>()) ? static_cast<uint8>(sizeof(T) * 8u - 1u) : 0);
    return nOfBits;
}

/**
 * @brief Returns the maximum possible value of the template integer type with the specified bit size.
 * @tparam T An integer type
 * @tparam bitSize The bit size of type T
 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
 */
template<typename T, uint8 bitSize>
const T MaxValue() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const T maxValue =
            (IsSigned<T>()) ?
                    static_cast<T>(static_cast<T>(static_cast<T>(1) << (bitSize - 1u)) - static_cast<T>(1)) :
                    static_cast<T>(static_cast<T>(-1) >> ((sizeof(T) * 8u) - bitSize));

    return maxValue;
}

/**
 * @brief Returns the minimum possible value of the template integer type with the specified bit size.
 * @tparam T An integer type
 * @tparam bitSize The bit size of type T
 * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
 */
template<typename T, uint8 bitSize>
const T MinValue() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const T minValue = (IsSigned<T>()) ? static_cast<T>(static_cast<T>(-1) << (bitSize - 1u)) : static_cast<T>(0);
    return minValue;
}

/**
 * @brief Returns the type usable bit size with the specified bit size.
 * @tparam T An integer type
 * @tparam bitSize The bit size of type T
 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1)
 * @return the type usable bit size.
 */
template<typename T, uint8 bitSize>
const uint8 UsableBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint8 nOfBits = (IsSigned<T>() ? (bitSize - 1u) : bitSize);
    return nOfBits;
}

};

#endif

/**
 * @briefs converts any number to any other number saturating the conversion
 * @return false if saturation was necessary
 */
template <typename inputType,typename outputType>
inline bool SafeNumber2Number(inputType src,outputType &dest){
	 bool ret = true;
	 // more bits in the input format. Might need to saturate
	 if (TypeCharacteristics<inputType>::UsableBitSize()>TypeCharacteristics<outputType>::UsableBitSize()){
		 const inputType maxSource = static_cast<inputType>(TypeCharacteristics<outputType>::MaxValue());
		 if (src > maxSource) {
			 src = maxSource;
			 ret = false;
		 }
	 }
	 if (TypeCharacteristics<inputType>::UsableNegativeBitSize()>TypeCharacteristics<outputType>::UsableNegativeBitSize()){
		 const inputType minSource = static_cast<inputType>(TypeCharacteristics<outputType>::MinValue());
		 if (src < minSource) {
			 src = minSource;
			 ret = false;
		 }
	 }

	 dest = static_cast<outputType>(src);

	 return ret;
}

#if 1
/**
 * @brief Saturates the input if it does not fit within the range of numbers with the specified bit size.
 * @tparam outputType An integer type
 * @tparam inputType An integer type
 * @tparam bitSize The bit size value of type outputType
 * @param[in] input is the input value.
 * @return If the input value is minor than the maximum value (depending on the specified type and bit size)
 * and greater than the minimum value it will be returned untouched. Otherwise this function returns the
 * maximum value if it is smaller than the input, or the minimum value if is greater than the input.
 */
template<typename outputType, typename inputType, uint8 bitSize>
outputType SaturateInteger(const inputType input) {

    const bool isSigned = TypeCharacteristics<outputType>::IsSigned();

	const outputType minValue = (isSigned)?(std::numeric_limits<outputType>::min()>>(sizeof(outputType)*8 - bitSize)):0;
	const outputType maxValue = std::numeric_limits<outputType>::max()>>(sizeof(outputType)*8 - bitSize);

    //default assignment
    outputType value = static_cast<outputType>(input);

    // saturation to max
    if (input >= static_cast<inputType>(0)) {

        // cast to the type which has the max usable size
        if (TypeCharacteristics<inputType>::UsableBitSize() > TypeCharacteristics<outputType>::UsableBitSize()) {
            if (input > static_cast<inputType>(maxValue)) {
                value = maxValue;
            }
        }
        else {
            // input<0 and base type unsigned bug
            if (static_cast<outputType>(input) > maxValue) {
                value = maxValue;
            }
        }
    }

    //saturation to min
    else {
        // check min
        if (!isSigned) {
            value = static_cast<outputType>(0);
        }
        else {

            // only consider signed against signed for minimum
            // unsigned have 0 as minimum which is greater than the minimum of all fractional signed
            if (TypeCharacteristics<inputType>::UsableBitSize() > TypeCharacteristics<outputType>::UsableBitSize()) {
                if (input < static_cast<inputType>(minValue)) {
                    value = minValue;
                }
            }
            else {

                // input>0 and base type signed bug
                if (static_cast<outputType>(input) < minValue) {
                    value = minValue;
                }
            }
        }
    }
    return value;
}

#endif
}

#endif /* TYPECHARACTERISTICS_H_ */

