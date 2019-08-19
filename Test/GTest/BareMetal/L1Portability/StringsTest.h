/**
 * @file StringsTest.h
 * @brief Header file for class StringsTest
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

 * @details This header file contains the declaration of the class StringsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGSTEST_H_
#define STRINGSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test StringHelper functions.
 */
namespace  StringsTest {

    /**
     * @brief Tests the CCString::GetSize() function.
     * @param[in] string .
     * @return true if the returned length of the string is correct.
     */
    bool TestLength(CCString string,uint32 expectedSize);

    /**
     * @brief Tests the CCString::IsSame() function.
     * @details Compares two strings
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string that should be equal to the first.
     * @param[in] sameString is the expected result.
     * @return true if the expected result is obtained
     */
    bool TestCompare(CCString string,CCString stringEqual,bool sameString,uint32 maxCompare=0xFFFFFFFF);

    /**
     * @brief Tests the CCString::CompareContent() function.
     * @details Compares two strings .
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string that should be equal to the first.
     * @return true if expectedResult is obtained
     */
    bool TestCompareContent(CCString string,CCString stringEqual,int32 expectedResult,uint32 maxCompare=0xFFFFFFFF);

    /**
     * @brief Tests the AppendN() function.
     * @details Checks if the copied string is equal to the original, then checks the return value of the function with NULL arguments.
     * @param[in] string is the string to copy.
     * @param[in] maxCopy is the range to copy.
     * @param[in] nTypes is how many times to append string.
     * @param[in] isOk is whether we expect the copy to succeed.
     * @return true if all as expected.
     */
    bool TestAppend(CCString string,uint32 maxCopy=0xFFFFFFFF,uint32 nTimes=1,bool isOk=true);

    /**
     * @brief Tests the Find,FindPattern,FindString,FindPatternString functions
     * @details uses either Find or FindPattern depending if the pattern is one character size or not.
     * checks both the Findxxx and FindxxxString functions at the same time
     * @return true if pattern is found on string at the right place or if it is not found and toBeFound is false.
     */
    bool TestFindXXX(CCString string, CCString pattern, uint32 expectedPosition, bool toBeFound=true);

    /**
      * @brief Test StringHelper::TokenizeByChars() function.
      * @details Given a long string tokenize it using different delimiters. The test check the returned
      * pointer, the result where the token is buffered and checks the behaviour using a NULL string.
      * @return true if the string is tokenized and the returned pointer points to the next part of
      * the string to be tokenized.
      */
    bool TestTokenize(CCString string,ZeroTerminatedArray<CCString> expectedTokens,CCString const delimiters,CCString const skipCharacters);
    bool TestTokenizeString(CCString string,ZeroTerminatedArray<CCString> expectedTokens,ZeroTerminatedArray<const CCString> delimiters,CCString const skipCharacters);


}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* StringsTest_H_ */

