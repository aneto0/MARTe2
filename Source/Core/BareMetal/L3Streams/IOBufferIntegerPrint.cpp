/**
 * @file IOBufferIntegerPrint.cpp
 * @brief Source file for class IOBufferIntegerPrint
 * @date 02/10/2015
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
 * the class IOBufferIntegerPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Calculates the order of a number, namely its number of digits
 * minus one.
 * @details The function operates by comparing with 10**N with converging
 * by bisection to the correct value.
 * @param[in] positiveNumber is the number argument which must be positive.
 * @return the number of digits minus one.
 */
/*lint -e{835} [Warning A zero has been given as right argument to operator '*']. Justification: for types with size >= 64 bit the right hand operator will not be zero*/
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
static uint16 GetOrderOfMagnitude(const T positiveNumber) {
    T tenToExponent = static_cast<T>(1);
    uint8 nDigits = 0u;
    // check whether exponent greater than 10
    if (sizeof(T) >= 8u) { // max 19
        /*lint -e{734} */
        T temp = tenToExponent * static_cast<T>(10000000000); // 10 zeros
        if (positiveNumber >= temp) {
            tenToExponent = temp;
            nDigits += 10u;
        }
    }

    // check whether exponent greater than 5
    if (sizeof(T) >= 4u) { // max 9
        /*lint -e{734} */
        T temp = tenToExponent * static_cast<T>(100000); // 5 zeros
        if (positiveNumber >= temp) {
            tenToExponent = temp;
            nDigits += 5u;
        }
    }

    // check whether exponent greater than 2
    if (sizeof(T) >= 2u) { // max 4 zeros
        /*lint -e{734} */
        T temp = tenToExponent * static_cast<T>(100); // 2 zeros
        if (positiveNumber >= temp) {
            tenToExponent = temp;
            nDigits += 2u;
        }
    }

    // check whether exponent greater than 1
    /*lint -e{734} */
    T temp = tenToExponent * static_cast<T>(10); // 1
    if (positiveNumber >= temp) {
        tenToExponent = temp;
        nDigits++;
    }

    // check whether exponent greater than 1
    /*lint -e{734} */
    temp = tenToExponent * static_cast<T>(10);  // 1
    // avoid overflowing in case of signed number
    if (temp > tenToExponent) {
        if (positiveNumber >= temp) {
            nDigits++;
        }
    }
    return nDigits;
}

