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
 * @brief Functions to print integer numbers on generic streams.
*/
#if !defined IOBUFFER_INTEGER_PRINT
#define IOBUFFER_INTEGER_PRINT

#include "GeneralDefinitions.h"
#include "FormatDescriptor.h"
#include "BitSetToInteger.h"
#include "IOBuffer.h"
#include "Shift.h"
#include <math.h>


// returns the exponent
// positiveNumber is the abs (number)
/** @brief Calculates the order of a number, namely its number of digits minus one.
  * @param positiveNumber is the number argument which must be positive.
  * @return the number of digits minus one.
  * 
  * The function operates by comparing with 10**N with converging by bisection to the correct value. */
template <typename T> uint16 GetOrderOfMagnitude(T positiveNumber){       
    T tenToExponent = 1;
    uint8 exp = 0;
    // check whether exponent greater than 10  
    if (sizeof(T)>=8){ // max 19
        T temp = tenToExponent * 10000000000; // 10 zeros 
        if (positiveNumber >= temp )  {
            tenToExponent = temp;
            exp += 10;
        }
    }
    
    // check whether exponent greater than 5  
    if (sizeof(T)>=4){ // max 9 
        T temp = tenToExponent * 100000; // 5 zeros
        if (positiveNumber >= temp ) {
            tenToExponent = temp;
            exp += 5;
        }
    }
    
    // check whether exponent greater than 2  
    if (sizeof(T)>=2){ // max 4 zeros
        T temp = tenToExponent * 100; // 2 zeros
        if (positiveNumber >= temp ) {
            tenToExponent = temp;
            exp += 2;
        }
    }
    
    // check whether exponent greater than 1  
    T temp = tenToExponent * 10; // 1 
    if (positiveNumber >= temp ){
            tenToExponent = temp;
            exp ++;
    }
    
    // check whether exponent greater than 1  
    temp = tenToExponent * 10;  // 1
    // avoid overflowing in case of signed number
    if (temp > tenToExponent){
    	if (positiveNumber >= temp ){
            tenToExponent = temp;
            exp ++;
        }
    }
    return exp;
}

// returns the number of digits necessary to represent this number -1 
/** @brief Calculates the number of digits for a hexadecimal representation.
  * @param number is the number argument.
  * @return the minimum number of digits for an hexadecimal print.
  *
  * This function operates comparing the number with 16**N numbers with N=1,2,4,8 converging by 
  * bisection to the correct value. */ 

template <typename T> uint16 GetNumberOfDigitsHexNotation(T number){
	uint8 exp = 1;
	
	// negative numbers are 2 complements and have therefore all bits
	if (number < 0) return sizeof(T) * 2;

	uint8 shift = 0;
	// check if larger than 2**32
	if (sizeof(T)==8)
		if  (number >= 0x100000000){
			exp += 8;
			shift = 32; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		}

	// check if larger than 2**16
	if (sizeof(T)>=4)
		if  (number >= 0x10000){
			exp += 4;
			shift = 16; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		}

	// check if larger than 2**8
	if (sizeof(T)>=2)
		if  (number >= 0x100){
			exp += 2;
			number >>= 8;
		}

	// check if larger than 2**4
	if  (number >= 0x10){
		exp += 1;
		number >>= 4;
	}

    return exp;
}

// returns the number of digits necessary to represent this number -1 
/** @brief Calculates the number of digits for an octal representation.
  * @param number is the number argument.
  * @return the minimum number of digits for an octal print.
  *
  * This function operates comparing the number with 8**N numbers with N=1,2,4,8,16 converging by 
  * bisection to the correct value. */ 

template <typename T> uint16 GetNumberOfDigitsOctalNotation(T number){
	// negative numbers are 2 complements and have therefore all bits
	if (number < 0) return (sizeof(T) * 8 + 2) / 3;
            

	uint8 shift = 0;
	uint8 exp = 1;
	if (sizeof(T)==8)
		if  (number >= 0x1000000000000){
			exp += 16;
			shift = 48; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		} 

	if (sizeof(T)>=4)
		if  (number >= 0x1000000){
			exp += 8;
			shift = 24; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		}

	// check if larger than 2**12
	if (sizeof(T)>=2)
		if  (number >= 0x1000){
			exp += 4;
			shift = 12; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		}

	// check if larger than 2**6
	if  (number >= 0x40){
		exp += 2;
		number >>= 6;
	}

	// check if larger than 2**2
	if  (number >= 0x8){
		exp += 1;
		number >>= 3;
	}

    return exp;
}


