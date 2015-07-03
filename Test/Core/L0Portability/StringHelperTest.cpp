/**
 * @file StringHelperTest.cpp
 * @brief Source file for class StringHelperTest
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
 * the class StringHelperTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "StringHelperTest.h"
#include "StringTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool StringHelperTest::TestLength(const char8* string) {
    //Calculate the length of the string.
    int32 tryLength = StringHelper::Length(string);
    int32 length = StringTestHelper::Size(string);

    //Check if the length is correct.
    if (tryLength != length) {
        return false;
    }

    //Check if the function returns a negative number (-1) in case of NULL argument.
    tryLength = StringHelper::Length(NULL);

    if (tryLength > 0) {
        return false;
    }

    return true;
}

bool StringHelperTest::TestCompare(const char8* string,
                                   const char8* stringEqual) {

    //Check if strings are equal and if the result is correct.
    bool tryEqual = StringHelper::Compare(string, stringEqual) == 0;
    bool equal = StringTestHelper::Compare(string, stringEqual);

    if (tryEqual != equal) {
        return false;
    }

    const char8* string1 = "HelloWorld";
    const char8* string2 = "Hello";

    uint32 size = 5;

    //Check the equality until size chars.
    if (StringHelper::CompareN(string1, string2, size) != 0) {
        return false;
    }

    //Check if it returns 1 since string2 is minor than string1.
    if (StringHelper::Compare(string2, string1) != 1) {
        return false;
    }

    //Check if it returns 2 since string1 is greater than string2.
    if (StringHelper::CompareN(string1, string2, size + 1) != 2) {
        return false;
    }

    //Check the return value in case of NULL arguments.
    if (StringHelper::Compare(NULL, NULL) != -1) {
        return false;
    }

    return true;
}

bool StringHelperTest::TestCopy(const char8* string) {

    //Copy string in the buffer.
    char8 buffer[32];
    if (!StringHelper::Copy(buffer, string)) {
        return false;
    }

    //Check if string and buffer are equals.
    if (!StringTestHelper::Compare(buffer, string)) {
        return false;
    }

    //Test the copy function until sizeToCopy chars.
    const char8* stringPartial = "HelloWorld";
    uint32 sizeToCopy = 5;
    if (!StringHelper::CopyN(buffer, stringPartial, sizeToCopy)) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, "Hello")) {
        return false;
    }

    //If the size is too much, the function copies all string.
    sizeToCopy = 20;

    if (!StringHelper::CopyN(buffer, stringPartial, sizeToCopy)) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, "HelloWorld")) {
        return false;
    }

    return true;
}

bool StringHelperTest::TestConcatenate() {

    char8 bufferTry[32];
    char8 buffer[32];

    StringHelper::Copy(bufferTry, "Hello");
    StringHelper::Copy(buffer, "World");

    if (!StringHelper::Concatenate(bufferTry, buffer)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return false;
    }

    //Test the concatenate function with a max number of chars to append.
    uint32 sizeToAppend = 3;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorldWor")) {
        return false;
    }

    //If the number of chars to append is too much, all second string is appended and size is modified.
    sizeToAppend = 10;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorldWorWorld")) {
        return false;
    }

    return true;
}

bool StringHelperTest::TestSearch() {
    const char8* buffer = "Hello World";
    const char8* retPointer;

    //Since u is not in the string this must be return false
    if (StringHelper::SearchChar(buffer, 'u') != NULL) {
        return false;
    }

    //Test the return pointer (search first occurrence).
    if ((retPointer = StringHelper::SearchChar(buffer, 'l')) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return false;
    }

    //Test the return pointer (search last occurrence).
    if ((retPointer = StringHelper::SearchLastChar(buffer, 'l')) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "ld")) {
        return false;
    }

    if ((retPointer = StringHelper::SearchChars(buffer, " e")) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "ello World")) {
        return false;
    }

    if ((retPointer = StringHelper::SearchChars(retPointer + 1, " e")) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, " World")) {
        return false;
    }

    //Test the search with a list of chars as argument and index as return (return index to the first occurrence).
    const char8* charsToSearch = "dow";

    if (StringHelper::SearchIndex(buffer, charsToSearch) != 4) {
        return false;
    }

    //Test if the index is the length if chars are not found.
    charsToSearch = "zug";

    if (StringHelper::SearchIndex(buffer, charsToSearch) != StringHelper::Length(buffer)) {
        return false;
    }

    //This substring is not in the buffer.
    const char8* substring = "lloW";

    if (StringHelper::SearchString(buffer, substring) != NULL) {
        return false;
    }

    //Test if the return pointer is correct.
    substring = "llo W";

    if ((retPointer = StringHelper::SearchString(buffer, substring)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return false;
    }

    return true;
}
