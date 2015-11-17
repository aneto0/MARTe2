/**
 * @file StringToInteger.cpp
 * @brief Source file for class StringToInteger
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
 * the class StringToInteger (public, protected, and private). Be aware that some 
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
#include "BitSetToInteger.h"
#include "IOBuffer.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Converts a 0-terminated C-String token representing an integer in decimal notation to an integer.
 * @param[in] input is the token to be converted.
 * @param[out] number is the conversion result in output.
 * @return true if the token in input represents an integer in decimal notation which fits in the provided output type.
 * A failure can occur in case of overflow or if the token is invalid.
 * @pre
 *   input must be a valid token for integer in decimal notation;
 *   +/- at the beginning specifies if the number is negative or positive (no sign means positive);
 * @post
 *   If the token represents a number too big for the output size, the conversion will stop returning
 *   false and the number read until the overflow exception. (i.e "128" for signed 8-bit integer will return 12);
 *   If an invalid character (not in [0-9] range) is found, the conversion will stop returning
 *   false and the number read until the overflow exception. (i.e "12ab34" will return 12);
 *   If the token represents a negative number ('-' at the beginning) and the output type is unsigned, the
 *   function returns true generating a warning and the number is set to 0;
 *   number is the result of the conversion from the const char* token;
 */
template<typename T>
static bool StringToIntegerDecimalNotation(const char8* const input,
                                           T &number) {

    bool ret = true;
    bool canReturn = false;

    number = static_cast<T>(0);

    uint32 i = 0u;

    bool isSigned = (static_cast<T>(-1) < static_cast<T>(0));
    bool isNegative = false;

    // in case of - check if the type is signed, otherwise return 0
    if (input[i] == '-') {
        i++;
        if (!isSigned) {
            // take the following number
            //put a warning if is not a -0
            if (input[i] != '0') {
                canReturn = true;
                REPORT_ERROR(ErrorManagement::Warning, "StringToIntegerDecimalNotation: A negative number to unsigned type will be saturated to 0.");
            }
        }
        else {

            // take it as negative
            isNegative = true;
        }
    }
    else {
        // skip the eventual sign
        if (input[i] == '+') {
            i++;
        }
    }

    if (!canReturn) {

        // flag to check if the number is 0x8000...
        bool isMin = false;

        // the maximum before add a digit
        //T maxmax = (~static_cast<T>(0));
        T maxmax = static_cast<T>(-1);

        /*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: the result is always in [0-9] */
        int8 maxLastDigit = static_cast<int8>(maxmax % static_cast<T>(10));

        if (isSigned) {
            maxmax = Shift::LogicalRightSafeShift(maxmax, 1u);
            /*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: the result is always in [0-9] */
            maxLastDigit = static_cast<int8>(maxmax % static_cast<T>(10));
            if (isNegative) {
                maxLastDigit++;
            }
        }

        T max = maxmax / static_cast<T>(10);

        for (; (!canReturn) && (i < 1000u); i++) {
            //get the new digit
            char8 digit = input[i];
            int8 zero = static_cast<int8>('0');
            int8 newDigit = (static_cast<int8>(digit) - zero);

            // break in case of incorrect digit
            if ((newDigit >= 0) && (newDigit <= 9)) {

                // number > max
                if (number > max) {
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerDecimalNotation: Overflow.");
                    canReturn = true;
                    ret = false;
                }
                if (number == max) {

                    if (newDigit <= maxLastDigit) {
                        // you can add one in case of negative number
                        if ((newDigit == maxLastDigit) && (isNegative)) {
                            isMin = true;
                            newDigit--;
                        }
                        number *= static_cast<T>(10);
                        /*lint -e{571} .Justification: Remove the warning: Suspicious cast. */
                        number += static_cast<T>(newDigit);
                        // it will exit at the next loop
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerDecimalNotation: Overflow.");
                        canReturn = true;
                        ret = false;
                    }
                }
                if (number < max) {
                    number *= static_cast<T>(10);
                    /*lint -e{571} .Justification: Removing the warning: Suspicious cast. */
                    number += static_cast<T>(newDigit);
                }
            }
            else {
                if (digit != '\0') {
                    ret = false;
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerDecimalNotation: Invalid token.");
                }
                canReturn = true;
            }
        }

        if ((isNegative) && (isSigned)) {

            /*lint -e{732} -e{501} -e{9134}  [MISRA C++ Rule 5-3-2]. Justification: the type is signed. */
            number = -number;

            // the minimum negative number 0x800...
            if (isMin) {
                number--;
            }
        }
    }

    return ret;
}



