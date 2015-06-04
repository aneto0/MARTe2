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
 * $Id:  $
 *
 **/
/**
 * @file
 * @brief Basic portable of char* string management.
 */

#ifndef STRING_PORTABLE_H
#define STRING_PORTABLE_H

#include "GeneralDefinitions.h"

extern "C" {

/** @see StringPortable:Length() */
int32 StringPortableLength(const char* string);

//Returns true if the strings are equal, false otherwise
/** @see StringPortable:Equal() */
bool StringPortableEqual(const char* string1, const char* string2);

//Returns true if the strings are equal, false otherwise
/** @see StringPortable:EqualN() */
bool StringPortableEqualN(const char* string1, const char* string2, uint32 size);

//Concatenate two strings giving result in another string
/** @see StringPortable:Append() */
bool StringPortableAppend(char* string1, const char* string2, char* result);

//Concatenate two strings giving result in another string
/** @see StringPortable:AppendN() */
bool StringPortableAppendN(char* string1, const char* string2, char* result,
                           uint32 &size);

//Concatenate the second string to the first
/** @see StringPortable:Cat() */
bool StringPortableCat(char* string1, const char* string2);

//Concatenate the second string to the first
/** @see StringPortable:CatN() */
bool StringPortableCatN(char* string1, const char* string2, uint32 &size);

/** @see StringPortable:SearchChar() */
const char* StringPortableSearchChar(const char* string, char c);

/** @see StringPortable:Copy() */
bool StringPortableCopy(char* destination, const char* source);

/** @see StringPortable:CopyN() */
bool StringPortableCopyN(char* destination, const char* source, uint32 &size);

//return the index of the first occurrence in string1 of a char in string2
/** @see StringPortable:SearchIndex() */
int32 StringPortableSearchIndex(const char* string1, const char* string2);

//return the pointer of the first occurrence in string1 of a char in string2
/** @see StringPortable:SearchChars() */
const char* StringPortableSearchChars(const char* string1, const char* string2);

//Return a pointer at the last char c founded in string
/** @see StringPortable:SearchLastChar() */
const char* StringPortableSearchLastChar(const char* string, char c);

//Return a pointer to the first occurrence of substring in string.
/** @see StringPortable:SearchSubstr() */
const char* StringPortableSearchSubstr(const char* string,
                                       const char* substring);

/** @see StringPortable:TokenizeByChars() */
char* StringPortableTokenizeByChars(char* string, const char* delimiter,
                                    char* result);

/** @see StringPortable:TokenizeByString() */
const char* StringPortableTokenizeByString(const char* string,
                                           const char* terminator,
                                           char* result);

/** @see StringPortable:Substr() */
bool StringPortableSubstr(int32 begin, int32 end, const char* string,
                          char* result);

}

/** @brief This class provides static portable functions for strings management.
  *
  * All functions are developed without use extern library functions so could be
  * used everywhere. */

class StringPortable {

public:
    /** @brief Returns the length of a string.
     * @param string is the string argument.
     * @return the size of the string in the argument. */
    static int32 Length(const char* string) {
        return StringPortableLength(string);
    }

    /** @brief Returns true if the strings are equal, false otherwise.
     * @param string1 is the first string.
     * @param string2 is the second string.
     * @return true if the two strings are equals. */
    static bool Equal(const char* string1, const char* string2) {
        return StringPortableEqual(string1, string2);
    }

    /** @brief Compare two strings until 'size' characters.
     * @param string1 is the first string.
     * @param string2 is the second string.
     * @param size is the max number of bytes to compare.
     * @return true if strings are equal for 'size' characters. */
    static bool EqualN(const char* string1, const char* string2, uint32 size) {
        return StringPortableEqualN(string1, string2, size);
    }

    /** @brief Concatenate two strings giving result in another string.
     * @param string1 is the string at the beginning.
     * @param string2 is the string to append.
     * @param result is the concatenate string.
     * @return true if strings are not NULL. */
    static bool Append(char* string1, const char* string2, char* result) {
        return StringPortableAppend(string1, string2, result);
    }

    /** @brief Append max 'size' characters of string2 to string1.
     * @param string1 is the string at the beginning.
     * @param string2 is the string to append.
     * @param result is the concatenate string.
     * @param size is the maximum number of characters to append.
     * @return true if strings are not NULL. */
    static bool AppendN(char* string1, const char* string2, char* result,
                        uint32 &size) {
        return StringPortableAppendN(string1, string2, result, size);
    }

