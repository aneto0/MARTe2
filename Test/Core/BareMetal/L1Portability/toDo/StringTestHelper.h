/**
 * @file StringTestHelper.h
 * @brief Header file for class StringTestHelper
 * @date 30/06/2015
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

 * @details This header file contains the declaration of the class StringTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGTESTHELPER_H_
#define STRINGTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Basic methods for strings manipulation used in tests.
 */
class StringTestHelper {
public:
    /**
     * @brief Calculates the length of a string.
     * @param[in] string is the string in input.
     * @return the string length
     */
    inline static uint32 Size(const char8 *string) {
        uint32 i = 0;
        if (string == NULL) {
            return -1;
        }

        while (string[i] != '\0') {
            i++;
        }
        return i;
    }

    /**
     * @brief Compares two strings.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the second string.
     * @return true if the strings are equal, false otherwise.
     */
    inline static bool Compare(const char8 *string1,
                               const char8 *string2) {
        int32 i = 0;
        if (string1 == NULL || string2 == NULL) {
            return false;
        }
        while (1) {
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

    /**
     * @brief Concatenates two strings.
     * @param[in] string1 is the first string.
     * @param[in] string2 is the string to be appended.
     * @param[out] result is the concatenated string.
     * @return true if successful, false otherwise.
     */
    inline static bool Append(const char8 *string1,
                              const char8 *string2,
                              char8 *result) {
        int32 i = 0;
        int32 j = 0;

        if (string1 == NULL || string2 == NULL || result == NULL) {
            return false;
        }
        while (1) {
            result[i] = string1[i];
            if (string1[i] == '\0') {
                break;
            }
            i++;
        }
        while (1) {
            result[i] = string2[j];
            if (string2[j] == '\0') {
                return true;
            }
            i++;
            j++;
        }
    }
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGTESTHELPER_H_ */

