/**
 * @file StringHelper_Portable_Generic.cpp
 * @brief Source file for module StringHelper
 * @date 21/07/2015
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
 * the module StringHelper (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace StringHelper {

uint32 Length(const char8* const string) {
    uint32 i = 0U;

    if (string != NULL) {

        while (string[i] != '\0') {
            i++;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return i;
}

int32 Compare(const char8* const string1,
              const char8* const string2) {

    int32 ret = -1;

    if ((string1 != NULL) && (string2 != NULL)) {
        bool end = false;
        int32 i = 0;
        while (!end) {

            if (static_cast<int32>(string1[i]) > static_cast<int32>(string2[i])) {
                end = true;
                ret = 2;
            }
            if (static_cast<int32>(string1[i]) < static_cast<int32>(string2[i])) {
                end = true;
                ret = 1;
            }
            if ((string1[i] == '\0') && (string2[i] == '\0')) {
                end = true;
                ret = 0;
            }

            i++;
        }

    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

int32 CompareN(const char8* const string1,
               const char8* const string2,
               const uint32 size) {

    int32 ret = -1;

    if ((string1 != NULL) && (string2 != NULL)) {
        bool end = false;
        ret = 0;

        uint32 i = 0u;
        while ((!end) && (i < size)) {

            if (static_cast<int32>(string1[i]) > static_cast<int32>(string2[i])) {
                ret = 2;
                end = true;
            }
            if (static_cast<int32>(string1[i]) < static_cast<int32>(string2[i])) {
                ret = 1;
                end = true;
            }
            if ((string1[i] == '\0') && (string2[i] == '\0')) {
                end = true;

            }

            i++;
        }

    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}

int32 CompareNoCaseSensN(const char8* const string1,
                         const char8* const string2,
                         const uint32 size) {

    int32 ret = -1;

    if ((string1 != NULL) && (string2 != NULL)) {
        bool end = false;
        ret = 0;

        uint32 i = 0u;
        while ((!end) && (i < size)) {
            uint8 c1=static_cast<uint8>(string1[i]);
            uint8 c2=static_cast<uint8>(string2[i]);
            if((c1>=static_cast<uint8>('A')) && (c1<=static_cast<uint8>('Z'))) {
                c1+=32u;
            }
            if((c2>=static_cast<uint8>('A')) && (c2<=static_cast<uint8>('Z'))) {
                c2+=32u;
            }
            if (c1 > c2) {
                ret = 2;
                end = true;
            }
            if (c1 < c2) {
                ret = 1;
                end = true;
            }
            if ((string1[i] == '\0') && (string2[i] == '\0')) {
                end = true;

            }

            i++;
        }

    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}

bool Concatenate(char8* const destination,
                 const char8* const source) {

    bool ret = false;
    if ((source != NULL) && (destination != NULL)) {
        uint32 indexString1 = Length(destination);
        uint32 indexString2 = 0U;
        while (source[indexString2] != '\0') {
            destination[indexString1] = source[indexString2];
            indexString2++;
            indexString1++;
        }
        ret = true;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

bool ConcatenateN(char8* const destination,
                  const char8* const source,
                  const uint32 size) {

    bool ret = false;
    if ((source != NULL) && (destination != NULL)) {
        uint32 indexString1 = Length(destination);
        uint32 indexString2 = 0U;
        while ((source[indexString2] != '\0') && (indexString2 < size)) {
            destination[indexString1] = source[indexString2];
            indexString2++;
            indexString1++;
        }

        ret = true;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;

}

const char8* SearchChar(const char8* const string,
                        const char8 c) {

    const char8* ret = static_cast<const char8*>(NULL);

    if (string != NULL) {
        bool end = false;

        uint32 i = 0u;

        while (!end) {

            if (string[i] == '\0') {
                end = true;

            }
            if (string[i] == c) {
                end = true;
                ret = &string[i];
            }
            i++;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;

}

bool Copy(char8* const destination,
          const char8* const source) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        destination[0] = '\0';
        ret = Concatenate(destination, source);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

bool CopyN(char8* const destination,
           const char8* const source,
           const uint32 size) {
    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {
        destination[0] = '\0';
        ret = ConcatenateN(destination, source, size);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

int32 SearchIndex(const char8* const string1,
                  const char8* const string2) {

    int32 ret = -1;

    if ((string1 != NULL) && (string2 != NULL)) {
        bool end1 = false;
        bool end2 = false;
        int32 i = 0;
        while (!end1) {
            int32 j = 0;
            end2 = false;
            while (!end2) {

                if ((string1[i] == string2[j]) || (string1[i] == '\0')) {
                    end1 = true;
                    end2 = true;
                    ret = i;
                }
                if (string2[j] == '\0') {
                    end2 = true;

                }
                j++;
            }
            i++;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

const char8* SearchChars(const char8* const string1,
                         const char8* const string2) {

    const char8* ret = static_cast<const char8*>(NULL);

    if ((string1 != NULL) && (string2 != NULL)) {
        bool end1 = false;
        bool end2 = false;

        int32 i = 0;
        while (!end1) {
            int32 j = 0;
            end2 = false;
            while (!end2) {

                if (string1[i] == '\0') {
                    end1 = true;
                    end2 = true;
                    ret = static_cast<const char8*>(NULL);
                }
                else {
                    if (string1[i] == string2[j]) {
                        end1 = true;
                        end2 = true;
                        ret = &string1[i];
                    }
                }

                if (string2[j] == '\0') {
                    end2 = true;
                }
                j++;
            }

            i++;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}

const char8* SearchLastChar(const char8* const string,
                            const char8 c) {

    const char8 *ret = static_cast<const char8*>(NULL);

    if (string != NULL) {
        uint32 index = Length(string);

        while (index > 0u) {

            if (string[index - 1u] == c) {
                ret = &string[index - 1u];
                //exit from the loop
                index = 1u;
            }
            index--;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}

const char8* SearchString(const char8* const string,
                          const char8* const substring) {

    const char8* ret = static_cast<const char8*>(NULL);
    int32 size1 = static_cast<int32>(Length(string));
    int32 size2 = static_cast<int32>(Length(substring));

    if ((size1 >= 0) && (size2 >= 0)) {
        int32 i = 0;

        while ((size1 - i) >= size2) {
            uint32 sizeArg = static_cast<uint32>(size2);
            if (CompareN(&string[i], substring, sizeArg) == 0) {
                ret = &string[i];
                //exit from the loop
                i = size1 - size2;
            }
            i++;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}

bool SetChar(char8* const string,
             const uint32 size,
             const char8 c) {
    bool ret = false;

    if (string != NULL) {
        uint32 i;
        for (i = 0u; i < size; i++) {
            string[i] = c;
        }

        ret = true;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

}

}
