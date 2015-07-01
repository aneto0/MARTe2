/**
 * @file StringHelper.h
 * @brief Header file for class StringHelper
 * @date 30/giu/2015
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

 * @details This header file contains the declaration of the class StringHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGHELPER_H_
#define 		STRINGHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

extern "C" {

/**
 * @see StringHelper::Concatenate().
 */
char* StringHelperConcatenate(char* destination,
                              const char* source);

/**
 * @see StringHelper::ConcatenateN().
 */
char* StringHelperConcatenateN(char* destination,
                               const char* source,
                               uint32 size);
/**
 * @see StringHelper::SearchChar().
 */
const char* StringHelperSearchChar(const char* string,
                                   char c);

/**
 * @see StringHelper::Compare().
 */
int32 StringHelperCompare(const char* string1,
                          const char* string2);

/**
 * @see StringHelper::CompareN().
 */
int32 StringHelperCompareN(const char* string1,
                           const char* string2,
                           uint32 size);

/**
 * @see StringHelper::Copy().
 */
bool StringHelperCopy(char* destination,
                      const char* source);

/**
 * @see StringHelper::CopyN().
 */
bool StringHelperCopyN(char* destination,
                       const char* source,
                       uint32 size);

/**
 * @see StringHelper::SearchIndex().
 */
int32 StringHelperSearchIndex(const char* string1,
                              const char* string2);

/**
 * @see StringHelper::Length().
 */
int32 StringHelperLength(const char* string);

/** @see StringHelper::SearchChars(). */
const char* StringHelperSearchChars(const char* string1,
                                    const char* string2);

/**
 *  @see StringHelper::SearchLastChar().
 */
const char* StringHelperSearchLastChar(const char* string,
                                       char c);

/**
 * @see StringHelper::SearchString().
 */
const char* StringHelperSearchString(const char* string,
                                     const char* substring);

}

/**
 * @brief Implementation of functions to manipulate strings.
 *
 * @details These methods allows the most used and basic functions to manipulate char* strings and
 * are particularly useful in the implementation of higher levels string and stream implementations.
 *
 * @details Most of the implementation is delegated to StringHelperOS.h.
 */

class StringHelper {

public:

    /**
     * @brief Concatenate two strings.
     * @param[in,out] destination is the string where source must be appended.
     * @param[in] source is the string to append to destination.
     * @return destination.
     */
    static char* Concatenate(char* destination,
                             const char* source);

    /**
     * @brief Concatenate two strings until 'size' chars.
     * @param[in,out] destination is the string where source must be appended.
     * @param[in] source is the string to append to destination.
     * @param[in] size is the maximum number of bytes to append.
     * @return destination.
     */
    static char* ConcatenateN(char* destination,
                              const char* source,
                              uint32 size);

    /**
     * @brief Search a character in a string.
     * @param[in] string is the source string.
     * @param[in] c is the character to find in c.
     * @return a pointer to the first occurrence of c in string, NULL if c is not found.
     */
    static const char* SearchChar(const char* string,
                                  char c);

    /**
     * @brief Compare two strings.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
     */
    static int32 Compare(const char* string1,
                         const char* string2);

    /**
     * @brief Compare two strings until 'size' characters.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @param[in] size is the maximum number of char to compare.
     * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
     */
    static int32 CompareN(const char* string1,
                          const char* string2,
                          uint32 size);

    /**
     * @brief Copy source in destination.
     * @param[out] destination is the destination string.
     * @param[in] source is the string to copy in destination.
     * @return destination.
     */
    static bool Copy(char* destination,
                     const char* source);

    /**
     * @brief Copy source in destination until 'size' chars.
     * @param[in,out] destination is the destination string.
     * @param[in] source is the string to copy in destination.
     * @param[in] size is the maximum number of byte to copy.
     * @return destination.
     */
    static bool CopyN(char* destination,
                      const char* source,
                      uint32 size);

    /**
     * @brief Return the index position of the first char in string2 founded in string1 -> "abcde" "12d" returns 3.
     * @param[in] string1 is the source string.
     * @param[in] string2 contains characters which must be searched in string1.
     * @return the index at the first occurrence of the first character in string2 found in string1.
     */
    static int32 SearchIndex(const char* string1,
                             const char* string2);

    /**
     * @brief Return the length of a string.
     * @param[in] string is the source string.
     * @return the length of the string.
     */
    static int32 Length(const char* string);

    /**
     * @brief Return the pointer of the first char in string1 matched with one of chars in string2.
     * @param[in] string1 is the source string.
     * @param[in] string2 contains characters which must be searched in string1.
     * @return a pointer to the first occurrence of a character in string2 contained in string1.
     */
    static const char* SearchChars(const char* string1,
                                   const char* string2);

    /**
     * @brief Return a pointer at the last char c founded in string.
     * @param[in] string is the source string.
     * @param[in] c is the character to search.
     * @return the last occurrence of c in string.
     */
    static const char* SearchLastChar(const char* string,
                                      char c);

    /**
     * @brief Return a pointer to the first occurrence of substring in string.
     * @param[in] string is the source string.
     * @param[in] substring is the string which must be searched in string.
     * @returns a pointer to the first occurrence of substring in string.
     */
    static const char* SearchString(const char* string,
                                    const char* substring);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGHELPER_H_ */

