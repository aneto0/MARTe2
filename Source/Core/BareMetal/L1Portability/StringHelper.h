/**
 * @file StringHelper.h
 * @brief Header file for module StringHelper
 * @date 30/06/2015
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

 * @details This header file contains the declaration of the module StringHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGHELPER_H_
#define STRINGHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "CString.h"
#include "CCString.h"
#include "DynamicCString.h"
#include "GeneralDefinitions.h"


/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{762} the functions are declared in the public header file and defined in the header file of the specific architecture.*/
namespace StringHelper {


/**
 * @brief Returns a pointer to the first occurrence of substring in string.
 * @param[in] string the source string.
 * @param[in] substring the string which must be searched in string.
 * @return a pointer to the first occurrence of substring in string.
 */
DLL_API CCString SearchString(CCString  const string, CCString  const substring);

#if 0   // already implemented in ZeroTerminatedArray::CompareContent
/**
 * @brief Compares two strings.
 * @param[in] string1 the first string.
 * @param[in] string2 the second string.
 * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
 */
DLL_API int32 Compare(CCString  const string1, CCString  const string2);

/**
 * @brief Compares two strings for a maximum of 'size' characters.
 * @param[in] string1 is the first string.
 * @param[in] string2 is the second string.
 * @param[in] size is the maximum number of characters to compare.
 * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), (-1 in case of NULL strings).
 * @pre size <= largest length of the string1 or string2.
 */
DLL_API int32 CompareN(CCString  const string1,CCString  const string2,const uint32 size);

#endif

/**
 * @brief Get the token using characters as delimiters.
 * @param[in] string is the string to tokenize.
 * @param[in] delimiterd contains character delimiters.
 * @param[in] skip contains characters not to be included in token
 * @param[in] keepTerm if true moves the char pointer in the output up to the terminator, not up to the next character
 * @param[out] token is the substring between delimiters (including delimeters if not in skip).
 * @return a pointer to the next position after the delimiter for a successive tokenize operation or
 * the end of the string if terminator is not found. It returns NULL if at least one of the
 * arguments is NULL.
 */
DLL_API CCString  Tokenize(CCString  const string, DynamicCString &token, CCString const delimiters, CCString const skip,bool keepTerm=false);

/**
 * @brief Get the token using characters as delimiters.
 * @param[in] string is the string to tokenize.
 * @param[in] delimiters contains string delimiters.
 * @param[in] skip contains characters not to be included in token
 * @param[out] limit is -1 if the limit was the end of string. Otherwise the index within \a delimiters
 * @param[out] token is the substring between delimiters (including delimeters if not in skip).
 * @return a pointer to the next position after the delimiter for a successive tokenize operation or
 * the end of the string if terminator is not found. It returns NULL if at least one of the
 * arguments is NULL.
 */
DLL_API CCString  Tokenize(CCString  const string, DynamicCString &token,int32 &limit, ZeroTerminatedArray<const CCString> const delimiters, CCString const skip);


/**
 * @brief Returns the index position of the first character in string2 found in string1 (e.g. "abcde" "12d" returns 3).
 * @param[in] string1 is the source string.
 * @param[in] string2 contains the characters which must be searched in string1.
 * @return the index at the first occurrence of the first character from string2 found in string1.
 * If the stcppring1 or string2 is NULL returns -1.
 * If the string2 is not found in the string1 returns the length of the string1 (the last Index + 1).
 */
DLL_API int32 SearchIndex(CCString  const string1,   CCString  const string2);

 
}


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGHELPER_H_ */