// returns the number of digits necessary to represent this number -1 
/** @brief Calculates the number of digits for a binary representation.
  * @param number is the number argument.
  * @return the minimum number of digits for a binary print.
  *
  * This function operates comparing the number with 2**N numbers with N=1,2,4,8,16,32 converging by 
  * bisection to the correct value. */ 

template <typename T> uint16 GetNumberOfDigitsBinaryNotation(T number){
	// negative numbers are 2 complements and have therefore all bits
	if (number < 0) return sizeof(T) * 8 ;

	uint8 exp = 1;
	uint8 shift = 0;
	// check if larger than 2**32
	// if so shift 
	if (sizeof(T)==8)
		if  (number >= 0x100000000){
			exp += 32;
			shift = 32; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		}

	// check if larger than 2**16
	if (sizeof(T)>=4)
		if  (number >= 0x10000){
			exp += 16;
			shift = 16; // used a variable to trick the compiler warning not to issue a warning of too long shift
			number >>= shift;
		}

	// check if larger than 2**8
	if (sizeof(T)>=2)
		if  (number >= 0x100){
			exp += 8;
			number >>= 8;
		}
	
	// check if larger than 2**4
	if  (number >= 0x10){
		exp += 4;
		number >>= 4;
	}

	// check if larger than 2**2
	if  (number >= 0x4){
		exp += 2;
		number >>= 2;
	}

	// check if larger than 2**1
	if  (number >= 0x2){
		exp += 1;
		number >>= 1;
	}

    return exp;
}

/** @brief Prints an integer number on a general stream in decimal notation.
  * @param s is a general stream class which implements a putC() function.
  * @param positiveNumber is the number to print (it must be positive the '-' is added a part).
  * @param numberFillLength is the minimum number of digits requested for each 16 bit number (<5 because 2**16 has 5 digits) and 
  * the function fillw the different between it and the minimum necessary space with zeros.
  * 
  * This function implements a 2 step conversion - step1 32/64 to 16bit step2 10bit to decimal.
  * This way the number of 32/64 bit operations are reduced.
  * NumberFillLength is used to specify how many digits to prints at least (this would include trailingzeros).
  * It will never prints more trailing zeros than the maximum size of a number of that format.
  * Streamer must have a PutC(char) method. It will be used to output the digits. */
