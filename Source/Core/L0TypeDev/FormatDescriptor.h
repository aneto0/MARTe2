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
 * A struct used to choose the float and binary representation modes
 */

namespace Notation {

typedef uint32 Float;
typedef uint32 Binary;

/**
 * Notations used for float types representation.
 */

/**
 * Fixed point notation.
 * The precision is the number of decimals (i.e. 123.45 => precision=2).
 */
const Float FixedPointNotation = 0u;

/**
 * Fixed point relative notation.
 * The precision is the total number of significant digits (i.e. 123.45 => precision=5).
 */
const Float FixedPointRNotation = 1u;

/**
 * Exponent notation.
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const Float ExponentNotation = 2u;

/**
 * Engineering notation.
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const Float EngineeringNotation = 3u;

/**
 * Engineering notation with size symbols.
 * Symbols defined within [E+24, E-24]
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const Float SmartNotation = 6u;
/**
 * Most meaningful notation fitting within constraints
 * Choice among FixedPointRNotation, ExponentNotation and SmartNotation.
 */
const Float CompactNotation = 7u;

/**
 * Notations used for binary representations.
 */

/**
 * Decimal notation.
 */
const Binary DecimalNotation = 0u;

/**
 * Hexadecimal notation.
 */
const Binary HexNotation = 1u;

/**
 * Binary notation.
 */
const Binary BitNotation = 2u;

/**
 * Octal notation.
 */
const Binary OctalNotation = 3u;
}

/**
 * @brief FormatDescriptor class.
 * 
 * @details This class contains a structure to store the informations the print of each type.\n
 * It defines also functions due to fill the structure getting the information from
 * a printf like constant char8* string which specifies the desired print format.\n
 *
 * It is the basic brick for all Printf functions. */
class FormatDescriptor {
public:

    /**
     * @brief Converts a const char8* string to a FormatDescriptor structure.
     * @param[in] string is the string which contains a printf like format.
     * @return false if string is null or empty.
     *
     * @details Takes a printf like string already pointing at the character after % (see below format)
     * and parses it recovering all the useful information, discarding all redundant ones,
     * and fills up the fields in this structure.
     * At the end the pointer string is moved to the next character after the parsed block
     *
     * The overall printf-like format supported is the following:
     * %[flags][width][.precision]type
     * Note that the full printf would be this:
     * %[parameter][flags][width][.precision][length]type
     * !which is not supported!

     * [flags]: // slightly different from standard printf notation
     * ' ' Activates padding:
     * fills up to width using spaces
     * -  	Left-align : put padding spaces after printing the object
     * #   Activate fullNotation:
     * + in front of integers
     * 0x/0b/0o in front of Hex/octal/binary
     * 0  	Prepends zeros for Hex Octal and Binary notations (binaryPadded activated)
     * Number of zeros depends on number precision and chosen notation (64 bit int and binary notation = up to 64 zeros)

     * [width].[precision]  two numbers
     * [width] specifies the MAXIMUM or EXACT number of characters to output, depending on the padding [flags] being set on or not
     * NOTE that in a normal printf this would be the MINIMUM or EXACT number of characters...
     * [Precision]
     * This is the minimum number of meaningful digits used to represent a number
     * Differently from printf this includes numbers before .
     * if the exact representation of the number uses less digits [precision] is not considered
     * if [width] is such that a numeric representation with the given precision cannot be fully represented than the number is replaced with a ?
     * type
     * This is one character among the following
     *
     * d,i,u,s,c --> no effect (format depends on actual data type not the letter here!)
     * f --> fixed point numeric format selected - absolute precision mode : precision is the number of digits below 1
     * F --> fixed point relative precision selected
     * e --> exponential format
     * E --> engineering format
     * g --> smart format   - like E but replaces E-12 to E+12 with one of the following letters "TGMK munp"
     * G --> compact format
     * a,x,p --> activate hexadecimal display (p activates full notation: header+trailing zeros)
     * o --> activate octal display
     * b --> activate binary display
     */
    bool InitialiseFromString(const char8 *&string);
    /**
     * @brief Default constructor.
     *
     * @details Precision = -1 means default precision for float types.
     */
    inline FormatDescriptor();

    /**
     * @brief Constructor from unsigned integer.
     * @param[in] x contains the bits for the FormatDescriptor structure.
     *
     * Just copy bit by bit. */
 //   inline FormatDescriptor(const uint32 x);

    /**
     * @brief Copy operator.
     * @param[in] src is the format descriptor to be copied in this.
     */
 //   inline FormatDescriptor& operator=(const FormatDescriptor &src);

    /**
     * @brief Bitwise or operator
     * @param[in] src is format descriptor argument.
     */
 //   inline void operator |=(const FormatDescriptor &src);

    /**
     * @brief Is equal operator.
     * @param[in] src is the other format descriptor to be compared with this.
     * @return true if every field is equal.
     */
 //   inline bool operator ==(const FormatDescriptor &src);

    /**
     * @brief Is different operator.
     * @param[in] src is the other format descriptor to be compared with this.
     * @return true if one field is different.
     */
//    inline bool operator !=(const FormatDescriptor &src);