/**
 * @brief Calculates the number of digits for a hexadecimal representation.
 * @details This function operates comparing the number with 16**N numbers
 * with N=1,2,4,8 converging by bisection to the correct value.
 * @param[in] number is the number argument.
 * @return the minimum number of digits for an hexadecimal print.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
static uint16 GetNumberOfDigitsHexNotation(T number) {
    uint8 nDigits = 1u;

    // negative numbers are 2 complements and have therefore all bits
    if (number >= static_cast<T>(0)) {

        uint8 shift = 0u;
        // check if larger than 2**32
        if (sizeof(T) == 8u) {
            if (number >= static_cast<T>(0x100000000u)) {
                nDigits += 8u;
                shift = 32u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**16
        if (sizeof(T) >= 4u) {
            if (number >= static_cast<T>(0x10000u)) {
                nDigits += 4u;
                shift = 16u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**8
        if (sizeof(T) >= 2u) {
            if (number >= static_cast<T>(0x100u)) {
                nDigits += 2u;
                shift = 8u;
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**4
        if (number >= static_cast<T>(0x10u)) {
            nDigits += 1u;
        }
    }
    else {
        nDigits = static_cast<uint8>(sizeof(T) * 2u);
    }

    return nDigits;
}

/**
 * @brief Calculates the number of digits for an octal representation.
 * @details This function operates comparing the number with 8**N numbers
 * with N=1,2,4,8,16 converging by bisection to the correct value.
 * @param[in] number is the number argument.
 * @return the minimum number of digits for an octal print.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
static uint16 GetNumberOfDigitsOctalNotation(T number) {
    // negative numbers are 2 complements and have therefore all bits
    uint8 nDigits = 1u;

    if (number >= static_cast<T>(0)) {

        uint8 shift = 0u;
        if (sizeof(T) == 8u) {
            if (number >= static_cast<T>(0x1000000000000u)) {
                nDigits += 16u;
                shift = 48u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        if (sizeof(T) >= 4u) {
            if (number >= static_cast<T>(0x1000000u)) {
                nDigits += 8u;
                shift = 24u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**12
        if (sizeof(T) >= 2u) {
            if (number >= static_cast<T>(0x1000)) {
                nDigits += 4u;
                shift = 12u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**6
        if (number >= static_cast<T>(0x40u)) {
            nDigits += 2u;
            shift = 6u;
            number = Shift::LogicalRightSafeShift(number, shift);
        }

        // check if larger than 2**2
        if (number >= static_cast<T>(0x8u)) {
            nDigits += 1u;
        }
    }
    else {
        nDigits = static_cast<uint8>(((sizeof(T) * 8u) + 2u) / 3u);
    }

    return nDigits;
}

/**
 * @brief Calculates the number of digits for a binary representation.
 * @details This function operates comparing the number with 2**N numbers
 * with N=1,2,4,8,16,32 converging by bisection to the correct value.
 * @param[in] number is the number argument.
 * @return the minimum number of digits for a binary print.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
static uint16 GetNumberOfDigitsBinaryNotation(T number) {
    uint8 nDigits = 1u;

    // negative numbers are 2 complements and have therefore all bits
    if (number >= static_cast<T>(0)) {

        uint8 shift = 0u;
        // check if larger than 2**32
        // if so shift
        if (sizeof(T) == 8u) {
            if (number >= static_cast<T>(0x100000000u)) {
                nDigits += 32u;
                shift = 32u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**16
        if (sizeof(T) >= 4u) {
            if (number >= static_cast<T>(0x10000u)) {
                nDigits += 16u;
                shift = 16u; // used a variable to trick the compiler warning not to issue a warning of too long shift
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**8
        if (sizeof(T) >= 2u) {
            if (number >= static_cast<T>(0x100u)) {
                nDigits += 8u;
                shift = 8u;
                number = Shift::LogicalRightSafeShift(number, shift);
            }
        }

        // check if larger than 2**4
        if (number >= static_cast<T>(0x10u)) {
            nDigits += 4u;
            shift = 4u;
            number = Shift::LogicalRightSafeShift(number, shift);
        }

        // check if larger than 2**2
        if (number >= static_cast<T>(0x4u)) {
            nDigits += 2u;
            shift = 2u;
            number = Shift::LogicalRightSafeShift(number, shift);
        }

        // check if larger than 2**1
        if (number >= static_cast<T>(0x2u)) {
            nDigits += 1u;
        }
    }
    else {
        nDigits = static_cast<uint8>(sizeof(T) * 8u);
    }

    return nDigits;
}

/**
 * @brief Prints an integer number on a general buffer in decimal notation.
 * @details This function implements a 2 step conversion - step1 32/64 to
 * 16bit step2 10bit to decimal. This way the number of 32/64 bit operations
 * are reduced. NumberFillLength is used to specify how many digits to prints
 * at least (this would include trailingzeros). It will never prints more
 * trailing zeros than the maximum size of a number of that format. Streamer
 * must have a PutC(char8) method. It will be used to output the digits.
 * @param[out] s is a general buffer class which implements a putC() function.
 * @param[in] positiveNumber is the number to print (it must be positive the
 * '-' is added a part).
 * @param[in] numberFillLength is the minimum number of digits requested for
 * each 16 bit number (<5 because 2**16 has 5 digits) and the function fills
 * the different between it and the minimum necessary space with zeros.
 */