template <typename T> 
static inline void Number2StreamDecimalNotationPrivate(IOBuffer &s, T positiveNumber,int16 numberFillLength=0){

	// no negative!
	if (numberFillLength < 0) numberFillLength=0;

	// 64 bits
	if (sizeof(T)==8){  
		// treat 64 bit numbers dividing them into 5 blocks of max 4 digits
		// 16 12 8 4 zeroes
		const int64 tests[4] = {10000000000000000,1000000000000,100000000,10000};

		// how many figures are below the current test point
		uint16 figures = 16;
		int i;
		for (i=0;i<4;i++){
			// enter if a big number or if zero padding required			
			if ((positiveNumber > (T)tests[i]) || 
			    (numberFillLength > figures))  {
				// call this template with 16 bit number
				// otherwise infinite recursion!
				uint16 x       = positiveNumber / tests[i];
				positiveNumber = positiveNumber % tests[i];
				
				// process the upper part as uint16
				// recurse into this function
				Number2StreamDecimalNotationPrivate(s,x,numberFillLength-figures);
				
				// print all the blocks in full from now on 
				numberFillLength = figures;
			}
			// update
			figures -= 4;
		}
		// call this template with 16 bit number
		// otherwise infinite recursion!
		uint16 x = positiveNumber;
		// recurse into this function
		Number2StreamDecimalNotationPrivate(s,x,numberFillLength);
		return;
	}  

	// treat 32 bit numbers dividing them into 3 blocks of max 4 digits
	if (sizeof(T)==4){  
		// 8 4 zeroes
		const int32 tests[2] = {100000000,10000};
		// how many figures are below the current test point
		int16 figures = 8;
		int i;
		for (i=0;i<2;i++){
			if ((positiveNumber > (T)tests[i]) || 
			    (numberFillLength > figures))  {
				// call this template with 16 bit number
				// otherwise infinite recursion!
				uint16 x       = positiveNumber / tests[i];
				positiveNumber = positiveNumber % tests[i];

				// process the upper part as uint16
				// recurse into this function
				Number2StreamDecimalNotationPrivate(s,x,numberFillLength-figures);

				// print all the blocks in full from now on 
				numberFillLength = figures;
			} // after this 11 max
			figures -= 4;
		}
		// call this template with 16 bit number
		// otherwise infinite recursion!
		uint16 x = positiveNumber;
		// recurse into this function
		Number2StreamDecimalNotationPrivate(s,x,numberFillLength);
		return;
	}

	// 16 bit code 
	if (sizeof(T)<=2){
		// sufficient for  a 16 - 8 bit number NO terminator needed
		char buffer[5]; 

		int index = sizeof(buffer)-1;
	
		// if not zero extract digits backwards
		do {
			uint8 digit    = positiveNumber % 10;
			positiveNumber = positiveNumber / 10;
			buffer[index--] = '0' + digit;
		} while(positiveNumber > 0);
		
		// first fill in all necessary zeros 
		int i = 0;		
		if (numberFillLength > 0){
			// clamp to 5
			if (numberFillLength > 5)numberFillLength = 5;
			// fill up with zeros
			for (i=(5-numberFillLength);i<=index;i++) s.PutC('0');
		}
		// then complete by outputting all digits 
		for (i=index+1;i<=4;i++) s.PutC(buffer[i]);
	}
}


/** @brief Prints a string on a generic stream.
  * @param stream is a generic stream class which implements a PutC() function.
  * @param s is the string to be printed. */
template <class streamer>
static inline void PutS(streamer & stream,const char *s){
	while (*s != 0){
		stream.PutC(*s);
		s++;
	}
}

/** @brief Prints an integer number on a general stream in decimal notation.
  * @param stream is a general stream class which implements a PutC() function.
  * @param maximumSize is the maximum requested space for the number print. 
  * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
  * @param leftAligned specifies if the number must be print with left or right alignment.
  * @param addPositiveSign specifies if we want print the '+' before positive numbers.
  * @return true.
  *
  * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
  * Respects maximumSize and number integrity. 
  * If not possible (maximum size minor than the minimum space for the number print) outputs is ? */
template <typename T> 
bool IntegerToStreamDecimalNotation(
			IOBuffer &		stream,                        // must have a GetC(c) function where c is of a type that can be obtained from chars  
			T 				number,			
			uint16 			maximumSize			= 0,       // 0 means that the number is printed in its entirety
			bool 			padded				= false,   // if maximumSize!=0 & align towards the right or the left
			bool 			leftAligned			= false,   // if padded and maximumSize!=0 align towards the left
			bool 			addPositiveSign		= false)   // prepend with + not just with - for negative numbers
{

	// if no limits set the numberSize as big enough for the largest integer
	if (maximumSize==0){
		maximumSize=20;
		padded=false;
	}
	
	// put here the unsigned version of the number
	T positiveNumber;
	// put here the total space needed for the number
	// 1 always for the one figure to print
	uint16 numberSize = 1;

		// if negative invert it and account for the '-' in the size
	if (number < 0) {
		positiveNumber = -number;
		numberSize++;
		
	} else {
		// if positive copy it and account for the '+' in the size if addPositiveSign set
		positiveNumber = number;
		if (addPositiveSign) numberSize++;
	}

	// 0x800000....
	if (positiveNumber < 0){
		if ((sizeof(T)==8) && (maximumSize >= 20 )){
			PutS(stream,"-9223372036854775808");
			return true;
		}
		if ((sizeof(T)==4) && (maximumSize >= 10 )){
			PutS(stream,"-2147483648");
			return true;
		}
		if ((sizeof(T)==2) && (maximumSize >= 6 )){
			PutS(stream,"-32768");
			return true;
		}
		if ((sizeof(T)==1) && (maximumSize >= 4 )){
			PutS(stream,"-128");
			return true;
		}			
		
		// does not fit
		numberSize = maximumSize+1; 
	} else {
	
		// add the number of figures beyond the first 
		numberSize += GetOrderOfMagnitude(positiveNumber);
	}

    // is there enough space for the number?
	if (maximumSize < numberSize){
		// if no than we shall print '?' so the size is 1 now
		numberSize = 1; 

		// fill up to from 1 maximumSize with ' '
		if (padded && !leftAligned){
			for (int i=1;i < maximumSize;i++) stream.PutC(' ');
		}
		
		// put the ?
		stream.PutC('?');
		
	} else { // enough space
		
		// fill up from numberSize to maximumSize with ' '
		if (padded && !leftAligned){
			for (int i=numberSize;i < maximumSize;i++) stream.PutC(' ');
		}

		// add sign 
		if (number <  0)     stream.PutC('-');
		else 
        if (addPositiveSign) stream.PutC('+');

		// put number
		Number2StreamDecimalNotationPrivate(stream, positiveNumber);
	}
	
	// fill up from numberSize to maximumSize with ' '
	if (padded && leftAligned){
		for (int i=numberSize;i < maximumSize;i++) stream.PutC(' ');
	}
    return true;	
}


