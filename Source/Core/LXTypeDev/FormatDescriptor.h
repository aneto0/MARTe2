/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
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
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/
/**
 * @file FormatDescriptor.h
 *
 * A structure which contains all the necessary informations for an element print.
 */
#ifndef FORMAT_DESCRIPTOR_H
#define FORMAT_DESCRIPTOR_H

#include "GeneralDefinitions.h"
/** 
    Used to choose the float and binary representation modes
*/
struct Notation{
    enum Float{
        /** 
          0.99  3.5 35000.2 etc...
        */
        FixedPointNotation  =   0,

        /** 
          0.99  3.5 35000.2 etc... but number of significative digits = precision
          the others are all zeroes 
        */
        FixedPointRNotation =   1,

        /** 
          9.9E-1 3.5E0 3.5E4 etc....
        */
        ExponentNotation    =   2,

        /** 
          990E-3 3.5E0 35E3 etc....
        */
        EngineeringNotation =   3,

        /** 
          Most meaningful notation fitting within constraints
          Engineering notation with size symbols
          3000 -> 3K  3.6E12  3.6T
        */
        SmartNotation       =   6,
        /** 
          Most meaningful notation fitting within constraints
          Choice among FixedPointRel, Exponent Notation and Smart notation 
        */
        CompactNotation     =   7
    };

    enum Binary {

        /** 
          print in the native format (integer base 10, float as ieee or whatever ....)
        */
        DecimalNotation     =   0,

        /** 
          print in hexadecimal -> the data pointed to by the pointer is treated as a char *
          each character is printed as two hexes (0-F)
        */
        HexNotation         =   1,

        /** 
          print in binary -> the data pointed to by the pointer is treated as a char *
          each character is printed as 8 bits (0-1)
        */
        BitNotation         =   2,

        /** 
          print in octal -> the data pointed to by the pointer is treated as a uint24 *
          each character is printed as three octals (0-7)
        */
        OctalNotation       =   3
    };
};

class FormatDescriptor;

/** @see FormatDescriptor::InitialiseFromString */
extern "C"  bool FDInitialiseFromString(FormatDescriptor &fd,const char *&string);



/**
 * @brief FormatDescriptor class.
 * 
 * This class contains a structure to store the informations the print of each type.
 * It defines also functions due to fill the structure getting the information from
 * a printf like constant char* string which specifies the desired print format.
 *
 * It is the basic brick for all Printf functions. */
class FormatDescriptor{
public:
//*** MAYBE REPLACE with finite set of options ( *' ' *0  *' '0x *' ', ....)
	
    /** maximum size of representation  max 255
        0 means unlimited
    */
    unsigned int            size:8;

    /**
        minimum (whenever applicable) number of meaningful digits (unless overridden by width)  max 64 
		differently from printf this includes characters before comma
		excludes characters used for the exponents or for the sign and the .
		0.34 has precision 2        -> (precision =8)  0.3400000 
		234 has precision 3         -> (precision =8)  234.00000 
		2345678000 has precision 10 -> (precision =8) unchanged still precision 10
		2.345678E9 has precision 7  -> (precision =8) 2.3456780E9 
		234 (int) has precision 3   -> (precision =8) unchanged  still precision 3  
		0x4ABCD has precision 5     -> (precision =8) unchanged  still precision 5  
		1-64 
	*/	 
    int 					precision:8;

    /**
		True means produce a number of characters equal to width 
		fill up using spaces 
	*/
    bool                    padded:1;

    /** 
		True means to produce pad spaces after printing the object representation
    */
    bool                    leftAligned:1; 

    
    /// in case of a float, this field is used to determine how to print it
    Notation::Float         floatNotation:3;

    /// used for ints, floats, pointers, char * etc...
    Notation::Binary        binaryNotation:2;

	/** 
		only meaningful for numbers in Hex/octal/binary
		Fills the number on the left with 0s up to the full representation 
		Number of 0s depends on the size of the number (hex 64 bit ==> numbers+trailing zero = 16)
	*/
    bool                    binaryPadded:1;

    /** 
		only meaningful for numbers
		Add the missing + or 0x 0B or 0o
	*/
    bool                    fullNotation:1;

    //       
    int                     spareBits:7;
    
