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

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test StringHelper functions.
 */
class StringHelperTest {

public:

    /**
     * @brief Tests the StringHelper::Length() function.
     * @param[in] string .
     * @return true if the returned length of the string is correct.
     */
    bool TestLength(const char8 *string);

    /**
     * @brief Tests the StringHelper::Compare() function.
     * @details Compares two equal strings and then checks the return value giving different strings.
     * @param[in] string is the first string.
     * @param[in] stringEqual is the second string that should be equal to the first.
     * @return true if the StringHelper::Compare returns the expected value (0 if equal strings, 1 if
     * string1 < string2 and 2 if string1 > string2)
     */
    bool TestCompare(const char8 *string,
                     const char8 *stringEqual);

    /**
     * @brief Tests the StringHelper::CompareN() function.
     * @details Compares, using different size, two strings only equal for the first 5 characters and checks the result of the
     * comparison for each size.
     * @return true if StringHelper::Compare() returns the expected value (0 if equal strings, 1 if
     * string1 < string2 and 2 if string1 > string2).
     */
    bool TestCompareN();

    /**
     * @brief Tests the StringHelper::Copy() function.
     * @details Checks if the copied string is equal to the original, then checks the return value of the function with NULL arguments.
     * @param[in] string is the string to copy.
     * @return true if tringHelper::Copy() returns true and the copy is equal than the source and a copy of a NULL returns false.
     */
    bool TestCopy(const char8 *string);

    /**
     * @brief Tests the StringHelper::CopyN() function.
     * @details Checks the return value of the function also using size equal to zero or greater than the
     * length of the string to be copied.
     * @return true if StringHelper::CopyN() returns true and the the N characters are correctly copied.
     */
    bool TestCopyN();

    /**
     * @brief Tests the StringHelper::Concatenate() function.
     * @details Checks if the result of the concatenation is correct, then checks the return value passing
     * NULL arguments.
     * @return true if StringHelper::Concatenate() returns true when no NULL is used and the concatenation
     * string is what as expected.
     */
    bool TestConcatenate();

    /**
     * @brief Tests the StringHelper::ConcatenateN() function.
     * @details Checks the result of the concatenation also using size equal to zero or greater than the
     * string to be appended.
     * @return true if StringHelper::Concatenate() returns true when no NULL is used and the concatenation
     * string is what as expected.
     */
    bool TestConcatenateN();

    /**
     * @brief Tests the StringHelper::SearchChar function
     * @details Checks the return value  of StringHelper::SearchChar passing a character which is in
     * the string and another which is not in the string.
     * @return true if the returned value of StringHelper::SearchChar is a not a NULL pointer and it
     * points the searched character and returns NULL pointer when search for non existing character.
     */
    bool TestSearchChar();

    /**
     * @brief Tests the StringHelper::SearchChars function
     * @details Checks the return value of StringHelper::SearchChars passing a subset characters which
     * are in the string and another subset which are not in the string.
     * @return true if the returned value of StringHelper::SearchChars is a not a NULL pointer and it
     * points the first character of the subset searched character and returns NULL pointer when search
     * for non existing subset of character.
     */
    bool TestSearchChars();

    /**
     * @brief Tests the StringHelper::SearchLastChar function
     * @details Checks the return value  of StringHelper::SearchLastChar passing a character which is in
     * the string and another which is not in the string.
     * @return true if the returned value of StringHelper::SearchLastChar is a not a NULL pointer and it
     * points the searched character and returns NULL pointer when search for non existing character.
     */
    bool TestSearchLastChar();

    /**
     * @brief Tests the StringHelper::SearchString function
     * @details Checks the return value of StringHelper::SearchString passing a subset characters which
     * are in the string and another subset which are not in the string.
     * @return true if the returned value of StringHelper::SearchString is a not a NULL pointer and it
     * points the first character of the subset searched character and returns NULL pointer when search
     * for non existing subset of character.
     */
    bool TestSearchString();

    /**
     * @brief Tests the StringHelper::SearchIndex() function
     * @details Checks the return value of StringHelper::SearchIndex() passing a subset of characters
     * which are in the string and another subset which are not in the string.
     * @return true if the returned value of StringHelper::SearchIndex() is the expected position and
     * the index is the length of the string when the substring is not found and the position is -1
     * when one of the input strings is NULL.
     */
    bool TestSearchIndex();

    /**
       * @brief Test StringHelper::TokenizeByChars() function.
       * @details Given a long string tokenize it using different delimiters. The test check the returned
       * pointer, the result where the token is buffered and checks the behaviour using a NULL string.
       * @return true if the string is tokenized and the returned pointer points to the next part of
       * the string to be tokenized.
       */
      bool TestTokenizeByChars();

      /**
       * @brief Test StringHelper::TokenizeByString() function.
       * @details Given a long string tokenize it using string delimiter. The test check the returned
       * pointer, the result where the token is buffered and checks the behaviour using a NULL string.
       * @return true if the string is tokenized and the returned pointer points to the next part of
       * the string to be tokenized.
       */
      bool TestTokenizeByString();

      /**
       * @brief Test StringHelper::Substr() function.
       * @details Given a string subtract part of it in different positions. Also checks the behaviour
       * for a NULL string.
       * @return true if the function returns true, and the result contains part of the string and returns
       * false when a NULL string or invalid indexes are used.
       */
      bool TestSubstr();

    /**
     * @brief Test the StringHelper::SetChar() function.
     * @detail Try to overwrite the first three characters of a string, and then checks if only the
     * specified number of characters are overwrite.
     * @return true if the string after overwrite is 'aaagth7' and the returned value of
     * StringHelper::SetChar() is true.
     */
    bool TestSetChar();

    /**
     * @brief Test the StringHelper::SetChar() function.
     * @detail Try to overwrite 0 characters (size = 0) of a string, and then checks the returned value
     * @return true if the returned value StringHelper::SetChar is false.
     */
    bool TestSetChar0Size();

    /**
     * @brief Test the StringHelper::SetChar() function.
     * @detail Try to overwrite the first three of a string with the string = NULL, and then checks
     * the returned value
     * @return true if the returned value StringHelper::SetChar is false.
     */
    bool TestSetCharNull();

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

#endif /* STRINGHELPERTEST_H_ */

