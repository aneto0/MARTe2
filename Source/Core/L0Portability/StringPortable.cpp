/**
 * @file StringPortable.cpp
 * @brief Source file for class StringPortable
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

 * @details This source file contains the definition of all the methods for
 * the class StringPortable (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringPortable.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int32 StringPortable::Length(const char8* const string) {
    int32 ret = -1;

    int32 i = 0;

    if (string == NULL) {
        ret = -1;
    }
    else {

        while (ret == -1) {
            if (string[i] == '\0') {
                ret = i;
            }
            i++;
        }
    }

    return ret;
}

int32 StringPortable::Compare(const char8* const string1,
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
    return ret;
}

int32 StringPortable::CompareN(const char8* const string1,
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

    return ret;
}

bool StringPortable::Concatenate(const char8* const string1,
                                 const char8* const string2,
                                 char8* const result) {

    bool ret = false;

    if ((result != NULL) && (string1 != NULL) && (string2 != NULL)) {
        bool end = false;

        int32 i = 0;
        while (!end) {

            result[i] = string1[i];
            if (string1[i] == '\0') {
                end = true;
            }
            else {
                i++;
            }
        }

        end = false;
        int32 j = 0;

        while (!end) {

            result[i] = string2[j];
            if (string2[j] == '\0') {
                ret = true;
                end = true;
            }
            i++;
            j++;
        }
    }

    return ret;
}

bool StringPortable::ConcatenateN(const char8* const string1,
                                  const char8* const string2,
                                  char8* const result,
                                  uint32 &size) {

    bool ret = false;

    if ((string1 != NULL) && (string2 != NULL) && (result != NULL)) {
        bool end = false;
        uint32 i = 0u;
        while (!end) {

            result[i] = string1[i];
            if (string1[i] == '\0') {
                end = true;
            }
            else {
                i++;
            }
        }
        uint32 j = 0u;

        while (j < size) {

            result[i] = string2[j];
            if (string2[j] == '\0') {
                size = j;
                ret = true;
            }
            i++;
            j++;
        }

        if (!ret) {
            result[i] = '\0';
            ret = true;
        }
    }

    return ret;
}

bool StringPortable::Concatenate(char8* const string1,
                                 const char8* const string2) {

    bool ret = false;

    if ((string1 != NULL) && (string2 != NULL)) {
        bool end = false;

        int32 j = 0;
        int32 beginIndex = StringPortable::Length(string1);
        if (beginIndex < 0) {
            end = true;
        }

        while (!end) {
            int32 index = beginIndex + j;
            string1[index] = string2[j];
            if (string2[j] == '\0') {
                ret = true;
                end = true;
            }
            j++;
        }
    }

    return ret;
}

bool StringPortable::ConcatenateN(char8* const string1,
                                  const char8* const string2,
                                  uint32 &size) {

    bool ret = false;
    if ((string1 != NULL) && (string2 != NULL)) {

        uint32 j = 0u;

        int32 beginIndex = StringPortable::Length(string1);

        if (beginIndex < 0) {
            //do not enter in the loop
            j = size;
        }

        while (j < size) {
            uint32 index = (static_cast<uint32>(beginIndex)) + j;
            string1[index] = string2[j];
            if (string2[j] == '\0') {
                size = j;
                ret = true;
            }
            else {
                j++;
            }
        }

        if (!ret) {
            uint32 index = (static_cast<uint32>(beginIndex)) + j;
            string1[index] = '\0';
            ret = true;
        }

    }
    return ret;
}

const char8* StringPortable::SearchChar(const char8* const string,
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

    return ret;

}

bool StringPortable::Copy(char8* const destination,
                          const char8* const source) {

    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {

        int32 i = 0;
        while (!ret) {
            destination[i] = source[i];
            if (source[i] == '\0') {
                ret = true;
            }
            i++;
        }
    }
    return ret;
}

bool StringPortable::CopyN(char8* const destination,
                           const char8* const source,
                           uint32 &size) {

    bool ret = false;
    if ((destination != NULL) && (source != NULL)) {

        uint32 i = 0u;
        while (i < size) {

            destination[i] = source[i];
            if (source[i] == '\0') {
                size = i;
                ret = true;
            }
            i++;
        }

        if (!ret) {
            destination[i] = '\0';
            ret = true;
        }
    }
    return ret;
}

int32 StringPortable::SearchIndex(const char8* const string1,
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
    return ret;
}

const char8* StringPortable::SearchChars(const char8* const string1,
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

    return ret;
}

const char8* StringPortable::SearchLastChar(const char8* const string,
                                            const char8 c) {

    const char8 *ret = static_cast<const char8*>(NULL);
    int32 index = (StringPortable::Length(string) - 1);

    while (index >= 0) {

        if (string[index] == c) {
            ret = &string[index];
            //exit from the loop
            index = 0;
        }
        index--;
    }

    return ret;
}

const char8* StringPortable::SearchString(const char8* const string,
                                          const char8* const substring) {

    const char8* ret = static_cast<const char8*>(NULL);
    int32 size1 = StringPortable::Length(string);
    int32 size2 = StringPortable::Length(substring);

    if ((size1 >= 0) && (size2 >= 0)) {

        int32 i = 0;

        while ((size1 - i) >= size2) {
            uint32 sizeArg = static_cast<uint32>(size2);
            if (StringPortable::CompareN(&string[i], substring, sizeArg) == 0) {
                ret = &string[i];
                //exit from the loop
                i = size1 - size2;
            }
            i++;
        }
    }

    return ret;
}

const char8* StringPortable::TokenizeByChars(const char8* const string,
                                             const char8* const delimiter,
                                             char8* const result) {

    const char8 *ret = static_cast<const char8*>(NULL);

    if ((string != NULL) && (delimiter != NULL) && (result != NULL)) {
        bool end = false;
        int32 i = 0;

        while (ret == NULL) {
            int32 j = 0;
            end = false;
            while (!end) {

                if (string[i] == '\0') {
                    result[i] = '\0';
                    ret = &string[i];
                    end = true;
                }
                else {
                    if (string[i] == delimiter[j]) {
                        result[i] = '\0';
                        int32 index = i + 1;
                        ret = &string[index];
                        end = true;
                    }
                }

                if (delimiter[j] == '\0') {
                    end = true;
                }
                j++;
            }

            //copy in result if terminator not found yet
            if (ret == NULL) {
                result[i] = string[i];
            }
            i++;
        }
    }
    return ret;
}

const char8* StringPortable::TokenizeByString(const char8* const string,
                                              const char8* const terminator,
                                              char8* const result) {

    const char8 *ret = static_cast<const char8*>(NULL);
    int32 size1 = StringPortable::Length(string);
    int32 size2 = StringPortable::Length(terminator);

    if ((size1 >= 0) && (size2 >= 0) && (result != NULL)) {

        int32 i = 0;
        while ((size1 - i) >= size2) {
            uint32 sizeArg = static_cast<uint32>(size2);

            if (StringPortable::CompareN(&string[i], terminator, sizeArg) == 0) {
                result[i] = '\0';
                int32 indexRet = i + size2;
                ret = &string[indexRet];

                //exit from the loop
                i = size1 - size2;

            }
            else {
                result[i] = string[i];
            }
            i++;
        }

        if (ret == NULL) {
            (void) StringPortable::Copy(&result[i], &string[i]);
        }
    }
    return ret;
}

bool StringPortable::Substr(const uint32 begin,
                            const uint32 end,
                            const char8* const string,
                            char8* const result) {

    bool ret = true;
    if ((string == NULL) || (result == NULL) || (end < begin)) {
        ret = false;
    }
    else {
        uint32 i = 0u;

        while ((i < ((end - begin) + 1u)) && (ret)) {
            uint32 index = begin + i;
            result[i] = string[index];
            if (string[index] == '\0') {
                ret = false;
            }
            i++;
        }

        if (ret) {
            result[i] = '\0';
        }
    }

    return ret;

}

bool StringPortable::SetChar(char8* const string,
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
    return ret;
}