template<typename T>
static inline void Number2StreamDecimalNotationPrivate(IOBuffer &s,
                                                       T positiveNumber,
                                                       int16 numberFillLength = 0) {

    // no negative!
    if (numberFillLength < 0) {
        numberFillLength = 0;
    }

    // 64 bits
    if (sizeof(T) == 8u) {
        // treat 64 bit numbers dividing them into 5 blocks of max 4 digits
        // 16 12 8 4 zeroes
        const int64 tests[4] = { 10000000000000000, 1000000000000, 100000000, 10000 };

        // how many figures are below the current test point
        int16 figures = 16;
        int32 i;
        for (i = 0; i < 4; i++) {
            // enter if a big number or if zero padding required
            if ((positiveNumber > static_cast<T>(tests[i])) || (numberFillLength > figures)) {
                // call this template with 16 bit number
                // otherwise infinite recursion!

                /*lint -e{9125} -e{9123} -e{571} [MISRA C++ Rule 5-0-9] [MISRA C++ Rule 5-0-8]. Justification: The result has always a size < 16 bit and the input number is always positive.*/
                uint16 x = static_cast<uint16>(positiveNumber / static_cast<T>(tests[i]));
                positiveNumber = positiveNumber % static_cast<T>(tests[i]);

                // process the upper part as uint16
                // recurse into this function
                Number2StreamDecimalNotationPrivate(s, x, numberFillLength - figures);

                // print all the blocks in full from now on
                numberFillLength = figures;
            }
            // update
            figures -= 4;
        }
        // call this template with 16 bit number
        // otherwise infinite recursion!
        /*lint -e{9125} -e{571} [MISRA C++ Rule 5-0-9]. Justification: The result has always a size < 16 bit and the input number is always positive.*/
        uint16 x = static_cast<uint16>(positiveNumber);
        // recurse into this function
        Number2StreamDecimalNotationPrivate(s, x, numberFillLength);
    }

    // treat 32 bit numbers dividing them into 3 blocks of max 4 digits
    if (sizeof(T) == 4u) {
        // 8 4 zeroes
        const int32 tests[2] = { 100000000, 10000 };
        // how many figures are below the current test point
        int16 figures = 8;
        int32 i;
        for (i = 0; i < 2; i++) {
            /*lint -e{571} . Removed Warning: Suspicious Cast.*/
            if ((positiveNumber > static_cast<T>(tests[i])) || (numberFillLength > figures)) {
                // call this template with 16 bit number
                // otherwise infinite recursion!
                /*lint -e{9125} -e{9123} -e{571} [MISRA C++ Rule 5-0-9] [MISRA C++ Rule 5-0-8]. Justification: The result has always a size < 16 bit and the input number is always positive.*/
                uint16 x = static_cast<uint16>(positiveNumber / static_cast<T>(tests[i]));
                positiveNumber = positiveNumber % static_cast<T>(tests[i]);

                // process the upper part as uint16
                // recurse into this function
                Number2StreamDecimalNotationPrivate(s, x, numberFillLength - figures);

                // print all the blocks in full from now on
                numberFillLength = figures;
            } // after this 11 max
            figures -= 4;
        }
        // call this template with 16 bit number
        // otherwise infinite recursion!
        /*lint -e{9125} -e{571} [MISRA C++ Rule 5-0-9]. Justification: The result has always a size < 16 bit and the input number is always positive.*/
        uint16 x = static_cast<uint16>(positiveNumber);
        // recurse into this function
        Number2StreamDecimalNotationPrivate(s, x, numberFillLength);
    }

    // 16 bit code
    if (sizeof(T) <= 2u) {
        // sufficient for  a 16 - 8 bit number NO terminator needed
        char8 buffer[5];

        int16 index = static_cast<int16>(sizeof(buffer)) - 1;

        // if not zero extract digits backwards
        do {
            /*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: the result is always in [0-9] */
            int8 digit = static_cast<int8>((positiveNumber) % static_cast<T>(10));
            int8 zero = static_cast<int8>('0');
            positiveNumber = positiveNumber / static_cast<T>(10);

            buffer[index] = static_cast<char8>(zero + digit);
            index--;
        }
        while (positiveNumber > static_cast<T>(0));

        // first fill in all necessary zeros
        int16 i = 0;
        if (numberFillLength > 0) {
            // clamp to 5
            if (numberFillLength > 5) {
                numberFillLength = 5;
            }
            // fill up with zeros
            for (i = (5 - numberFillLength); i <= index; i++) {
                if (!s.PutC('0')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    break;
                }
            }
        }
        // then complete by outputting all digits
        for (i = index + 1; i <= 4; i++) {
            if (!s.PutC(buffer[i])) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferFloatPrint: Failed IOBuffer::PutC()");
                break;
            }
        }
    }
}

/**
 * @brief Prints a string on a generic buffer.
 * @param[out] ioBuffer is a generic ioBuffer class which implements a PutC() function.
 * @param[in] s is the string to be printed.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool PutS(IOBuffer &ioBuffer,
                 const char8 *s) {
    bool ok = true;
    while (ok && (s[0] != '\0')) {
        if (ioBuffer.PutC(s[0])) {
            s = &s[1];
        }
        else {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts any integer type, signed and unsigned to a sequence of
 * characters inserted into the ioBuffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @param[in] actualBitSize is the actual bit size.
 * @return true if the format is correct, false otherwise.
 */
