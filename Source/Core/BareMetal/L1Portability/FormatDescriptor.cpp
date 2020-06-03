/**
 * @file FormatDescriptor.cpp
 * @brief Source file for class FormatDescriptor
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

 * @details This source file contains the definition of all the methods for
 * the class FormatDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FormatDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief To implement a look-up table between characters in the
 * printf format encoding and the set of bits to be set in
 * the FormatDescriptor
 */
struct FDLookup {
    /**
     * the character in the printf format
     */
    char8 character;

    /**
     * the set of flags
     */
    FormatDescriptor format;
};

/**
 * 0 terminated vector of FDLookups
 */
static const FDLookup flagsLookup[] = { { ' ', FormatDescriptor(PrintAnything, 0u, 0u, true, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { '-', FormatDescriptor(PrintAnything, 0u, 0u, true, true, FixedPointNotation, DecimalNotation, false, false) },  // '-'
        { '0', FormatDescriptor(PrintAnything, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, true, false) },  // '0'
        { '#', FormatDescriptor(PrintAnything, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, true) },  // '#'
        { 'S', FormatDescriptor(0x00000000u) },  // 'J'
        { 'J', FormatDescriptor(0x10000000u) },  // 'J'
        { 'X', FormatDescriptor(0x20000000u) },  // 'X'
        { static_cast<char8>(0), FormatDescriptor(0u) } };

/**
 * 0 terminated vector of FDLookups
 */
static const FDLookup typesLookup[] = { { '!', FormatDescriptor(PrintAnything, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { '?', FormatDescriptor(PrintInfo, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { '@', FormatDescriptor(PrintStruct, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { 'd', FormatDescriptor(PrintInteger, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { 'i', FormatDescriptor(PrintInteger, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { 'u', FormatDescriptor(PrintInteger, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { 's', FormatDescriptor(PrintString, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { 'c', FormatDescriptor(PrintString, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) }, // ' '
        { 'f', FormatDescriptor(PrintFloat, 0u, 0u, false, false, FixedPointNotation, DecimalNotation, false, false) },  //f
        { 'F', FormatDescriptor(PrintFloat, 0u, 0u, false, false, FixedPointRNotation, DecimalNotation, false, false) },  //F
        { 'e', FormatDescriptor(PrintFloat, 0u, 0u, false, false, ExponentNotation, DecimalNotation, false, false) },  //e
        { 'E', FormatDescriptor(PrintFloat, 0u, 0u, false, false, EngineeringNotation, DecimalNotation, false, false) },  //E
        { 'g', FormatDescriptor(PrintFloat, 0u, 0u, false, false, SmartNotation, DecimalNotation, false, false) },  //g
        { 'G', FormatDescriptor(PrintFloat, 0u, 0u, false, false, CompactNotation, DecimalNotation, false, false) },  //
        { 'x', FormatDescriptor(PrintInteger, 0u, 0u, false, false, FixedPointNotation, HexNotation, false, false) },  //x
        { 'p', FormatDescriptor(PrintPointer, 0u, 0u, false, false, FixedPointNotation, HexNotation, true, true) },  //p
        { 'o', FormatDescriptor(PrintInteger, 0u, 0u, false, false, FixedPointNotation, OctalNotation, false, false) },  //o
        { 'b', FormatDescriptor(PrintInteger, 0u, 0u, false, false, FixedPointNotation, BitNotation, false, false) },  //b
        { static_cast<char8>(0), FormatDescriptor(0u) } };

/**
 * strchr equivalent
 */
static inline const FDLookup *LookupCharacter(const char8 c,
                                              const FDLookup *lookupTable) {
    if (lookupTable != NULL) {
        while (lookupTable->character != '\0') {
            if (lookupTable->character == c) {
                break;
            }
            lookupTable++;
        }
        if (lookupTable->character == '\0') {
            lookupTable = static_cast<FDLookup *>(NULL);
        }
    }
    return lookupTable;
}

/**
 * add to format the identified flag
 */
static bool ParseCharacter(const char8 c,
                           FormatDescriptor &format,
                           const FDLookup * const lookupTable) {
    // find the position of c in flagsLookup
    const FDLookup *found = LookupCharacter(c, lookupTable);
    bool ret = (found != NULL);
    // not found!
    if (ret) {

        // add the missing bits
        format.format_as_uint32 |= found->format.format_as_uint32;
    }
    return ret;
}

/**
 * 0-9 if it is a digit
 * otherwise negative
 */
static inline int8 GetDigit(const char8 c) {
    int8 digit = static_cast<int8>(c - '0');

    return ((digit <= 9) && (digit>=0)) ? (digit) : (-1);

}

/**
 * Parses a number out of string
 * returns 0 if no number is encountered
 */
static inline uint32 GetIntegerNumber(const char8 *&string) {

    uint32 number = 0u;

    if (string != NULL) {

        //
        int32 digit = GetDigit(string[0]);
        while (digit >= 0) {
            number *= 10u;
            number += static_cast<uint32>(digit);
            string++;
            digit = GetDigit(string[0]);
        }
    }

    return number;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


/*lint -e{9119} assignment of integer to size field*/
bool FormatDescriptor::InitialiseFromString(const char8 *&string) {

    // prepare clean FormatDescriptor
    // copy to this only if parsing successful
    FormatDescriptor temporaryFormat;

    bool ret = false;
    // check pointer
    if (string != NULL) {

        // expect at least a character
        if (string[0] != '\0') {
            //parse options
            while (ParseCharacter(string[0], temporaryFormat, &flagsLookup[0])) {
                string++;
            }

            // get any integer number from string if any
            temporaryFormat.size = GetIntegerNumber(string);
            // after a dot look for the precision field
            if (string[0] == '.') {
                string++;

                if (GetDigit(string[0]) < 0) {
                    //If the precision field is empty return -1 to use default precision.
                    temporaryFormat.precision = defaultPrecision;
                }
                else {
                    // get any integer number from string if any
                    temporaryFormat.precision = GetIntegerNumber(string);
                }
            }

            // the next must be the code!
            if (ParseCharacter(string[0], temporaryFormat, &typesLookup[0])) {
                *this = temporaryFormat;
                string++;
                ret = true;
            }

            if (!ret) {
                string++;
            }

        }
    }
    return ret;
}

}
