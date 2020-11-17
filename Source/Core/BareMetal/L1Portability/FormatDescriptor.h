/**
 * @file FormatDescriptor.h
 * @brief Header file for class FormatDescriptor
 * @date 26/08/2015
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

 * @details This header file contains the declaration of the class FormatDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FORMATDESCRIPTOR_H_
#define FORMATDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "BitRange.h"
#include "BitBoolean.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Notations used for float types representation.
 */
typedef uint3 FloatNotation;

/**
 * Fixed point notation.
 * The precision is the number of decimals (i.e. 123.45 => precision=2).
 */
const FloatNotation FixedPointNotation = 0u;

/**
 * Fixed point relative notation.
 * The precision is the total number of significant digits (i.e. 123.45 => precision=5).
 */
const FloatNotation FixedPointRNotation = 1u;

/**
 * Exponent notation.
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const FloatNotation ExponentNotation = 2u;

/**
 * Engineering notation.
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const FloatNotation EngineeringNotation = 3u;

/**
 * Engineering notation with size symbols.
 * Symbols defined within [E+24, E-24]
 * The precision is the total number of digits (i.e. 123.45 => precision=5).
 */
const FloatNotation SmartNotation = 6u;

/**
 * Most meaningful notation fitting within constraints
 * Choice among FixedPointRNotation, ExponentNotation and SmartNotation.
 */
const FloatNotation CompactNotation = 7u;

/**
 * Notations used for binary representations.
 */
typedef uint2 BinaryNotation;

/**
 * Decimal notation.
 */
const BinaryNotation DecimalNotation = 0u;

/**
 * Hexadecimal notation.
 */
const BinaryNotation HexNotation = 1u;

/**
 * Binary notation.
 */
const BinaryNotation BitNotation = 2u;

/**
 * Octal notation.
 */
const BinaryNotation OctalNotation = 3u;

/**
 * Enumeration-like type for encoding the desired printing action */
typedef uint3 DesiredAction;

/**
 * The user intention was to print whatever is passed
 * the value has to be 0 as it used to mark no decision and it will or-combine correctly with the other constants
 */
const DesiredAction PrintAnything = 0u;

/**
 * The user intention was to print an integer (of any size)
 */
const DesiredAction PrintInteger = 1u;

/**
 * The user intention was to print a float type
 */
const DesiredAction PrintFloat = 2u;

/**
 * The user intention was to print a pointer
 */
const DesiredAction PrintPointer = 3u;

/**
 * The user intention was to print a sequence of characters (from a char * or char[] or Stream ... )
 */
const DesiredAction PrintString = 4u;

/**
 * The user intention was to print information about the object
 */
const DesiredAction PrintInfo = 5u;

/**
 * The user intention was to print structural information about object
 */
const DesiredAction PrintStruct = 6u;

/**
 * Enumeration-like type for encoding the desired printing grammar
 */
typedef uint4 DesiredGrammar;

/**
 * Print in the standard MARTe grammar
 */
const DesiredGrammar PrintInStandardGrammar = 0u;

/**
 * Print using the json grammar
 */
const DesiredGrammar PrintInJsonGrammar = 1u;

/**
 * Print using the XML grammar
 */
const DesiredGrammar PrintInXMLGrammar = 2u;


/**
 * @brief Definition of a format descriptor.
 * @details This class is used to describe the output format of a variable.\n
 * The format representation is made by:
 *   - the user-selected output format (e.g. integer, string...);
 *   - the maximum size (in characters);
 *   - the precision (for float values);
 *   - a set of flags to fine tune white space padding;
 *   - a set of flags to define the output notation (fixed point, hexadecimal...).
 *
 * The FormatDescriptor provides also a function (InitialiseFromString) to automatically
 * populate the class from a const char8* string which specifies the desired print format
 * in a printf-like fashion.
 *
 * @remark The FormatDescriptor is internally represented as a 32-bit bitfield-like union with the following structure:
 * | size   | precision  | padded  | leftAligned | floatNotation | binaryNotation | binaryPadded | fullNotation | desiredAction | desiredGrammar |
 * | :----: | :----:     | :----:  | :----:      | :----:        | :----:         | :----:       | :----:       | :----:        | :----:    |
 * |  8     | 8          | 1       | 1           | 3             | 2              | 1            | 1            | 3             | 4         |
 */
class DLL_API FormatDescriptor {
public:

    /**
     * @brief Converts a const char8* string to a FormatDescriptor structure.
     * @param[in] string is the string which contains a printf like format.
     * @return false if string is null or empty.
     *
     * @details Takes a printf like string already pointing at the character after % (see below format),
     * parses it, and fills up the fields in this class.\n
     * At the end the pointer string is moved to the next character after the parsed block.\n
     *
     * The overall printf-like format supported is the following:\n
     * @verbatim %[flags][width][.precision]type @endverbatim
     *
     * The <tt>[flags]</tt> notation is slightly different from the standard printf notation:
     *   - ' ': activates padding with spaces (fills up to width using spaces);
     *   - '0': activates padding with zeroes (fills up to width using zeroes);
     *   - '-': left-align (put padding spaces after printing the object);
     *   - '#': activates fullNotation, i.e.:
     *     - " enclosing strings
     *     - 0x/0b/0o add zeroes in front of Hex/octal/binary
     *       The number of zeros depends on precision and chosen notation (64 bit int and binary notation = up to 64 zeros)
     *
     * <tt>[width].[precision]</tt> are two integer numbers.
     *
     * <tt>[width]</tt> specifies the MAXIMUM or EXACT number of characters to output, depending on the padding being set on or not.
     * Note that in a normal printf this would be the MINIMUM or EXACT number of characters.
     *
     * <tt>[precision]</tt> defines the minimum number of meaningful digits used to represent a number.
     * Differently from printf this includes the numbers before the decimal separator.\n
     * If the exact representation of the number uses less digits, <tt>[precision]</tt> is not considered.\n
     * If <tt>[width]</tt> is such that a numeric representation with the given precision cannot be fully represented,
     * then the number is replaced with a '?'.
     *
     * <tt>type</tt> is one character among the following:
     * - d,i,u: integer format;
     * - s: string format(any source of characters is ok);
     * - f: fixed point numeric format selected - absolute precision mode (precision is the number of digits below 1);
     * - F: fixed point relative precision format;
     * - e: exponential format;
     * - E: engineering format;
     * - g: "smart" format (like E but replaces E-12 to E+12 with one of the following letters "TGMK munp");
     * - G: compact format;
     * - x,p: hexadecimal display (p activates full notation: header+trailing zeros);
     * - p: pointer format (not compatible with integers);
     * - o: octal format;
     * - b: binary format;
     * - !: AnyType - prints the default value;
     * - ?: AnyType - prints information about the type;
     * - @: AnyType - prints full content in case of known structures.
     *
     * @remark Note that if the data type does not match <tt>type</tt> a warning will be issued but the correct print will be performed.
     *
     * @warning Note that the full printf syntax is:\n
     * @verbatim %[parameter][flags][width][.precision][length]type @endverbatim
     * This syntax is not supported by this function.\n
     */
    /*
     * * ######## NON STANDARD TBD
     * ! --> Any type is fine - prints the default value
     * ? --> Any type is fine - prints information about the type
     * @ --> Any type is fine - prints full content in case of known structures
     */
    bool InitialiseFromString(const char8 *&string);

    /**
     * @brief Default constructor.
     *
     * @details Initialises FormatDescriptor with default values, namely:
     *  - Print anything;
     *  - No maximum size;
     *  - Default precision;
     *  - Fixed-point notation for floats, decimal notation for binaries;
     *  - No padding.
     */
    inline FormatDescriptor();

    /**
     * @brief Constructor from unsigned integer.
     * @details Simply copies bit by bit the content of x.
     * @param[in] x contains the bits for the FormatDescriptor structure.
     */
    inline FormatDescriptor(const uint32 x);

    /**
     * @brief Copy operator.
     * @param[in] src is the FormatDescriptor to be copied.
     */
    inline FormatDescriptor& operator=(const FormatDescriptor &src);

    /**
     * @brief Constructor field by field.
     * @param[in] desiredActionToSet specifies the action to be performed (i.e. print an integer or print a string)
     * @param[in] sizeToSet is the desired maximum size for the print.
     * @param[in] precisionToSet specifies the precision for float number print.
     * @param[in] isPadded specifies if the space remained until the desired size must be filled with ' '.
     * @param[in] isLeftAligned specifies if the padding is at the beginning (false) or at the end (true).
     * @param[in] floatNotationToSet specifies the desired notation for float numbers (fixed point, exponential, ...)
     * @param[in] binaryNotationToSet specifies the desired notation for integer numbers (decimal, exadecimal, ...)
     * @param[in] isBinaryPadded specifies if the trailing zeros must be added for integer prints.
     * @param[in] isFullNotation specifies if the header (0x, 0o, 0b) must be added for integer prints.
     */
    inline FormatDescriptor(const DesiredAction &desiredActionToSet,
                            const uint8 sizeToSet,
                            const uint8 precisionToSet,
                            const bool isPadded,
                            const bool isLeftAligned,
                            const FloatNotation &floatNotationToSet,
                            const BinaryNotation &binaryNotationToSet,
                            const bool isBinaryPadded,
                            const bool isFullNotation);

