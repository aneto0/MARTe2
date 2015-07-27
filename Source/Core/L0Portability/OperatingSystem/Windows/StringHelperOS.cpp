/**
 * @file StringHelperOS.cpp
 * @brief Source file for class StringHelperOS
 * @date 21/07/2015
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
 * the class StringHelperOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <string.h>
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

char8 *StringHelper::Concatenate(char8 *destination,
                                 const char8 *source) {
    char8 *ret = static_cast<char8 *>(NULL);
    if ((source != NULL) && (destination != NULL)) {
        ret = strcat(destination, source);
    }
    return ret;
}

char8 *StringHelper::ConcatenateN(char8 *destination,
                                  const char8 *source,
                                  const uint32 size) {
    char8 *ret = static_cast<char8 *>(NULL);
    if ((source != NULL) && (destination != NULL)) {
        ret = strncat(destination, source, static_cast<osulong>(size));
    }
    return ret;
}

const char8 *StringHelper::SearchChar(const char8 *string,
                                      const char8 c) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if (string != NULL) {
        ret = strchr(string, c);
    }
    return ret;
}

int32 StringHelper::Compare(const char8 *string1,
                            const char8 *string2) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = strcmp(string1, string2);
        if (ret < 0) {
            ret = 1; //1 if string1 < string2
        }
        else if (ret > 0) {
            ret = 2; //2 if string1 > string2
        }
        else {
            ret = 0; //ret = 0 if string1=string2
        }
    }
    return ret;
}

int32 StringHelper::CompareN(const char8 *string1,
                             const char8 *string2,
                             const uint32 size) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = strncmp(string1, string2, static_cast<osulong>(size));
        if (ret < 0) {
            ret = 1; //1 if string1 < string2
        }
        else if (ret > 0) {
            ret = 2; //2 if string1 > string2
        }
        else {
            ret = 0;
        }
    }
    return ret;
}

bool StringHelper::Copy(char8 *destination,
                        const char8 *source) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        ret = (strcpy(destination, source) != NULL);
    }
    return ret;
}

bool StringHelper::CopyN(char8 *destination,
                         const char8 *source,
                         const uint32 size) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        ret = (strncpy(destination, source, static_cast<osulong>(size)) != NULL);
    }
    return ret;
}

int32 StringHelper::SearchIndex(const char8 *string1,
                                const char8 *string2) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = static_cast<int32>(strcspn(string1, string2));
    }
    return ret;
}

int32 StringHelper::Length(const char8 *string) {
    int32 ret = -1;
    if (string != NULL) {
        ret = static_cast<int32>(strlen(string));
    }
    return ret;
}

const char8 *StringHelper::SearchChars(const char8 *string1,
                                       const char8 *string2) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = strpbrk(string1, string2);
    }
    return ret;
}

const char8 *StringHelper::SearchLastChar(const char8 *string,
                                          char8 c) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if (string != NULL) {
        ret = strrchr(string, c);
    }
    return ret;
}

const char8 *StringHelper::SearchString(const char8 *string,
                                        const char8 *substring) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if ((string != NULL) && (substring != NULL)) {
        ret = strstr(string, substring);
    }
    return ret;
}

bool StringHelper::SetChar(char8 *string,
                           const uint32 &size,
                           const char8 &c) {
    bool ret = false;
    if ((string != NULL) && (size > 0u)) {
        void *resetString = memset(string, c, static_cast<osulong>(size));
        if (resetString == string) {
            ret = true;
        }
    }
    return ret;
}
