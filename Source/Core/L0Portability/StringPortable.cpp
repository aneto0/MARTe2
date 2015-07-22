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

//Returns the size of the string.
int32 StringPortable::Length(const char* string) {
    int32 i = 0;

    while (1) {
        if ((string + i) == NULL) {
            return -1;
        }
        if (string[i] == '\0') {
            return i;
        }
        i++;
    }
    return i;
}

//Returns true if the strings are equal, false otherwise
bool StringPortable::Equal(const char* string1,
                         const char* string2) {
    int32 i = 0;

    while (1) {

        if ((string1 + i) == NULL || (string2 + i) == NULL) {
            return false;
        }

        if (string1[i] != string2[i]) {
            return false;
        }
        if (string1[i] == '\0' && string2[i] == '\0') {
            return true;
        }
        if (string1[i] == '\0' || string2[i] == '\0') {
            return false;
        }
        i++;
    }
}

//Returns true if the strings are equal, false otherwise
bool StringPortable::EqualN(const char* string1,
                          const char* string2,
                          const uint32 size) {

    uint32 i = 0;
    while (i < size) {

        if ((string1 + i) == NULL || (string2 + i) == NULL) {
            return false;
        }

        if (string1[i] != string2[i]) {
            return false;
        }
        if (string1[i] == '\0' && string2[i] == '\0') {
            return true;
        }
        if (string1[i] == '\0' || string2[i] == '\0') {
            return false;
        }
        i++;
    }
    return true;
}

//Concatenate two strings giving result in another string
bool StringPortable::Append(const char* string1,
                          const char* string2,
                          char* result) {

    if (result == NULL) {
        return false;
    }

    int32 i = 0;
        int32 j = 0;
        while (1) {
            if ((string1 + i) == NULL || (result + i) == NULL) {
                return false;
            }
            result[i] = string1[i];
            if (string1[i] == '\0') {
                break;
            }
            i++;
        }
        while (1) {
            if ((string2 + j) == NULL || (result + i) == NULL) {
                return false;
            }

            result[i] = string2[j];
            if (string2[j] == '\0') {
                return true;
            }
            i++;
            j++;
        }
}

//Concatenate two strings giving result in another string
bool StringPortable::AppendN(const char* string1,
                           const char* string2,
                           char* result,
                           uint32 &size) {

    if (result == NULL) {
        return false;
    }

    uint32 i = 0;
    uint32 j = 0;
    while (1) {
        if ((string1 + i) == NULL || (result + i) == NULL) {
            return false;
        }
        result[i] = string1[i];
        if (string1[i] == '\0') {
            break;
        }
        i++;
    }
    while (j < size) {
        if ((string2 + j) == NULL || (result + i) == NULL) {
            return false;
        }
        result[i] = string2[j];
        if (string2[j] == '\0') {
            size = j;
            return true;
        }
        i++;
        j++;
    }
    return true;
}

//Concatenate the second string to the first
bool StringPortable::Cat(char* string1,
                       const char* string2) {

    if (string1 == NULL || string2 == NULL) {
        return false;
    }

    int32 j = 0;
    int32 beginIndex = StringPortable::Length(string1);
    if (beginIndex < 0) {
        return false;
    }

    while (1) {
        string1[beginIndex + j] = string2[j];
        if (string2[j] == '\0') {
            return true;
        }
        j++;
    }
}

//Concatenate the second string to the first
bool StringPortable::CatN(char* string1,
                        const char* string2,
                        uint32 &size) {
    uint32 j = 0;

    if (string1 == NULL || string2 == NULL || size < 0) {
        return false;
    }

    int32 beginIndex = StringPortable::Length(string1);

    if (beginIndex < 0) {
        return false;
    }

    while (j < size) {
        string1[beginIndex + j] = string2[j];
        if (string2[j] == '\0') {
            size = j;
            return true;
        }
        j++;
    }
    string1[beginIndex + j] = '\0';
    return true;
}

const char* StringPortable::SearchChar(const char* string,
                                     const char c) {

    int32 i = 0;
    while (1) {
        if ((string + i) == NULL) {
            return NULL;
        }

        if (string[i] == '\0') {
            return NULL;
        }
        if (string[i] == c) {
            return string + i;
        }
        i++;
    }
    return NULL;
}

bool StringPortable::Copy(char* destination,
                        const char* source) {

    int32 i = 0;
    while (1) {

        if ((destination + i) == NULL || (source + i) == NULL) {
            return false;
        }

        destination[i] = source[i];
        if (source[i] == '\0') {
            return true;
        }
        i++;
    }
    return false;
}