/** @brief Prints an integer number on a general stream in hexadecimal notation.
  * @param stream is a general stream class which implements a PutC() function.
  * @param maximumSize is the maximum requested space for the number print. 
  * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
  * @param leftAligned specifies if the number must be print with left or right alignment.
  * @param zeroPaddedBitSize specifies the actual number of bits in format. Set to sizeof(T) means to add trailing zeros, any smaller, non zero value means determines how many bits to print 
  * @param addHeader specifies if we want to add the hex header '0x' before the number.
  * @return true.
  *
  * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
  * Uses hexadecimal notation. 
  * Respects maximumSize and number integrity. 
  * If not possible (maximum size minor than the minimum space for the number print) output is ? */
template <typename T> 
bool IntegerToStreamExadecimalNotation(
			IOBuffer &		stream, 
			T 				number,
			uint16 			maximumSize			= 0,       // 0 means that the number is printed in its entirety
			bool 			padded				= false,   // if maximumSize!=0 & align towards the right or the left
			bool 			leftAligned			= false,   // if padded and maximumSize!=0 align towards the left
		    uint16          zeroPaddedBitSize   = 0,       // if not 0 is used to determine how many trailing zeroes to print 
			bool 			addHeader    		= false)   // prepend with 0x
{       
	// put here size of number
	uint16 headerSize       = 0;

	// adding two chars 0x header
	if (addHeader) headerSize = 2;

	// actual meaningful digits
	uint16 numberOfDigits   = GetNumberOfDigitsHexNotation(number);

	// add header for total size if padded
	uint16 numberSize  = headerSize + numberOfDigits;

	// 1000 = no limits
	if (maximumSize == 0){
		maximumSize = 1000;
		padded=false;
	}

	// cannot fit the number even without trailing zeroes
	if (maximumSize < numberSize){
		numberSize = 1; // just the '?'
		
		// pad on the left
		if (padded && !leftAligned){
			for (int i=1;i < maximumSize;i++) stream.PutC(' ');
		}
		// put the ?
		stream.PutC('?');
		
	} else {
		// work out what it means in terms of hex digits
		uint16 zeroPaddedDigits = (zeroPaddedBitSize +3)/4; 

		//In case of trailing zeros the digits are the maximum possible or equal to maximum size (-2 if there is header)
		if (zeroPaddedDigits > numberOfDigits){

			// add header for total size if padded
			uint16 fullNumberSize  = headerSize + zeroPaddedDigits;

			// check if adding all zeros number will not fit
			if (fullNumberSize > maximumSize){
				// how much is exceeding?
				uint16 excess   = fullNumberSize - maximumSize;
				// number is made to fit the available space
				numberSize     = maximumSize;
				// we cannot print all the zeros, remove excess
				numberOfDigits = zeroPaddedDigits - excess; 
			} else {	
				// we will use the full number size
				numberSize     = fullNumberSize;
				// we will print all digits
				numberOfDigits = zeroPaddedDigits; 
			}
		}

		// in case of left alignment
		if (padded && !leftAligned){
			for (int i=numberSize;i < maximumSize;i++) stream.PutC(' ');
		}

		// add header
		if (addHeader) {
			stream.PutC('0');
			stream.PutC('x');
		}
		
		// work out how much to shift number to extract most significative hex
		// we use the above claculate number size 
		int bits=(numberOfDigits-1)*4;	
	
		// loop backwards stepping each nibble
		for (int i = bits; i>=0; i-=4){
			//to get the digit, shift the number and by masking select only the 4 LSB bits  
			uint8 digit = Shift::LogicalSafeShift(number, -i) & 0xF;			
			
			// skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
//			if ((digit != 0) || (putTrailingZeros)){
//				putTrailingZeros = true;
				if (digit < 10)   stream.PutC('0'+digit);
				else              stream.PutC('A'+digit-10);
//			} 
		}
	}
	
	// in case of right alignment
	if (padded && leftAligned){
		for (int i = numberSize;i < maximumSize;i++) stream.PutC(' ');
	}
    return true;	
	
}



