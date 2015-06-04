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
#include "StringHelper.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,StringHelperOS.h)

//Concatenate 2 strings
char* StringHelperConcatenate(char* destination, const char* source) {
    return StringOsConcatenate(destination, source);
}

//Concatenate 2 strings until size chars.
char* StringHelperConcatenateN(char* destination, const char* source,
                               uint32 size) {
    return StringOsConcatenateN(destination, source, size);
}

//Returns a pointer to the first occurrence of c in string.
const char* StringHelperSearchChar(const char* string, char c) {
    return StringOsSearchChar(string, c);
}

//Compare two strings.
int32 StringHelperCompare(const char* string1, const char* string2) {
    return StringOsCompare(string1, string2);
}

//Compare two strings until size chars.
int32 StringHelperCompareN(const char* string1, const char* string2,
                           uint32 size) {
    return StringOsCompareN(string1, string2, size);
}

//Copy source in destination.
bool StringHelperCopy(char* destination, const char* source) {
    return StringOsCopy(destination, source);
}

//Copy source in destination until size chars.
bool StringHelperCopyN(char* destination, const char* source, uint32 size) {
    return StringOsCopyN(destination, source, size);
}

//Return the index position of the first char in string2 founded in string1 -> "abcde" "12d" returns 3.
int32 StringHelperSearchIndex(const char* string1, const char* string2) {
    return StringOsSearchIndex(string1, string2);
}

//Return the length of a string.
int32 StringHelperLength(const char* string) {
    return StringOsLength(string);
}

//Return the pointer of the first char in string1 matched with one of chars in string2.
const char* StringHelperSearchChars(const char* string1, const char* string2) {
    return StringOsSearchChars(string1, string2);
}

//Return a pointer at the last char c founded in string
const char* StringHelperSearchLastChar(const char* string, char c) {
    return StringOsSearchLastChar(string, c);
}

//Return a pointer to the first occurrence of substring in string.
const char* StringHelperSearchString(const char* string, const char* substring) {
    return StringOsSearchString(string, substring);
}

//if string is NULL the function tokenize from the pointer of the previous success strtok function.
/*char* StringHelperTokenizer(char* string, const char* delimiter) {
    return StringOsTokenizer(string, delimiter);
}*/

