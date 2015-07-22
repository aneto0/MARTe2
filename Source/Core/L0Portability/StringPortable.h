/**
 * @file StringPortable.h
 * @brief Header file for class StringPortable
 * @date 21/07/2015
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class StringPortable
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGPORTABLE_H_
#define STRINGPORTABLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This class provides a collection of static functions for the handling
 *  of C-style strings (i.e. arrays of chars terminated with a '0' char).
 *
 * @details All these functions are portable, that is, it is assured that
 * any of them use external library functions, so they can be used on all
 * platforms.
 *
 * @warning The functions of this class expect that the C-style strings passed
 * as arguments be terminated with a '0' char. If they are not, then some
 * functions can loop out of bounds, so the user must be careful.
 */

class StringPortable {

public:

    /**
     * @brief Returns the length of a string.
     * @param[in] string is the string argument.
     * @return the size of the string in the argument.
     */
    static int32 Length(const char* string);

    /**
     * @brief Returns true if the strings are equal, false otherwise.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @return true if the two strings are equals.
     */
    static bool Equal(const char* string1,
                      const char* string2);

    /**
     * @brief Compare two strings until 'size' characters.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @param[in] size is the max number of bytes to compare.
     * @return true if strings are equal for 'size' characters.
     */
    static bool EqualN(const char* string1,
                       const char* string2,
                       const uint32 size);

    /**
     * @brief Concatenate two strings giving result in another string.
     * @param[in] string1 is the string at the beginning.
     * @param[in] string2 is the string to append.
     * @param[out] result is the concatenate string.
     * @return true if strings are not NULL.
     */
    static bool Append(const char* string1,
                       const char* string2,
                       char* result);

    /**
     * @brief Append max 'size' characters of string2 to string1.
     * @param[in] string1 is the string at the beginning.
     * @param[in] string2 is the string to append.
     * @param[out] result is the concatenate string.
     * @param[in,out] size is the maximum number of characters to append.
     * @return true if strings are not NULL.
     */
    static bool AppendN(const char* string1,
                        const char* string2,
                        char* result,
                        uint32 &size);

    /**
     * @brief Concatenate the second string to the first.
     * @param[out] string1 is the string at the beginning (and the result).
     * @param[in] string2 is the string to append.
     * @return true if strings are not NULL.
     */
    static bool Cat(char* string1,
                    const char* string2);

    /**
     * @brief Concatenate the second string to the first untile size characters.
     * @param[out] string1 is the string at the beginning (and the result).
     * @param[in] string2 is the string to append.
     * @param[in,out] size is the maximum number of characters to append.
     * @return true if strings are not NULL.
     */
    static bool CatN(char* string1,
                     const char* string2,
                     uint32 &size);

    /**
     * @brief Get the pointer to the first occurrence of c in string.
     * @param[in] string is the string.
     * @param[in] c is the character which must be searched in string.
     * @return the pointer to the first occurrence of c in string, NULL if c is not found.
     */
    static const char* SearchChar(const char* string,
                                  const char c);

    /**
     * @brief Copy operation.
     * @param[out] destination is the destination string.
     * @param[in] source is the source string.
     * @return true of strings are not NULL.
     */
    static bool Copy(char* destination,
                     const char* source);

    /**
     * @brief Copy operation with 'size' as the max number of characters to copy.
     * @param[out] destination is the destination string,
     * @param[in] source is the source string.
     * @param[in,out] size is the max number of bytes to copy.
     * @return true if strings are not NULL.
     */
    static bool CopyN(char* destination,
                      const char* source,
                      uint32 &size);

    /**
     * @brief Return the index of the first occurrence in string1 of a char in string2.
     * @param[in] string1 is the string where to search.
     * @param[in] string2 contains characters which must be searched in string1.
     * @return the index of the first occurrence in string1 of a char in string2, length of string1 if c is not found.
     */
    static int32 SearchIndex(const char* string1,
                             const char* string2);

    /**
     * @brief Get the pointer of the first occurrence in string1 of a char in string2.
     * @param[in] string1 is the string where to search.
     * @param[in] string2 contains characters which must be searched in string1.
     * @return the pointer to the first occurrence in string1 of a char in string2, NULL if c is not found.
     */
    static const char* SearchChars(const char* string1,
                                   const char* string2);

    /**
     * @brief Return a pointer at the last char c founded in string.
     * @param[in] string is the string where to search c.
     * @param[in] c is the character which must be searched in string.
     * @return the pointer to the last occurrence of c in string, NULL if c is not found.
     */
    static const char* SearchLastChar(const char* string,
                                      const char c);

    /**
     * @brief Search a substring.
     * @param[in] string is the string where to search substring.
     * @param[in] substring is the string which must be searched in string.
     * @return a pointer to the first occurrence of substring in string, NULL if substring is not found.
     */
    static const char* SearchSubstr(const char* string,
                                    const char* substring);

    /**
     * @brief Get the token using chars as delimiters.
     * @details If result is NULL this function put a terminated char in string at the delimiter.
     * @param[in] string is the string to tokenize.
     * @param[in] delimiter contains char delimiters.
     * @param[out] result is the substring between delimiters.
     * @return a pointer to the next position after the delimiter for a successive tokenize operation.
     */
    static char* TokenizeByChars(char* string,
                                 const char* delimiter,
                                 char* result);

    /**
     * @brief Get the token using a string as delimiter.
     * @param[in] string is the string to tokenize.
     * @param[in] terminator is the string delimiter.
     * @param[out] result is the next position for a successive operation (NULL if terminator is not found or in case of errors).
     */
    static const char* TokenizeByString(const char* string,
                                        const char* terminator,
                                        char* result);

    /**
     * @brief Get the substring between two indexes.
     * @param[in] begin is the first index.
     * @param[in] end is the second index.
     * @param[in] string is the string where to search the indexes
     * @param[out] result is the substring.
     * @return true if indexes are correct and the strings are not NULL.
     */
    static bool Substr(const int32 begin,
                       const int32 end,
                       const char* string,
                       char* result);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGPORTABLE_H_ */

