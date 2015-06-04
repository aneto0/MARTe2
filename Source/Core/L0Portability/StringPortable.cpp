/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id:  $
 *
 **/

#include "StringPortable.h"

//Returns the size of the string.
int32 StringPortableLength(const char* string) {
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
bool StringPortableEqual(const char* string1, const char* string2) {
    int32 i = 0;

    while (1) {

        if ((string1 + i) == NULL || (string2 + i) == NULL) {
            return False;
        }

        if (string1[i] != string2[i]) {
            return False;
        }
        if (string1[i] == '\0' && string2[i] == '\0') {
            return True;
        }
        if (string1[i] == '\0' || string2[i] == '\0') {
            return False;
        }
        i++;
    }
}

//Returns true if the strings are equal, false otherwise
bool StringPortableEqualN(const char* string1, const char* string2,
                          uint32 size) {

    uint32 i = 0;
    while (i < size) {

        if ((string1 + i) == NULL || (string2 + i) == NULL) {
            return False;
        }

        if (string1[i] != string2[i]) {
            return False;
        }
        if (string1[i] == '\0' && string2[i] == '\0') {
            return True;
        }
        if (string1[i] == '\0' || string2[i] == '\0') {
            return False;
        }
        i++;
    }
    return True;
}

//Concatenate two strings giving result in another string
bool StringPortableAppend(char* string1, const char* string2, char* result) {

    if (result == NULL) {
        result = string1;
    }

    int32 i = 0;
    int32 j = 0;
    while (1) {
        if ((string1 + i) == NULL || (result + i) == NULL) {
            return False;
        }
        result[i] = string1[i];
        if (string1[i] == '\0') {
            break;
        }
        i++;
    }
    while (1) {
        if ((string2 + j) == NULL || (result + i) == NULL) {
            return False;
        }

        result[i] = string2[j];
        if (string2[j] == '\0') {
            return True;
        }
        i++;
        j++;
    }
}

//Concatenate two strings giving result in another string
bool StringPortableAppendN(char* string1, const char* string2, char* result,
                           uint32 &size) {

    if (result == NULL) {
        result = string1;
    }

    uint32 i = 0;
    uint32 j = 0;
    while (1) {
        if ((string1 + i) == NULL || (result + i) == NULL) {
            return False;
        }
        result[i] = string1[i];
        if (string1[i] == '\0') {
            break;
        }
        i++;
    }
    while (j < size) {
        if ((string2 + j) == NULL || (result + i) == NULL) {
            return False;
        }
        result[i] = string2[j];
        if (string2[j] == '\0') {
            size = j;
            return True;
        }
        i++;
        j++;
    }
    return True;
}

//Concatenate the second string to the first
bool StringPortableCat(char* string1, const char* string2) {

    if (string1 == NULL || string2 == NULL) {
        return False;
    }

    int32 j = 0;
    int32 beginIndex = StringPortableLength(string1);
    if (beginIndex < 0) {
        return False;
    }

    while (1) {
        string1[beginIndex + j] = string2[j];
        if (string2[j] == '\0') {
            return True;
        }
        j++;
    }
}

//Concatenate the second string to the first
bool StringPortableCatN(char* string1, const char* string2, uint32 &size) {
    uint32 j = 0;

    if (string1 == NULL || string2 == NULL || size < 0) {
        return False;
    }

    int32 beginIndex = StringPortableLength(string1);

    if (beginIndex < 0) {
        return False;
    }

    while (j < size) {
        string1[beginIndex + j] = string2[j];
        if (string2[j] == '\0') {
            size = j;
            return True;
        }
        j++;
    }
    string1[beginIndex + j] = '\0';
    return True;
}

const char* StringPortableSearchChar(const char* string, char c) {

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

bool StringPortableCopy(char* destination, const char* source) {

    int32 i = 0;
    while (1) {

        if ((destination + i) == NULL || (source + i) == NULL) {
            return False;
        }

        destination[i] = source[i];
        if (source[i] == '\0') {
            return True;
        }
        i++;
    }
    return False;
}

bool StringPortableCopyN(char* destination, const char* source, uint32 &size) {

    uint32 i = 0;
    while (i < size) {
        if ((destination + i) == NULL || (source + i) == NULL) {
            return False;
        }
        destination[i] = source[i];
        if (source[i] == '\0') {
            size = i;
            return True;
        }
        i++;
    }
    destination[i] = '\0';
    return True;
}

//return the index of the first occurrence in string1 of a char in string2
int32 StringPortableSearchIndex(const char* string1, const char* string2) {

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
const char* StringPortableSearchChars(const char* string1,
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
const char* StringPortableSearchLastChar(const char* string, char c) {

    if (string == NULL) {
        return NULL;
    }

    int32 index;
    if ((index = (StringPortableLength(string) - 1)) < 0) {
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
const char* StringPortableSearchSubstr(const char* string,
                                       const char* substring) {

    if (string == NULL || substring == NULL) {
        return NULL;
    }

    int32 size1 = StringPortableLength(string);
    int32 size2 = StringPortableLength(substring);

    if (size1 < 0 || size2 < 0) {
        return NULL;
    }

    int32 i = 0;

    while ((size1 - i) >= size2) {
        if (StringPortableEqualN(string + i, substring, size2)) {
            return string + i;
        }
        i++;
    }

    return NULL;
}

char* StringPortableTokenizeByChars(char* string, const char* delimiter,
                                    char* result) {

    int32 i = 0;
    int32 j = 0;
    if (result == NULL) {
        result = string;
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

const char* StringPortableTokenizeByString(const char* string,
                                           const char* terminator,
                                           char* result) {

    int32 size1 = StringPortableLength(string);
    int32 size2 = StringPortableLength(terminator);

    if (size1 < 0 || size2 < 0) {
        return NULL;
    }

    int32 i = 0;
    while ((size1 - i) >= size2) {
        if (StringPortableEqualN(string + i, terminator, size2)) {
            result[i] = '\0';
            return string + i + size2;
        }
        if ((result + i) == NULL || (string + i) == NULL) {
            return NULL;
        }
        result[i] = string[i];
        i++;
    }

    StringPortableCopy(result + i, string + i);
    return NULL;
}

bool StringPortableSubstr(int32 begin, int32 end, const char* string,
                          char* result) {

    int32 i = 0;

    while (i < (end - begin + 1)) {
        if ((string + begin + i) == NULL || (result + i) == NULL) {
            return False;
        }
        result[i] = string[begin + i];
        if (string[begin + i] == '\0') {
            return False;
        }
        i++;
    }

    result[i] = '\0';
    return True;

}

