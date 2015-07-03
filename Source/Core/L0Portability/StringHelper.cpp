/**
 * @file StringHelper.cpp
 * @brief Source file for class StringHelper
 * @date 30/giu/2015
 * @author Giuseppe Ferr�
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

char8* StringHelperConcatenate(char8* destination, const char8* source) {
    if (source == NULL || destination == NULL) {
        return NULL;
    }
    return StringOS::Concatenate(destination, source);
}

char8* StringHelperConcatenateN(char8* destination,
                                const char8* source,
                                uint32 size) {
    if (destination == NULL || source == NULL) {
        return NULL;
    }
    return StringOS::ConcatenateN(destination, source, size);
}

const char8* StringHelperSearchChar(const char8* string,
                                    char8 c) {
    if (string == NULL) {
        return NULL;
    }
    return StringOS::SearchChar(string, c);
}

int32 StringHelperCompare(const char8* string1,
                          const char8* string2) {
    if (string1 == NULL || string2 == NULL) {
        return -1;
    }
    return StringOS::Compare(string1, string2);
}

int32 StringHelperCompareN(const char8* string1,
                           const char8* string2,
                           uint32 size) {
    if (string1 == NULL || string2 == NULL) {
        return -1;
    }
    return StringOS::CompareN(string1, string2, size);
}

bool StringHelperCopy(char8* destination,
                      const char8* source) {
    if (source == NULL || destination == NULL) {
        return false;
    }
    return StringOS::Copy(destination, source);
}

bool StringHelperCopyN(char8* destination,
                       const char8* source,
                       uint32 size) {
    if (source == NULL || destination == NULL) {
        return false;
    }
    return StringOS::CopyN(destination, source, size);
}

int32 StringHelperSearchIndex(const char8* string1,
                              const char8* string2) {
    if (string1 == NULL || string2 == NULL) {
        return -1;
    }
    return StringOS::SearchIndex(string1, string2);
}

int32 StringHelperLength(const char8* string) {
    if (string == NULL) {
        return -1;
    }
    return StringOS::Length(string);
}

const char8* StringHelperSearchChars(const char8* string1,
                                     const char8* string2) {
    if (string1 == NULL || string2 == NULL) {
        return NULL;
    }
    return StringOS::SearchChars(string1, string2);
}

const char8* StringHelperSearchLastChar(const char8* string,
                                        char8 c) {
    if (string == NULL) {
        return NULL;
    }
    return StringOS::SearchLastChar(string, c);
}

const char8* StringHelperSearchString(const char8* string,
                                      const char8* substring) {
    if (string == NULL || substring == NULL) {
        return NULL;
    }
    return StringOS::SearchString(string, substring);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

char8* StringHelper::Concatenate(char8* destination,
                                 const char8* source) {
    return StringHelperConcatenate(destination, source);
}

char8* StringHelper::ConcatenateN(char8* destination,
                                  const char8* source,
                                  uint32 size) {
    return StringHelperConcatenateN(destination, source, size);
}

const char8* StringHelper::SearchChar(const char8* string,
                                      char8 c) {
    return StringHelperSearchChar(string, c);
}

int32 StringHelper::Compare(const char8* string1,
                            const char8* string2) {
    return StringHelperCompare(string1, string2);
}

int32 StringHelper::CompareN(const char8* string1,
                             const char8* string2,
                             uint32 size) {
    return StringHelperCompareN(string1, string2, size);
}

bool StringHelper::Copy(char8* destination,
                        const char8* source) {
    return StringHelperCopy(destination, source);
}

bool StringHelper::CopyN(char8* destination,
                         const char8* source,
                         uint32 size) {
    return StringHelperCopyN(destination, source, size);
}

int32 StringHelper::SearchIndex(const char8* string1,
                                const char8* string2) {
    return StringHelperSearchIndex(string1, string2);
}

int32 StringHelper::Length(const char8* string) {
    return StringHelperLength(string);
}

const char8* StringHelper::SearchChars(const char8* string1,
                                       const char8* string2) {
    return StringHelperSearchChars(string1, string2);
}

const char8* StringHelper::SearchLastChar(const char8* string,
                                          char8 c) {
    return StringHelperSearchLastChar(string, c);
}

const char8* StringHelper::SearchString(const char8* string,
                                        const char8* substring) {
    return StringHelperSearchString(string, substring);
}

