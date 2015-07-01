/**
 * @file StringHelperOS.h
 * @brief Header file for class StringHelperOS
 * @date 30/06/2015
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

 * @details This header file contains the declaration of the class StringHelperOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGHELPEROS_H_
#define 		STRINGHELPEROS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <string.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Basic functions for string's management.
 */
class StringOS {

public:

    /**
     * @see StringHelper::Concatenate
     */
    static char* Concatenate(char* destination,
                             const char* source) {
        if (source == NULL || destination == NULL) {
            return NULL;
        }
        return strcat(destination, source);
    }

    /**
     * @see StringHelper::ConcatenateN
     */
    static char* ConcatenateN(char* destination,
                              const char* source,
                              uint32 size) {
        if (destination == NULL || source == NULL) {
            return NULL;
        }
        return strncat(destination, source, size);
    }

    /**
     * @see StringHelper::SearchChar
     */
    static const char* SearchChar(const char* string,
                                  char c) {
        if (string == NULL) {
            return NULL;
        }
        return strchr(string, c);
    }

    /**
     * @see StringHelper::Compare
     */
    static int32 Compare(const char* string1,
                         const char* string2) {
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

    /**
     * @see StringHelper::CompareN
     */
    static int32 CompareN(const char* string1,
                          const char* string2,
                          uint32 size) {
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

    /**
     * @see StringHelper::Copy
     */
    static bool Copy(char* destination,
                     const char* source) {
        if (source == NULL || destination == NULL) {
            return False;
        }
        return strcpy(destination, source) != NULL;
    }

    /**
     * @see StringHelper::CopyN
     */
    static bool CopyN(char* destination,
               const char* source,
               uint32 size) {
        if (source == NULL || destination == NULL) {
            return False;
        }
        return strncpy(destination, source, size) != NULL;
    }

    /**
     * @see StringHelper::SearchIndex
     */
    static int32 SearchIndex(const char* string1,
                             const char* string2) {
        if (string1 == NULL || string2 == NULL) {
            return -1;
        }
        return (int32) (strcspn(string1, string2));
    }

    /**
     * @see StringHelper::Length
     */
    static int32 Length(const char* string) {
        if (string == NULL) {
            return -1;
        }
        return (int32) (strlen(string));
    }

    /**
     * @see StringHelper::SearchChars.
     */
    static const char* SearchChars(const char* string1,
                                   const char* string2) {
        if (string1 == NULL || string2 == NULL) {
            return NULL;
        }
        return strpbrk(string1, string2);
    }

    /**
     * @see StringHelper::SearchLastChar
     */
    static const char* SearchLastChar(const char* string,
                                      char c) {
        if (string == NULL) {
            return NULL;
        }
        return strrchr(string, c);
    }

    /**
     * @see StringHelper::SearchString
     */
    static const char* SearchString(const char* string,
                                    const char* substring) {
        if (string == NULL || substring == NULL) {
            return NULL;
        }
        return strstr(string, substring);
    }

};

#endif /* STRINGHELPEROS_H_ */