template<typename T>
static bool IntegerToStreamPrivate(IOBuffer &ioBuffer,
                                   const T number,
                                   const FormatDescriptor &format,
                                   uint16 actualBitSize = static_cast<uint16>(sizeof(T) * 8u)) {

    bool ret = false;
// do not use actual Bit Size if binaryPadded is not set
    if (!format.binaryPadded) {
        actualBitSize = 0u;
    }

    if (format.binaryNotation == DecimalNotation) {
        ret = IntegerToStreamDecimalNotation(ioBuffer, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, format.binaryPadded);
    }
    if (format.binaryNotation == HexNotation) {
        ret = IntegerToStreamExadecimalNotation(ioBuffer, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, actualBitSize,
                                                format.fullNotation);
    }
    if (format.binaryNotation == OctalNotation) {
        ret = IntegerToStreamOctalNotation(ioBuffer, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, actualBitSize,
                                           format.fullNotation);
    }
    if (format.binaryNotation == BitNotation) {
        ret = IntegerToStreamBinaryNotation(ioBuffer, number, static_cast<uint16>(format.size), format.padded, format.leftAligned, actualBitSize,
                                            format.fullNotation);
    }

    return ret;
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts any integer type, signed and unsigned to a sequence of
 * characters inserted into the ioBuffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] numberAddress is the integer to print.
 * @param[in] numberBitShift is the shift for the number to print.
 * @param[in] numberBitSize is the desired size in bit for the number to print.
 * @param[in] numberIsSigned specifies if the number to print will be
 * considered signed (true) or not (false).
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
static bool BitSetToStreamPrivate(IOBuffer &ioBuffer,
                                  uint32 *numberAddress,
                                  uint8 numberBitShift,
                                  const uint8 numberBitSize,
                                  const bool numberIsSigned,
                                  const FormatDescriptor &format) {

    bool ret = true;

    // smaller than 16 bit
    if (numberBitSize <= 16u) {

        if (numberBitSize <= 8u) {
            if (numberIsSigned) {
                int8 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }
            }
            else {
                uint8 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }
            }

        }
        else {
            if (numberIsSigned) {
                int16 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }

            }
            else {
                uint16 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }

            }
        }

    }
    else {

        if (numberBitSize <= 32u) {
            if (numberIsSigned) {
                int32 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }
            }
            else {
                uint32 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }
            }

        }
        else {
            if (numberIsSigned) {
                int64 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }
            }
            else {
                uint64 destination;
                ret = BitSetToInteger(destination, numberAddress, numberBitShift, numberBitSize, numberIsSigned);
                if (ret) {
                    ret = IntegerToStreamPrivate(ioBuffer, destination, format, numberBitSize);
                }
            }
        }
    }

    return ret;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Prints an integer number on a general ioBuffer in decimal notation.
 * @details Converts any integer type, signed and unsigned to a sequence of
 * characters inserted into the ioBuffer ioBuffer by mean of a method PutC.
 * Respects maximumSize and number integrity. If not possible (maximum size
 * minor than the minimum space for the number print) outputs is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements a PutC() function.
 * @param[in] number is the number to print.
 * @param[in] maximumSize is the maximum requested space for the number print
 * (0 means that the number is printed in its entirety).
 * @param[in] padded specifies if the difference between maximumSize and the
 * necessary space for the number must be filled by spaces ' '
 * (if maximumSize!=0 & align towards the right or the left).
 * @param[in] leftAligned specifies if the number must be print with left or
 * right alignment (if padded and maximumSize!=0 align towards the left).
 * @return true.
 */
