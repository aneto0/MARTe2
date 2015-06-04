/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence for the specific language governing 
 permissions and limitations under the Licence. 
 *
 * $Id: CStream.cpp 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/**
 * @file 
 * @brief Functions to print float numbers on generic streams.
*/

#if !defined IOBUFFER_FLOAT_PRINT
#define IOBUFFER_FLOAT_PRINT

#include "GeneralDefinitions.h"
#include "IOBufferIntegerPrint.h"
#include "FormatDescriptor.h"
#include "IOBuffer.h"
#include <math.h>

#define CHECK_AND_REDUCE(number,step,exponent)\
if (number >= 1E ## step){ \
	exponent+=step; \
	number *= 1E- ## step; \
} 

#define CHECK_AND_REDUCE_Q(number,step,exponent)\
if (number >= 1E ## step ## Q){ \
	exponent+=step; \
	number *= 1E- ## step ## Q; \
} 

#define CHECK_AND_INCREASE(number,step,exponent)\
if (number <= 1E- ## step){ \
	exponent-=(step+1); \
	number *= 10E ## step; \
} 

#define CHECK_AND_INCREASE_Q(number,step,exponent)\
if (number <= 1E- ## step ## Q){ \
	exponent-=(step+1); \
	number *= 10E ## step ## Q; \
} 

/** @brief Normalize numbers in the form x.xxxxx 10**y, calculating the related exponent y.
  * @param positiveNumber is the number to normalize.
  * @param is the exponent in return.
  *
  * Exponent is increased or decreased, not set.
  * Supports numbers up to quad precision. */
