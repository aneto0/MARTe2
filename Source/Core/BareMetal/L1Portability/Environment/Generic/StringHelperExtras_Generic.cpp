/**
 * @file StringHelperExtras_Generic.cpp
 * @brief Source file for module StringHelper
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

#define NULLS NULL_PTR(char8 *)

namespace MARTe{

namespace StringHelper {

CCString  SearchChar(CCString  const string,  const char8 c) {

    CCString  ret(NULLS);

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
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}

CCString  SearchString(CCString  const string,CCString  const substring) {

    CCString  ret(NULLS);
    int32 size1 = static_cast<int32>(string.GetSize());
    int32 size2 = static_cast<int32>(substring.GetSize());

    if ((size1 >= 0) && (size2 >= 0)) {
        int32 i = 0;

        while ((size1 - i) >= size2) {
            uint32 sizeArg = static_cast<uint32>(size2);
            if (CompareN(string.GetList()+i, substring, sizeArg) == 0) {
                ret = string.GetList()+i;
                //exit from the loop
                i = size1 - size2;
            }
            i++;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}


int32 Compare(CCString  const string1,CCString  const string2) {

    int32 ret = -1;

    if ((string1.GetList() != NULLS) && (string2.GetList() != NULLS)) {
        bool end = false;
        int32 i = 0;
        while (!end) {

            if (static_cast<int32>(string1[i]) > static_cast<int32>(string2[i])) {
                end = true;
                ret = 2;
            } else
            if (static_cast<int32>(string1[i]) < static_cast<int32>(string2[i])) {
                end = true;
                ret = 1;
            } else
            if ((string1[i] == '\0') && (string2[i] == '\0')) {
                end = true;
                ret = 0;
            }

            i++;
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}


int32 CompareN(CCString  const string1, CCString  const string2, const uint32 size) {

    int32 ret = -1;

    if ((string1.GetList() != NULLS) && (string2.GetList() != NULLS)) {
        bool end = false;
        ret = 0;

        uint32 i = 0u;
        while ((!end) && (i < size)) {

            if (static_cast<int32>(string1[i]) > static_cast<int32>(string2[i])) {
                ret = 2;
                end = true;
            } else
            if (static_cast<int32>(string1[i]) < static_cast<int32>(string2[i])) {
                ret = 1;
                end = true;
            } else
            if ((string1[i] == '\0') || (string2[i] == '\0')) {
                end = true;
            }

            i++;
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }

    return ret;
}



CCString TokenizeByChars(CCString const string,CCString const delimiter,CString const result,uint32 resultStorageSize) {

    CCString ret(NULLS);
    uint32 resultStorageLeft = resultStorageSize-1;

    if ((string.GetList() != NULLS) && (delimiter.GetList() != NULLS) && (result.GetList() != NULLS)) {
        uint32 inputIndex = 0;
        uint32 outputIndex = 0;

        // next character to be processed;
        char8 c = string[0];

        while ((c!=0) && (SearchChar(delimiter,c) == NULLS) && (resultStorageLeft > 0)){
            result[outputIndex] = c;
            outputIndex++;
            inputIndex++;
            c = string[inputIndex];
        }
        // skip separator - too save time avoid calling CompareN - check other reason to have terminatred previous loop
        if ((c!=0) &&  (resultStorageLeft > 0)){
            inputIndex++;
        }

        result[outputIndex] = 0;
        ret = string[inputIndex];
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;
}

CCString  TokenizeByChars(CCString  const string, CCString  const delimiter,DynamicCString result){

    CCString ret(NULLS);

    if ((string.GetList() != NULLS) && (delimiter.GetList() != NULLS) ) {
        uint32 inputIndex = 0;

        // next character to be processed;
        char8 c = string[0];

        while ((c!=0) && (SearchChar(delimiter,c) == NULLS) ){
            result.Append(c);
            inputIndex++;
            c = string[inputIndex];
        }
        // skip separator - too save time avoid calling CompareN - check other reason to have terminatred previous loop
        if (c!=0) {
            inputIndex++;
        }

        ret = string[inputIndex];
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;
}


CCString TokenizeByString(CCString const string,CCString const terminator,CString const result,uint32 resultStorageSize) {

    CCString ret(NULLS);
    uint32 resultStorageLeft = resultStorageSize-1;
    uint32 terminatorSize = terminator.GetSize();

    if ((string.GetList() != NULLS) && (terminator.GetList() != NULLS) && (result.GetList() != NULLS)) {
        uint32 inputIndex = 0;
        uint32 outputIndex = 0;

        // next character to be processed;
        CCString stringP(string.GetList());
        char8 c = string[0];

        while ((c!=0) && (CompareN(terminator,stringP,terminatorSize) != 0) && (resultStorageLeft > 0)){
            result[outputIndex] = c;
            outputIndex++;
            inputIndex++;
            c = string[inputIndex];
            stringP = CCString (string.GetList()+inputIndex);
        }
        // skip separator - too save time avoid calling CompareN - check other reason to have terminatred previous loop
        if ((c!=0) &&  (resultStorageLeft > 0)){
            inputIndex+= terminatorSize;
        }

        result[outputIndex] = 0;
        ret = CCString (string.GetList()+inputIndex);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;

}


#if 0

bool Concatenate(CCString const string1,
                 CCString const string2,
                 CString const result) {

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
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }

    return ret;
}

bool ConcatenateN(CCString const string1,
                  CCString const string2,
                  CString const result,
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
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;
}


bool Substr(const uint32 begin,
            const uint32 end,
            CCString const string,
            CString const result) {

    bool ret = true;
    if ((string == NULL) || (result == NULL) || (end < begin)) {
        ret = false;
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");

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

#endif

}

}
