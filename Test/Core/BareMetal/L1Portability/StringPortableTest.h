/**
 * @file StringPortableTest.h
 * @brief Header file for class StringPortableTest
 * @date 27/07/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class StringPortableTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGPORTABLETEST_H_
#define STRINGPORTABLETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class for testing of StringPortable functions
 */
class StringPortableTest {

public:

    /**
     * @brief Default constructor
     */
    StringPortableTest();

    /**
     * @brief Destructor
     */
    ~StringPortableTest();

    /**
     * @brief Tests the StringPortable::Length() function.
     * @details Checks the length for an arbitrary string and for the NULL.
     * @param[in] string .
     * @return true if the returned length of the string is as expected when string = "arbitrary string"
     * and -1 when string = NULL.
     */
    bool TestLength(const char8 *string);

    /**
     * @brief Tests the StringPortable::Compare() function.
     * @details Compares strings in the following cases: string1 = string2, string1 > string2,
     * string1 < string2, string1 = NULL and string2 = NULL.
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string that should be equal to the first.
     * @return true if the result of the comparison is as expected and if a comparison with a NULL returns
     * -1.
     */
    bool TestCompare(const char8 *string,
                     const char8 *stringEqual);

    /**
     * @brief Tests the StringPortable::CompareN() function.
     * @details Compares strings up to certain size in the following cases: string1 = string2,
     *  string1 > string2, string1 < string2, string1 = NULL and string2 = NULL.
     * @return true if the result of the comparison is as expected and if a comparison with a NULL returns
     * -1.
     */
    bool TestCompareN();

    /**
     * @brief Tests the StringPortable::CompareNoCaseSensN() function.
     */
    bool TestCompareNoCaseSensN();

    /**
     * @brief Tests the StringPortable::Concatenate() function passing also the result parameter.
     * @details Checks if the result of the concatenation is correct, then checks the return value
     * passing NULL arguments.
     * @return true if the returned value of the concatenation is true and result is the correct
     * concatenations of the two input strings.
     */
    bool TestConcatenateWithResult();

    /**
     * @brief Tests the StringPortable::ConcatenateN() function passing also the result parameter.
     * @details Checks the result of the concatenation also using size equal to zero or greater than
     * the string to be appended.
     * @return true if the returned value of the concatenations is true (for No NULL strings) and result
     *  is the correct concatenations of the two input strings.
     */
    bool TestConcatenateNWithResult();

    /**
     * @brief Tests the StringPortable::Concatenate() function.
     * @details Checks if the result of the concatenation is correct, then checks the return value
     * passing NULL arguments.
     * @return true if the returned value of the concatenations is true (for No NULL strings) and
     * the first string is the correct concatenations of the first and second input strings.
     */
    bool TestConcatenateNoResult();

    /**
     * @brief Tests the StringPortable::ConcatenateN() function.
     * @details Checks the result of the concatenation also using size equal to zero or greater than the string to be appended.
     * @return true if the returned value of the concatenations is true (for No NULL strings) and
     * the first string is the correct concatenations of the first and second input strings.
     */
    bool TestConcatenateNNoResult();

    /**
     * @brief Tests the StringPortable::SearchChar() function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if the returned pointer points the searched character which is in the string
     * and returns a NULL pointer if the character is not in the string.
     */
    bool TestSearchChar();

    /**
     * @brief Tests the StringPortable::Copy() function.
     * @details Checks if the copied string is equal to the original, then checks the return value of
     * the function with NULL arguments.
     * @param[in] string is the string to copy.
     * @return true if the returned value of the copy, for a non NULL string, is true and the  copy is
     * equal to the original string and the copy for a NULL string returns false.
     */
    bool TestCopy(const char8 *string);

    /**
     * @brief Tests the StringPortable::CopyN() function.
     * @details Checks the return value of the function also using size equal to zero or greater than the length of the string to be copied.
     * @return true if the returned value of the copy, for a non NULL string, is true and the  copy is
     * equal to the original string and the copy for a NULL string returns false.
     */
    bool TestCopyN();

    /**
     * @brief Tests the StringPortable::SearchIndex() function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if the returned position is as expected and the string is not overwrite and the
     * returned value for a NULL string is -1.
     */
    bool TestSearchIndex();

    /**
     * @brief Tests the StringPortable::SearchChars() function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if the returned pointer points to the first character search in the case that
     * the substring is contained in the string and returns a NULL pointer in the case that the substring
     * is not contained in the string.
     */
    bool TestSearchChars();

    /**
     * @brief Tests the StringPortable::SearchLastChar() function
     * @details Checks the return value passing also characters that are not in the string.
     * @return true if the returned pointer points to the last character searched and returns a NULL
     * pointer when the search character is not contained in the string.
     */
    bool TestSearchLastChar();

    /**
     * @brief Tests the StringPortable::SearchString() function
     * @details Checks the return value passing also strings that are not in the string.
     * @return true if the returned pointer points to the first character search in the case that
     * the substring is contained in the string and returns a NULL pointer in the case that the substring
     * is not contained in the string.
     */
    bool TestSearchString();

    /**
     * @brief Test StringPortable::TokenizeByChars() function.
     * @details Given a long string tokenize it using different delimiters. The test check the returned
     * pointer, the result where the token is buffered and checks the behaviour using a NULL string.
     * @return true if the string is tokenized and the returned pointer points to the next part of
     * the string to be tokenized.
     */
    bool TestTokenizeByChars();

    /**
     * @brief Test StringPortable::TokenizeByString() function.
     * @details Given a long string tokenize it using string delimiter. The test check the returned
     * pointer, the result where the token is buffered and checks the behaviour using a NULL string.
     * @return true if the string is tokenized and the returned pointer points to the next part of
     * the string to be tokenized.
     */
    bool TestTokenizeByString();

    /**
     * @brief Test StringPortable::Substr() function.
     * @details Given a string subtract part of it in different positions. Also checks the behaviour
     * for a NULL string.
     * @return true if the function returns true, and the result contains part of the string and returns
     * false when a NULL string or invalid indexes are used.
     */
    bool TestSubstr();

    /**
     * @brief Test StringPortable::SetChar() function
     * @details Initialize a string then replace some of the characters using SetChar checking the result
     * of the operation. Also checks the behaviour using a NULL string.
     * @return true if SetCha() returns true and the only n-first characters are replaced and return
     * false when a NULL string is used.
     */
    bool TestSetChar();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGPORTABLETEST_H_ */

