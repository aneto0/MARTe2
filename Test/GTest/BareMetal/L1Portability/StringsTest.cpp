/**
 * @file StringsTest.cpp
 * @brief Source file for class StringsTest
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

 * @details This source file contains the definition of all the methods for
 * the class StringsTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ErrorManagement.h"
#include "CompositeErrorManagement.h"
#include "CCString.h"
#include "StringsTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool StringsTest::TestLength(CCString string,uint32 expectedSize) {
    //Check if the length is correct.
    return  (string.GetSize() == expectedSize);
}

bool StringsTest::TestCompare(CCString string,CCString stringEqual, bool sameString,uint32 maxCompare) {

	bool ret = false;
	if (sameString){
		ret = string.IsSameAs(stringEqual,maxCompare);
	} else {
		ret = !string.IsSameAs(stringEqual,maxCompare);
	}

	return ret;
}

bool StringsTest::TestCompareContent(CCString string,CCString stringEqual,int32 expectedResult,uint32 maxCompare) {

	int32 result = string.CompareContent(stringEqual,maxCompare);

	return (result == expectedResult);
}


bool StringsTest::TestAppend(CCString string,uint32 maxCopy,uint32 nTimes, bool isOk) {
	DynamicCString copy;
	ErrorManagement::ErrorType ret ;

	{ // limit the CStringTool existance to a frame
		CStringTool cs = copy();

		for (uint32 n = 0 ; n < nTimes; n++){
			cs.Append(string,maxCopy);
		}
		ret = cs;
	}

	if (ret){

		uint32 testStep = string.GetSize();
		if (testStep > maxCopy) testStep = maxCopy;

		for (uint32 n=0;(n < nTimes) && ret; n++){
			CCString part(&copy[testStep*n]);
			ret.comparisonFailure = !part.IsSameAs(string,testStep);
		}

	} else {
		// did we expect to fail in the copy?
		if (!isOk){
			ret = ErrorManagement::ErrorType();
		}
	}

	return ret;
}


bool StringsTest::TestFindXXX(CCString string, CCString pattern, uint32 expectedPosition, bool toBeFound) {
	ErrorManagement::ErrorType ret;
	uint32 sizeOfPattern = pattern.GetSize();

	CCString foundString;
	uint32 foundIndex    = 0xFFFFFFFFU;

	if (sizeOfPattern == 1){
    	foundString = string.FindString(pattern[0]);
    	foundIndex  = string.Find(pattern[0]);
	} else {
    	foundString = string.FindPatternString(pattern);
    	foundIndex  = string.FindPattern(pattern);
	}

	ret.outOfRange = (foundString.IsNullPtr() == toBeFound);
	REPORT_ERROR(ret,"not found when expected or found when not expected");

	if (ret){
		ret.fatalError = ((foundIndex == 0xFFFFFFFFU) != (foundString.IsNullPtr()));
    	COMPOSITE_REPORT_ERROR(ret,"FindXXXX =",foundIndex," not consistent with FindXXXXString = <",foundString,">");
	}

	if (toBeFound){
		if (ret){
			ret.fatalError = (foundIndex != expectedPosition);
	    	COMPOSITE_REPORT_ERROR(ret,"pattern found at position ",foundIndex," instead of ", expectedPosition);
		}

		if (ret){
			ret.fatalError = !pattern.IsSameAs(foundString,sizeOfPattern);
	    	COMPOSITE_REPORT_ERROR(ret,"FindPattern returns <", foundString,"> instead of <",pattern,">");
		}
	}
	return ret;

}

bool StringsTest::TestTokenize(CCString string,ZeroTerminatedArray<CCString> expectedTokens,CCString const delimiters,CCString const skipCharacters) {
	ErrorManagement::ErrorType ret;
	uint32 limit = 0;
	uint32 tokenNo = 0;


	while ((limit != 0xFFFFFFFF)&& ret){
		DynamicCString token;
		string = DynamicCString::Tokenize(string,token,limit,delimiters,skipCharacters);

		if ((limit != 0xFFFFFFFF) || (token.GetSize() > 0)){
			ret.fatalError = (tokenNo >= expectedTokens.GetSize());
			COMPOSITE_REPORT_ERROR(ret,"Found ",tokenNo+1, " tokens, 1 more than ",expectedTokens.GetSize());

			if (ret){
				ret.comparisonFailure = !token.IsSameAs(expectedTokens[tokenNo]);
				COMPOSITE_REPORT_ERROR(ret,"Found <",token,"> at position ",tokenNo," instead of <",expectedTokens[tokenNo],">");
			}

			if (ret){
				tokenNo++;
			}
		}
	}

	if (ret){
		ret.fatalError = (tokenNo  != expectedTokens.GetSize());
		COMPOSITE_REPORT_ERROR(ret,"Found ",tokenNo," tokens instead of ",expectedTokens.GetSize());
	}

	return ret;
}

bool StringsTest::TestTokenizeString(CCString string,ZeroTerminatedArray<CCString> expectedTokens,ZeroTerminatedArray<const CCString> delimiters,CCString const skipCharacters) {
	ErrorManagement::ErrorType ret;
	uint32 limit = 0;
	uint32 tokenNo = 0;

	while ((limit != 0xFFFFFFFF)&& ret){
		DynamicCString token;
		string = DynamicCString::Tokenize(string,token,limit,delimiters,skipCharacters);

		if ((limit != 0xFFFFFFFF) || (token.GetSize() > 0)){
			ret.fatalError = (tokenNo >= expectedTokens.GetSize());
			COMPOSITE_REPORT_ERROR(ret,"Found ",tokenNo+1, " tokens, 1 more than ",expectedTokens.GetSize());

			if (ret){
				ret.comparisonFailure = !token.IsSameAs(expectedTokens[tokenNo]);
				COMPOSITE_REPORT_ERROR(ret,"Found <",token,"> at position ",tokenNo," instead of <",expectedTokens[tokenNo],">");
			}

			if (ret){
				tokenNo++;
			}
		}
	}

	if (ret){
		ret.fatalError = (tokenNo  != expectedTokens.GetSize());
		COMPOSITE_REPORT_ERROR(ret,"Found ",tokenNo," tokens instead of ",expectedTokens.GetSize());
	}

	return ret;
}