template<typename T>
static inline void NormalizeFloatNumberPrivate(
		T &positiveNumber,
        int16 &exponent) {
	
    // used internally 
    if (positiveNumber <= 0.0)
        return;

    // check and normalize progressively following a logaritmic pattern
    if (positiveNumber >= 1.0) {
        if (sizeof(T) > 8) {
            CHECK_AND_REDUCE_Q(positiveNumber, 2048, exponent)
            CHECK_AND_REDUCE_Q(positiveNumber, 1024, exponent)
            CHECK_AND_REDUCE_Q(positiveNumber, 512, exponent)
        }
        if (sizeof(T) > 4) {
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
        if (sizeof(T) > 8) {
            CHECK_AND_INCREASE_Q(positiveNumber, 2048, exponent)
            CHECK_AND_INCREASE_Q(positiveNumber, 1024, exponent)
            CHECK_AND_INCREASE_Q(positiveNumber, 512, exponent)
        }
        if (sizeof(T) > 4) {
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


/** @brief Rapid calculation of 10**x passing the exponent x as argument.
  * @param output is 10**exponent.
  * @param exponent is the exponent.
  * 
  * Rapid calculation of 10 to n both positive and negative.
  * Supports up to quad precision. */
template<typename T>
static inline void FastPowerOf10Private(T &output, int16 exponent) {
    T radix = 10.0;
    if (exponent < 0) {
        radix = 0.1;
        exponent = -exponent;
    }

    // double logaritmic approach
    // decompose exponent in sum of powers of 2	
    output = 1.0;
    // limit to range of quad precision (128 bits)
    uint16 mask = 0x2000;
    // loop through trailing zeroes
    while ((mask > 0) && (!(exponent & mask)))
        mask >>= 1;
    // loop from first one
    while (mask > 0) {
        // at each step calculates the square of the power so far
        output *= output;
        // if the bit is set then multiply or divide by 10 
        if (exponent & mask)
            output *= radix;
        mask >>= 1;
    }
}

/** @brief Rapid determination of size of the exponent.
  * @param exponent is the exponent parameter.
  * @return the number of digits for the exponent notation E+n. */
static inline int16 NumberOfDigitsOfExponent(int16 exponent) {
    // no exponent!
    if (exponent == 0)
        return 0;

    // workout the size of exponent
    // the size of exponent is 2+expNDigits
    // sign of exponent is always produced
    int16 exponentNumberOfDigits = 3;	// E+n

    // remove sign
    if (exponent < 0)
        exponent = -exponent;

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
    return exponentNumberOfDigits;
}

/** @brief Decompose an exponent into a multiple of 3 and a remainder part.
  * @param is the exponent parameter.
  * @return the the exponent immediately precedent to argument multiple of 3.
  *
  * Exponent is modified to be the remainder.
  * Original exponent is the sum of the two parts. 
  */
static inline int16 ExponentToEngineeringPrivate(int16 &exponent) {
    int16 engineeringExponent = 0;
    // if negative we need to bias by 2 so that exp=-1 => eng_exp = -3 and not 0
    if (exponent < 0)
        engineeringExponent = (exponent - 2) / 3;
    // if positive it is simply exp/3  
    else
        engineeringExponent = exponent / 3;

    // multiply by 3 so that it is the actual exponent
    engineeringExponent *= 3;
    // calculate remainder 
    exponent = exponent - engineeringExponent;

    return engineeringExponent;
}



/** @brief Calculate the size necessary for the rappresentation in fixed point with precision as the number of decimal digits.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the number of decimal digits.
  * @param maximumSize is the desired maximum size.
  * @return the necessary space for the print of the number.
  *
  * Calculates size of fixed numeric representation considering the zeros and the . needed beyond the significative digits. 
  * Precision is int16 to allow safe subtraction and is updated to fit within maximumSize and converted into relative format 
  * (first significative digits of the number). Negative precision means overflow (? print), zero precision means underflow (0 print). */
static inline int16 NumberOfDigitsFixedNotation(
		int16 		exponent, 
		bool 		hasSign,
        int16 & 	precision,
        int16 		maximumSize) {
	
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
        return fixedNotationSize;
    }

    // do we need more
    if (desiredDecimalDigits > 0) {

        // no space for adding a single digit and the . exit
        if (spaceLeft < 2) {

	    if(exponent >=0)
                //precision remains the integer part size
		precision = exponent + 1;
	    else
                //precision remains 0 because it means underflow (maxSize < 3 in this case)
		precision = 0;

            // just exit here
            return fixedNotationSize;

        }
        else {
            // deduct the space for .
            spaceLeft--;

            // account for . in the space
            fixedNotationSize++;
        }

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

    return fixedNotationSize;
}


/** @brief Calculates the size necessary for the representation of a number in relative fixed point representation.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the number of the desired first significative digits to print.
  * @param maxSize is the desired maximum size.
  * @return the space necessary to print a number in fixed point relative notation.
  *
  * Calculate size of fixed numeric representation considering the zeros and the . needed beyond the significative digits 
  * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print),
  * zero precision means underflow (0 print). */ 
static inline int16 NumberOfDigitsFixedRelativeNotation(
		int16 		exponent,
        bool 		hasSign,
        int16 & 	precision,
        int16 		maximumSize) {
	
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
        return fixedNotationSize;
    }

    // do we need more
    if (desiredDecimalDigits > 0) {

        // no space for adding a single digit exit
        if (spaceLeft == 0) {

            // clip all of the precision expressed by decimals
            precision -= desiredDecimalDigits;

            // just exit here
            return fixedNotationSize;
        }

        // no space for adding a single digit and the . exit
        if (exponent >= 0) {
            if (spaceLeft == 1) {

                // clip all of the precision expressed by decimals
                precision -= desiredDecimalDigits;

                // just exit here
                return fixedNotationSize;

            }
            else {
                // deduct the space for .
                spaceLeft--;

                // account for . in the space
                fixedNotationSize++;
            }
        }

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

    return fixedNotationSize;
}



/** @brief Calculates the size necessary for the representation of a number in exponential representation.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the number of the desired first significative digits to print.
  * @param maxSize is the desired maximum size.
  * @return the space necessary to print a number in exponential notation.
  *
  * Calculate size of exponential representation considering the zeros and the . needed beyond the significative digits 
  * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsExponentialNotation(
		int16 		exponent,
        bool 		hasSign,
        int16 &		precision,
        int16 		maximumSize) {
	
    //	exponential notation number size
    int16 exponentNotationSize = 0;

    // include exponent size    
    exponentNotationSize += NumberOfDigitsOfExponent(exponent);

    // include mantissa size 
    int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(0, hasSign, precision, maximumSize - exponentNotationSize);

    // does not fit
    if (precision <= 0) {
        return 1;
    }

    return exponentNotationSize + mantissaSize;
}

/** @brief Calculates the size necessary for the representation of a number in engineering representation.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the number of the desired first significative digits to print.
  * @param maxSize is the desired maximum size.
  * @return the space necessary to print a number in engineering notation.
  *
  * Calculate size of engineering representation considering the zeros and the . needed beyond the significative digits 
  * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsEngineeringNotation(
		int16 		exponent,
        bool 		hasSign,
        int16 &		precision,
        int16 		maximumSize) {

    // decompose exponent in two parts 
    int16 exponentRemainder = exponent;
    int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

    int16 engineeringNotationSize = 0;

    // include exponent size 
    engineeringNotationSize += NumberOfDigitsOfExponent(engineeringExponent);

    // include mantissa size 
    int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(
            exponentRemainder, hasSign, precision,
            maximumSize - engineeringNotationSize);

    // does not fit
    if (precision <= 0) {
        return 1;
    }

    return engineeringNotationSize + mantissaSize;
}

/** @brief Calculates the size necessary for the representation of a number in smart representation.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the number of the desired first significative digits to print.
  * @param maxSize is the desired maximum size.
  * @return the space necessary to print a number in smart notation.
  *
  * Calculate size of smart representation considering the zeros and the . needed beyond the significative digits 
  * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsSmartNotation(int16 exponent, bool hasSign,
                                                int16 &precision,
                                                int16 maximumSize) {

    // decompose exponent in two parts 
    int16 exponentRemainder = exponent;
    int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

    int16 smartNotationSize = 0;

    // check if in range for smart replacement of exponent
    if ((engineeringExponent != 0) && (engineeringExponent <= 24)
            && (engineeringExponent >= -24)) {
        // if so the exponent is simply a letter
        smartNotationSize++;
    }
    else {
        // or the whole E-xxx
        smartNotationSize += NumberOfDigitsOfExponent(engineeringExponent);
    }

    // include mantissa size 
    int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision,maximumSize - smartNotationSize);

    // does not fit
    if (precision <= 0) {
        return 1;
    }

    return smartNotationSize + mantissaSize;
}

/** @brief Calculates the size necessary for the representation of a number in compact representation.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the number of the desired first significative digits to print.
  * @param maxSize is the desired maximum size.
  * @return the space necessary to print a number in compact notation.
  *
  * Calculate size of compact representation considering the zeros and the . needed beyond the significative digits 
  * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsCompactNotation(
		int16 		exponent, 
		bool 		hasSign,
		int16 &		precision,
        int16 		maximumSize) 
{

    // decompose exponent in two parts 
    int16 exponentRemainder = exponent;
    int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

    int16 smartNotationSize = 0;
    int16 mantissaSize = 0;    

    // check if in range for smart replacement of exponent
    if ((engineeringExponent <= 24) && (engineeringExponent >= -24)) {
        // if so the exponent is simply a letter
	if(engineeringExponent != 0)
            smartNotationSize++;

        // include mantissa size 
        mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision,maximumSize - smartNotationSize);
    
    }
    else {
        // or the whole E-xxx
    	smartNotationSize += NumberOfDigitsOfExponent(engineeringExponent+exponentRemainder);

        // include mantissa size 
        mantissaSize = NumberOfDigitsFixedRelativeNotation(0, hasSign, precision,maximumSize - smartNotationSize);
    
    }

    // does not fit
    if (precision <= 0) {
        return 1;
    }

    return smartNotationSize + mantissaSize;
}

/** @brief Calculates the necessary size for the print of the number for each format.
  * @param notation specifies what is the desired notation.
  * @param exponent is the exponent of the number.
  * @param hasSign is true if the number is negative.
  * @param precision is the desired first significative numbers to print.
  * @param maximumSize is the desired maximum size.
  *
  * Given the format the exponent, the sign and the available size
  * calculates number size and achievable precision. */
static inline int16 NumberOfDigitsNotation(
		Notation::Float notation,
		int16 			exponent, 
		bool 			hasSign,
		int16 &			precision,
        int16 			maximumSize) 
{
		
	int16 numberSize;

	// do round ups    
	switch (notation) {
	case Notation::FixedPointNotation: {

		numberSize = NumberOfDigitsFixedNotation(exponent, hasSign,precision,maximumSize);
	}
	break;
	case Notation::FixedPointRNotation: {

	    numberSize = NumberOfDigitsFixedRelativeNotation(exponent, hasSign, precision, maximumSize);
	}
	break;
	case Notation::ExponentNotation: {

		numberSize = NumberOfDigitsExponentialNotation(exponent, hasSign, precision, maximumSize);
		
	}
	break;
	case Notation::EngineeringNotation: {

		numberSize = NumberOfDigitsEngineeringNotation(exponent, hasSign, precision, maximumSize);
		
	}
	break;
	case Notation::SmartNotation: {

		numberSize = NumberOfDigitsSmartNotation(exponent, hasSign, precision, maximumSize);
		
	}
	break;
	case Notation::CompactNotation: {

		numberSize = NumberOfDigitsCompactNotation(exponent, hasSign,precision,maximumSize);

	} 
	break;
	}

	return numberSize;
	
}

/** @brief Print the number (without sign and padding) on a generic stream which implements a PutC() function.
  * @param stream is the generic stream.
  * @param positiveNumber is the absolute value of the normalized number.
  * @param exponent is the exponent of the number.
  * @param precision is the number of first significative digits to print.
  * @return false only in case of incorrect digits.
  *
  * Converts a couple of positiveNumber-exponent to a string using fixed format.
  * PositiveNumber is not 0 nor Nan nor Inf and is positive, precision should be strictly positive. */
template<typename T>
bool FloatToFixedPrivate(
		IOBuffer & 		stream, 
		T 				positiveNumber, 
		int16 			exponent,
		int16 			precision) {

	// impossible
	// should never be called like this
	// better handle it anyway
    if (positiveNumber<0 || positiveNumber>=10){
    	stream.PutC('!');
    	return false;
    }
    
    // numbers below 1.0
    // start with a 0.000 until we reach the first non zero digit
    if (exponent < 0) {

        stream.PutC('0');
        stream.PutC('.');

        // loop and add zeros
        int i;
        for (i = 0; i < -(exponent + 1); i++) {
            stream.PutC('0');
        }
        // exponent has only the job of marking where to put the '.'
        // here is lowered by 1 to avoid adding a second '.' in the following code
        exponent--;
    }

    // loop to fulfil precision 
    // also must reach the end of the integer part thus exponent is checked
    while ((exponent >= 0) || (precision > 0)) {

        // before outputting the fractional part add a '.'
        if (exponent == -1) {
            stream.PutC('.');
        }
        
        // no more significative digits (all below rounding ) 
        // but still some exponent (fixed format)
        if (precision == 0) {
            stream.PutC('0');
        }
        else {
            // get a digit and shift the number
            int16 digit = (int16) (positiveNumber);
            positiveNumber -= digit;
            positiveNumber *= 10.0;

            stream.PutC('0' + digit);
        }

        // update precision 
        if (precision > 0) {
            precision--;
        }

        // update exponent 
        exponent--;
    }
    return true;
}

/** @brief Prints the notation E+/-n on a generic stream which implements a PutC() function.
  * @param stream is the generic stream.
  * @param exponent is the exponent of the number.
  */
static inline void ExponentToStreamPrivate(IOBuffer & stream, int16 exponent) {
    // output exponent if exists
    if (exponent != 0) {
        stream.PutC('E');
        // print the exponent sign (both)
        // get the absolute value
        if (exponent > 0) {
            stream.PutC('+');
        }
        else {
            exponent = -exponent;
            stream.PutC('-');
        }
        // fast convert to int
        Number2StreamDecimalNotationPrivate(stream, exponent);
    }
}

/** @brief Implements functions to print the number for each format on a generic stream which implements a PutC() function.
  * @param notation is the desired notation.
  * @param stream is the generic stream (any class with a PutC(char c) method )
  * @param normalizedNumber is the normalized number.
  * @param exponent is the exponent of the number.
  * @param precision is the number of the first significative digits to print.
  */
template<typename T>
bool FloatToStreamPrivate(
		Notation::Float 	notation,
		IOBuffer & 			stream,   
        T 					normalizedNumber,
        int16 				exponent,
        int16 				precision) {

	// do round ups    
	switch (notation) {
	case Notation::FixedPointRNotation: 
	case Notation::FixedPointNotation: {

        // does all the work of conversion but for the sign and special cases
        return FloatToFixedPrivate(stream, normalizedNumber, exponent, precision);
	}
	break;
	case Notation::ExponentNotation: {

        // does all the work of conversion but for the sign and special cases
        FloatToFixedPrivate(stream, normalizedNumber, 0, precision);

        // writes exponent
        ExponentToStreamPrivate(stream, exponent);
		
	}
	break;
	case Notation::EngineeringNotation: {

        // partitions the exponent between engineering part and residual 
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // does all the work of conversion but for the sign and special cases
        FloatToFixedPrivate(stream, normalizedNumber, exponent, precision);

        // writes exponent
        ExponentToStreamPrivate(stream, engineeringExponent);
		
	}
	break;
	case Notation::SmartNotation: {

        // partitions the exponent between engineering part and residual 
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // does all the work of conversion but for the sign and special cases
        FloatToFixedPrivate(stream, normalizedNumber, exponent, precision);
        // check if exponent in correct range
        if ((engineeringExponent != 0) && (engineeringExponent <= 24) && 
            (engineeringExponent >= -24)) 
        {
            static const char *symbols = "yzafpnum KMGTPEZY";
            stream.PutC(symbols[engineeringExponent / 3 + 8]);
        }
        else 
        {
            // writes exponent
            ExponentToStreamPrivate(stream, engineeringExponent);
        }
		
	}
	break;
	case Notation::CompactNotation: {

        // partitions the exponent between engineering part and residual 
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // check if exponent in correct range
        if ((engineeringExponent <= 24) && (engineeringExponent >= -24)) 
        {
		
 
            // does all the work of conversion but for the sign and special cases
            FloatToFixedPrivate(stream, normalizedNumber, exponent, precision);

            //Put the symbol only if the engineering exp is different than zero.
	    if(engineeringExponent != 0){
        	static const char *symbols = "yzafpnum KMGTPEZY";
                stream.PutC(symbols[engineeringExponent / 3 + 8]);
	    }
        }
        else 
        {
            // does all the work of conversion but for the sign and special cases
            FloatToFixedPrivate(stream, normalizedNumber, 0, precision);

        	// writes exponent
            ExponentToStreamPrivate(stream, engineeringExponent+exponent);
        }

	} 
	break;
	}
    return true;
};

/** @brief A list of avaiable display modalities to manage the behaviour of the function. */
enum FloatDisplayModes {
    Normal  					= 0,  // one of the float Notations
    ZeroFloat 					= 11, // 0 
    NanFloat 					= 22, // Nan
    InfPFloat 					= 33, //+Inf
    InfNFloat 					= 44, //-Inf
    InsufficientSpaceForFloat 	= 77, // not enough space
    NoFormat 					= 99  // undecided yet
};

/** @brief Check if a number is NaN or +/- Inf.
  * @param number is the number to be checked.
  * @param maximumSize is the desired maximum size.
  * @param neededSize returns the size necessary to print NaN or +/- Inf. It is 0 in case of normal number. 
 */
template<typename T>
FloatDisplayModes CheckNumber(
		T 			number, 
		int16 		maximumSize,
		int16 & 	neededSize) {

	// check for NaN
    if (isnan(number)) {
        if (maximumSize < 3) {
            neededSize = 1;
            return InsufficientSpaceForFloat;
        }
        neededSize = 3;
        return NanFloat;
    }

	// check for Inf
    if (isinf(number)) {
        if (maximumSize < 4) {
            neededSize = 1;
            return InsufficientSpaceForFloat;
        }
        neededSize = 4;
        if (number < 0)
            return InfNFloat;
        return InfPFloat;
    }

	// check for zero
    if (number == 0) {
        neededSize = 1;
        return ZeroFloat;
    }

    // not decided yet
    neededSize = 0;
    return NoFormat;
}

//const bool debug = false;

/** @brief Implements the rounding of the number looking at the precision.
  * @param number is the normalized number to be rounded.
  * @param precision is the desired precision for the round. 
  * @return the rounded number.
  *
  * It adds 5 to the first digit after the precision digits. */
template<typename T>
T RoundUpNumber(T number, int16 precision) {
    // what is the magnitude of the correction
    int16 correctionExponent = -precision;

    // to round up add a correction value just below last visible digit
    // calculates 5 * 10**correctionExponent
    T correction;
    FastPowerOf10Private(correction, correctionExponent);
    correction *= 5.0;
    number += correction;

    return number;
}


/** @brief Prints a float (or equivalent) number on a generic stream which implements a PutC() function.
  * @param stream is a generic stream class.
  * @param number is the number to print.
  * @param format specifies the desired format (padding, precision, max size)
  * @return true.
  *
  * In case of incorrect characters a '!' will be printed.
  * If the number cannot fit in the desired maximum size  because the overflow '?' will be printed, '0' in case of underflow.
  * It prints NaN in case of nan (i.e 0/0) or +Inf (i.e 1/0) or -Inf (i.e -1/0). */
template<typename T>
bool FloatToStream(
		IOBuffer & 			stream, // must have a GetC(c) function where c is of a type that can be obtained from chars  
        T 					number, 
        FormatDescriptor 	format) {

    FloatDisplayModes chosenMode = NoFormat;

    // the space to our disposal to print the number
    int16 maximumSize = format.size;
    
    // 0 means unlimited
    // to keep things symmetric we set to a large number
    // 1000 should not constitute a limit for a float!
    if (maximumSize == 0) {
        maximumSize = 1000;
        format.padded = false;
    }

    // on precision 0 the max useful precision is chosen 
    // based on the ieee float format number of significative digits
    if (format.precision == -1) {
        if (sizeof(T) > 8){
        	format.precision = 34;
        }
        if (sizeof(T) == 8){
        	format.precision = 15;
        }
        if (sizeof(T) < 8){
        	format.precision = 7;
        }

        //Default 6 decimal digits for fixed point notation.
        if(format.floatNotation == Notation::FixedPointNotation)
           	format.precision = 6;
	}        
    
    // number of decimal digits or number of significative digits
    int16 precision = format.precision;
    
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
    if (number < 0) {
        hasSign = true;
        positiveNumber = -number;
    }

    // check for special cases where notation is not relevant
    // if found them then mode and size are assigned
    chosenMode = CheckNumber(number, maximumSize, numberSize);

    // no chosen mode yet try all formats
    if (chosenMode == NoFormat) {

        // normalize number
        NormalizeFloatNumberPrivate(positiveNumber, exponent);

//if (debug)printf ("normN = %f exp =%i prec= %i\n",positiveNumber,exponent,precision);

        // work out achievable precision  and number size
    	numberSize = NumberOfDigitsNotation(format.floatNotation,exponent, hasSign, precision, maximumSize);
        
//if (debug)printf ("Nsize = %i prec= %i\n",numberSize,precision);
    	
		//the precision to use after the rounding in case of overflow.
        int16 maxPrecisionAfterRounding = format.precision;
        
        // apply rounding up. Remember that for fix point precision is different.
        if (format.floatNotation == Notation::FixedPointNotation || format.floatNotation == Notation::FixedPointRNotation){
        	
        	if (precision >= 0){
        		//If the precision is clipped, it will be clipped again because the size could only increase for fix point after round up.
        		maxPrecisionAfterRounding = format.precision;

				//Round up.
				positiveNumber = RoundUpNumber(positiveNumber, precision);
        	}
        	//If a fixed point fails, it will fail also after the rounding up, so do nothing.
        }    
        else {
        	if (precision >= 0){
        		//In case of overflow and precision clip it add a digit.
        		maxPrecisionAfterRounding = precision + 1;
                        
        		//Round up.
        		positiveNumber = RoundUpNumber(positiveNumber, precision);
//if (debug)printf ("roundN = %f maxP = %i\n",positiveNumber,maxPrecisionAfterRounding);
        	} else {
        		//We enter here only in case of exponential forms.
        		
        		// Consider for the approximation precision=1 (i.e 950e3 in format 4.x become 1e6, 949.999 not).
                // (or in exp format 9.5e-10 in 4.x format become 1E-9, 9.4e-10 not).
        		maxPrecisionAfterRounding = 2;
			
        		//Round up at the first decimal number.
        		positiveNumber = RoundUpNumber(positiveNumber, 1);
//if (debug)printf ("roundN = %f \n",positiveNumber);
        		
        	}
        }

        // if we have an overflow recalculate numbersize and precision
        if (positiveNumber >= 10) {

            //The precision can't be greater than format.precision
	    if(maxPrecisionAfterRounding > format.precision)
                maxPrecisionAfterRounding = format.precision;
	            
             positiveNumber /= 10;
             exponent++;
       	
             precision = maxPrecisionAfterRounding;

//if (debug)printf ("2nd normN = %f exp =%i prec= %i\n",positiveNumber,exponent,precision);

            // work out achievable precision  and number size
             numberSize = NumberOfDigitsNotation(format.floatNotation,exponent, hasSign, precision, maximumSize);
//if (debug)printf ("2nd Nsize = %i prec= %i\n",numberSize,precision);
            
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

    // in case of right alignment
    if (format.padded && !format.leftAligned) {
        for (int i = numberSize; i < maximumSize; i++)
            stream.PutC(' ');
    }

    switch (chosenMode) {
    case Normal: {

        // output sign
        if (hasSign){
            stream.PutC('-');
        }
    	
    	FloatToStreamPrivate(format.floatNotation,stream,positiveNumber,exponent,precision);
    }
    break;
    case NoFormat:
    case InsufficientSpaceForFloat: {
        stream.PutC('?');
    }
    break;
    case NanFloat: {
        stream.PutC('N');
        stream.PutC('a');
        stream.PutC('N');
    }
        break;
    case InfPFloat: {
        stream.PutC('+');
        stream.PutC('I');
        stream.PutC('n');
        stream.PutC('f');
    }
        break;
    case InfNFloat: {
        stream.PutC('-');
        stream.PutC('I');
        stream.PutC('n');
        stream.PutC('f');
    }
        break;
    case ZeroFloat: {
        stream.PutC('0');
    }
        break;
    }

    // in case of left alignment
    if (format.padded && format.leftAligned) {
        for (int i = numberSize; i < maximumSize; i++)
            stream.PutC(' ');
    }

    return true;
}

#endif	