    /* @union
     * @brief Prova descrizione
     */
    /*lint -e{9018} Use of union allows to use this memory to describe both objects and basic types.*/
    union {

        /**
         * The whole set of bits fits in a 32 bit unsigned
         */
        uint32 format_as_uint32;

        /**
         * The maximum size of representation.
         * 0 = unlimited, maximum size = 255.
         */
        BitRange<uint32, 8u, 0u> size;

        /**
         * The minimum (whenever applicable) number of meaningful digits (unless overridden by <tt>width</tt>)\n
         * The maximum is 64 (differently from the standard printf, this includes the characters before the decimal
         * separator, and excludes characters used for the exponents or for the sign and the .).\n
         * Examples:
         *   - 0.34 has precision 2        -> (precision =8)  0.3400000\n
         *   - 234 has precision 3         -> (precision =8)  234.00000\n
         *   - 2345678000 has precision 10 -> (precision =8) unchanged still precision 10\n
         *   - 2.345678E9 has precision 7  -> (precision =8) 2.3456780E9\n
         *   - 234 (int) has precision 3   -> (precision =8) unchanged  still precision 3\n
         *   - 0x4ABCD has precision 5     -> (precision =8) unchanged  still precision 5\n
         */
        BitRange<uint32, 8u, 8u> precision;

        /**
         * Setting this to true means printing a number of characters equal to <tt>width</tt>
         * fill up using spaces.
         */
        BitBoolean<uint32, 16u> padded;

        /**
         * Setting this to true will print the padding spaces at the right of the value, so that the resulting output
         * will be left-aligned.
         */
        BitBoolean<uint32, 17u> leftAligned;

        /**
         * In case of a float, this field is used to determine how to print it (see the FloatNotation constants).
         */
        BitRange<uint32, 3u, 18u> floatNotation;

        /**
         * The notation used for binary representation (see the BinaryNotation constants).
         */
        BitRange<uint32, 2u, 21u> binaryNotation;

        /**
         * Fills the number on the left with 0s up to the full representation.\n
         * Number of zeros depends on the size of the number (hex 64 bit ==> numbers+trailing zero = 16).\n
         */
        BitBoolean<uint32, 23u> binaryPadded;

        /**
         * Only meaningful for numbers.
         * Adds the missing + or 0x 0B or 0o as header.
         */
        BitBoolean<uint32, 24u> fullNotation;

        /**
         * Specifies the type that the user wants to print.
         * This can be different from what the system can do,
         * i.e. print an integer when a float is passed.
         */
        BitRange<uint32, 3u, 25u> desiredAction;


        /**
         * Specifies the desired grammar, i.e:
         *   Standart MARTe grammar
         *   Json grammar
         *   XML grammar
         */
        BitRange<uint32, 4u, 28u> desiredGrammar;


    };

};

/**
 * Default precision: is set if the user does not specify the desired precision.
 * A fixed default precision will be used depending on the type to be printed.
 */
const uint32 defaultPrecision = 0xffu;

/**
 * Default Format Descriptor.
 */
static const FormatDescriptor standardFormatDescriptor(PrintAnything, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

FormatDescriptor::FormatDescriptor() {
    format_as_uint32 = 0u;
    desiredAction = PrintAnything;
    size = 0u;
    precision = defaultPrecision;
    padded = false;
    leftAligned = false;
    floatNotation = FixedPointNotation;
    binaryNotation = DecimalNotation;
    binaryPadded = false;
    fullNotation = false;
    desiredGrammar = 0u;
}

FormatDescriptor::FormatDescriptor(const uint32 x) {
    format_as_uint32 = x;
}

FormatDescriptor& FormatDescriptor::operator=(const FormatDescriptor &src) {
    if (this != &src) {
        format_as_uint32 = src.format_as_uint32;
    }
    return *this;
}

FormatDescriptor::FormatDescriptor(const DesiredAction &desiredActionToSet,
                                   const uint8 sizeToSet,
                                   const uint8 precisionToSet,
                                   const bool isPadded,
                                   const bool isLeftAligned,
                                   const FloatNotation &floatNotationToSet,
                                   const BinaryNotation &binaryNotationToSet,
                                   const bool isBinaryPadded,
                                   const bool isFullNotation) {

    format_as_uint32 = 0u;
    desiredAction = desiredActionToSet;
    size = sizeToSet;
    precision = precisionToSet;
    padded = isPadded;
    leftAligned = isLeftAligned;
    floatNotation = floatNotationToSet;
    binaryNotation = binaryNotationToSet;
    binaryPadded = isBinaryPadded;
    fullNotation = isFullNotation;
    desiredGrammar = 0u;
}

}

#endif /* FORMATDESCRIPTOR_H_ */