/**
 * @brief Converts a 0-terminated C-String token representing an integer in hexadecimal notation to an integer.
 * @param[in] input is the token to be converted.
 * @param[out] number is the conversion result in output.
 * @return true if the token in input represents an integer in hexadecimal notation which fits in the provided output type.
 * A failure can occur in case of overflow or if the token is invalid.
 * @pre
 *   input must be a valid token for integer in decimal notation;
 * @post
 *   If the token contains too many digits for the output type, the conversion will stop returning
 *   false and the number read until the overflow exception. (i.e "0BC" for 8-bit integer will return 0x0B);
 *   If an invalid character (not in [0-F] range) is found, the conversion will stop returning
 *   false and the number read until the exception. (i.e "8ABFMA" will return 0x8ABF);
 *   number is the result of the conversion from the const char* token;
 */
template<typename T>
static bool StringToIntegerExadecimalNotation(const char8* const input,
                                              T &number) {

    bool ret = true;
    bool canReturn = false;

    number = static_cast<T>(0);

    uint32 i = 0u;

    uint8 totalShift = 0u;

    for (; (!canReturn) && (i < 1000u); i++) {
        //get the new digit
        char8 digit = input[i];
        int8 zero = static_cast<int8>('0');
        int8 newDigit = (static_cast<int8>(digit) - zero);

        // break in case of incorrect digit
        if ((newDigit < 0) || (newDigit > 9)) {
            int8 ten = static_cast<int8>('A');
            newDigit = 10 + (static_cast<int8>(digit) - ten);
            if ((newDigit < 10) || (newDigit > 15)) {
                if (digit != '\0') {
                    ret = false;
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerExadecimalNotation: Invalid token.");
                }
                canReturn = true;
            }
        }
        if (!canReturn) {
            totalShift += 4u;
            if (totalShift > (static_cast<uint32>(sizeof(T) * 8u))) {
                REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerExadecimalNotation: Overflow.");
                ret = false;
                canReturn = true;
            }
            else {
                number = Shift::LogicalLeftSafeShift(number, 4u);
                /*lint -e{571} .Justification: Removing the warning: Suspicious cast. */
                number += static_cast<T>(newDigit);
            }
        }
    }

    return ret;

}


/**
 * @brief Converts a 0-terminated C-String token representing an integer in octal notation to an integer.
 * @param[in] input is the token to be converted.
 * @param[out] number is the conversion result in output.
 * @return true if the token in input represents an integer in octal notation which fits in the provided output type.
 * A failure can occur in case of overflow or if the token is invalid.
 * @pre
 *   input must be a valid token for integer in decimal notation;
 * @post
 *   If the token contains too many digits or if the number is too big for the output type, the conversion will stop returning
 *   false and the number read until the overflow exception. (i.e "411" for 8-bit integer will return 0o41);
 *   If an invalid character (not in [0-7] range) is found, the conversion will stop returning
 *   false and the number read until the exception. (i.e "12381" will return 0o123);
 *   number is the result of the conversion from the const char* token;
 */
