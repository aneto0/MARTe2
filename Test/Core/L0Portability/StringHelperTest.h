/**
 * @file StringHelperTest.h
 * @brief Header file for class StringHelperTest
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

 * @details This header file contains the declaration of the class StringHelperTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGHELPERTEST_H_
#define STRINGHELPERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test StringHelper functions.
 */
class StringHelperTest {

private:

public:

    /**
     * @brief Tests the StringHelper::Length function.
     * @param[in] string .
     * @return true if the returned length of the string is correct.
     */
    bool TestLength(const char8 *string);

    /**
     * @brief Tests the StringHelper::Compare function.
     * @details Compares two equal strings and then checks the return value giving different strings.
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string that should be equal to the first.
     * @return true if successful, false otherwise.
     */
    bool TestCompare(const char8 *string,
                     const char8 *stringEqual);

    /**
     * @brief Tests the StringHelper::CompareN function.
     * @details Compares two strings equal at the beginning before with a size such that the return value of the function should assert that they are equal.
     * Then calling the function with a greater size such that the strings are different, checks if the return value of the function is correct.
     * @return true if successful, false otherwise.
     */
    bool TestCompareN();

    /**
     * @brief Tests the StringHelper::Copy function.
     * @details Checks if the copied string is equal to the original, then checks the return value of the function with NULL arguments.
     * @param[in] string is the string to copy.
     * @return true if successful, false otherwise.
     */
    bool TestCopy(const char8 *string);

    /**
     * @brief Tests the StringHelper::CopyN function.
     * @details Checks the return value of the function also using size equal to zero or greater than the length of the string to be copied.
     * @return true if successful, false otherwise.
     */
    bool TestCopyN();

    /**
     * @brief Tests the StringHelper::Concatenate function.
     * @details Checks if the result of the concatenation is correct, then checks the return value passing NULL arguments.
     * @return true if successful, false otherwise.
     */
    bool TestConcatenate();

    /**
     * @brief Tests the StringHelper::ConcatenateN function.
     * @details Checks the result of the concatenation also using size equal to zero or greater than the string to be appended.
     * @return true if successful, false otherwise.
     */
    bool TestConcatenateN();

    /**
     * @brief Tests the StringHelper::SearchChar function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchChar();

    /**
     * @brief Tests the StringHelper::SearchChars function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchChars();

    /**
     * @brief Tests the StringHelper::SearchLastChar function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchLastChar();

    /**
     * @brief Tests the StringHelper::SearchString function
     * @details Checks the return value passing also strings that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchString();

    /**
     * @brief Tests the StringHelper::SearchIndex function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchIndex();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGHELPERTEST_H_ */