bool StringPortable::CopyN(char* destination,
                         const char* source,
                         uint32 &size) {

    uint32 i = 0;
    while (i < size) {
        if ((destination + i) == NULL || (source + i) == NULL) {
            return false;
        }
        destination[i] = source[i];
        if (source[i] == '\0') {
            size = i;
            return true;
        }
        i++;
    }
    destination[i] = '\0';
    return true;
}

//return the index of the first occurrence in string1 of a char in string2
int32 StringPortable::SearchIndex(const char* string1,
                                const char* string2) {

    int32 i = 0;
    int32 j = 0;
    while (1) {
        j = 0;
        while (1) {

            if ((string1 + i) == NULL || (string2 + j) == NULL) {
                return -1;
            }

            if (string1[i] == string2[j]) {
                return i;
            }
            if (string2[j] == '\0') {
                break;
            }
            j++;
        }
        if (string1[i] == '\0') {
            return i;
        }
        i++;
    }
    return -1;
}

//return the pointer of the first occurrence in string1 of a char in string2
const char* StringPortable::SearchChars(const char* string1,
                                      const char* string2) {

    int32 i = 0;
    int32 j = 0;
    while (1) {
        j = 0;
        while (1) {
            if ((string1 + i) == NULL || (string2 + j) == NULL) {
                return NULL;
            }
            if (string1[i] == string2[j]) {
                return string1 + i;
            }
            if (string2[j] == '\0') {
                break;
            }
            j++;
        }
        if (string1[i] == '\0') {
            return NULL;
        }
        i++;
    }
    return NULL;
}

//Return a pointer at the last char c founded in string
const char* StringPortable::SearchLastChar(const char* string,
                                         char c) {

    if (string == NULL) {
        return NULL;
    }

    int32 index;
    if ((index = (StringPortable::Length(string) - 1)) < 0) {
        return NULL;
    }
    while (index >= 0) {
        if (string[index] == c) {
            return string + index;
        }
        index--;
    }
    return NULL;
}

//Return a pointer to the first occurrence of substring in string.
const char* StringPortable::SearchSubstr(const char* string,
                                       const char* substring) {

    if (string == NULL || substring == NULL) {
        return NULL;
    }

    int32 size1 = StringPortable::Length(string);
    int32 size2 = StringPortable::Length(substring);

    if (size1 < 0 || size2 < 0) {
        return NULL;
    }

    int32 i = 0;

    while ((size1 - i) >= size2) {
        if (StringPortable::EqualN(string + i, substring, size2)) {
            return string + i;
        }
        i++;
    }

    return NULL;
}

char* StringPortable::TokenizeByChars(char* string,
                                    const char* delimiter,
                                    char* result) {

    int32 i = 0;
    int32 j = 0;
    if (result == NULL) {
        return false;
    }
    while (1) {
        j = 0;
        while (1) {
            if ((string + i) == NULL || (delimiter + j) == NULL) {
                return NULL;
            }

            if (string[i] == delimiter[j]) {
                result[i] = '\0';
                return string + i + 1;
            }

            if (delimiter[j] == '\0') {
                break;
            }
            j++;
        }
        if ((result + i) == NULL) {
            return NULL;
        }

        result[i] = string[i];
        if (string[i] == '\0') {
            return result;
        }
        i++;
    }
    return NULL;
}

const char* StringPortable::TokenizeByString(const char* string,
                                           const char* terminator,
                                           char* result) {

    int32 size1 = StringPortable::Length(string);
    int32 size2 = StringPortable::Length(terminator);

    if (size1 < 0 || size2 < 0) {
        return NULL;
    }

    int32 i = 0;
    while ((size1 - i) >= size2) {
        if (StringPortable::EqualN(string + i, terminator, size2)) {
            result[i] = '\0';
            return string + i + size2;
        }
        if ((result + i) == NULL || (string + i) == NULL) {
            return NULL;
        }
        result[i] = string[i];
        i++;
    }

    StringPortable::Copy(result + i, string + i);
    return NULL;
}

bool StringPortable::Substr(int32 begin,
                          int32 end,
                          const char* string,
                          char* result) {

    int32 i = 0;

    while (i < (end - begin + 1)) {
        if ((string + begin + i) == NULL || (result + i) == NULL) {
            return false;
        }
        result[i] = string[begin + i];
        if (string[begin + i] == '\0') {
            return false;
        }
        i++;
    }

    result[i] = '\0';
    return true;

}