	/** 
         * @brief Converts a const char* string to a FormatDescriptor structure.
         * @param string is the string which contains a printf like format.
         * @return false if string is null or empty.
  	 *
	   Takes a printf like string already pointing at the character after % (see below format)
	    and parses it recovering all the useful information, discarding all redundant ones,
		and fills up the fields in this structure.
	    At the end the pointer string is moved to the next character after the parsed block
		
		The overall printf-like format supported is the following:
		%[flags][width][.precision]type
		Note that the full printf would be this:
		%[parameter][flags][width][.precision][length]type
		!which is not supported!
			
		[flags]: // slightly different from standard printf notation
		' ' Activates padding:
			fills up to width using spaces
		-  	Left-align : put padding spaces after printing the object
		#   Activate fullNotation:
			+ in front of integers
			0x/0b/0o in front of Hex/octal/binary
        0  	Prepends zeros for Hex Octal and Binary notations (binaryPadded activated)
			Number of zeros depends on number precision and chosen notation (64 bit int and binary notation = up to 64 zeros)
		
		[width][.precision]  two numbers 
		[width] specifies the MAXIMUM or EXACT number of characters to output, depending on the padding [flags] being set on or not 
		NOTE that in a normal printf this would be the MINIMUM or EXACT number of characters... 
        [Precision] 
		This is the minimum number of meaningful digits used to represent a number 
		Differently from printf this includes numbers before .
		if the exact representation of the number uses less digits [precision] is not considered
		if [width] is such that a numeric representation with the given precision cannot be fully represented than the number is replaced with a ?
		type
		This is one character among the following
		
		d,i,u,s,c --> no effect (format depends on actual data type not the letter here!)
        f --> fixed point numeric format selected - absolute precision mode : precision is the number of digits below 1
        F --> fixed point relative precision selected 
        e --> exponential format
        E --> engineering format
		g --> smart format   - like E but replaces E-12 to E+12 with one of the following letters "TGMK munp" 
		G --> compact format 
		a,x,p --> activate exadecimal display (p activates full notation: header+trailing zeros)
        o --> activate octal display
        b --> activate binary display
	*/
    inline bool InitialiseFromString(const char *&string){
    	return FDInitialiseFromString(*this,string);
    }

    /**
     * @brief Default constructor.
     *
     * Precision -1 means default precision for float types.
     */
	FormatDescriptor(){
		this->size = 0;
		this->precision = -1;
		this->padded = false;
		this->leftAligned = false;
		this->floatNotation = Notation::FixedPointNotation;
		this->binaryNotation = Notation::DecimalNotation;
		this->binaryPadded = false;
		this->fullNotation = false;
        this->spareBits = 0;
    }

	/** 
	 * @brief Constructor from unsigned integer.
         * @param x contains the bits for the FormatDescriptor structure.
         *
	 * Just copy bit by bit. */
	FormatDescriptor(uint32 x){
		uint32 *p = (uint32 * )this;
		*p = x;
	}
    
    /**
     * @brief Copy operator. 
     * @param src is the format descriptor to be copied in this.
    */
    void operator=(const FormatDescriptor &src){
        uint32 *d = (uint32 *)this;
        const uint32 *s = (const uint32 *)&src;
        *d = *s;
    }

    /**
     * @brief Bitwise or operator
     * @param src is format descriptor argument.
    */
    void operator |=(const FormatDescriptor &src){
        uint32 *d = (uint32 *)this;
        const uint32 *s = (const uint32 *)&src;
        *d |= *s;
    }
    
    /**
     * @brief Is equal operator.
     * @param src is the other format descriptor to be compared with this.
     * @return true if every field is equal.
    */
    bool operator ==(const FormatDescriptor &src){
        uint32 *p = (uint32 * )this;
        uint32 *s = (uint32 * )&src;
		return *p == *s;
    }

    /**
     * @brief Is different operator.
     * @param src is the other format descriptor to be compared with this.
     * @return true if one field is different.
     */
    bool operator !=(const FormatDescriptor &src){
        uint32 *p = (uint32 * )this;
        uint32 *s = (uint32 * )&src;
		return *p != *s;
    }

	/** 
         * @brief Constructor field by field.
 	 * @param size is the desired maximum size for the print.
 	 * @param precision specifies the precision for float number print.
 	 * @param padded specifies if the space remained until the desired size must be filled with ' '.
         * @param leftAligned specifies if the padding is at the beginning (false) or at the end (true).
         * @param floatNotation specifies the desired notation for float numbers (fixed point, exponential, ...)
         * @param binaryNotation specifies the desired notation for integer numbers (decimal, exadecimal, ...)
         * @param binaryPadded specifies if the trailing zeros must be added for integer prints.
         * @param fullNotation specifies if the header (0x, 0o, 0b) must be added for integer prints.
	*/
    FormatDescriptor(
    		uint8 				size, 
    		uint8 				precision, 
    		bool 				padded, 
    		bool 				leftAligned, 
    		Notation::Float 	floatNotation,
    		Notation::Binary 	binaryNotation, 
    		bool 				binaryPadded, 
    		bool 				fullNotation 
){

		this->size = size;
		this->precision = precision;
		this->padded = padded;
		this->leftAligned = leftAligned;
		this->floatNotation = floatNotation;
		this->binaryNotation = binaryNotation;
		this->binaryPadded = binaryPadded;
		this->fullNotation = fullNotation;
        this->spareBits = 0;
	}
};

/** 
   default printf notation %i or %f or %s etc... 
*/
static const FormatDescriptor  standardFormatDescriptor(0,0,false,false,Notation::FixedPointNotation,Notation::DecimalNotation,False,False);


#endif