    /** @brief Concatenate the second string to the first.
      * @param string1 is the string at the beginning (and the result).
      * @param string2 is the string to append.
      * @return true if strings are not NULL. */
    static bool Cat(char* string1, const char* string2) {
        return StringPortableCat(string1, string2);
    }

    /** @brief Concatenate the second string to the first untile size characters.
      * @param string1 is the string at the beginning (and the result).
      * @param string2 is the string to append.
      * @param size is the maximum number of characters to append.
      * @return true if strings are not NULL. */   
    static bool CatN(char* string1, const char* string2, uint32 &size) {
        return StringPortableCatN(string1, string2, size);
    }

    /** @brief Get the pointer to the first occurrence of c in string.
     * @param string is the string.
     * @param c is the character which must be searched in string.
     * @return the pointer to the first occurrence of c in string, NULL if c is not found. */
    static const char* SearchChar(const char* string, char c) {
        return StringPortableSearchChar(string, c);
    }

    /** @brief Copy operation.
     * @param destination is the destination string.
     * @param source is the source string.
     * @return true of strings are not NULL. */
    static bool Copy(char* destination, const char* source) {
        return StringPortableCopy(destination, source);
    }

    /** @brief Copy operation with 'size' as the max number of characters to copy.
     * @param destination is the destination string, 
     * @param source is the source string.
     * @param size is the max number of bytes to copy.
     * @return true if strings are not NULL. */
    static bool CopyN(char* destination, const char* source, uint32 &size) {
        return StringPortableCopyN(destination, source, size);
    }

    /** @brief Return the index of the first occurrence in string1 of a char in string2.
     * @param string1 is the string where to search.
     * @param string2 contains characters which must be searched in string1.
     * @return the index of the first occurrence in string1 of a char in string2, length of string1 if c is not found. */
    static int32 SearchIndex(const char* string1, const char* string2) {
        return StringPortableSearchIndex(string1, string2);
    }

    /** @brief Get the pointer of the first occurrence in string1 of a char in string2.
     * @param string1 is the string where to search.
     * @param string2 contains characters which must be searched in string1.
     * @return the pointer to the first occurrence in string1 of a char in string2, NULL if c is not found. */
    static const char* SearchChars(const char* string1, const char* string2) {
        return StringPortableSearchChars(string1, string2);
    }

    /** @brief Return a pointer at the last char c founded in string.
     * @param string is the string where to search c.
     * @param c is the character which must be searched in string.
     * @return the pointer to the last occurrence of c in string, NULL if c is not found. */
    static const char* SearchLastChar(const char* string, char c) {
        return StringPortableSearchLastChar(string, c);
    }

    /** @brief Search a substring.
     * @param string is the string where to search substring.
     * @param substring is the string which must be searched in string.
     * @return a pointer to the first occurrence of substring in string, NULL if substring is not found. */
    static const char* SearchSubstr(const char* string, const char* substring) {
        return StringPortableSearchSubstr(string, substring);
    }

    /** @brief Get the token using chars as delimiters.
     * @param string is the string to tokenize.
     * @param delimiter contains char delimiters.
     * @param result is the substring between delimiters.
     * @return a pointer to the next position after the delimiter for a successive tokenize operation.
     *
     * If result is NULL this function put a terminated char in string at the delimiter. */
    static char* TokenizeByChars(char* string, const char* delimiter,
                                 char* result) {
        return StringPortableTokenizeByChars(string, delimiter, result);
    }

    /** @brief Get the token using a string as delimiter.
     * @param string is the string to tokenize.
     * @param terminator is the string delimiter.
     * @result is the next position for a successive operation (NULL if terminator is not found or in case of errors).*/
    static const char* TokenizeByString(const char* string,
                                        const char* terminator, char* result) {
        return StringPortableTokenizeByString(string, terminator, result);
    }

    /** @brief Get the substring between two indexes.
     * @param begin is the first index.
     * @param end is the second index.
     * @param result is the substring.
     * @return true if indexes are correct and the strings are not NULL. */
    static bool Substr(int32 begin, int32 end, const char* string,
                       char* result) {
        return StringPortableSubstr(begin, end, string, result);
    }

};

#endif
