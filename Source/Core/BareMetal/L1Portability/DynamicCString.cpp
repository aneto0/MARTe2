/**
 * @file DynamicCString.cpp
 * @brief Header file for class AnyType
 * @date 14 Jun 2019
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#define DLL_API
#include "DynamicCString.h"
#include "ErrorManagement.h"


namespace MARTe{

#if 1

static CCString  SearchChar(CCString  const string,  const char8 c) {

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

CCString  DynamicCString::Tokenize(CCString  const string, DynamicCString &token, CCString const delimiters, CCString const skip,bool keepTerm){

    CCString ret;

    if ((!string.IsNullPtr()) && (!delimiters.IsNullPtr()) ) {
        uint32 inputIndex = 0;

        // next character to be processed;
        char8 c = string[0];
        // prepare tool to speed up appending to DynamicCString
        CStringTool tokenST = token();

        while ((c!=0) && (SearchChar(delimiters,c).IsNullPtr()) ){
        	if (SearchChar(skip,c).IsNullPtr())	{
        		tokenST.Append(c);
        	}
            inputIndex++;
            c = string[inputIndex];
        }

        // skip separator - to save time avoid calling CompareN - check other reason to have terminated previous loop
        if (c!=0 && (!keepTerm || (token.GetSize()==0))) {
        	if (SearchChar(skip,c).IsNullPtr())	{
        		tokenST.Append(c);
        	}
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
	uint32 indexU = 0;
	bool found = false;
	while (!matches[indexU].IsNullPtr()  && ! found){

		uint32 size = matches[indexU].GetSize();
		found = (matches[indexU].IsSameAs(string,size));
//		found = (CompareN(matches[static_cast<uint32>(index)],string,matches[static_cast<uint32>(index)].GetSize())==0);
		indexU++;
	}
	int32 ret = static_cast<int32>(indexU);
	if (!found){
		ret = -1;
	}
	return ret;
}


CCString  DynamicCString::Tokenize( CCString  const string,
					DynamicCString &token,
					int32          &limit,
					ZeroTerminatedArray<const CCString> const delimiters,
					CCString const skip){
    CCString ret;

    limit = -1;
    const char8 *stringP = string.GetList();
    if ((stringP != NULL_PTR(char8 *)) && (!delimiters.IsNullPtr()) ) {

    	// prepare tool to speed up appending to DynamicCString
        CStringTool tokenST = token();

        // next character to be processed;
        char8 c = *stringP;
        limit = Match(stringP,delimiters);
        while ((c!=0) && (limit < 0) ){
        	if (SearchChar(skip,c).IsNullPtr())	tokenST.Append(c);
        	stringP++;
            c = *stringP;
            if (c!= 0) {
            	limit = Match(stringP,delimiters);
            }
        }
        // consume terminator
        if (limit >= 0) {
        	CCString matchS = delimiters[static_cast<uint32>(limit)];
        	stringP += matchS.GetSize();
        }

        ret = stringP;
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "StringHelper: invalid input arguments");
    }
    return ret;

}


#endif

} //MARTe