/*lint -e{9143} [MISRA C++ Rule 5-3-2]. Justification: application of sign - is applied only in case of negative number (then signed numbers).*/
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
bool IntegerToStreamDecimalNotation(IOBuffer &ioBuffer,
                                    const T number,
                                    uint16 maximumSize = 0u,
                                    bool padded = false,
                                    const bool leftAligned = false,
                                    const bool binaryPadded = false) {

    bool ret = false;
    char8 padding = (binaryPadded) ? ('0') : (' ');
    
    if (binaryPadded) {
        padded = true;
    }
    
    // if no limits set the numberSize as big enough for the largest integer
    if (maximumSize == 0u) {
        maximumSize = 20u;
        padded = false;
    }

    // put here the unsigned version of the number
    T positiveNumber;
    // put here the total space needed for the number
    // 1 always for the one figure to print
    uint16 numberSize = 1u;

    // if negative invert it and account for the '-' in the size
    if (number < static_cast<T>(0)) {

        /*lint -e{9134} -e{501} -e{732} the number is signed */
        positiveNumber = -number;
        numberSize++;

    }
    else {
        positiveNumber = number;
    }

    bool ok = true;

    // 0x800000....
    if ((number == Shift::LogicalLeftSafeShift(static_cast<T>(1), static_cast<uint8>((sizeof(T) * 8u) - 1u))) && (number < static_cast<T>(0))) {
        if ((sizeof(T) == 8u) && (maximumSize >= 20u)) {
            if (!PutS(ioBuffer, "-9223372036854775808")) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            ret = true;
        }
        if ((sizeof(T) == 4u) && (maximumSize >= 11u)) {
            if (!PutS(ioBuffer, "-2147483648")) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            ret = true;
        }
        if ((sizeof(T) == 2u) && (maximumSize >= 6u)) {
            if (!PutS(ioBuffer, "-32768")) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            ret = true;
        }
        if ((sizeof(T) == 1u) && (maximumSize >= 4u)) {
            if (!PutS(ioBuffer, "-128")) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            ret = true;
        }

        // does not fit
        numberSize = maximumSize + 1u;
    }

    else {

        // add the number of figures beyond the first
        numberSize += GetOrderOfMagnitude(positiveNumber);
    }

    if (!ret) {

        // is there enough space for the number?
        if (maximumSize < numberSize) {
            // if no than we shall print '?' so the size is 1 now
            numberSize = 1u;

            // fill up to from 1 maximumSize with ' '
            if ((padded) && (!leftAligned)) {
                for (uint32 i = 1u; ok && (i < maximumSize); i++) {
                    if (!ioBuffer.PutC(padding)) {
                        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                        ok = false;
                    }
                }
            }

            // put the ?
            if (!ioBuffer.PutC('?')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }

        }
        else { // enough space

            // fill up from numberSize to maximumSize with ' '
            if ((padded) && (!leftAligned)) {
                for (uint32 i = numberSize; ok && (i < maximumSize); i++) {
                    if (!ioBuffer.PutC(padding)) {
                        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                        ok = false;
                    }
                }
            }

            // add sign
            if (number < static_cast<T>(0)) {
                if (!ioBuffer.PutC('-')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }

            // put number
            Number2StreamDecimalNotationPrivate(ioBuffer, positiveNumber);
        }

        // fill up from numberSize to maximumSize with ' '
        if ((padded) && (leftAligned)) {
            for (uint32 i = numberSize; ok && (i < maximumSize); i++) {
                if (!ioBuffer.PutC(' ')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
        }
    }

    return ok;
}

/**
 * @brief Prints an integer number on a general ioBuffer in hexadecimal
 * notation.
 * @details Converts any integer type, signed and unsigned to a sequence of
 * characters inserted into the ioBuffer ioBuffer by mean of a method PutC.
 * Uses hexadecimal notation. Respects maximumSize and number integrity.
 * If not possible (maximum size minor than the minimum space for the number
 * print) output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements a
 * PutC() function.
 * @param[in] number is the number to print.
 * @param[in] maximumSize is the maximum requested space for the number print
 * (0 means that the number is printed in its entirety).
 * @param[in] padded specifies if the difference between maximumSize and the
 * necessary space for the number must be filled by spaces ' '
 * (if maximumSize!=0 & align towards the right or the left).
 * @param leftAligned specifies if the number must be print with left or right
 * alignment (if padded and maximumSize!=0 align towards the left).
 * @param[in] zeroPaddedBitSize specifies the actual number of bits in format.
 * Set to sizeof(T) means to add trailing zeros, any smaller, non zero value
 * means determines how many bits to print (if not 0 is used to determine how
 * many trailing zeroes to print)
 * @param[in] addHeader specifies if we want to add the hex header '0x' before
 * the number (prepend with 0x).
 * @return true.
 */
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
bool IntegerToStreamExadecimalNotation(IOBuffer &ioBuffer,
                                       const T number,
                                       uint16 maximumSize = 0u,
                                       bool padded = false,
                                       const bool leftAligned = false,
                                       const uint16 zeroPaddedBitSize = 0u,
                                       const bool addHeader = false) {
    // put here size of number
    uint16 headerSize = 0u;

    // adding two chars 0x header
    if (addHeader) {
        headerSize = 2u;
    }

    // actual meaningful digits
    uint16 numberOfDigits = GetNumberOfDigitsHexNotation(number);

    // add header for total size if padded
    uint16 numberSize = headerSize + numberOfDigits;

    // 1000 = no limits
    if (maximumSize == 0u) {
        maximumSize = 1000u;
        padded = false;
    }

    // cannot fit the number even without trailing zeroes
    bool ok = true;
    if (maximumSize < numberSize) {
        numberSize = 1u; // just the '?'

        // pad on the left
        if ((padded) && (!leftAligned)) {
            for (uint32 i = 1u; ok && (i < maximumSize); i++) {
                if (!ioBuffer.PutC(' ')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
        }
        // put the ?
        if (!ioBuffer.PutC('?')) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
            ok = false;
        }

    }
    else {
        // work out what it means in terms of hex digits
        uint16 zeroPaddedDigits = (zeroPaddedBitSize + 3u) / 4u;

        //In case of trailing zeros the digits are the maximum possible or equal to maximum size (-2 if there is header)
        if (zeroPaddedDigits > numberOfDigits) {

            // add header for total size if padded
            uint16 fullNumberSize = headerSize + zeroPaddedDigits;

            // check if adding all zeros number will not fit
            if (fullNumberSize > maximumSize) {
                // how much is exceeding?
                uint16 excess = fullNumberSize - maximumSize;
                // number is made to fit the available space
                numberSize = maximumSize;
                // we cannot print all the zeros, remove excess
                numberOfDigits = zeroPaddedDigits - excess;
            }
            else {
                // we will use the full number size
                numberSize = fullNumberSize;
                // we will print all digits
                numberOfDigits = zeroPaddedDigits;
            }
        }

        // in case of left alignment
        if ((padded) && (!leftAligned)) {
            for (uint32 i = numberSize; ok && (i < maximumSize); i++) {
                if (!ioBuffer.PutC(' ')) {
                    ok = false;
                }
            }
        }

        // add header
        if (addHeader) {
            if (!ioBuffer.PutC('0')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            if (!ioBuffer.PutC('x')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }

        // work out how much to shift number to extract most significative hex
        // we use the above calculate number size
        uint8 bits = static_cast<uint8>((numberOfDigits - 1u) * 4u);

        // loop backwards stepping each nibble
        for (uint8 i = bits; ok && (static_cast<int8>(i) >= 0); i -= 4u) {
            //to get the digit, shift the number and by masking select only the 4 LSB bits
            uint8 digit = (static_cast<uint8>(Shift::LogicalRightSafeShift(number, i)) & (0xFu));

            // skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
            //if ((digit != 0) || (putTrailingZeros)){
            //putTrailingZeros = true;
            uint8 zero = static_cast<uint8>('0');
            uint8 ten = static_cast<uint8>('A');

            if (digit < 10u) {
                /*lint -e(9125) -e(9117) */
                if (!ioBuffer.PutC(static_cast<char8>(zero + digit))) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
            else {
                /*lint -e(9125) -e(9117) */
                if (!ioBuffer.PutC(static_cast<char8>((ten + digit) - 10u))) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }

        }
    }

    // in case of right alignment
    if ((padded) && (leftAligned)) {
        for (uint16 i = numberSize; ok && (i < maximumSize); i++) {
            if (!ioBuffer.PutC(' ')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
    }
    return ok;

}

/**
 * @brief Prints an integer number on a general ioBuffer in octal notation.
 * @details Converts any integer type, signed and unsigned to a sequence of
 * characters inserted into the ioBuffer ioBuffer by mean of a method PutC.
 * Uses octal notation. Respects maximumSize and number integrity. If not
 * possible (maximum size minor than the minimum space for the number print)
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the number to print.
 * @param[in] maximumSize is the maximum requested space for the number print
 * (0 means that the number is printed in its entirety).
 * @param[in] padded specifies if the difference between maximumSize and the
 * necessary space for the number must be filled by spaces ' '
 * (if maximumSize!=0 & align towards the right or the left).
 * @param[in] leftAligned specifies if the number must be print with left or
 * right alignment (if padded and maximumSize!=0 align towards the left).
 * @param[in] zeroPaddedBitSize specifies the actual number of bits in format.
 * Set to sizeof(T) means to add trailing zeros, any smaller, non zero value
 * means determines how many bits to print (if not 0 is used to determine how
 * many trailing zeroes to print)
 * @param[in] addHeader specifies if we want to add the oct header '0o' before
 * the number (prepend with 0o).
 * @return true.
 */
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
bool IntegerToStreamOctalNotation(IOBuffer &ioBuffer,
                                  const T number,
                                  uint16 maximumSize = 0u,
                                  bool padded = false,
                                  const bool leftAligned = false,
                                  const uint16 zeroPaddedBitSize = 0u,
                                  bool addHeader = false) {

    // put here size of number
    uint16 headerSize = 0u;

    // adding two chars 0x header
    if (addHeader) {
        headerSize = 2u;
    }

    // actual meaningful digits
    uint16 numberOfDigits = GetNumberOfDigitsOctalNotation(number);

    // add header for total size if padded
    uint16 numberSize = headerSize + numberOfDigits;

    // 1000 = no limits
    if (maximumSize == 0u) {
        maximumSize = 1000u;
        padded = false;
    }
    // cannot fit the number even without trailing zeroes
    bool ok = true;
    if (maximumSize < numberSize) {
        numberSize = 1u; // just the '?'

        // pad on the left
        if ((padded) && (!leftAligned)) {
            for (uint32 i = 1u; ok && (i < maximumSize); i++) {
                if (!ioBuffer.PutC(' ')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
        }
        // put the ?
        if (!ioBuffer.PutC('?')) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
            ok = false;
        }

    }
    else {
        // work out what it means in terms of octal digits
        uint16 zeroPaddedDigits = (zeroPaddedBitSize + 2u) / 3u;

        // if the zero padded number needs to be bigger
        // work out how many zeroes we can actually afford
        if (zeroPaddedDigits > numberOfDigits) {

            // add header for total size if padded
            uint16 fullNumberSize = headerSize + zeroPaddedDigits;

            // check if adding all zeros number will not fit
            if (fullNumberSize > maximumSize) {
                // how much is exceeding?
                uint16 excess = fullNumberSize - maximumSize;
                // number is made to fit the available space
                numberSize = maximumSize;
                // we cannot print all the zeros, remove excess
                numberOfDigits = zeroPaddedDigits - excess;
            }
            else {
                // we will use the full number size
                numberSize = fullNumberSize;
                // we will print all digits
                numberOfDigits = zeroPaddedDigits;
            }
        }

        // in case of left alignment
        if ((padded) && (!leftAligned)) {
            for (uint32 i = numberSize; ok && (i < maximumSize); i++) {
                if (!ioBuffer.PutC(' ')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
        }

        // add header
        if (addHeader) {
            if (!ioBuffer.PutC('0')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            if (!ioBuffer.PutC('o')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }

        // work out how much to shift number to extract most significative hex
        // we use the above calculate number size
        uint8 bits = static_cast<uint8>((numberOfDigits - 1u) * 3u);

        // loop backwards stepping each nibble
        for (uint8 i = bits; ok && (static_cast<int8>(i) >= 0); i -= 3u) {

            //right shift of the number
            uint8 digit = static_cast<uint8>(static_cast<uint8>(Shift::LogicalRightSafeShift(number, i)) & 0x7u);
            uint8 zero = static_cast<uint8>('0');
            /*lint -e(9125) -e(9117) */
            if (!ioBuffer.PutC(static_cast<char8>(zero + digit))) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
    }

// in case of right alignment
    if ((padded) && (leftAligned)) {
        for (uint16 i = numberSize; ok && (i < maximumSize); i++) {
            if (!ioBuffer.PutC(' ')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
    }
    return ok;
}

/**
 * @brief Prints an integer number on a general ioBuffer in binary notation.
 * @details Converts any integer type, signed and unsigned to a sequence of
 * characters inserted into the ioBuffer ioBuffer by mean of a method PutC.
 * Uses binary notation. Respects maximumSize and number integrity. If not
 * possible (maximum size minor than the minimum space for the number print)
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the number to print.
 * @param[in] maximumSize is the maximum requested space for the number print
 * (0 means that the number is printed in its entirety).
 * @param[in] padded specifies if the difference between maximumSize and the
 * necessary space for the number must be filled by spaces ' '
 * (if maximumSize!=0 & align towards the right or the left).
 * @param[in] leftAligned specifies if the number must be print with left or
 * right alignment (if padded and maximumSize!=0 align towards the left).
 * @param[in] zeroPaddedBitSize specifies the actual number of bits in format.
 * Set to sizeof(T) means to add trailing zeros, any smaller, non zero value
 * means determines how many bits to print (if not 0 is used to determine how
 * many trailing zeroes to print).
 * @param[in] addHeader specifies if we want to add the bin header '0b' before
 * the number (prepend with 0b).
 * @return true.
 */
/*lint -e{568} [Warning: non-negative quantity is never less than zero]. Justification: a template could be signed or unsigned.*/
template<typename T>
bool IntegerToStreamBinaryNotation(IOBuffer &ioBuffer,
                                   const T number,
                                   uint16 maximumSize = 0u,
                                   bool padded = false,
                                   const bool leftAligned = false,
                                   const uint16 zeroPaddedBitSize = 0u,
                                   const bool addHeader = false) {

// 1000 = no limits
    if (maximumSize == 0u) {
        maximumSize = 1000u;
        padded = false;
    }

    // put here size of number
    uint16 headerSize = 0u;

    // adding two chars 0x header
    if (addHeader) {
        headerSize = 2u;
    }

    // actual meaningful digits
    uint16 numberOfDigits = GetNumberOfDigitsBinaryNotation(number);

    // add header for total size if padded
    uint16 numberSize = headerSize + numberOfDigits;

    // cannot fit the number even without trailing zeroes
    bool ok = true;
    if (maximumSize < numberSize) {
        numberSize = 1u; // just the '?'

        // pad on the left
        if ((padded) && (!leftAligned)) {
            for (uint32 i = 1u; i < maximumSize; i++) {
                if (!ioBuffer.PutC(' ')) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
        }
        // put the ?
        if (!ioBuffer.PutC('?')) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
            ok = false;
        }

    }
    else {

        // if the zero padded number needs to be bigger
        // work out how many zeroes we can actually afford
        if (zeroPaddedBitSize > numberOfDigits) {

            // add header for total size if padded
            uint16 fullNumberSize = headerSize + zeroPaddedBitSize;

            // check if adding all zeros number will not fit
            if (fullNumberSize > maximumSize) {
                // how much is exceeding?
                uint16 excess = fullNumberSize - maximumSize;
                // number is made to fit the available space
                numberSize = maximumSize;
                // we cannot print all the zeros, remove excess
                numberOfDigits = zeroPaddedBitSize - excess;
            }
            else {
                // we will use the full number size
                numberSize = fullNumberSize;
                // we will print all digits
                numberOfDigits = zeroPaddedBitSize;
            }
        }

        // in case of left alignment
        if ((padded) && (!leftAligned)) {
            for (uint32 i = numberSize; ok && (i < maximumSize); i++) {
                if (!ioBuffer.PutC(' ')) {
                    ok = false;
                }
            }
        }

        // add header
        if (addHeader) {
            if (!ioBuffer.PutC('0')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            if (!ioBuffer.PutC('b')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }

        // work out how much to shift number to extract most significative hex
        // we use the above calculate number size
        uint8 bits = static_cast<uint8>(numberOfDigits - 1u);

        // loop backwards stepping each nibble
        for (uint8 i = bits; ok && (static_cast<int8>(i) >= 0); i--) {
            //to get the digit, shift the number and by masking select only the 4 LSB bits
            uint8 digit = (static_cast<uint8>(Shift::LogicalRightSafeShift(number, i)) & 0x1u);

            uint8 zero = static_cast<uint8>('0');

            // skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
            /*lint -e(9125) -e(9117) */
            if (!ioBuffer.PutC(static_cast<char8>(zero + digit))) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }

        }
    }

    // in case of right alignment
    if (padded && leftAligned) {
        for (uint16 i = 0u; ok && (i < (maximumSize - numberSize)); i++) {
            if (!ioBuffer.PutC(' ')) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "IOBufferIntegerPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
    }
    return ok;
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an uint8 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const uint8 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an int8 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const int8 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an uint16 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const uint16 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an int16 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const int16 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an uint32 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const uint32 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an int32 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const int32 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an uint64 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const uint64 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an int64 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] number is the integer to print.
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool IntegerToStream(IOBuffer &ioBuffer,
                     const int64 number,
                     const FormatDescriptor &format) {
    return IntegerToStreamPrivate(ioBuffer, number, format);
}

/**
 * @brief Print on a general ioBuffer using a specific format.
 * @details Converts an uint8 sequence of characters inserted
 * into the buffer ioBuffer by mean of a method PutC.
 * Uses notation specified in format. Also respects all relevant format
 * parameters. Respects format.size and number integrity. If not possible
 * output is ?
 * @param[out] ioBuffer is a general ioBuffer class which implements
 * a PutC() function.
 * @param[in] numberAddress is the integer to print.
 * @param[in] numberBitShift is the shift for the number to print.
 * @param[in] numberBitSize is the desired size in bit for the number to print.
 * @param[in] numberIsSigned specifies if the number to print will be
 * considered signed (true) or not (false).
 * @param[in] format is the desired format.
 * @return true if the format is correct, false otherwise.
 */
bool BitSetToStream(IOBuffer &ioBuffer,
                    uint32 * const numberAddress,
                    const uint8 numberBitShift,
                    const uint8 numberBitSize,
                    const bool numberIsSigned,
                    const FormatDescriptor &format) {
    return BitSetToStreamPrivate(ioBuffer, numberAddress, numberBitShift, numberBitSize, numberIsSigned, format);

}

}
