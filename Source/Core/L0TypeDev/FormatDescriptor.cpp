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
 * @file
 */

#include "FormatDescriptor.h"

/*

 DATA TABLES FIRST

 */

/** to implement a look-up table between characters in the 
 printf format encoding and the set of bits to be set in
 the FormatDescriptor
 */
struct FDLookup {
    // the character in the printf format
    char8 character;
    // the set of flags
    FormatDescriptor format;
};

/// 0 terminated vector of FDLookups 
static const FDLookup flagsLookup[] = { { ' ', FormatDescriptor(0u, 0, true, false, Notation::FixedPointNotation, Notation::DecimalNotation, false, false) }, // ' '
        { '-', FormatDescriptor(0u, 0, true, true, Notation::FixedPointNotation, Notation::DecimalNotation, false, false) },  // '-'
        { '0', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::DecimalNotation, true, false) },  // '0'
        { '#', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::DecimalNotation, false, true) },  // '#'
        { 0, FormatDescriptor(0) } };

/// 0 terminated vector of FDLookups 
static const FDLookup typesLookup[] = { { 'd', FormatDescriptor(0) },   //d
        { 'i', FormatDescriptor(0) },  //i
        { 'u', FormatDescriptor(0) },  //u
        { 's', FormatDescriptor(0) },  //s
        { 'c', FormatDescriptor(0) },  //c
        { 'f', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::DecimalNotation, false, false) },  //f
        { 'F', FormatDescriptor(0u, 0, false, false, Notation::FixedPointRNotation, Notation::DecimalNotation, false, false) },  //F
        { 'e', FormatDescriptor(0u, 0, false, false, Notation::ExponentNotation, Notation::DecimalNotation, false, false) },  //e
        { 'E', FormatDescriptor(0u, 0, false, false, Notation::EngineeringNotation, Notation::DecimalNotation, false, false) },  //E
        { 'g', FormatDescriptor(0u, 0, false, false, Notation::SmartNotation, Notation::DecimalNotation, false, false) },  //g
        { 'G', FormatDescriptor(0u, 0, false, false, Notation::CompactNotation, Notation::DecimalNotation, false, false) },  //
        { 'a', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::HexNotation, false, false) },  //a
        { 'x', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::HexNotation, false, false) },  //x
        { 'p', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::HexNotation, true, true) },  //p
        { 'o', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::OctalNotation, false, false) },  //o
        { 'b', FormatDescriptor(0u, 0, false, false, Notation::FixedPointNotation, Notation::BitNotation, false, false) },  //b
        { 0, FormatDescriptor(0) } };

/**

 FUNCTIONS BELOW

 */

/// strchr equivalent
static inline const FDLookup *LookupCharacter(const char8 c,
                                              const FDLookup *lookupTable) {

    if (lookupTable != NULL) {
        while (lookupTable->character != '\0') {
            if (lookupTable->character == c) {
                break;
            }
            lookupTable++;
        }
    }
    return lookupTable;
}

/// add to format the identified flag
static bool ParseCharacter(const char8 c,
                           FormatDescriptor &format,
                           const FDLookup * const lookupTable) {

    // find the position of c in flagsLookup
    const FDLookup *found = LookupCharacter(c, lookupTable);

    bool ret = (found != NULL);
    // not found!
    if (ret) {

        // add the missing bits
        format |= found->format;
    }
    return ret;
}

/**
 0-9 if it is a digit
 otherwise negative
 */
static inline int32 GetDigit(const char8 c) {
    int32 digit = static_cast<int32>(c - '0');

    return (digit <= 9) ? digit : -1;

}

/** 
 parses a number out of string
 returns 0 if no number is encountered
 */
static inline uint32 GetIntegerNumber(const char8 *&string) {

    ///////////////////////////////// tornare -1 se string=NULL?
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

/*lint -e{9119} assignment of integer to size field*/
bool FormatDescriptor::InitialiseFromString(const char8 *&string) {

    // prepare clean FormatDescriptor
    // copy to this only if parsing successful
    FormatDescriptor temporaryFormat;

    bool ret = false;

    /// check pointer
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
                    temporaryFormat.precision = -1;
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

