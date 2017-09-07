/**
 * @file IOBufferFloatPrint.cpp
 * @brief Source file for class IOBufferFloatPrint
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
 * the class IOBufferFloatPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
//#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "FormatDescriptor.h"
#include "IOBufferPrivate.h"
#include "BitSetToInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -save -e9023 -e9024
 * 9023, 9024  [MISRA C++ Rule 16-3-1] [MISRA C++ Rule 16-3-2]. Justification: Use of operators # and ## required by this implementation. */

#define CHECK_AND_REDUCE(number,step,exponent)\
if (number >= static_cast<T>(1E ## step)){ \
exponent+=step; \
number *= static_cast<T>(1E- ## step); \
}

/*
 #define CHECK_AND_REDUCE_Q(number,step,exponent)\
if (number >= 1E ## step ## Q){ \
exponent+=step; \
number *= 1E- ## step ## Q; \
}
 */

#define CHECK_AND_INCREASE(number,step,exponent)\
if (number <= static_cast<T>(1E- ## step)){ \
exponent-=(step+1); \
number *= static_cast<T>(10E ## step); \
}

/*
 #define CHECK_AND_INCREASE_Q(number,step,exponent)\
if (number <= 1E- ## step ## Q){ \
exponent-=(step+1); \
number *= 10E ## step ## Q; \
}
 */

namespace MARTe {

/**
 * @brief Prints an integer number on a general buffer in decimal notation.
 * @details This function implements a 2 step conversion - step1 32/64 to 16bit
 * step2 10bit to decimal.  This way the number of 32/64 bit operations are
 * reduced. NumberFillLength is used to specify how many digits to prints at
 * least (this would include trailing zeros). It will never prints more
 * trailing zeros than the maximum size of a number of that format. s must
 * have a PutC(char8) method. It will be used to output the digits.
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
                if(!s.PutC('0')){
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    break;
                }
            }
        }
        // then complete by outputting all digits
        for (i = index + 1; i <= 4; i++) {
            if(!s.PutC(buffer[i])){
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                break;
            }
        }
    }
}

/**
 * @brief Normalize numbers in the form x.xxxxx 10**y, calculating the
 * related exponent y.
 * @details Exponent is increased or decreased, not set. Supports numbers
 * up to quad precision.
 * @param[in,out] positiveNumber is the number to normalize.
 * @param[out] exponent is the exponent in return.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
/*lint -e{835} [MISRA C++ Rule 6-2-2]. Justification: False positive only true for types that are not used in the template.*/
template<typename T>
static inline void NormalizeFloatNumberPrivate(T &positiveNumber,
                                               int16 &exponent) {

    // used internally
    if (positiveNumber > static_cast<T>(0.0)) {

        // check and normalize progressively following a logaritmic pattern
        if (positiveNumber >= 1.0) {
            /*      if (sizeof(T) > 8) {
             CHECK_AND_REDUCE_Q(positiveNumber, 2048, exponent)
             CHECK_AND_REDUCE_Q(positiveNumber, 1024, exponent)
             CHECK_AND_REDUCE_Q(positiveNumber, 512, exponent)
             }*/
            if (sizeof(T) > 4u) {
                CHECK_AND_REDUCE(positiveNumber, 256, exponent)
                CHECK_AND_REDUCE(positiveNumber, 128, exponent)
                CHECK_AND_REDUCE(positiveNumber, 64, exponent)
            }
            CHECK_AND_REDUCE(positiveNumber, 32, exponent)
            CHECK_AND_REDUCE(positiveNumber, 16, exponent)
            CHECK_AND_REDUCE(positiveNumber, 8, exponent)
            CHECK_AND_REDUCE(positiveNumber, 4, exponent)
            CHECK_AND_REDUCE(positiveNumber, 2, exponent)
            CHECK_AND_REDUCE(positiveNumber, 1, exponent)
        }
        else {
            /*  if (sizeof(T) > 8) {
             CHECK_AND_INCREASE_Q(positiveNumber, 2048, exponent)
             CHECK_AND_INCREASE_Q(positiveNumber, 1024, exponent)
             CHECK_AND_INCREASE_Q(positiveNumber, 512, exponent)
             }*/
            if (sizeof(T) > 4u) {
                CHECK_AND_INCREASE(positiveNumber, 256, exponent)
                CHECK_AND_INCREASE(positiveNumber, 128, exponent)
                CHECK_AND_INCREASE(positiveNumber, 64, exponent)
            }
            CHECK_AND_INCREASE(positiveNumber, 32, exponent)
            CHECK_AND_INCREASE(positiveNumber, 16, exponent)
            CHECK_AND_INCREASE(positiveNumber, 8, exponent)
            CHECK_AND_INCREASE(positiveNumber, 4, exponent)
            CHECK_AND_INCREASE(positiveNumber, 2, exponent)
            CHECK_AND_INCREASE(positiveNumber, 1, exponent)
            CHECK_AND_INCREASE(positiveNumber, 0, exponent)
        }
    }
}

/**
 * @brief Rapid calculation of 10**x passing the exponent x as argument.
 * @details Rapid calculation of 10 to n both positive and negative. Supports
 * up to quad precision.
 * @param[out] output is 10**exponent.
 * @param[in] exponent is the exponent.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
static inline void FastPowerOf10Private(T &output,
                                        int16 exponent) {
    T radix = static_cast<T>(10.0);
    if (exponent < 0) {
        radix = static_cast<T>(0.1);
        exponent = -exponent;
    }

    // float64 logaritmic approach
    // decompose exponent in sum of powers of 2
    output = static_cast<T>(1.0);
    // limit to range of quad precision (128 bits)
    uint16 mask = 0x2000u;
    // loop through trailing zeroes
    while ((mask > 0u) && (((static_cast<uint16>(exponent)) & mask) == 0u)) {
        mask = Shift::LogicalRightSafeShift(mask, 1u);
    }
    // loop from first one
    while (mask > 0u) {
        // at each step calculates the square of the power so far
        output *= output;
        // if the bit is set then multiply or divide by 10
        if (((static_cast<uint16>(exponent)) & (mask)) != 0u) {
            output *= radix;
        }
        mask = Shift::LogicalRightSafeShift(mask, 1u);
    }
}

/**
 * @brief Rapid determination of size of the exponent.
 * @param[in] exponent is the exponent parameter.
 * @return the number of digits for the exponent notation E+n.
 */
static inline int16 NumberOfDigitsOfExponent(int16 exponent) {

    int16 exponentNumberOfDigits = 0;
    // no exponent!
    if (exponent != 0) {

        // workout the size of exponent
        // the size of exponent is 2+expNDigits
        // sign of exponent is always produced
        exponentNumberOfDigits = 3;    // E+n

        // remove sign
        if (exponent < 0) {
            exponent = -exponent;
        }

        // work out size
        if (exponent < 100) { // below 100 is either 3 or 4
            if (exponent >= 10) {
                exponentNumberOfDigits++;
            }
        }
        else { // above or equal 100 is at least 5
            exponentNumberOfDigits += 2;
            // just add one for each size step above
            if (exponent >= 1000) {
                exponentNumberOfDigits++;
            }
            // just add one for each size step above
            if (exponent >= 10000) {
                exponentNumberOfDigits++;
            }
        }
    }
    return exponentNumberOfDigits;
}

/**
 * @brief Decompose an exponent into a multiple of 3 and a remainder part.
 * @param[in,out] is the exponent parameter, which is modified to be the
 * remainder. The original exponent is the sum of the two parts.
 * @return the exponent immediately precedent to argument multiple of 3.
 */
static inline int16 ExponentToEngineeringPrivate(int16 &exponent) {
    int16 engineeringExponent = 0;
    // if negative we need to bias by 2 so that exp=-1 => eng_exp = -3 and not 0
    if (exponent < 0) {
        engineeringExponent = (exponent - 2) / 3;
    }
    // if positive it is simply exp/3
    else {
        engineeringExponent = exponent / 3;
    }

    // multiply by 3 so that it is the actual exponent
    engineeringExponent *= 3;
    // calculate remainder
    exponent = exponent - engineeringExponent;

    return engineeringExponent;
}

/**
 * @brief Calculate the size necessary for the representation in fixed point
 * with precision as the number of decimal digits.
 * @details Calculates size of fixed numeric representation considering the
 * zeros and the . needed beyond the significative digits. Precision is int16
 * to allow safe subtraction and is updated to fit within maximumSize and
 * converted into relative format (first significative digits of the number).
 * Negative precision means overflow (? print), zero precision means underflow
 * (0 print).
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the number of decimal digits.
 * @param[in] maximumSize is the desired maximum size.
 * @return the necessary space for the print of the number.
 */
static inline int16 NumberOfDigitsFixedNotation(const int16 exponent,
                                                const bool hasSign,
                                                int16 & precision,
                                                const int16 maximumSize) {

    bool returned = false;
    // this will be the output
    int16 fixedNotationSize = 0;

    // in the positive case
    // start by evaluating size of number of minimum resolution
    if (exponent >= 0) {

        // fixed notation for large numbers needs a number of digits = 1+ exponent in this case no . is used
        fixedNotationSize = exponent + 1;

    }
    else { // negative exponent

        // consider space for minimum resolution --> 0;
        fixedNotationSize = 1;

    }

    // these are the decimal digits required beyond the minimum size
    int16 desiredDecimalDigits = precision;

    // consider the sign
    if (hasSign) {
        fixedNotationSize++;
    }

    // how much space left?
    int16 spaceLeft = maximumSize - fixedNotationSize;

    // are we not within limits even when disregarding precision?
    if (spaceLeft < 0) {

        if (exponent >= 0) {

            // 1 for the ?
            fixedNotationSize = 1;

            // -1 means no space!
            precision = -1;

        }
        else {
            // 1 for the 0
            fixedNotationSize = 1;

            // 0  means no zero
            precision = 0;

        }

        // just exit here
        returned = true;
    }

    if (!returned) {
        // do we need more
        if (desiredDecimalDigits > 0) {

            // no space for adding a single digit and the . exit
            if (spaceLeft < 2) {

                if (exponent >= 0) {
                    //precision remains the integer part size
                    precision = exponent + 1;
                }
                else {
                    //precision remains 0 because it means underflow (maxSize < 3 in this case)
                    precision = 0;
                }

                // just exit here
                returned = true;

            }
            else {
                // deduct the space for .
                spaceLeft--;

                // account for . in the space
                fixedNotationSize++;
            }

            if (!returned) {
                // how many digits will I not be able to print?
                int16 excessOfDigits = desiredDecimalDigits - spaceLeft;

                // if any adjust precision and digits
                if (excessOfDigits > 0) {
                    desiredDecimalDigits = spaceLeft;
                    precision -= excessOfDigits;
                }

                // account for the extra digits
                fixedNotationSize += desiredDecimalDigits;
            }
        }

        if (!returned) {
            // convert precision into relative format
            precision += exponent;
            precision++;

            //Negative precision is only possible for negative exponent and not enough space for a single digit
            if (precision < 0) {

                //0 means no zero
                precision = 0;

                //space for 0
                fixedNotationSize = 1;
            }
        }
    }

    return fixedNotationSize;
}

/**
 * @brief Calculates the size necessary for the representation of a number in
 * relative fixed point representation.
 * @details Calculate size of fixed numeric representation considering the
 * zeros and the . needed beyond the significative digits precision is int16
 * to allow safe subtraction and is updated to fit within maximumSize. Negative
 * precision means overflow (? print), zero precision means underflow (0 print).
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the number of the desired first significative
 * digits to print.
 * @param[in] maximumSize is the desired maximum size.
 * @return the space necessary to print a number in fixed point
 * relative notation.
 */
static inline int16 NumberOfDigitsFixedRelativeNotation(const int16 exponent,
                                                        const bool hasSign,
                                                        int16 & precision,
                                                        const int16 maximumSize) {

    bool returned = false;

    // this will be the output
    int16 fixedNotationSize = 0;

    // how many digits to print below zero
    int16 desiredDecimalDigits = 0;

    // in the positive case
    // start by evaluating size of number without decimals
    if (exponent >= 0) {

        // size of integer part
        // this needs to be printed
        int16 integerPartSize = exponent + 1;

        // these are the decimal digits required
        desiredDecimalDigits = precision - integerPartSize;

        // fixed notation for large numbers needs a number of digits = 1+ exponent in this case no . is used
        fixedNotationSize = integerPartSize;
    }
    else { // negative exponent

        // consider space for minimum resolution;
        fixedNotationSize = 2 - exponent;

        // these are the decimal digits required beyond the minimum size
        desiredDecimalDigits = precision - 1;
    }

    // consider the sign
    if (hasSign) {
        fixedNotationSize++;
    }

    // how much space left?
    int16 spaceLeft = maximumSize - fixedNotationSize;

    // are we not within limits even when disregarding precision?
    if (spaceLeft < 0) {

        if (exponent >= 0) {

            // 1 for the ?
            fixedNotationSize = 1;

            // -1 means overflow - number too big to fit
            precision = -1;

        }
        else {
            // 1 for the 0
            fixedNotationSize = 1;

            // 0 means underflow - reduced to 0
            precision = 0;

        }

        // just exit here
        returned = true;
    }

    if (!returned) {
        // do we need more
        if (desiredDecimalDigits > 0) {

            // no space for adding a single digit exit
            if (spaceLeft == 0) {

                // clip all of the precision expressed by decimals
                precision -= desiredDecimalDigits;

                // just exit here
                returned = true;
            }

            if (!returned) {
                // no space for adding a single digit and the . exit
                if (exponent >= 0) {
                    if (spaceLeft == 1) {

                        // clip all of the precision expressed by decimals
                        precision -= desiredDecimalDigits;

                        // just exit here
                        returned = true;

                    }
                    else {
                        // deduct the space for .
                        spaceLeft--;

                        // account for . in the space
                        fixedNotationSize++;
                    }
                }

                if (!returned) {

                    // how many digits will I not be able to print?
                    int16 excessOfDigits = desiredDecimalDigits - spaceLeft;

                    // if any adjust precision and digits
                    if (excessOfDigits > 0) {
                        desiredDecimalDigits = spaceLeft;
                        precision -= excessOfDigits;
                    }

                    // account for the extra digits
                    fixedNotationSize += desiredDecimalDigits;
                }
            }
        }
    }

    return fixedNotationSize;
}

/**
 * @brief Calculates the size necessary for the representation of a number
 * in exponential representation.
 * @details Calculate size of exponential representation considering the zeros
 * and the . needed beyond the significative digits precision is int16 to allow
 * safe subtraction and is updated to fit within maximumSize. Negative precision
 * means overflow (? print).
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the number of the desired first significative digits to print.
 * @param[in] maximumSize is the desired maximum size.
 * @return the space necessary to print a number in exponential notation.
 */
static inline int16 NumberOfDigitsExponentialNotation(const int16 exponent,
                                                      const bool hasSign,
                                                      int16 & precision,
                                                      const int16 maximumSize) {
    // does not fit
    int16 retval = 1;

    if (precision > 0) {

        //exponential notation number size
        int16 exponentNotationSize = 0;

        // include exponent size
        exponentNotationSize += NumberOfDigitsOfExponent(exponent);

        // include mantissa size
        int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(0, hasSign, precision, maximumSize - exponentNotationSize);

        retval = exponentNotationSize + mantissaSize;
    }

    return retval;
}

/**
 * @brief Calculates the size necessary for the representation of a number in engineering representation.
 * @details Calculate size of engineering representation considering the zeros
 * and the . needed beyond the significative digits precision is int16 to allow
 * safe subtraction and is updated to fit within maximumSize. Negative
 * precision means overflow (? print).
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the number of the desired first significative digits to print.
 * @param[in] maximumSize is the desired maximum size.
 * @return the space necessary to print a number in engineering notation.
 */
static inline int16 NumberOfDigitsEngineeringNotation(const int16 exponent,
                                                      const bool hasSign,
                                                      int16 & precision,
                                                      const int16 maximumSize) {

    // does not fit
    int16 retval = 1;
    if (precision > 0) {

        // decompose exponent in two parts
        int16 exponentRemainder = exponent;
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

        int16 engineeringNotationSize = 0;

        // include exponent size
        engineeringNotationSize += NumberOfDigitsOfExponent(engineeringExponent);

        // include mantissa size
        int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision, maximumSize - engineeringNotationSize);

        retval = engineeringNotationSize + mantissaSize;
    }

    return retval;
}

/**
 * @brief Calculates the size necessary for the representation of a number in smart representation.
 * @details Calculate size of smart representation considering the zeros and
 * the . needed beyond the significative digits precision is int16 to allow
 * safe subtraction and is updated to fit within maximumSize. Negative
 * precision means overflow (? print).
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the number of the desired first significative digits to print.
 * @param[in] maximumSize is the desired maximum size.
 * @return the space necessary to print a number in smart notation.
 */
static inline int16 NumberOfDigitsSmartNotation(const int16 exponent,
                                                const bool hasSign,
                                                int16 &precision,
                                                const int16 maximumSize) {

    // does not fit
    int16 retval = 1;
    if (precision > 0) {

        // decompose exponent in two parts
        int16 exponentRemainder = exponent;
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

        int16 smartNotationSize = 0;

        // check if in range for smart replacement of exponent
        if ((engineeringExponent != 0) && (engineeringExponent <= 24) && (engineeringExponent >= -24)) {
            // if so the exponent is simply a letter
            smartNotationSize++;
        }
        else {
            // or the whole E-xyz
            smartNotationSize += NumberOfDigitsOfExponent(engineeringExponent);
        }

        // include mantissa size
        int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision, maximumSize - smartNotationSize);

        retval = smartNotationSize + mantissaSize;
    }

    return retval;
}

/**
 * @brief Calculates the size necessary for the representation of a number in compact representation.
 * @details Calculate size of compact representation considering the zeros and
 * the . needed beyond the significative digits precision is int16 to allow
 * safe subtraction and is updated to fit within maximumSize. Negative
 * precision means overflow (? print).
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the number of the desired first significative digits to print.
 * @param[in] maximumSize is the desired maximum size.
 * @return the space necessary to print a number in compact notation.
 */
static inline int16 NumberOfDigitsCompactNotation(const int16 exponent,
                                                  const bool hasSign,
                                                  int16 & precision,
                                                  const int16 maximumSize) {

    // does not fit
    int16 retval = 1;

    if (precision > 0) {

        // decompose exponent in two parts
        int16 exponentRemainder = exponent;
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

        int16 smartNotationSize = 0;
        int16 mantissaSize = 0;

        // check if in range for smart replacement of exponent
        if ((engineeringExponent <= 24) && (engineeringExponent >= -24)) {
            // if so the exponent is simply a letter
            if (engineeringExponent != 0) {
                smartNotationSize++;
            }

            // include mantissa size
            mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision, maximumSize - smartNotationSize);

        }
        else {
            // or the whole E-xyz
            smartNotationSize += NumberOfDigitsOfExponent(engineeringExponent + exponentRemainder);

            // include mantissa size
            mantissaSize = NumberOfDigitsFixedRelativeNotation(0, hasSign, precision, maximumSize - smartNotationSize);

        }

        retval = smartNotationSize + mantissaSize;

    }

    return retval;
}

/**
 * @brief Calculates the necessary size for the print of the number for each
 * format.
 * @details Given the format the exponent, the sign and the available size
 * calculates number size and achievable precision.
 * @param[in] notation specifies what is the desired notation.
 * @param[in] exponent is the exponent of the number.
 * @param[in] hasSign is true if the number is negative.
 * @param[in,out] precision is the desired first significative numbers to print.
 * @param[in] maximumSize is the desired maximum size.
 */
static inline int16 NumberOfDigitsNotation(const FloatNotation &notation,
                                           const int16 exponent,
                                           const bool hasSign,
                                           int16 & precision,
                                           const int16 maximumSize) {

    int16 numberSize = 0;

    // do round ups
    if (notation == FixedPointNotation) {

        numberSize = NumberOfDigitsFixedNotation(exponent, hasSign, precision, maximumSize);
    }
    if (notation == FixedPointRNotation) {

        numberSize = NumberOfDigitsFixedRelativeNotation(exponent, hasSign, precision, maximumSize);
    }

    if (notation == ExponentNotation) {

        numberSize = NumberOfDigitsExponentialNotation(exponent, hasSign, precision, maximumSize);

    }
    if (notation == EngineeringNotation) {

        numberSize = NumberOfDigitsEngineeringNotation(exponent, hasSign, precision, maximumSize);

    }
    if (notation == SmartNotation) {

        numberSize = NumberOfDigitsSmartNotation(exponent, hasSign, precision, maximumSize);

    }
    if (notation == CompactNotation) {

        numberSize = NumberOfDigitsCompactNotation(exponent, hasSign, precision, maximumSize);

    }

    return numberSize;

}

/**
 * @brief Prints the notation E+/-n on a generic ioBuffer which implements
 * a PutC() function.
 * @param[out] ioBuffer is the generic ioBuffer.
 * @param[in] exponent is the exponent of the number.
 */
static inline void ExponentToStreamPrivate(IOBuffer & ioBuffer,
                                           int16 exponent) {
    // output exponent if exists
    if (exponent != 0) {
        if (ioBuffer.PutC('E')) {
            bool ok = true;
            // print the exponent sign (both)
            // get the absolute value
            if (exponent > 0) {
                if (!ioBuffer.PutC('+')) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
            else {
                exponent = -exponent;
                if (!ioBuffer.PutC('-')) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
            // fast convert to int
            if (ok) {
                Number2StreamDecimalNotationPrivate(ioBuffer, exponent);
            }
        }
    }
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Print the number (without sign and padding) on a generic ioBuffer
 * which implements a PutC() function.
 * @details Converts a couple of positiveNumber-exponent to a string using
 * fixed format. PositiveNumber is not 0 nor NaN nor Inf and is positive,
 * precision should be strictly positive.
 * @param[out] ioBuffer is the generic ioBuffer.
 * @param[in] positiveNumber is the absolute value of the normalized number.
 * @param[in] exponent is the exponent of the number.
 * @param[in] precision is the number of first significative digits to print.
 * @return false only in case of incorrect digits.
 */
template<typename T>
bool FloatToFixedPrivate(IOBuffer & ioBuffer,
                         T positiveNumber,
                         int16 exponent,
                         int16 precision) {

    bool ok = true;

// impossible
// should never be called like this
// better handle it anyway
    if ((positiveNumber < static_cast<T>(0.0)) || (positiveNumber >= static_cast<T>(10.0))) {
        REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: The normalized number must be in [0, 10) range!");
        if (!ioBuffer.PutC('!')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
        }
        ok = false;
    }
    else {

        // numbers below 1.0
        // start with a 0.000 until we reach the first non zero digit
        if (exponent < 0) {
            if (!ioBuffer.PutC('0')) {
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
            if (!ioBuffer.PutC('.')) {
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                ok = false;
            }

            // loop and add zeros

            for (int16 i = 0; ok && (i < -(exponent + 1)); i++) {
                if (!ioBuffer.PutC('0')) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
            // exponent has only the job of marking where to put the '.'
            // here is lowered by 1 to avoid adding a second '.' in the following code
            exponent--;
        }

        // loop to fulfil precision
        // also must reach the end of the integer part thus exponent is checked
        while (ok && ((exponent >= 0) || (precision > 0))) {

            // before outputting the fractional part add a '.'
            if (exponent == -1) {
                if (!ioBuffer.PutC('.')) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }

            // no more significative digits (all below rounding )
            // but still some exponent (fixed format)
            if (precision == 0) {
                if (!ioBuffer.PutC('0')) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
            else {
                // get a digit and shift the number
                int8 digit = static_cast<int8>(positiveNumber);
                positiveNumber -= static_cast<T>(digit);
                positiveNumber *= static_cast<T>(10.0);

                int8 zero = static_cast<int8>('0');
                if (!ioBuffer.PutC(static_cast<char8>(zero + digit))) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }

            // update precision
            if (precision > 0) {
                precision--;
            }

            // update exponent
            exponent--;
        }
    }
    return ok;
}

/**
 * @brief Implements functions to print the number for each format on a
 * generic ioBuffer which implements a PutC() function.
 * @param[in] notation is the desired notation.
 * @param[out] ioBuffer is the generic ioBuffer (any class with a
 * PutC(char8 c) method )
 * @param[in] normalizedNumber is the normalized number.
 * @param[in] exponent is the exponent of the number.
 * @param[in] precision is the number of the first significative
 * digits to print.
 */
template<typename T>
bool FloatToStreamPrivate(const FloatNotation &notation,
                          IOBuffer & ioBuffer,
                          const T normalizedNumber,
                          int16 exponent,
                          const int16 precision) {

    bool ok = false;
// do round ups
    bool isFPNotation = (notation == FixedPointNotation);
    bool isFPRNotation = (notation == FixedPointRNotation);
    if ((isFPNotation) || (isFPRNotation)) {

        // does all the work of conversion but for the sign and special cases
        ok = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);

    }
    if (notation == ExponentNotation) {

        // does all the work of conversion but for the sign and special cases
        ok = FloatToFixedPrivate(ioBuffer, normalizedNumber, 0, precision);

        // writes exponent
        ExponentToStreamPrivate(ioBuffer, exponent);

    }
    if (notation == EngineeringNotation) {

        // partitions the exponent between engineering part and residual
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // does all the work of conversion but for the sign and special cases
        ok = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);

        // writes exponent
        ExponentToStreamPrivate(ioBuffer, engineeringExponent);

    }
    if (notation == SmartNotation) {

        // partitions the exponent between engineering part and residual
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // does all the work of conversion but for the sign and special cases
        ok = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);
        // check if exponent in correct range
        if ((engineeringExponent != 0) && (engineeringExponent <= 24) && (engineeringExponent >= -24)) {
            static const char8 * const symbols = "yzafpnum KMGTPEZY";
            if (!ioBuffer.PutC(symbols[(engineeringExponent / 3) + 8])) {
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
        else {
            // writes exponent
            ExponentToStreamPrivate(ioBuffer, engineeringExponent);
        }

    }
    if (notation == CompactNotation) {

        // partitions the exponent between engineering part and residual
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // check if exponent in correct range
        if ((engineeringExponent <= 24) && (engineeringExponent >= -24)) {

            // does all the work of conversion but for the sign and special cases
            ok = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);

            //Put the symbol only if the engineering exp is different than zero.
            if (engineeringExponent != 0) {
                static const char8 * const symbols = "yzafpnum KMGTPEZY";
                if (!ioBuffer.PutC(symbols[(engineeringExponent / 3) + 8])) {
                    REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                    ok = false;
                }
            }
        }
        else {
            // does all the work of conversion but for the sign and special cases
            ok = FloatToFixedPrivate(ioBuffer, normalizedNumber, 0, precision);

            // writes exponent
            ExponentToStreamPrivate(ioBuffer, engineeringExponent + exponent);
        }

    }
    return ok;
}

/**
 * @brief A list of avaiable display modalities to manage the behaviour of the function.
 */
enum FloatDisplayModes {
    Normal = 0,  // one of the float32 Notations
    ZeroFloat = 11, // 0
    NanFloat = 22, // Nan
    InfPFloat = 33, //+Inf
    InfNFloat = 44, //-Inf
    InsufficientSpaceForFloat = 77, // not enough space
    NoFormat = 99  // undecided yet
};

/**
 * @brief Check if a number is NaN or +/- Inf.
 * @param[in] number is the number to be checked.
 * @param[in] maximumSize is the desired maximum size.
 * @param[out] neededSize returns the size necessary
 * to print NaN or +/- Inf. It is 0 in case of normal number.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
FloatDisplayModes CheckNumber(const T number,
                              const int16 maximumSize,
                              int16 & neededSize) {

// not decided yet
    neededSize = 0;
    FloatDisplayModes ret = NoFormat;

// check for NaN
    if (IsNaN(number)) {
        if (maximumSize < 3) {
            neededSize = 1;
            ret = InsufficientSpaceForFloat;
        }
        else {
            neededSize = 3;
            ret = NanFloat;
        }
    }

// check for Inf
    if (IsInf(number)) {
        if (maximumSize < 4) {
            neededSize = 1;
            ret = InsufficientSpaceForFloat;
        }
        else {
            neededSize = 4;
            if (number < static_cast<T>(0)) {
                ret = InfNFloat;
            }
            else {
                ret = InfPFloat;
            }
        }
    }

// check for zero
    /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: This condition is intended to be true when the float number is precisely equal to zero
     * and not in a small range.*/
    if (number == static_cast<T>(0.0)) {
        neededSize = 1;
        ret = ZeroFloat;
    }

    return ret;
}

/**
 * @brief Implements the rounding of the number looking at the precision.
 * @details It adds 5 to the first digit after the precision digits.
 * @param[in] number is the normalized number to be rounded.
 * @param[in] precision is the desired precision for the round.
 * @return the rounded number.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename T>
T RoundUpNumber(T number,
                const int16 precision) {
// what is the magnitude of the correction
    int16 correctionExponent = -precision;

// to round up add a correction value just below last visible digit
// calculates 5 * 10**correctionExponent
    T correction;
    FastPowerOf10Private(correction, correctionExponent);
    correction *= static_cast<T>(5.0);
    number += correction;

    return number;
}

/**
 * @brief Prints a float (or equivalent) number on a generic ioBuffer
 * which implements a PutC() function.
 * @details In case of incorrect characters a '!' will be printed. If the
 * number cannot fit in the desired maximum size  because the overflow '?'
 * will be printed, '0' in case of underflow. It prints NaN in case of nan
 * (i.e 0/0) or +Inf (i.e 1/0) or -Inf (i.e -1/0).
 * @param[out] ioBuffer is a generic ioBuffer class.
 * @param[in] number is the number to print.
 * @param[in] format specifies the desired format (padding, precision,
 * max size)
 * @return true.
 */
template<typename T>
bool FloatToStreamPrivate(IOBuffer & ioBuffer,
                          const T number,
                          const FormatDescriptor &format) {

// the space to our disposal to print the number
    int16 maximumSize = static_cast<int16>(format.size);
// in case of right alignment
    bool isPadded = format.padded;

// 0 means unlimited
// to keep things symmetric we set to a large number
// 1000 should not constitute a limit for a float32!
    if (maximumSize == 0) {
        maximumSize = 1000;
        isPadded = false;
    }

    int16 formatPrecision = static_cast<int16>(format.precision);

// on precision 0 the max useful precision is chosen
// based on the ieee float32 format number of significative digits
    if (format.precision == defaultPrecision) {
        if (sizeof(T) > 8u) {
            formatPrecision = 34;
        }
        if (sizeof(T) == 8u) {
            formatPrecision = 15;
        }
        if (sizeof(T) < 8u) {
            formatPrecision = 7;
        }

        //Default 6 decimal digits for fixed point notation.
        if (format.floatNotation == FixedPointNotation) {
            formatPrecision = 6;
        }
    }

// number of decimal digits or number of significative digits
    int16 precision = formatPrecision;

// this is the second main objective of the first part
// to find out the size that is needed or if there is no space
    int16 numberSize;

// this will be used everywhere!
    T positiveNumber = number;

// hold the exponent after normalisation
    int16 exponent = 0;

// whether the - needs to be output
    bool hasSign = false;

// adjust sign
    if (number < 0.0) {
        hasSign = true;
        positiveNumber = -number;
    }

// check for special cases where notation is not relevant
// if found them then mode and size are assigned
    FloatDisplayModes chosenMode = CheckNumber(number, maximumSize, numberSize);

// no chosen mode yet try all formats
    if (chosenMode == NoFormat) {

        // normalize number
        NormalizeFloatNumberPrivate(positiveNumber, exponent);

        // work out achievable precision  and number size
        uint8 notation = static_cast<uint8>(format.floatNotation);
        numberSize = NumberOfDigitsNotation(notation, exponent, hasSign, precision, maximumSize);

        //the precision to use after the rounding in case of overflow.
        int16 maxPrecisionAfterRounding = formatPrecision;

        // apply rounding up. Remember that for fix point precision is different.
        bool isFPNotation = format.floatNotation == FixedPointNotation;
        bool isFPRNotation = format.floatNotation == FixedPointRNotation;
        if ((isFPNotation) || (isFPRNotation)) {

            if (precision >= 0) {
                //If the precision is clipped, it will be clipped again because the size could only increase for fix point after round up.
                maxPrecisionAfterRounding = formatPrecision;

                //Round up.
                positiveNumber = RoundUpNumber(positiveNumber, precision);
            }
            //If a fixed point fails, it will fail also after the rounding up, so do nothing.
        }
        else {

            if (precision >= 0) {
                //In case of overflow and precision clip it add a digit.
                maxPrecisionAfterRounding = precision + 1;

                //Round up.
                positiveNumber = RoundUpNumber(positiveNumber, precision);
            }
            else {
                //We enter here only in case of exponential forms.

                // Consider for the approximation precision=1 (i.e 950e3 in format 4.x become 1e6, 949.999 not).
                // (or in exp format 9.5e-10 in 4.x format become 1E-9, 9.4e-10 not).
                maxPrecisionAfterRounding = 2;

                //Round up at the first decimal number.
                positiveNumber = RoundUpNumber(positiveNumber, 1);
            }
        }

        // if we have an overflow recalculate numbersize and precision
        if (positiveNumber >= static_cast<T>(10.0)) {

            //The precision can't be greater than format.precision
            if (maxPrecisionAfterRounding > formatPrecision) {
                maxPrecisionAfterRounding = formatPrecision;
            }

            positiveNumber /= static_cast<T>(10.0);
            exponent++;

            precision = maxPrecisionAfterRounding;

            // work out achievable precision  and number size
            notation = static_cast<uint8>(format.floatNotation);
            numberSize = NumberOfDigitsNotation(notation, exponent, hasSign, precision, maximumSize);
        }

        // assume we can print
        chosenMode = Normal;

        // then check
        // If precision is zero print only 0
        if (precision == 0) {
            numberSize = 1;
            chosenMode = ZeroFloat;
        }

        // then check
        // If precision is negative print only ?
        if (precision < 0) {
            numberSize = 1;
            chosenMode = InsufficientSpaceForFloat;
        }

    }

    bool ok = true;
    bool isLeftAligned = format.leftAligned;
    if ((isPadded) && (!isLeftAligned)) {
        for (int32 i = numberSize; i < maximumSize; i++) {
            if (!ioBuffer.PutC(' ')) {
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
    }

    switch (chosenMode) {
    case Normal: {

        // output sign
        if (hasSign) {
            if (!ioBuffer.PutC('-')) {
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                ok = false;
            }
        }
        uint8 notation = static_cast<uint8>(format.floatNotation);
        if (!FloatToStreamPrivate(notation, ioBuffer, positiveNumber, exponent, precision)) {
            ok = false;
        }
    }
        break;
    case NoFormat:
    case InsufficientSpaceForFloat: {
        if (!ioBuffer.PutC('?')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
    }
        break;
    case NanFloat: {

        if (!ioBuffer.PutC('N')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('a')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('N')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
    }
        break;
    case InfPFloat: {
        if (!ioBuffer.PutC('+')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('I')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('n')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('f')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
    }
        break;
    case InfNFloat: {
        if (!ioBuffer.PutC('-')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('I')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('n')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
        if (!ioBuffer.PutC('f')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
    }
        break;
    case ZeroFloat: {
        if (!ioBuffer.PutC('0')) {
            REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
            ok = false;
        }
    }
        break;
    }

// in case of left alignment
    if ((isPadded) && (isLeftAligned)) {
        for (int32 i = numberSize; i < maximumSize; i++) {
            if (!ioBuffer.PutC(' ')) {
                REPORT_ERROR(ErrorManagement::FatalError,"IOBufferFloatPrint: Failed IOBuffer::PutC()");
                ok = false;
                break;
            }
        }
    }

    return ok;
}

/**
 * @brief Prints a float32 (or equivalent) number on a generic ioBuffer
 * which implements a PutC() function.
 * @details In case of incorrect characters a '!' will be printed. If the
 * number cannot fit in the desired maximum size  because the overflow '?'
 * will be printed, '0' in case of underflow. It prints NaN in case of nan
 * (i.e 0/0) or +Inf (i.e 1/0) or -Inf (i.e -1/0).
 * @param[out] buffer is a generic buffer class.
 * @param[in] number is the number to print.
 * @param[in] format specifies the desired format (padding, precision,
 * max size)
 * @return true.
 */
bool FloatToStream(IOBuffer &buffer,
                   const float32 number,
                   const FormatDescriptor &format) {
    return FloatToStreamPrivate(buffer, number, format);
}

/**
 * @brief Prints a float64 (or equivalent) number on a generic ioBuffer
 * which implements a PutC() function.
 * @details In case of incorrect characters a '!' will be printed. If the
 * number cannot fit in the desired maximum size  because the overflow '?'
 * will be printed, '0' in case of underflow. It prints NaN in case of nan
 * (i.e 0/0) or +Inf (i.e 1/0) or -Inf (i.e -1/0).
 * @param[out] buffer is a generic buffer class.
 * @param[in] number is the number to print.
 * @param[in] format specifies the desired format (padding, precision,
 * max size)
 * @return true.
 */
bool FloatToStream(IOBuffer &buffer,
                   const float64 number,
                   const FormatDescriptor &format) {
    return FloatToStreamPrivate(buffer, number, format);
}

}