/** @brief Prints an integer number on a general stream in octal notation.
  * @param stream is a general stream class which implements a PutC() function.
  * @param maximumSize is the maximum requested space for the number print. 
  * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
  * @param leftAligned specifies if the number must be print with left or right alignment.
  * @param zeroPaddedBitSize specifies the actual number of bits in format. Set to sizeof(T) means to add trailing zeros, any smaller, non zero value means determines how many bits to print 
  * @param addHeader specifies if we want to add the oct header '0o' before the number.
  * @return true.
  *
  * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
  * Uses octal notation.
  * Respects maximumSize and number integrity.
  * If not possible (maximum size minor than the minimum space for the number print) output is ?  */
template <typename T> 
bool IntegerToStreamOctalNotation(       
	IOBuffer &		stream, 
	T 				number,
	uint16 			maximumSize			= 0,       // 0 means that the number is printed in its entirety
	bool 			padded				= false,   // if maximumSize!=0 & align towards the right or the left
	bool 			leftAligned			= false,   // if padded and maximumSize!=0 align towards the left
//	bool 			putTrailingZeros	= false,   // trailing zeroes are not omitted (unless breaking the maximumSize)
    uint16          zeroPaddedBitSize   = 0,       // if not 0 is used to determine how many trailing zeroes to print 
	bool 			addHeader    		= false)   // prepend with 0o
{
	
	// put here size of number
	uint16 headerSize       = 0;

	// adding two chars 0x header
	if (addHeader) headerSize =2;

	// actual meaningful digits
	uint16 numberOfDigits   = GetNumberOfDigitsOctalNotation(number);

	// add header for total size if padded
	uint16 numberSize  = headerSize + numberOfDigits;

	// 1000 = no limits
	if (maximumSize == 0){
		maximumSize = 1000;
		padded = false;
	}
	// cannot fit the number even without trailing zeroes
	if (maximumSize < numberSize){
		numberSize = 1; // just the '?'
		
		// pad on the left
		if (padded && !leftAligned){
			for (int i=1;i < maximumSize;i++) stream.PutC(' ');
		}
		// put the ?
		stream.PutC('?');
		
	} else {
		// work out what it means in terms of octal digits
		uint16 zeroPaddedDigits = (zeroPaddedBitSize +2)/3; 
		
		// if the zero padded number needs to be bigger
		// work out how many zeroes we can actually afford
		if (zeroPaddedDigits > numberOfDigits){

			// add header for total size if padded
			uint16 fullNumberSize  = headerSize + zeroPaddedDigits;

			// check if adding all zeros number will not fit
			if (fullNumberSize > maximumSize){
				// how much is exceeding?
				uint16 excess   = fullNumberSize - maximumSize;
				// number is made to fit the available space
				numberSize     = maximumSize;
				// we cannot print all the zeros, remove excess
				numberOfDigits = zeroPaddedDigits - excess; 
			} else {	
				// we will use the full number size
				numberSize     = fullNumberSize;
				// we will print all digits
				numberOfDigits = zeroPaddedDigits; 
			}
		}
		
		// in case of left alignment
		if (padded && !leftAligned){
			for (int i=numberSize;i < maximumSize;i++) stream.PutC(' ');
		}

		// add header
		if (addHeader) {
			stream.PutC('0');
			stream.PutC('o');
		}
		
		// work out how much to shift number to extract most significative hex
		// we use the above claculate number size 
		int bits=(numberOfDigits-1)*3;	
	
	
		// loop backwards stepping each nibble
		for (int i = bits; i >= 0; i-= 3){
			
			//right shift of the number
			uint8 digit = Shift::LogicalSafeShift(number,-i) & 0x7;
			
			// skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
//			if ((digit != 0) || (putTrailingZeros)){
//				putTrailingZeros = true;
				stream.PutC('0'+digit);
//			} 
		}	
	}
	
	// in case of right alignment
	if (padded && leftAligned){
		for (int i = numberSize;i < maximumSize;i++) stream.PutC(' ');
	}
    return true;	
}


