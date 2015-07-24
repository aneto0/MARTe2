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
 * @file StringPortableTest.h
 * @brief Tests the StringPortable class and associated functions.
 *
 * The test consists in manipulate strings in different ways trying 
 * to check the StringPortable functions behavior.
 */

#ifndef BASIC_CONSOLE_TEST_H
#define BASIC_CONSOLE_TEST_H

#include "StringPortable.h"

/** @brief A class for testing of StringPortable functions. */
class StringPortableTest {

private:

public:


    /**
     * @brief Tests the StringPortable::Length function.
     * @param[in] string .
     * @return true if the returned length of the string is correct.
     */
    bool TestLength(const char8 *string);

    /**
     * @brief Tests the StringPortable::Compare function.
     * @details Compares two equal strings and then checks the return value giving different strings.
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string that should be equal to the first.
     * @return true if successful, false otherwise.
     */
    bool TestCompare(const char8 *string,
                     const char8 *stringEqual);

    /**
     * @brief Tests the StringPortable::CompareN function.
     * @details Compares two strings equal at the beginning before with a size such that the return value of the function should assert that they are equal.
     * Then calling the function with a greater size such that the strings are different, checks if the return value of the function is correct.
     * @return true if successful, false otherwise.
     */
    bool TestCompareN();

    /**
     * @brief Tests the StringPortable::Copy function.
     * @details Checks if the copied string is equal to the original, then checks the return value of the function with NULL arguments.
     * @param[in] string is the string to copy.
     * @return true if successful, false otherwise.
     */
    bool TestCopy(const char8 *string);

    /**
     * @brief Tests the StringPortable::CopyN function.
     * @details Checks the return value of the function also using size equal to zero or greater than the length of the string to be copied.
     * @return true if successful, false otherwise.
     */
    bool TestCopyN();

    /**
     * @brief Tests the StringPortable::Cat function.
     * @details Checks if the result of the concatenation is correct, then checks the return value passing NULL arguments.
     * @return true if successful, false otherwise.
     */
    bool TestConcatenateNoResult();

    /**
     * @brief Tests the StringPortable::CatN function.
     * @details Checks the result of the concatenation also using size equal to zero or greater than the string to be appended.
     * @return true if successful, false otherwise.
     */
    bool TestConcatenateNNoResult();

    /**
     * @brief Tests the StringPortable::Concatenate function passing also the result parameter.
     * @details Checks if the result of the concatenation is correct, then checks the return value passing NULL arguments.
     * @return true if successful, false otherwise.
     */
    bool TestConcatenateWithResult();

    /**
     * @brief Tests the StringPortable::ConcatenateN function passing also the result parameter.
     * @details Checks the result of the concatenation also using size equal to zero or greater than the string to be appended.
     * @return true if successful, false otherwise.
     */
    bool TestConcatenateNWithResult();

    /**
     * @brief Tests the StringPortable::SearchChar function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchChar();

    /**
     * @brief Tests the StringPortable::SearchChars function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchChars();

    /**
     * @brief Tests the StringPortable::SearchLastChar function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchLastChar();

    /**
     * @brief Tests the StringPortable::SearchString function
     * @details Checks the return value passing also strings that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchString();

    /**
     * @brief Tests the StringPortable::SearchIndex function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if successful, false otherwise.
     */
    bool TestSearchIndex();

    /**
     * @brief Test the tokenize functions (by string, by chars).
     * @return true if the operations return the expected result.
     */
    bool TestTokenizeByChars();


    bool TestTokenizeByString();
    bool TestSubstr();
    bool TestSetChar();

};

#endif
