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
 *  of C-style strings (i.e. arrays of chars terminated with a '0' character).
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
     * @param[in] string is the source string.
     * @return the size of the string in the argument.
     */
    static int32 Length(const char8* const string);

    /**
     * @brief Compares two strings.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @return (0 if string1==string2) (1 if string1<string2) (2 if string1>string2).
     */
    static int32 Compare(const char8* const string1,
                         const char8* const string2);

    /**
     * @brief Compares two strings until a specified number characters.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @param[in] size is the max number of bytes to compare.
     * @return (0 if string1==string2) (1 if string1<string2) (2 if string1>string2).
     */
    static int32 CompareN(const char8* const string1,
                          const char8* const string2,
                          const uint32 size);

    /**
     * @brief Concatenates two strings giving result in another string.
     * @param[in] string1 is the string at the beginning.
     * @param[in] string2 is the string to append.
     * @param[out] result is the concatenated string.
     * @return true if strings are not NULL.
     */
    static bool Concatenate(const char8* const string1,
                            const char8* const string2,
                            char8* const result);

    /**
     * @brief Appends max 'size' characters of string2 to string1.
     * @param[in] string1 is the string at the beginning.
     * @param[in] string2 is the string to append.
     * @param[out] result is the concatenated string.
     * @param[in,out] size is the maximum number of characters to append.
     * @return true if strings are not NULL.
     */
    static bool ConcatenateN(const char8* const string1,
                             const char8* const string2,
                             char8* const result,
                             uint32 &size);

    /**
     * @brief Concatenates the second string to the first.
     * @param[in,out] string1 is the string at the beginning (and the result).
     * @param[in] string2 is the string to append.
     * @return true if strings are not NULL.
     */
    static bool Concatenate(char8* const string1,
                            const char8* const string2);

    /**
     * @brief Concatenates a specified number of characters from the second string to the first.
     * @param[in,out] string1 is the string at the beginning (and the result).
     * @param[in] string2 is the string to append.
     * @param[in,out] size is the maximum number of characters to append.
     * @return true if strings are not NULL.
     */
    static bool ConcatenateN(char8* const string1,
                             const char8* const string2,
                             uint32 &size);

    /**
     * @brief Gets the pointer to the first occurrence of c in string.
     * @param[in] string is the string.
     * @param[in] c is the character which must be searched in string.
     * @return the pointer to the first occurrence of c in string, NULL if c is not found.
     */
    static const char8* SearchChar(const char8* const string,
                                   const char8 c);

    /**
     * @brief Copies a string into a buffer.
     * @param[out] destination is the destination string.
     * @param[in] source is the source string.
     * @return true of strings are not NULL.
     */
    static bool Copy(char8* const destination,
                     const char8* const source);

    /**
     * @brief Copies a specified number of characters from the string into a buffer.
     * @param[out] destination is the destination string,
     * @param[in] source is the source string.
     * @param[in,out] size is the max number of bytes to copy.
     * @return true if strings are not NULL.
     */
    static bool CopyN(char8* const destination,
                      const char8* const source,
                      uint32 &size);

    /**
     * @brief Returns the index of the first occurrence in string1 of a character in string2.
     * @param[in] string1 is the string where to search.
     * @param[in] string2 contains characters which must be searched in string1.
     * @return the index of the first occurrence in string1 of a character of string2 or the length of string1 if c is not found.
     */
    static int32 SearchIndex(const char8* const string1,
                             const char8* const string2);

    /**
     * @brief Gets the pointer of the first occurrence in string1 of a character in string2.
     * @param[in] string1 is the string where to search.
     * @param[in] string2 contains characters which must be searched in string1.
     * @return the pointer to the first occurrence in string1 of a character of string2, NULL if c is not found.
     */
    static const char8* SearchChars(const char8* const string1,
                                    const char8* const string2);

    /**
     * @brief Returns a pointer at the last character c founded in string.
     * @param[in] string is the string where to search c.
     * @param[in] c is the character which must be searched in string.
     * @return the pointer to the last occurrence of c in string, NULL if c is not found.
     */
    static const char8* SearchLastChar(const char8* const string,
                                       const char8 c);

    /**
     * @brief Search a substring.
     * @param[in] string is the string where to search substring.
     * @param[in] substring is the string which must be searched in string.
     * @return a pointer to the first occurrence of substring in string, NULL if substring is not found.
     */
    static const char8* SearchString(const char8* const string,
                                     const char8* const substring);

    /**
     * @brief Get the token using characters as delimiters.
     * @details If result is NULL this function put a terminated character in string when the delimiter is found i.e
     * the first parameter string could be also the result.
     * @param[in,out] string is the string to tokenize.
     * @param[in] delimiter contains character delimiters.
     * @param[out] result is the substring between delimiters.
     * @return a pointer to the next position after the delimiter for a successive tokenize operation or NULL if terminator is not found.
     */
    static const char8* TokenizeByChars(const char8* const string,
                                        const char8* const delimiter,
                                        char8* const result);

    /**
     * @brief Get the token using a string as delimiter.
     * @param[in] string is the string to tokenize.
     * @param[in] terminator is the string delimiter.
     * @param[out] result is the next position for a successive operation (NULL if terminator is not found).
     * @return a pointer to the next position after the substring for a successive tokenize operation or NULL if the substring is not found.
     */
    static const char8* TokenizeByString(const char8* const string,
                                         const char8* const terminator,
                                         char8* const result);

    /**
     * @brief Gets the substring between two indexes.
     * @param[in] begin is the first index.
     * @param[in] end is the second index.
     * @param[in] string is the string where to search the indexes
     * @param[out] result is the substring.
     * @return true if indexes are valid and the strings are not NULL.
     */
    static bool Substr(const uint32 begin,
                       const uint32 end,
                       const char8* const string,
                       char8* const result);

    /**
     * @brief Sets the first specified characters of a string.
     * @param[in,out] string to be filled with the char.
     * @param[in] size length of the string in bytes.
     * @param[in] c the char to fill the string.
     * @returns true if the string is not null.
     * @pre The size of the string in input must be greater than the number of characters to set specified in the argument.
     */
    static bool SetChar(char8* const string,
                        const uint32 size,
                        const char8 c);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGPORTABLE_H_ */

