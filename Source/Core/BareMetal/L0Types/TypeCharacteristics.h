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
static inline bool IsFloat() {
    /*lint -e{???}   Operator '!=' always evaluates to True\False. Justification: it depends by the template instance. */
    return ((static_cast<T>(0.1F)) != static_cast<T>(0));
}
/**
 *  @brief Returns true if the integer type is signed, false otherwise.
 *  @tparam T An integer type
 *  @return true if the type is signed, false otherwise.
 */
static inline bool IsSigned() {
    /*lint -e{948}   Operator '<' always evaluates to True\False. Justification: it depends by the template instance. */
    return IsFloat() || ((static_cast<T>(-1)) < 0);
}

/**
 * @brief Returns the maximum possible value of the template integer/float type.
 * @tparam T An integer/float type
 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
 */
static inline const T MaxValue() {
	return std::numeric_limits<T>::max();
}

/**
 * @brief Returns the minimum possible value of the template integer type.
 * @tparam T An integer/float type
 * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
 */
static inline const T MinValue() {
	T ret = std::numeric_limits<T>::min();
	return ret;
}

/**
 * @brief Returns the type usable bit size.
 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1). For floats it is the exponent size
 * @tparam T An integer type
 * @return the type usable bit size.
 */
static inline const uint16 UsableBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint16 nOfBits = (IsFloat()) ? static_cast<uint16>((sizeof(T)==8)? (DBL_MAX_EXP) : (FLT_MAX_EXP)) :
    		((IsSigned()) ? static_cast<uint16>(sizeof(T) * 8u - 1u) : static_cast<uint16>(sizeof(T) * 8u));
    return nOfBits;
}

/**
 * @brief Returns the type usable bit size in the negative range.
 * @details For unsigned types the usable bit size is 0, for signed types is (sizeof(T)*8-1). For floats it is the exponent size
 * @tparam T An integer type
 * @return the type usable bit size.
 */
static inline const uint16 UsableNegativeBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint16 nOfBits = (IsFloat()) ? ((sizeof(T)==8)? (DBL_MAX_EXP) : (FLT_MAX_EXP)) :
    		((IsSigned()) ? static_cast<uint8>(sizeof(T) * 8u - 1u) : 0u);
    return nOfBits;
}

};

template <>
static const float TypeCharacteristics<float>::MinValue(){
	return -std::numeric_limits<float>::max();
}

template <>
static const double TypeCharacteristics<double>::MinValue(){
	return -std::numeric_limits<double>::max();
}

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

	const outputType minValue = (isSigned)?static_cast<outputType>(std::numeric_limits<outputType>::min()>>(sizeof(outputType)*8u - bitSize)):static_cast<outputType>(0);
	const outputType maxValue = static_cast<outputType>(std::numeric_limits<outputType>::max()>>(sizeof(outputType)*8u - bitSize));

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

/**
 * Maximum value for integers of 64 bits
 */
static const int64 MAX_INT64 = TypeCharacteristics<int64>::MaxValue();

/**
 * Maximum value for unsigned integers of 64 bits
 */
static const uint64 MAX_UINT64 = TypeCharacteristics<uint64>::MaxValue();

/**
 * Minimum value for integers of 64 bits
 */
static const int64 MIN_INT64 = TypeCharacteristics<int64>::MinValue();

/**
 * Minimum value for unsigned integers of 64 bits
 */
static const uint64 MIN_UINT64 = TypeCharacteristics<uint64>::MinValue();

/**
 * Maximum value for integers of 32 bits
 */
static const int32 MAX_INT32 = TypeCharacteristics<int32>::MaxValue();

/**
 * Maximum value for unsigned integers of 32 bits
 */
static const uint32 MAX_UINT32 = TypeCharacteristics<uint32>::MaxValue();

/**
 * Minimum value for integers of 32 bits
 */
static const int32 MIN_INT32 = TypeCharacteristics<int32>::MinValue();

/**
 * Minimum value for unsigned integers of 32 bits
 */
static const uint32 MIN_UINT32 = TypeCharacteristics<uint32>::MinValue();

/**
 * Maximum value for integers of 16 bits
 */
static const int16 MAX_INT16 = TypeCharacteristics<int16>::MaxValue();

/**
 * Maximum value for unsigned integers of 16 bits
 */
static const uint16 MAX_UINT16 = TypeCharacteristics<uint16>::MaxValue();

/**
 * Minimum value for integers of 16 bits
 */
static const int16 MIN_INT16 = TypeCharacteristics<int16>::MinValue();

/**
 * Minimum value for unsigned integers of 16 bits
 */
static const uint16 MIN_UINT16 = TypeCharacteristics<uint16>::MinValue();

/**
 * Maximum value for integers of 8 bits
 */
static const int8 MAX_INT8 = TypeCharacteristics<int8>::MaxValue();

/**
 * Maximum value for unsigned integers of 8 bits
 */
static const uint8 MAX_UINT8 = TypeCharacteristics<uint8>::MaxValue();

/**
 * Minimum value for integers of 8 bits
 */
static const int8 MIN_INT8 = TypeCharacteristics<int8>::MinValue();

/**
 * Minimum value for unsigned integers of 8 bits
 */
static const uint8 MIN_UINT8 = TypeCharacteristics<uint8>::MinValue();

/**
 * Maximum value for 32 bit floats
 */
static const float32 MAX_FLOAT32 = TypeCharacteristics<float32>::MaxValue();

/**
 * Minimum value for 32 bit floats
 */
static const float32 MIN_FLOAT32 = TypeCharacteristics<float32>::MinValue();

/**
 * Maximum value for 32 bit floats
 */
static const float64 MAX_FLOAT64 = TypeCharacteristics<float64>::MaxValue();

/**
 * Minimum value for 32 bit floats
 */
static const float64 MIN_FLOAT64 = TypeCharacteristics<float64>::MinValue();


static const uint32 EPSILON_FLOAT32 = 0x34000000u;
static const uint64 EPSILON_FLOAT64 = 0x3CB0000000000000ull;

}

#endif /* TYPECHARACTERISTICS_H_ */

