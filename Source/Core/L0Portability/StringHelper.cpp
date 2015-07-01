/**
 * @file StringHelper.cpp
 * @brief Source file for class StringHelper
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

 * @details This source file contains the definition of all the methods for
 * the class StringHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,StringHelperOS.h)
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/



char* StringHelperConcatenate(char* destination, const char* source) {
    return StringOS::Concatenate(destination, source);
}

char* StringHelperConcatenateN(char* destination,
                               const char* source,
                               uint32 size) {
    return StringOS::ConcatenateN(destination, source, size);
}

const char* StringHelperSearchChar(const char* string,
                                   char c) {
    return StringOS::SearchChar(string, c);
}

int32 StringHelperCompare(const char* string1,
                          const char* string2) {
    return StringOS::Compare(string1, string2);
}

int32 StringHelperCompareN(const char* string1,
                           const char* string2,
                           uint32 size) {
    return StringOS::CompareN(string1, string2, size);
}

bool StringHelperCopy(char* destination,
                      const char* source) {
    return StringOS::Copy(destination, source);
}

bool StringHelperCopyN(char* destination,
                       const char* source,
                       uint32 size) {
    return StringOS::CopyN(destination, source, size);
}

int32 StringHelperSearchIndex(const char* string1,
                              const char* string2) {
    return StringOS::SearchIndex(string1, string2);
}

int32 StringHelperLength(const char* string) {
    return StringOS::Length(string);
}

const char* StringHelperSearchChars(const char* string1,
                                    const char* string2) {
    return StringOS::SearchChars(string1, string2);
}

const char* StringHelperSearchLastChar(const char* string,
                                       char c) {
    return StringOS::SearchLastChar(string, c);
}

const char* StringHelperSearchString(const char* string,
                                     const char* substring) {
    return StringOS::SearchString(string, substring);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

char* StringHelper::Concatenate(char* destination,
                                const char* source) {
    return StringHelperConcatenate(destination, source);
}

char* StringHelper::ConcatenateN(char* destination,
                                 const char* source,
                                 uint32 size) {
    return StringHelperConcatenateN(destination, source, size);
}

const char* StringHelper::SearchChar(const char* string,
                                     char c) {
    return StringHelperSearchChar(string, c);
}

int32 StringHelper::Compare(const char* string1,
                            const char* string2) {
    return StringHelperCompare(string1, string2);
}

int32 StringHelper::CompareN(const char* string1,
                             const char* string2,
                             uint32 size) {
    return StringHelperCompareN(string1, string2, size);
}

bool StringHelper::Copy(char* destination,
                        const char* source) {
    return StringHelperCopy(destination, source);
}

bool StringHelper::CopyN(char* destination,
                         const char* source,
                         uint32 size) {
    return StringHelperCopyN(destination, source, size);
}

int32 StringHelper::SearchIndex(const char* string1,
                                const char* string2) {
    return StringHelperSearchIndex(string1, string2);
}

int32 StringHelper::Length(const char* string) {
    return StringHelperLength(string);
}

const char* StringHelper::SearchChars(const char* string1,
                               const char* string2) {
    return StringHelperSearchChars(string1, string2);
}

const char* StringHelper::SearchLastChar(const char* string,
                                  char c) {
    return StringHelperSearchLastChar(string, c);
}

const char* StringHelper::SearchString(const char* string,
                                const char* substring) {
    return StringHelperSearchString(string, substring);
}

