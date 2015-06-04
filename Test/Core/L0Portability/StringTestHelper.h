/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/

#ifndef STRING_TEST_HELPER_H
#define STRING_TEST_HELPER_H

class StringTestHelper {
public:
    //Returns the size of the string.
    static uint32 Size(const char* string) {
        uint32 i = 0;
        if (string == NULL) {
            return -1;
        }

        while (string[i] != '\0') {
            i++;
        }
        return i;
    }

    //Returns true if the strings are equal, false otherwise
    static bool Compare(const char* string1, const char* string2) {
        int32 i = 0;
        while (1) {
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

    //Concatenate two strings
    static bool Append(const char* string1, const char* string2, char* result) {
        int32 i = 0;
        int32 j = 0;
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
                return True;
            }
            i++;
            j++;
        }
    }
};
#endif

