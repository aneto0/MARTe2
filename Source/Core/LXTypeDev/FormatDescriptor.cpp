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
struct FDLookup{
    // the character in the printf format
    char character;
    // the set of flags
    FormatDescriptor format;
};

/// 0 terminated vector of FDLookups 
static const FDLookup flagsLookup[] = {
    { ' ', 	FormatDescriptor(0,0,True ,False,Notation::FixedPointNotation, Notation::DecimalNotation,False,False) },  // ' '	
	{ '-', 	FormatDescriptor(0,0,True ,True ,Notation::FixedPointNotation, Notation::DecimalNotation,False,False) },  // '-'
    { '0', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::DecimalNotation,True ,False) },  // '0'
    { '#', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::DecimalNotation,False,True)  },  // '#'
    { 0  ,  FormatDescriptor(0)}
};

/// 0 terminated vector of FDLookups 
static const FDLookup typesLookup[] = {
    { 'd', 	FormatDescriptor(0) },   //d	
    { 'i', 	FormatDescriptor(0) },  //i
    { 'u', 	FormatDescriptor(0) },  //u
    { 's', 	FormatDescriptor(0) },  //s
    { 'c', 	FormatDescriptor(0) },  //c
    { 'f', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::DecimalNotation,False,False) },  //f
    { 'F', 	FormatDescriptor(0,0,False,False,Notation::FixedPointRNotation,Notation::DecimalNotation,False,False) },  //F
    { 'e', 	FormatDescriptor(0,0,False,False,Notation::ExponentNotation  , Notation::DecimalNotation,False,False) },  //e
    { 'E', 	FormatDescriptor(0,0,False,False,Notation::EngineeringNotation,Notation::DecimalNotation,False,False) },  //E
    { 'g', 	FormatDescriptor(0,0,False,False,Notation::SmartNotation     , Notation::DecimalNotation,False,False) },  //g
    { 'G', 	FormatDescriptor(0,0,False,False,Notation::CompactNotation   , Notation::DecimalNotation,False,False) },  //
    { 'a', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::HexNotation   ,False,False) },  //a
    { 'x', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::HexNotation   ,False,False) },  //x
    { 'p', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::HexNotation   ,True ,True ) },  //p
    { 'o', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::OctalNotation ,False,False) },  //o
    { 'b', 	FormatDescriptor(0,0,False,False,Notation::FixedPointNotation, Notation::BitNotation   ,False,False) },  //b
    { 0  ,  FormatDescriptor(0)}
};

/**

FUNCTIONS BELOW

*/


/// strchr equivalent
static inline const FDLookup *LookupCharacter(char c, const FDLookup *lookupTable){
    if (lookupTable == NULL) {
        return NULL;
    }
    while(lookupTable->character != 0){
        if (lookupTable->character == c){
            return lookupTable;
        }
        lookupTable++;
    }
    return NULL;
}


/// add to format the identified flag
static bool ParseCharacter(char c, FormatDescriptor &format,const FDLookup *lookupTable){
    
    // find the position of c in flagsLookup
     const FDLookup *found = LookupCharacter(c, lookupTable);
    
    // not found!
    if (found == NULL ) {
        return False;
    }
    
    // add the missing bits
    format |= found->format;   
    
    return True;
}

/**
    0-9 if it is a digit 
    otherwise negative
*/
static inline int32 GetDigit(char c){
    int32 digit = c - '0';
    if (digit > 9) {
        return -1;
    }
    return digit;
}

/** 
    parses a number out of string 
    returns 0 if no number is encountered
*/
static inline uint32 GetIntegerNumber(const char *&string){
    if (string == NULL) return False;
    
    uint32 number = 0;
    int32 digit = 0; 

    // 
    while ((digit = GetDigit(string[0]))>=0){
        number *= 10;
        number += digit;
        string++;
    }
    
    return number;
}


//#include <stdio.h>
bool FDInitialiseFromString(FormatDescriptor &fd, const char *&string){
    
    // prepare clean FormatDescriptor
    // copy to this only if parsing successful
    FormatDescriptor temporaryFormat;
    
    /// check pointer
    if (string == NULL) {
        return False;
    }
    
    // expect at least a character
    if (string[0] == 0) {
        return False;
    }

    //parse options
    while (ParseCharacter(string[0],temporaryFormat,&flagsLookup[0])) {
        string++;
    }
    
    // get any integer number from string if any
    temporaryFormat.size = GetIntegerNumber(string);
    
    // after a dot look for the precision field
    if (string[0] == '.') {        
        string++;
        
	if(GetDigit(string[0]) < 0)
	    //If the precision field is empty return -1 to use default precision.
            temporaryFormat.precision = -1;
	else
	    // get any integer number from string if any
            temporaryFormat.precision = GetIntegerNumber(string);        
    }
    
    // the next must be the code!
    if (ParseCharacter(string[0],temporaryFormat,&typesLookup[0])){
        fd = temporaryFormat;
        string++;
        return True;
    }
           
    string++;
    return False;
}