template<typename T>
static bool StringToIntegerOctalNotation(const char8 * const input,
                                         T &number) {

    bool ret = true;
    bool canReturn = false;

    number = static_cast<T>(0);

    uint32 i = 0u;

    uint8 totalShift = 0u;

    int8 firstDigit = 0;
    for (; (!canReturn) && (i < 1000u); i++) {
        //get the new digit
        char8 digit = input[i];
        int8 zero = static_cast<int8>('0');
        int8 newDigit = (static_cast<int8>(digit) - zero);

        if (i == 0u) {
            firstDigit = newDigit;
        }
        // break in case of incorrect digit
        if ((newDigit < 0) || (newDigit > 7)) {
            if (digit != '\0') {
                ret = false;
                REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerExadecimalNotation: Invalid token.");
            }
            canReturn = true;
        }
        else {

            totalShift += 3u;
            if (totalShift > static_cast<uint32>(sizeof(T) * 8u)) {
                if (totalShift == static_cast<uint32>((sizeof(T) * 8u) + 1u)) {
                    if (firstDigit > 3) {
                        REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerOctalNotation: Overflow.");
                        canReturn = true;
                        ret = false;
                    }
                }
                if (totalShift == static_cast<uint32>((sizeof(T) * 8u) + 2u)) {
                    if (firstDigit > 1) {
                        REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerOctalNotation: Overflow.");
                        canReturn = true;
                        ret = false;
                    }
                }
                if (totalShift > static_cast<uint32>((sizeof(T) * 8u) + 2u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerOctalNotation: Overflow.");
                    canReturn = true;
                    ret = false;
                }
            }
            if (!canReturn) {
                number = Shift::LogicalLeftSafeShift(number, 3u);
                /*lint -e{571} .Justification: Removing the warning: Suspicious cast. */
                number += static_cast<T>(newDigit);
            }
        }
    }

    return ret;
}


/**
 * @brief Converts a 0-terminated C-String token representing an integer in binary notation to an integer.
 * @param[in] input is the token to be converted.
 * @param[out] number is the conversion result in output.
 * @return true if the token in input represents an integer in binary notation which fits in the provided output type.
 * A failure can occur in case of overflow or if the token is invalid.
 * @pre
 *   input must be a valid token for integer in decimal notation;
 * @post
 *   If the token contains too many digits, the conversion will stop returning
 *   false and the number read until the overflow exception. (i.e "100111001" for 8-bit integer will return 0b10011100);
 *   If an invalid character (not in [0-1] range) is found, the conversion will stop returning
 *   false and the number read until the exception. (i.e "10012111" will return 0b1001);
 *   number is the result of the conversion from the const char* token;
 */
template<typename T>
static bool StringToIntegerBinaryNotation(const char8* const input,
                                          T &number) {

    bool ret = true;
    bool canReturn = false;

    number = static_cast<T>(0);

    uint32 i = 0u;

    uint8 totalShift = 0u;

    for (; (!canReturn) && (i < 1000u); i++) {
        //get the new digit
        char8 digit = input[i];
        int8 zero = static_cast<int8>('0');
        int8 newDigit = (static_cast<int8>(digit) - zero);

        // break in case of incorrect digit
        if ((newDigit < 0) || (newDigit > 2)) {
            if (digit != '\0') {
                ret = false;
                REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerExadecimalNotation: Invalid token.");
            }
            canReturn = true;
        }
        else {
            totalShift++;
            if (totalShift > static_cast<uint32>(sizeof(T) * 8u)) {
                REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerBinaryNotation: Overflow.");
                canReturn = true;
                ret = false;
            }
            else {
                number = Shift::LogicalLeftSafeShift(number, 1u);
                /*lint -e{571} .Justification: Removing the warning: Suspicious cast. */
                number += static_cast<T>(newDigit);
            }
        }
    }

    return ret;
}



/**
 * @brief Recognizes the integer notation in the input token and calls the related conversion functions.
 * @param[in] input is the token to be converted.
 * @param[out] number is the conversion result in output.
 * @return false if the token does not represent an integer in the known notations.
 * @pre
 *   input must represent an integer. The print notation is recognized by the header at the beginning;
 *   "" : (no header) Decimal notation;
 *   "0x": Hexadecimal notation;
 *   "0o": Octal notation;
 *   "0b": Binary notation;
 * @post
 *   number is the result of the conversion from the const char* token.
 */
template<typename T>
static bool StringToIntegerPrivate(const char8 * const input,
                                   T &number) {

    number = static_cast<T>(0);
    bool ret = false;

    if (input[0] == '0') {
        switch (input[1]) {
        case ('x'): {
            ret = StringToIntegerExadecimalNotation(&input[2], number);
        }
            break;
        case ('o'): {
            ret = StringToIntegerOctalNotation(&input[2], number);
        }
            break;
        case ('b'): {
            ret = StringToIntegerBinaryNotation(&input[2], number);
        }
            break;
        case ('\0'): {
            ret = true;
        }
            break;
        default: {
            REPORT_ERROR(ErrorManagement::FatalError, "StringToIntegerPrivate: Invalid token.");
            ret = false;
        }
        }
    }
    else {
        ret = StringToIntegerDecimalNotation(input, number);

    }

    return ret;
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     uint8 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     uint16 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     uint32 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     uint64 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     int8 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     int16 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     int32 &number) {
    return StringToIntegerPrivate(input, number);
}

/**
 * @see StringToIntegerPrivate(*).
 */
bool StringToInteger(const char8 * const input,
                     int64 &number) {
    return StringToIntegerPrivate(input, number);
}

}
