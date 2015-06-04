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
/**
 * @file
 * @brief char* string basic management
 */

#ifndef STRING_HELPER_OS_H
#define STRING_HELPER_OS_H

#include <string.h>

/** @brief Concatenate two strings.
  * @param destination is the first (and final) string.
  * @param source is the string to append to destination.
  * @return destination or NULL if arguments are NULL. */
char* StringOsConcatenate(char* destination, const char* source) {
    if (source == NULL || destination == NULL) {
        return NULL;
    }
    return strcat(destination, source);
}

/** @brief Concatenate 2 strings until size characters.
  * @param destination is the first (and final) string.
  * @source is the string to append to destination.
  * @size is the maximum number of characters to append. */
char* StringOsConcatenateN(char* destination, const char* source, uint32 size) {
    if (destination == NULL || source == NULL) {
        return NULL;
    }
    return strncat(destination, source, size);
}

/** @brief Search a character in a string.
  * @param string is the string.
  * @param c is the character to search.
  * @return a pointer to the first occurrence of c in string. */
const char* StringOsSearchChar(const char* string, char c) {
    if (string == NULL) {
        return NULL;
    }
    return strchr(string, c);
}

/** @brief Compare two strings.
  * @param string1 is the first string.
  * @param string2 is the second string.
  * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), -1 if arguments are NULL. */
int32 StringOsCompare(const char* string1, const char* string2) {
    if (string1 == NULL || string2 == NULL) {
        return -1;
    }

    int32 ret = strcmp(string1, string2);
    if (ret < 0) {
        return 1; //1 if string1 < string2
    }
    if (ret > 0) {
        return 2; //2 if string1 > string2
    }
    return ret; //0 if string1=string2
}

/** @brief Compare two strings until size characters.
  * @param string1 is the first string.
  * @param string2 is the second string.
  * @param size is the maximum number of characters to compare.
  * @return (0 if string1 = string2), (1 if string1 < string2), (2 if string1 > string2), -1 if arguments are NULL. */
int32 StringOsCompareN(const char* string1, const char* string2, uint32 size) {
    if (string1 == NULL || string2 == NULL) {
        return -1;
    }

    int32 ret = strncmp(string1, string2, size);
    if (ret < 0) {
        return 1; //1 if string1 < string2
    }
    if (ret > 0) {
        return 2; //2 if string1 > string2
    }
    return ret; //0 if string1=string2
}

/** @brief Copy source in destination.
  * @param destination is the destination string.
  * @param source is the source string.
  * @return true if source, destination and destination after the copy are not NULL. */
bool StringOsCopy(char* destination, const char* source) {
    if (source == NULL || destination == NULL) {
        return False;
    }
    return strcpy(destination, source) != NULL;
}

/** @brief Copy source in destination until size characters.
  * @param destination is the destination string.
  * @param source is the source string.
  * @param size is the maximum number of characters to copy.
  * @return true if source, destination and destination after the copy are not NULL. */
bool StringOsCopyN(char* destination, const char* source, uint32 size) {
    if (source == NULL || destination == NULL) {
        return False;
    }
    return strncpy(destination, source, size) != NULL;
}

/** @see StringHelper::SearchIndex */
int32 StringOsSearchIndex(const char* string1, const char* string2) {
    if (string1 == NULL || string2 == NULL) {
        return -1;
    }
    return (int32) (strcspn(string1, string2));
}

/** @see StringHelper::Length */
int32 StringOsLength(const char* string) {
    if (string == NULL) {
        return -1;
    }
    return (int32) (strlen(string));
}

/** @see StringHelper::SearchChars. */
const char* StringOsSearchChars(const char* string1, const char* string2) {
    if (string1 == NULL || string2 == NULL) {
        return NULL;
    }
    return strpbrk(string1, string2);
}

/** @see StringHelper::SearchLastChar */
const char* StringOsSearchLastChar(const char* string, char c) {
    if (string == NULL) {
        return NULL;
    }
    return strrchr(string, c);
}

/** @see StringHelper::SearchString */
const char* StringOsSearchString(const char* string, const char* substring) {
    if (string == NULL || substring == NULL) {
        return NULL;
    }
    return strstr(string, substring);
}

/** @see StringHelper::Tokenizer */
/*char* StringOsTokenizer(char* string, const char* delimiter) {
    return strtok(string, delimiter);
}
*/
#endif