/** @brief Prints an integer number on a general stream in binary notation.
  * @param stream is a general stream class which implements a PutC() function.
  * @param maximumSize is the maximum requested space for the number print. 
  * @param padded specifies if the difference between maximumSize and the necessary space for the number must be filled by spaces ' '.
  * @param leftAligned specifies if the number must be print with left or right alignment.
  * @param zeroPaddedBitSize specifies the actual number of bits in format. Set to sizeof(T) means to add trailing zeros, any smaller, non zero value means determines how many bits to print 
  * @param addHeader specifies if we want to add the bin header '0b' before the number.
  * @return true.
  *
  * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
  * Uses binary notation. 
  * Respects maximumSize and number integrity. 
  * If not possible (maximum size minor than the minimum space for the number print) output is ?  */
template <typename T> 
bool IntegerToStreamBinaryNotation(
		IOBuffer &		stream, 
		T 				number,
		uint16 			maximumSize			= 0,       // 0 means that the number is printed in its entirety
		bool 			padded				= false,   // if maximumSize!=0 & align towards the right or the left
		bool 			leftAligned			= false,   // if padded and maximumSize!=0 align towards the left
	    uint16          zeroPaddedBitSize   = 0,       // if not 0 is used to determine how many trailing zeroes to print 
 		bool 			addHeader    		= false)   // prepend with 0b
{

	// 1000 = no limits
	if (maximumSize == 0){
		maximumSize = 1000;
		padded = false;
	}	
	
	// put here size of number
	uint16 headerSize       = 0;

	// adding two chars 0x header
	if (addHeader) headerSize = 2;
	
	// actual meaningful digits
	uint16 numberOfDigits = GetNumberOfDigitsBinaryNotation(number);

	// add header for total size if padded
	uint16 numberSize  = headerSize + numberOfDigits;

	// cannot fit the number even without trailing zeroes
	if (maximumSize < numberSize){
		numberSize = 1; // just the '?'
		
		// pad on the left
		if (padded && !leftAligned){
			for (int i=1;i < maximumSize;i++) stream.PutC(' ');
		}
		// put the ?
		stream.PutC('?');
		
	} else {
		
		// if the zero padded number needs to be bigger
		// work out how many zeroes we can actually afford
		if (zeroPaddedBitSize > numberOfDigits){

			// add header for total size if padded
			uint16 fullNumberSize  = headerSize + zeroPaddedBitSize;

			// check if adding all zeros number will not fit
			if (fullNumberSize > maximumSize){
				// how much is exceeding?
				uint16 excess   = fullNumberSize - maximumSize;
				// number is made to fit the available space
				numberSize     = maximumSize;
				// we cannot print all the zeros, remove excess
				numberOfDigits = zeroPaddedBitSize - excess; 
			} else {	
				// we will use the full number size
				numberSize     = fullNumberSize;
				// we will print all digits
				numberOfDigits = zeroPaddedBitSize; 
			}
		}		
		
		// in case of left alignment
		if (padded && !leftAligned){
			for (int i=numberSize;i < maximumSize;i++) stream.PutC(' ');
		}

		// add header
		if (addHeader) {
			stream.PutC('0');
			stream.PutC('b');
		}

		// work out how much to shift number to extract most significative hex
		// we use the above claculate number size 
		int bits = numberOfDigits-1;	

		// loop backwards stepping each nibble
		for (int i = bits; i >= 0; i--){
			//to get the digit, shift the number and by masking select only the 4 LSB bits  
			uint8 digit = Shift::LogicalSafeShift(number,-i) & 0x1;			
			
			// skips trailing zeros until we encounter the first non zero, or if putTrailingZeros was already set
//			if ((digit != 0) || (putTrailingZeros)){
//				putTrailingZeros = true;
				stream.PutC('0'+digit);
//			} 
		}	
	}

// in case of right alignment
	if (padded && leftAligned){
		for (int i=0;i < maximumSize-numberSize;i++) {
			stream.PutC(' ');
		}
	}
    return true;	
}

