/**
 * @file StringToFloat.cpp
 * @brief Source file for class StringToFloat
 * @date 11/11/2015
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

 * @details This source file contains the definition of all the methods for
 * the class StringToFloat (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "FormatDescriptor.h"
#include "IOBuffer.h"
#include "BitSetToInteger.h"
#include "ErrorManagement.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define CHECK_AND_MUL(number,step,exponent)\
if (exponent >= step){ \
number *= static_cast<T>(1E ## step); \
exponent-=step;\
}

#define CHECK_AND_DIV(number,step,exponent)\
if (exponent >= step){ \
number *= static_cast<T>(1E- ## step); \
exponent-=step;\
}

namespace MARTe {

/**
 * @brief Given number and exponent performs the operation number*(10^exponent).
 * @param[in,out] number is the float number in input which will be multiplied with 10^exponent.
 * @param[in] exponent is the absolute value of the exponent.
 * @param[in] expPositive specifies the sign of the exponent (true=positive, false=negative).
 * @post
 *   number=number*(10^(+/- exponent));
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
static void AddExponent(T &number,
                        int32 exponent,
                        const bool expPositive) {

    // check and mul/div progressively following a logarithmic pattern
    if (expPositive) {
        if (sizeof(T) > 4u) {
            CHECK_AND_MUL(number, 256, exponent)
            CHECK_AND_MUL(number, 128, exponent)
            CHECK_AND_MUL(number, 64, exponent)
        }
        CHECK_AND_MUL(number, 32, exponent)
        CHECK_AND_MUL(number, 16, exponent)
        CHECK_AND_MUL(number, 8, exponent)
        CHECK_AND_MUL(number, 4, exponent)
        CHECK_AND_MUL(number, 2, exponent)
        if (exponent >= 1) {
            number *= static_cast<T>(10.0);
        }
    }
    else {
        if (sizeof(T) > 4u) {
            CHECK_AND_DIV(number, 256, exponent)
            CHECK_AND_DIV(number, 128, exponent)
            CHECK_AND_DIV(number, 64, exponent)
        }
        CHECK_AND_DIV(number, 32, exponent)
        CHECK_AND_DIV(number, 16, exponent)
        CHECK_AND_DIV(number, 8, exponent)
        CHECK_AND_DIV(number, 4, exponent)
        CHECK_AND_DIV(number, 2, exponent)
        if (exponent >= 1) {
            number /= static_cast<T>(10.0);
        }
    }
}

/**
 * @brief Performs the conversion from a token to a float number.
 * @param[in] input is the token in input.
 * @param[out] number is the conversion result.
 * @return true if the token represents a float number, false in case of invalid token or in case of overflow.
 * @pre
 *   input must represent a float number;
 *   "+/-" at the beginning specifies if the number is positive or negative (no sign means positive);
 *     "." specifies the beginning of the decimal part.
 *     "E" specifies that the exponent follows.
 *       "+/-" specifies the sign of the exponent (no sign means positive exponent).
 * @post
 *   If the token represents a number too big for the output size, the conversion will stop returning false
 *   and the number converted until the overflow exception (i.e for 32-bit float "123.5E+100" will return 123.5);
 *   If an invalid character is read, the conversion will stop returning false
 *   and the number converted until the overflow exception (i.e "123.5e+100" will return 123 because 'e' is invalid);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
static bool StringToNormalFloatPrivate(const char8 * const input,
                                       T &number) {

    bool canReturn = false;
    bool ret = true;
    bool isNegative = false;

    number = static_cast<T>(0.0);

    uint32 i = 0u;

    char8 digit = '\0';

    if (input[i] == '-') {
        isNegative = true;
        i++;
    }
    else {
        //skip the eventual sign
        if (input[i] == '+') {
            i++;
        }
    }

    // Find the dot or the exponent
    for (; (!canReturn) && (i < 1000u); i++) {

        digit = input[i];
        int8 zero = static_cast<int8>('0');
        int8 newDigit = (static_cast<int8>(digit) - zero);

        if ((newDigit >= 0) && (newDigit <= 9)) {
            T numberTemp = number * static_cast<T>(10.0);
            bool nanNumber = isNaN(numberTemp);
            bool infNumber = isInf(numberTemp);
            if ((!nanNumber) && (!infNumber)) {
                numberTemp += static_cast<T>(newDigit);
                nanNumber = isNaN(numberTemp);
                infNumber = isInf(numberTemp);
                if ((!nanNumber) && (!infNumber)) {
                    number = numberTemp;
                }
                else {
                    canReturn = true;
                    ret = false;
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Overflow");
                }
            }
            else {
                canReturn = true;
                ret = false;
                REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Overflow");
            }
        }
        else {
            if ((digit == '.') || (digit == 'E') || (digit == 'e')) {
                break;
            }
            else {
                if (digit != '\0') {
                    ret = false;
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Invalid Token");
                }
                //return false is nothing is read
                canReturn = true;
            }
        }
    }

    if (!canReturn) {

        // Find the exponent
        if (digit == '.') {
            i++;
            T decimalPart = static_cast<T>(0.0);
            T decimalExp = static_cast<T>(1.0);

            for (; (!canReturn) && (i < 1000u); i++) {
                digit = input[i];
                int8 zero = static_cast<int8>('0');
                int8 newDigit = (static_cast<int8>(digit) - zero);

                if ((newDigit >= 0) && (newDigit <= 9)) {
                    decimalExp *= static_cast<T>(10.0);
                    bool nanExp = isNaN(decimalExp);
                    bool infExp = isInf(decimalExp);
                    if ((!nanExp) && (!infExp)) {
                        decimalPart += static_cast<T>(newDigit) / decimalExp;
                    }
                    else {
                        canReturn = true;
                        REPORT_ERROR(ErrorManagement::Warning, "StringToFloatPrivate: Too much decimal precision for this float type");
                    }
                }
                else {

                    if ((digit == 'E') || (digit == 'e')) {
                        break;
                    }
                    else {
                        if (digit != '\0') {
                            ret = false;
                            REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Invalid Token");
                        }
                        canReturn = true;
                    }
                }
            }
            T numberTemp = number;
            numberTemp += decimalPart;
            bool nanNumber = isNaN(numberTemp);
            bool infNumber = isInf(numberTemp);
            if ((!nanNumber) && (!infNumber)) {
                number = numberTemp;
            }
            else {
                canReturn = true;
                ret = false;
                REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Overflow");
            }
        }

        // take the exponent (in case of compact...)
        if (((digit == 'E') || (digit == 'e')) && (!canReturn)) {
            i++;
            int32 exponent = 0;
            bool expPositive = true;
            digit = input[i];
            if (digit == '+') {
                i++;
            }
            else {
                if (digit == '-') {
                    expPositive = false;
                    i++;
                }
            }

            for (; (!canReturn) && (i < 1000u); i++) {
                digit = input[i];
                int8 zero = static_cast<int8>('0');
                int8 newDigit = (static_cast<int8>(digit) - zero);
                if ((newDigit >= 0) && (newDigit <= 9)) {
                    exponent *= 10;
                    exponent += newDigit;
                    if (exponent >= 512) {
                        ret = false;
                        REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Overflow");
                        canReturn = true;
                    }
                }
                else {
                    if (digit != '\0') {
                        ret = false;
                        REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Invalid Token");
                    }
                    break;
                }
            }
            if (!canReturn) {
                T numberTemp = number;
                AddExponent(numberTemp, exponent, expPositive);
                bool nanNumber = isNaN(numberTemp);
                bool infNumber = isInf(numberTemp);
                if ((!nanNumber) && (!infNumber)) {
                    number = numberTemp;
                }
                else {
                    ret = false;
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToFloatPrivate: Overflow");
                }
            }
        }
    }

    if (isNegative) {
        number = -number;
    }

    return ret;
}

/**
 * @brief In case of number in hexadecimal, octal or binary format, converts it to a float number, otherwise calls StringToNormalFloatPrivate(*).
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
static bool StringToFloatPrivate(const char8 * const input,
                                 T &number) {

    bool ret = false;
    if (input[0] == '0') {
        // performs the conversion from integer in hex, oct and bin format to float.
        if ((input[1] == 'x') || (input[1] == 'o') || (input[1] == 'b')) {
            uint64 numberTemp = 0u;
            ret = TypeConvert(numberTemp, input);
            if (ret) {
                ret = TypeConvert(number, numberTemp);
            }
        }
        else {
            ret = StringToNormalFloatPrivate(input, number);
        }
    }
    else {
        ret = StringToNormalFloatPrivate(input, number);
    }
    return ret;
}

/**
 * @see StringToFloatPrivate(*).
 */
bool StringToFloatGeneric(const char8 * const input,
                          float32 * const number,
                          const uint32 destBitSize) {
    bool ret = false;
    if (destBitSize == 32u) {
        ret = StringToFloatPrivate(input, *number);
    }
    if (destBitSize == 64u) {
        ret = StringToFloatPrivate(input, *reinterpret_cast<float64*>(number));

    }
    return ret;
}

}
