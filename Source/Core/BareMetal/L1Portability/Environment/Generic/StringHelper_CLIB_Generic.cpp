/**
 * @file StringHelper_CLIB_Generic.cpp
 * @brief Source file for module StringHelper
 * @date 30/06/2015
 * @author Giuseppe Ferro
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
 * the module StringHelper (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <string.h>
#else
#include "../Linux/lint-linux.h"
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
namespace MARTe {

/*lint -e{586} many of the string.h functions are deprecated.
 * The string generic implementation is not certifiable.*/
namespace StringHelper {

bool Concatenate(char8 * const destination,
                 const char8 * const source) {
    bool ret = false;
    if ((source != NULL) && (destination != NULL)) {
        ret= true;
        strcat(destination, source);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

bool ConcatenateN(char8 * const destination,
                  const char8 * const source,
                  const uint32 size) {
    bool ret = false;
    if ((source != NULL) && (destination != NULL)) {
        ret = true;
        strncat(destination, source, static_cast<osulong>(size));
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

const char8 *SearchChar(const char8 * const string,
                        const char8 c) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if (string != NULL) {
        ret = strchr(string, c);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

int32 Compare(const char8 * const string1,
              const char8 * const string2) {
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
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

int32 CompareN(const char8 * const string1,
               const char8 * const string2,
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
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

int32 CompareNoCaseSensN(const char8* const string1,
                                 const char8* const string2,
                                 const uint32 size){
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = strncasecmp(string1, string2, static_cast<osulong>(size));
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
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}


bool Copy(char8 * const destination,
          const char8 * const source) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        ret =true;
        strcpy(destination, source);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

bool CopyN(char8 * const destination,
           const char8 * const source,
           const uint32 size) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        ret =true;
        strncpy(destination, source, static_cast<osulong>(size));
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

int32 SearchIndex(const char8 * const string1,
                  const char8 * const string2) {
    int32 ret = -1;
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = static_cast<int32>(strcspn(string1, string2));
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

uint32 Length(const char8 * const string) {
    uint32 ret = 0u;
    if (string != NULL) {
        ret = static_cast<uint32>(strlen(string));
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

const char8 *SearchChars(const char8 * const string1,
                         const char8 * const string2) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if ((string1 != NULL) && (string2 != NULL)) {
        ret = strpbrk(string1, string2);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

const char8 *SearchLastChar(const char8 * const string,
                            const char8 c) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if (string != NULL) {
        ret = strrchr(string, c);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

const char8 *SearchString(const char8 * const string,
                          const char8 * const substring) {
    const char8 *ret = static_cast<const char8 *>(NULL);
    if ((string != NULL) && (substring != NULL)) {
        ret = strstr(string, substring);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

bool SetChar(char8 * const string,
             const uint32 size,
             const char8 c) {
    bool ret = false;
    if ((string != NULL) && (size > 0u)) {
        void *resetString = memset(string, c, static_cast<osulong>(size));
        if (resetString == string) {
            ret = true;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

}

}
