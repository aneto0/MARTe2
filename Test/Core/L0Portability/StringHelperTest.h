/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
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

/**
 * @file StringHelperTest.h
 * @brief Tests the StringHelper class and associated functions.
 *
 * The test consists in manipulate strings in different ways trying 
 * to check the StringPortable functions behavior.
 */

#ifndef BASIC_CONSOLE_TEST_H
#define BASIC_CONSOLE_TEST_H

#include "StringHelper.h"

/** @brief A class for testing of StringHelper functions. */
class StringHelperTest {

private:

public:


    /** @brief Tests the Length() function.
     * @param string .
     * @return true if the length of the string is correct. */
    bool TestLength(const char* string);

    /** @brief Tests if two strings are equals.
     * @param string is the first string.
     * @param stringEqual is the second string.
     * @return true if the results of Equal(), EqualN() functions are correct. */
    bool TestCompare(const char* string, const char* stringEqual);

    /** @brief Tests the Copy() function.
     * @param string is the string to copy.
     * @return true if the copied string is equal to the original. */
    bool TestCopy(const char* string);

    /** @brief Tests Append() and Cat() functions.
     * @return true if the concatenate string is correct. */
    bool TestConcatenate();

    /** @brief Tests the different search functions (by char, by string).
     * @return true if the operations return the expected result. */
    bool TestSearch();

//    bool TestToken();

};

#endif
