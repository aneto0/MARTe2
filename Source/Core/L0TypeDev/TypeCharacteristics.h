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

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

namespace TypeCharacteristics {

/**
 *  @brief Returns true if the integer type is signed, false otherwise.
 *  @return true if the type is signed, false otherwise.
 */
template<typename T>
bool IsSigned() {
    /*lint -e{948}   Operator '<' always evaluates to True\False. Justification: it depends by the template instance. */
    return ((static_cast<T>(-1)) < 0);
}

/**
 * @brief Returns the maximum possible value of the template integer type.
 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
 */
template<typename T>
const T MaxValue() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const T maxValue = (IsSigned<T>()) ? static_cast<T>((static_cast<T>(1) << (sizeof(T) * 8u - 1u)) - static_cast<T>(1)) : static_cast<T>(-1);
    return maxValue;
}

/**
 * @brief Returns the minimum possible value of the template integer type.
 * @return 0x00...0 if the type is unsigned, 0x80...0 is if it is signed
 */
template<typename T>
const T MinValue() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const T minValue = (IsSigned<T>()) ? static_cast<T>(1 << (sizeof(T) * 8u - 1u)) : static_cast<T>(0);
    return minValue;
}

/**
 * @brief Returns the type usable bit size.
 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1)
 * @return the type usable bit size.
 */
template<typename T>
const uint8 UsableBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint8 nOfBits = (IsSigned<T>()) ? static_cast<uint8>(sizeof(T) * 8u - 1u) : static_cast<uint8>(sizeof(T) * 8u);
    return nOfBits;
}

/**
 * @brief Returns the maximum possible value of the template integer type with the specified bit size.
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
 * @details For unsigned types the usable bit size is (sizeof(T)*8), for signed types is (sizeof(T)*8-1)
 * @return the type usable bit size.
 */
template<typename T, uint8 bitSize>
const uint8 UsableBitSize() {
    /*lint -e{944}  Left argument for operator '?' always evaluates to True\False. Justification: it depends by the template instance. */
    const uint8 nOfBits = (IsSigned<T>() ? (bitSize - 1u) : bitSize);
    return nOfBits;
}

}

/**
 * @brief Saturates the input if it does not fit within the range of numbers with the specified bit size.
 * @param[in] input is the input value.
 * @return If the input value is minor than the maximum value (depending on the specified type and bit size)
 * and greater than the minimum value it will be returned untouched. Otherwise this function returns the
 * maximum value if it is minor than the input, or the minimum value if is greater than the input.
 */
template<typename outputType, typename inputType, uint8 bitSize>
outputType SaturateInteger(const inputType input) {

    const bool isSigned = TypeCharacteristics::IsSigned<outputType>();

    const outputType minValue = TypeCharacteristics::MinValue<outputType, bitSize>();

    const outputType maxValue = TypeCharacteristics::MaxValue<outputType, bitSize>();

    //default assignment
    outputType value = static_cast<outputType>(input);

    // saturation to max
    if (input >= static_cast<inputType>(0)) {

        // cast to the type which has the max usable size
        if (TypeCharacteristics::UsableBitSize<inputType>() > TypeCharacteristics::UsableBitSize<outputType>()) {
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
            if (TypeCharacteristics::UsableBitSize<inputType>() > TypeCharacteristics::UsableBitSize<outputType>()) {
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

}

#endif /* TYPECHARACTERISTICS_H_ */