    /**
     * @brief Constructor field by field.
     * @param[in] sizeToSet is the desired maximum size for the print.
     * @param[in] precisionToSet specifies the precision for float number print.
     * @param[in] isPadded specifies if the space remained until the desired size must be filled with ' '.
     * @param[in] isLeftAligned specifies if the padding is at the beginning (false) or at the end (true).
     * @param[in] isFloatNotation specifies the desired notation for float numbers (fixed point, exponential, ...)
     * @param[in] isBinaryNotation specifies the desired notation for integer numbers (decimal, exadecimal, ...)
     * @param[in] isBinaryPadded specifies if the trailing zeros must be added for integer prints.
     * @param[in] isFullNotation specifies if the header (0x, 0o, 0b) must be added for integer prints.
     */
    inline FormatDescriptor(uint8 sizeToSet,
                            int8 precisionToSet,
                            bool isPadded,
                            bool isLeftAligned,
                            const Notation::Float floatNotationToSet,
                            const Notation::Binary binaryNotationToSet,
                            bool isBinaryPadded,
                            bool isFullNotation);

private:

    //*** MAYBE REPLACE with finite set of options ( *' ' *0  *' '0x *' ', ....)

    /**
     * The maximum size of representation.
     * 0 = unlimited
     * maximum size = 255.
     */
    uint32 size :8;

    /**
     * The minimum (whenever applicable) number of meaningful digits (unless overridden by width)  max 64
     * differently from printf this includes characters before comma
     * excludes characters used for the exponents or for the sign and the .
     * 0.34 has precision 2        -> (precision =8)  0.3400000
     * 234 has precision 3         -> (precision =8)  234.00000
     * 2345678000 has precision 10 -> (precision =8) unchanged still precision 10
     * 2.345678E9 has precision 7  -> (precision =8) 2.3456780E9
     * 234 (int) has precision 3   -> (precision =8) unchanged  still precision 3
     * 0x4ABCD has precision 5     -> (precision =8) unchanged  still precision 5
     */
    int32 precision :8;

    /**
     * True means produce a number of characters equal to width
     * fill up using spaces.
     */
    bool padded :1;

    /**
     * True means to produce pad spaces after printing the object representation.
     */
    bool leftAligned :1;

    /**
     * In case of a float, this field is used to determine how to print it.
     */
    Notation::Float floatNotation :3;

    /**
     * The notation used for binary representation.
     */
    Notation::Binary binaryNotation :2;

    /**
     * Fills the number on the left with 0s up to the full representation.\n
     * Number of zeros depends on the size of the number (hex 64 bit ==> numbers+trailing zero = 16).\n
     */
    bool binaryPadded :1;

    /**
     * Only meaningful for numbers.
     * Add the missing + or 0x 0B or 0o as header.
     */
    bool fullNotation :1;

    /**
     * Remained bits to fill up a 32 bit space.
     */
    uint32 spareBits :7;

};

/**
 * Default Format Descriptor.
 */
static const FormatDescriptor standardFormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::DecimalNotation, false, false);

/**********************************************
 * INLINE FUNCTIONS
 * *********************************************/

/*lint -e{9119} assignment of integer to 3-bit floatNotation and 2-bit binaryNotation justified*/
FormatDescriptor::FormatDescriptor() {
    this->size = 0u;
    this->precision = -1;
    this->padded = false;
    this->leftAligned = false;
    this->floatNotation = Notation::FixedPointNotation;
    this->binaryNotation = Notation::DecimalNotation;
    this->binaryPadded = false;
    this->fullNotation = false;
    this->spareBits = 0u;
}


/////// SERVONO QUESTE FUNZIONI E QUESTI OPERATORI??
/////// DOVREBBE ESSERE GIA' IL COMPORTAMENTO STANDARD
/*
 * FormatDescriptor::FormatDescriptor(const uint32 x) {
    uint32 *p = static_cast<uint32 *>(this);
    *p = x;
}

FormatDescriptor& FormatDescriptor::operator=(const FormatDescriptor &src) {
    uint32 *d = static_cast<uint32 *>(this);
    const uint32 *s = static_cast<const uint32 *>(&src);
    *d = *s;
}

void FormatDescriptor::operator |=(const FormatDescriptor &src) {
    uint32 *d = static_cast<uint32 *>(this);
    const uint32 *s = static_cast<const uint32 *>(&src);
    *d |= *s;
}

bool FormatDescriptor::operator ==(const FormatDescriptor &src) {
    uint32 *p = static_cast<uint32 *>(this);
    uint32 *s = static_cast<uint32 *>(&src);
    return *p == *s;
}

bool FormatDescriptor::operator !=(const FormatDescriptor &src) {
    uint32 *p = static_cast<uint32 *>(this);
    uint32 *s = static_cast<uint32 *>(&src);
    return *p != *s;
}
*/

/*lint -e{9119} assignment of integer to 3-bit floatNotation and 2-bit binaryNotation justified*/
FormatDescriptor::FormatDescriptor(uint8 sizeToSet,
                                   int8 precisionToSet,
                                   bool isPadded,
                                   bool isLeftAligned,
                                   const Notation::Float floatNotationToSet,
                                   const Notation::Binary binaryNotationToSet,
                                   bool isBinaryPadded,
                                   bool isFullNotation) {

    this->size = sizeToSet;
    this->precision = precisionToSet;
    this->padded = isPadded;
    this->leftAligned = isLeftAligned;
    this->floatNotation = floatNotationToSet;
    this->binaryNotation = binaryNotationToSet;
    this->binaryPadded = isBinaryPadded;
    this->fullNotation = isFullNotation;
    this->spareBits = 0u;
}

#endif

