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

char8 *StringHelperConcatenate(char8 *destination, const char8 *source) {
    char8 *ret = static_cast<char8 *>(NULL);
    if ((source != NULL) && (destination != NULL)) {
        ret = StringOS::Concatenate(destination, source);
    }
    return ret;
}

char8 *StringHelperConcatenateN(char8 *destination, const char8 *source, const uint32 size) {
    char8 *ret = static_cast<char8 *>(NULL);
    if ((source != NULL) && (destination != NULL)) {
        ret = StringOS::ConcatenateN(destination, source, size);
    }
    return ret;
}

const char8 *StringHelperSearchChar(const char8 *string, const char8 c) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if (string != NULL) {
        ret = StringOS::SearchChar(string, c);
    }
    return ret;
}

int32 StringHelperCompare(const char8 *string1, const char8 *string2) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = StringOS::Compare(string1, string2);
    }
    return ret;
}

int32 StringHelperCompareN(const char8 *string1, const char8 *string2, const uint32 size) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = StringOS::CompareN(string1, string2, size);
    }
    return ret;
}

bool StringHelperCopy(char8 *destination, const char8 *source) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        ret = StringOS::Copy(destination, source);
    }
    return ret;
}

bool StringHelperCopyN(char8 *destination, const char8 *source, const uint32 size) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        ret = StringOS::CopyN(destination, source, size);
    }
    return ret;
}

int32 StringHelperSearchIndex(const char8 *string1, const char8 *string2) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = StringOS::SearchIndex(string1, string2);
    }
    return ret;
}

int32 StringHelperLength(const char8 *string) {
    int32 ret = -1;
    if (string != NULL) {
        ret = StringOS::Length(string);
    }
    return ret;
}

const char8 *StringHelperSearchChars(const char8 *string1, const char8 *string2) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = StringOS::SearchChars(string1, string2);
    }
    return ret;
}

const char8 *StringHelperSearchLastChar(const char8 *string, char8 c) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if (string != NULL) {
        ret = StringOS::SearchLastChar(string, c);
    }
    return ret;
}

const char8 *StringHelperSearchString(const char8 *string, const char8 *substring) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if ((string != NULL) && (substring != NULL)) {
        ret = StringOS::SearchString(string, substring);
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

char8 *StringHelper::Concatenate(char8 *destination, const char8 *source) {
    return StringHelperConcatenate(destination, source);
}

char8 *StringHelper::ConcatenateN(char8 *destination, const char8 *source, const uint32 size) {
    return StringHelperConcatenateN(destination, source, size);
}

const char8 *StringHelper::SearchChar(const char8 *string, const char8 c) {
    return StringHelperSearchChar(string, c);
}

int32 StringHelper::Compare(const char8 *string1, const char8 *string2) {
    return StringHelperCompare(string1, string2);
}

int32 StringHelper::CompareN(const char8 *string1, const char8 *string2, const uint32 size) {
    return StringHelperCompareN(string1, string2, size);
}

bool StringHelper::Copy(char8 *destination, const char8 *source) {
    return StringHelperCopy(destination, source);
}

bool StringHelper::CopyN(char8 *destination, const char8 *source, const uint32 size) {
    return StringHelperCopyN(destination, source, size);
}

int32 StringHelper::SearchIndex(const char8 *string1, const char8 *string2) {
    return StringHelperSearchIndex(string1, string2);
}

int32 StringHelper::Length(const char8 *string) {
    return StringHelperLength(string);
}

const char8 *StringHelper::SearchChars(const char8 *string1, const char8 *string2) {
    return StringHelperSearchChars(string1, string2);
}

const char8 *StringHelper::SearchLastChar(const char8 *string, const char8 c) {
    return StringHelperSearchLastChar(string, c);
}

const char8 *StringHelper::SearchString(const char8 *string, const char8 *substring) {
    return StringHelperSearchString(string, substring);
}

