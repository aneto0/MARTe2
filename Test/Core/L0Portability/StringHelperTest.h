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
#define 		STRINGHELPERTEST_H_

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
     * @brief Tests the Length() function.
     * @param[in] string .
     * @return true if the length of the string is correct.
     */
    bool TestLength(const char8* string);

    /**
     * @brief Tests if two strings are equals.
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string.
     * @return true if the results of StringHelper::Equal, StringHelper::EqualN return as expected.
     */
    bool TestCompare(const char8* string,
                     const char8* stringEqual);

    /**
     * @brief Tests the Copy() function.
     * @param[in] string is the string to copy.
     * @return true if the copied string is equal to the original.
     */
    bool TestCopy(const char8* string);

    /**
     * @brief Tests StringHelper::Append and StringHelper::Cat functions.
     * @return true if the concatenate string is correct.
     */
    bool TestConcatenate();

    /**
     * @brief Tests the different search functions (by char8, by string).
     * @return true if the operations return the expected result.
     */
    bool TestSearch();



};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGHELPERTEST_H_ */