/** @brief Print on a general stream using a specific format.
  * @param stream is a general stream class which implements a PutC() function.
  * @param number is the integer to print.
  * @param format is the desired format.
  * @return true if the format is correct, false otherwise.
  *
  * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
  * Uses notation specified in format.
  * Also respects all relevant format parameters.
  * Respects format.size and number integrity. 
  * If not possible output is ? */
template <typename T> 
bool IntegerToStream(
		IOBuffer &			stream, 
		T 					number,
		FormatDescriptor	format,
		uint16              actualBitSize=(sizeof(T)*8)){
	
	// do not use actual Bit Size if binaryPadded is not set
	if (!format.binaryPadded) actualBitSize = 0; 
	
	switch (format.binaryNotation){
	case Notation::DecimalNotation:{
		return IntegerToStreamDecimalNotation(stream,number,format.size,format.padded,format.leftAligned,format.fullNotation);
	}break;
	case Notation::HexNotation:{
		return IntegerToStreamExadecimalNotation(stream,number,format.size,format.padded,format.leftAligned,actualBitSize,format.fullNotation); 
	}break;
	case Notation::OctalNotation:{
		return IntegerToStreamOctalNotation(stream,number,format.size,format.padded,format.leftAligned,actualBitSize,format.fullNotation); 
	}break;
	case Notation::BitNotation:{
		return IntegerToStreamBinaryNotation(stream,number,format.size,format.padded,format.leftAligned,actualBitSize,format.fullNotation); 
	}break;
	}
	
	return false;
}

/** @brief Print on a general stream using a specific format.
  * @param stream is a general stream class which implements a PutC() function.
  * @param number is the integer to print.
  * @param format is the desired format.
  * @return true if the format is correct, false otherwise.
  *
  * Converts any integer type, signed and unsigned to a sequence of characters inserted into the stream stream by mean of a method PutC.
  * Uses notation specified in format.
  * Also respects all relevant format parameters.
  * Respects format.size and number integrity.
  * If not possible output is ? */
static  
bool BitSetToStream(
		IOBuffer &			stream, 
		unsigned int *		numberAddress,
		uint8               numberBitShift,
		uint8               numberBitSize,
		bool 				numberIsSigned,
		FormatDescriptor	format){
		
	bool ret = true;
	
	// smaller than 16 bit
	if (numberBitSize <= 16){

		if (numberBitSize <= 8){
			if(numberIsSigned){
				int8 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);
			}
			else{
				uint8 destination;	
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);
			}
			
		} else {
			if(numberIsSigned){
				int16 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);

			}
			else{
				uint16 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);

			}
		}
		
	} else {
		
		if (numberBitSize <= 32){
			if(numberIsSigned){
				int32 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);
			}
			else{
				uint32 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);
			}
			
		} else {
			if(numberIsSigned){
				int64 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);
			}
			else{
				uint64 destination;
				ret = BitSetToInteger(destination,numberAddress,numberBitShift,numberBitSize,numberIsSigned);
				if (ret) return IntegerToStream(stream,destination,format,numberBitSize);
			}
		}		
	}
		
	return ret;
}


#endif	
