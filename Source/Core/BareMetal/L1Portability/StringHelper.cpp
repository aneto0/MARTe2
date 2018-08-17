/**
 * @file StringHelper.cpp
 * @brief Source file for module StringHelper
 * @date Aug 13, 2015
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

#include "StringHelper.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace StringHelper {

CCString  SearchChar(CCString  const string,  const char8 c) {

    CCString  ret;

    if (!string.IsNullPtr()) {
        bool end = false;

        uint32 i = 0u;

        while (!end) {

            if (string[i] == '\0') {
                end = true;

            }
            if (string[i] == c) {
                end = true;
                ret = string.GetList()+i;
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

    CCString  ret;
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

    if ((!string1.IsNullPtr()) && (!string2.IsNullPtr())) {
        bool end = false;
        int32 i = 0;
        while (!end) {
            char8 c1 = string1[i];
            char8 c2 = string2[i];
            int32 i1 = static_cast<int32>(c1);
            int32 i2 = static_cast<int32>(c2);

            if (i1 > i2) {
                end = true;
                ret = 2;
            } else
            if (i1 < i2) {
                end = true;
                ret = 1;
            } else
            if ((c1 == '\0') && (c2 == '\0')) {
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

    if ((!string1.IsNullPtr()) && (!string2.IsNullPtr())) {
        bool end = false;
        ret = 0;

        uint32 i = 0u;
        while ((!end) && (i < size)) {
            char8 c1 = string1[i];
            char8 c2 = string2[i];
            int32 i1 = static_cast<int32>(c1);
            int32 i2 = static_cast<int32>(c2);

            if (i1 > i2) {
                ret = 2;
                end = true;
            } else
            if (i1 < i2) {
                ret = 1;
                end = true;
            } else
            if ((c1 == '\0') || (c2 == '\0')) {
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

CCString  Tokenize(CCString  const string, DynamicCString &token, CCString const delimiters, CCString const skip,bool keepTerm){

    CCString ret;

    if ((!string.IsNullPtr()) && (!delimiters.IsNullPtr()) ) {
        uint32 inputIndex = 0;

        // next character to be processed;
        char8 c = string[0];

        while ((c!=0) && (SearchChar(delimiters,c).IsNullPtr()) ){
        	if (SearchChar(skip,c).IsNullPtr())	token.Append(c);
            inputIndex++;
            c = string[inputIndex];
        }

        // skip separator - to save time avoid calling CompareN - check other reason to have terminated previous loop
        if (c!=0 && (!keepTerm || (token.GetSize()==0))) {
        	if (SearchChar(skip,c).IsNullPtr())	token.Append(c);
            inputIndex++;
        }

        ret = (string.GetList() + inputIndex);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;
}

static int32 Match(CCString const string, ZeroTerminatedArray<const CCString> const matches){
	int32 index = 0;
	bool found = false;
	while (!matches[index].IsNullPtr()  && ! found){
		found = (CompareN(matches[index],string,matches[index].GetSize())==0);
		index++;
	}
	if (!found){
		index = -1;
	} else {
		index = index -1;
	}
	return index;
}


CCString  Tokenize( CCString  const string,
					DynamicCString &token,
					int32          &limit,
					ZeroTerminatedArray<const CCString> const delimiters,
					CCString const skip){
    CCString ret;

    limit = -1;
    const char8 *stringP = string.GetList();
    if ((stringP != NULL_PTR(char8 *)) && (!delimiters.IsNullPtr()) ) {

        // next character to be processed;
        char8 c = *stringP;
        limit = Match(stringP,delimiters);
        while ((c!=0) && (limit < 0) ){
        	if (SearchChar(skip,c).IsNullPtr())	token.Append(c);
        	stringP++;
            c = *stringP;
            if (c!= 0) {
            	limit = Match(stringP,delimiters);
            }
        }
        // consume terminator
        if (limit >= 0) {
        	CCString matchS = delimiters[limit];
        	stringP += matchS.GetSize();
        }

        ret = stringP;
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;

}


int32 SearchIndex(CCString  const string1, CCString  const string2) {

    int32 ret = -1;

    if ((!string1.IsNullPtr()) && (!string2.IsNullPtr())) {
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
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: Invalid input arguments");
    }
    return ret;
}
 




}

}
