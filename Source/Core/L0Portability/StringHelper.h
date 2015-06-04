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
 * @brief Basic char* string management
 */

#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include "GeneralDefinitions.h"

extern "C" {

/** @see StringHelper::Concatenate(). */
char* StringHelperConcatenate(char* destination, const char* source);

/** @see StringHelper::ConcatenateN(). */
char* StringHelperConcatenateN(char* destination, const char* source,
                               uint32 size);
/** @see StringHelper::SearchChar(). */
const char* StringHelperSearchChar(const char* string, char c);

/** @see StringHelper::Compare(). */
int32 StringHelperCompare(const char* string1, const char* string2);

/** @see StringHelper::CompareN(). */
int32 StringHelperCompareN(const char* string1, const char* string2,
                           uint32 size);

/** @see StringHelper::Copy(). */
bool StringHelperCopy(char* destination, const char* source);

/** @see StringHelper::CopyN(). */
bool StringHelperCopyN(char* destination, const char* source, uint32 size);

/** @see StringHelper::SearchIndex(). */
int32 StringHelperSearchIndex(const char* string1, const char* string2);

/** @see StringHelper::Length(). */
int32 StringHelperLength(const char* string);

/** @see StringHelper::SearchChars(). */
const char* StringHelperSearchChars(const char* string1, const char* string2);

/** @see StringHelper::SearchLastChar(). */
const char* StringHelperSearchLastChar(const char* string, char c);

/** @see StringHelper::SearchString(). */
const char* StringHelperSearchString(const char* string, const char* substring);

/** @see StringHelper::Tokenizer(). */
/*char* StringHelperTokenizer(char* string, const char* delimiter);*/
}

/**
 * @brief Implementation of functions to manipulate strings. 
 *
 * These methods allows the most used and basic functions to manipulate char* strings and
 * are particularly useful in the implementation of higher levels string and stream implementations.
 *
 * Most of the implementation is delegated to StringHelperOS.h.
 */

class StringHelper {

public:
    
    /** @brief Concatenate two strings.
      * @param destination is the string where source must be appended.
      * @param source is the string to append to destination.
      * @return destination. */
    static char* Concatenate(char* destination, const char* source) {
        return StringHelperConcatenate(destination, source);
    }
    /** @brief Concatenate two strings until 'size' chars.
      * @param destination is the string where source must be appended.
      * @param source is the string to append to destination.
      * @param size is the maximum number of bytes to append.
      * @return destination. */
    static char* ConcatenateN(char* destination, const char* source,
                              uint32 size) {
        return StringHelperConcatenateN(destination, source, size);
    }

    /** @brief Search a character in a string.
      * @param string is the source string.
      * @param c is the character to find in c.
      * @return a pointer to the first occurrence of c in string, NULL if c is not found. */
    static const char* SearchChar(const char* string, char c) {
        return StringHelperSearchChar(string, c);
    }

    /** @brief Compare two strings.
      * @param string1 is the first string.
      * @param string2 is the second string.
      * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings). */
    static int32 Compare(const char* string1, const char* string2) {
        return StringHelperCompare(string1, string2);
    }

    /** @brief Compare two strings until 'size' characters.
      * @param string1 is the first string.
      * @param string2 is the second string.
      * @param size is the maximum number of char to compare.
      * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings). */
    static int32 CompareN(const char* string1, const char* string2,
                          uint32 size) {
        return StringHelperCompareN(string1, string2, size);
    }

    /** @brief Copy source in destination.
      * @param destination is the destination string.
      * @param source is the string to copy in destination.
      * @return destination. */
    static bool Copy(char* destination, const char* source) {
        return StringHelperCopy(destination, source);
    }

    /** @brief Copy source in destination until 'size' chars.
      * @param destination is the destination string.
      * @param source is the string to copy in destination.
      * @param size is the maximum numbero of byte to copy.
      * @return destination. */ 
    static bool CopyN(char* destination, const char* source, uint32 size) {
        return StringHelperCopyN(destination, source, size);
    }

    /** @brief Return the index position of the first char in string2 founded in string1 -> "abcde" "12d" returns 3.
      * @param string1 is the source string.
      * @param string2 contains characters which must be searched in string1.
      * @return the index at the first occurrence of the first character in string2 found in string1. */
    static int32 SearchIndex(const char* string1, const char* string2) {
        return StringHelperSearchIndex(string1, string2);
    }


    /** @brief Return the length of a string.
      * @param string is the source string.
      * @return the length of the string. */
    static int32 Length(const char* string) {
        return StringHelperLength(string);
    }

    /** @brief Return the pointer of the first char in string1 matched with one of chars in string2.
      * @param string1 is the source string.
      * @param string2 contains characters which must be searched in string1.
      * @return a pointer to the first occurrence of a character in string2 contained in string1. */
    static const char* SearchChars(const char* string1, const char* string2) {
        return StringHelperSearchChars(string1, string2);
    }


    /** @brief Return a pointer at the last char c founded in string.
      * @param string is the source string.
      * @param c is the character to search.
      * @return the last occurrence of c in string. */
    static const char* SearchLastChar(const char* string, char c) {
        return StringHelperSearchLastChar(string, c);
    }

    /** @brief Return a pointer to the first occurrence of substring in string.
      * @param string is the source string.
      * @param substring is the string which must be searched in string.
      * @returns a pointer to the first occurrence of substring in string. */
    static const char* SearchString(const char* string, const char* substring) {
        return StringHelperSearchString(string, substring);
    }

    /** @brief Returns the substring of string until delimiter.
      * If string is NULL the function tokenize from the pointer of the previous success strtok function.
      * @param string is the source string.
      * @param delimiter contains delimiter characters.
      * @return a pointer to string terminated where delimiter is found. */
   /* static char* Tokenizer(char* string, const char* delimiter) {
        return StringHelperTokenizer(string, delimiter);
    }*/

};

#endif
