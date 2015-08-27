/**
 * @file StringHelperExtras_Generic.cpp
 * @brief Source file for class StringHelperExtras
 * @date 13/08/2015
 * @author Filippo Sartori
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

#include "../../StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace StringHelper {

bool Concatenate(const char8* const string1,
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
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Error: invalid input arguments")
    }

    return ret;
}

bool ConcatenateN(const char8* const string1,
                  const char8* const string2,
                  char8* const result,
                  const uint32 size) {

    bool ret = false;
    uint32 sizeCopy = size;

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

        while (j < sizeCopy) {

            result[i] = string2[j];
            if (string2[j] == '\0') {
                sizeCopy = j;
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
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Error: invalid input arguments")
    }
    return ret;
}

const char8* TokenizeByChars(const char8* const string,
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
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Error: invalid input arguments")
    }
    return ret;
}

const char8* TokenizeByString(const char8* const string,
                              const char8* const terminator,
                              char8* const result) {

    const char8 *ret = static_cast<const char8*>(NULL);
    int32 size1 = static_cast<int32>(Length(string));
    int32 size2 = static_cast<int32>(Length(terminator));

    if ((size1 >= 0) && (size2 >= 0) && (result != NULL)) {

        int32 i = 0;
        while ((size1 - i) >= size2) {
            uint32 sizeArg = static_cast<uint32>(size2);

            if (CompareN(&string[i], terminator, sizeArg) == 0) {
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
            (void) Copy(&result[i], &string[i]);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Error: invalid input arguments")
    }
    return ret;
}

bool Substr(const uint32 begin,
            const uint32 end,
            const char8* const string,
            char8* const result) {

    bool ret = true;
    if ((string == NULL) || (result == NULL) || (end < begin)) {
        ret = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: invalid input arguments")

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

}
