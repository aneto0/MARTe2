/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
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

#include "GeneralDefinitions.h"
#include "StringHelperTest.h"
#include "StringTestHelper.h"


bool StringHelperTest::TestLength(const char* string) {
    //Calculate the length of the string.
    int32 tryLength = StringHelper::Length(string);
    int32 length = StringTestHelper::Size(string);

    //Check if the length is correct.
    if (tryLength != length) {
        return False;
    }

    //Check if the function returns a negative number (-1) in case of NULL argument.
    tryLength = StringHelper::Length(NULL);

    if (tryLength > 0) {
        return False;
    }

    return True;
}

bool StringHelperTest::TestCompare(const char* string,
                                   const char* stringEqual) {

    //Check if strings are equal and if the result is correct.	
    bool tryEqual = StringHelper::Compare(string, stringEqual) == 0;
    bool equal = StringTestHelper::Compare(string, stringEqual);

    if (tryEqual != equal) {
        return False;
    }

    const char* string1 = "HelloWorld";
    const char* string2 = "Hello";

    uint32 size = 5;

    //Check the equality until size chars.
    if (StringHelper::CompareN(string1, string2, size) != 0) {
        return False;
    }

    //Check if it returns 1 since string2 is minor than string1.
    if (StringHelper::Compare(string2, string1) != 1) {
        return False;
    }

    //Check if it returns 2 since string1 is greater than string2.
    if (StringHelper::CompareN(string1, string2, size + 1) != 2) {
        return False;
    }

    //Check the return value in case of NULL arguments.
    if (StringHelper::Compare(NULL, NULL) != -1) {
        return False;
    }

    return True;
}

bool StringHelperTest::TestCopy(const char* string) {

    //Copy string in the buffer.
    char buffer[32];
    if (!StringHelper::Copy(buffer, string)) {
        return False;
    }

    //Check if string and buffer are equals.
    if (!StringTestHelper::Compare(buffer, string)) {
        return False;
    }

    //Test the copy function until sizeToCopy chars.
    const char* stringPartial = "HelloWorld";
    uint32 sizeToCopy = 5;
    if (!StringHelper::CopyN(buffer, stringPartial, sizeToCopy)) {
        return False;
    }

    if (!StringTestHelper::Compare(buffer, "Hello")) {
        return False;
    }

    //If the size is too much, the function copies all string.
    sizeToCopy = 20;

    if (!StringHelper::CopyN(buffer, stringPartial, sizeToCopy)) {
        return False;
    }

    if (!StringTestHelper::Compare(buffer, "HelloWorld")) {
        return False;
    }

    return True;
}

bool StringHelperTest::TestConcatenate() {

    char bufferTry[32];
    char buffer[32];

    StringHelper::Copy(bufferTry, "Hello");
    StringHelper::Copy(buffer, "World");

    if (!StringHelper::Concatenate(bufferTry, buffer)) {
        return False;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return False;
    }

    //Test the concatenate function with a max number of chars to append.
    uint32 sizeToAppend = 3;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return False;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorldWor")) {
        return False;
    }

    //If the number of chars to append is too much, all second string is appended and size is modified.
    sizeToAppend = 10;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return False;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorldWorWorld")) {
        return False;
    }

    return True;
}

bool StringHelperTest::TestSearch() {
    const char* buffer ="Hello World";
    const char* retPointer;

    //Since u is not in the string this must be return false
    if (StringHelper::SearchChar(buffer, 'u') != NULL) {
        return False;
    }

    //Test the return pointer (search first occurrence).
    if ((retPointer =StringHelper::SearchChar(buffer, 'l')) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return False;
    }

    //Test the return pointer (search last occurrence).
    if ((retPointer = StringHelper::SearchLastChar(buffer, 'l')) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "ld")) {
        return False;
    }

    if ((retPointer = StringHelper::SearchChars(buffer, " e")) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "ello World")) {
        return False;
    }

    if ((retPointer = StringHelper::SearchChars(retPointer + 1, " e"))
            == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, " World")) {
        return False;
    }

    //Test the search with a list of chars as argument and index as return (return index to the first occurrence).
    const char* charsToSearch = "dow";

    if (StringHelper::SearchIndex(buffer, charsToSearch) != 4) {
        return False;
    }

    //Test if the index is the length if chars are not found.
    charsToSearch = "zug";

    if (StringHelper::SearchIndex(buffer, charsToSearch)
            != StringHelper::Length(buffer)) {
        return False;
    }

    //This substring is not in the buffer.
    const char* substring = "lloW";

    if (StringHelper::SearchString(buffer, substring) != NULL) {
        return False;
    }

    //Test if the return pointer is correct.
    substring = "llo W";

    if ((retPointer = StringHelper::SearchString(buffer, substring)) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return False;
    }

    return True;
}
/*
bool StringHelperTest::TestToken() {

    char buffer1[32];
    const char* charDelimiters = ":,";
    char* retPointer;

    StringHelper::Copy(buffer1, "Hello:World,I am...Giuseppe");

    //Test the tokenize giving chars as delimiters.
    if ((retPointer = StringHelper::Tokenizer(buffer1, charDelimiters))
            == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "Hello")) {
        return False;
    }

    //If NULL is the source argument it tokenize from the next pointer.
    if ((retPointer = StringHelper::Tokenizer(NULL, charDelimiters)) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "World")) {
        return False;
    }

    if ((retPointer = StringHelper::Tokenizer(NULL, charDelimiters)) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "I am...Giuseppe")) {
        return False;
    }

    return True;

}*/
