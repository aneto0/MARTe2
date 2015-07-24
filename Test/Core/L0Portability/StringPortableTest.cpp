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
#include "StringPortableTest.h"
#include "StringTestHelper.h"

bool StringPortableTest::TestLength(const char8* string) {
    //Calculate the length of the string.
    int32 tryLength = StringPortable::Length(string);
    int32 length = StringTestHelper::Size(string);

    //Check if the length is correct.
    if (tryLength != length) {
        return false;
    }

    //Check if the function returns a negative number (-1) in case of NULL argument.
    tryLength = StringPortable::Length(NULL);

    if (tryLength > 0) {
        return false;
    }

    return true;
}

bool StringPortableTest::TestCompare(const char8* string,
                                     const char8* stringCompare) {

    //Check if strings are Compare and if the result is correct.
    bool tryCompare = StringPortable::Compare(string, stringCompare) == 0;
    bool Compare = StringTestHelper::Compare(string, stringCompare);

    if (tryCompare != Compare) {
        return false;
    }

    const char8 *string1 = "HelloWorld";
    const char8 *string2 = "Hello";

    //Check if it returns 1 since string2 is minor than string1.
    if (StringPortable::Compare(string2, string1) != 1) {
        return false;
    }

    //Check if it returns 2 since string1 is minor than string2.
    if (StringPortable::Compare(string1, string2) != 2) {
        return false;
    }

    //Check the return value in case of NULL arguments.
    return (StringPortable::Compare(NULL, NULL) == -1 && StringPortable::Compare(string1, NULL) == -1 && StringPortable::Compare(NULL, string2) == -1);
}

bool StringPortableTest::TestCompareN() {

    const char8 *string1 = "HelloWorld";
    const char8 *string2 = "HelloD";

    uint32 size = 5;

//Check the equality until size chars.
    if (StringPortable::CompareN(string1, string2, size) != 0) {
        return false;
    }

//Check if it returns 2 since string1 is greater than string2.
    if (StringPortable::CompareN(string1, string2, size + 1) != 2) {
        return false;
    }

//Check if it returns 1 since string2 is greater than string1.
    if (StringPortable::CompareN(string2, string1, size + 1) != 1) {
        return false;
    }

    size = 0;
    if (StringPortable::CompareN(string1, string2, size) != 0) {
        return false;
    }

//Check the return value in case of NULL arguments.
    size = 1;
    return (StringPortable::CompareN(NULL, NULL, size) == -1 && StringPortable::CompareN(string1, NULL, size) == -1
            && StringPortable::CompareN(NULL, string2, size) == -1);

}
bool StringPortableTest::TestCopy(const char8* string) {

    //Copy string in the buffer.
    char8 buffer[32];

    if (!StringPortable::Copy(buffer, string)) {
        return false;
    }

    //Check if string and buffer are equals.
    if (!StringTestHelper::Compare(buffer, string)) {
        return false;
    }

    //checks the null return.
    return (!StringPortable::Copy(NULL, string) && !StringPortable::Copy(buffer, NULL) && !StringPortable::Copy(NULL, NULL));
}

bool StringPortableTest::TestCopyN() {
    //Copy string in the buffer.
    char8 buffer[32];
    StringPortable::SetChar(buffer, 32, 0);

    //Test the copy function until sizeToCopy chars.
    const char8 *stringPartial = "HelloWorld";
    uint32 sizeToCopy = 5;
    if (!StringPortable::CopyN(buffer, stringPartial, sizeToCopy)) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, "Hello")) {
        return false;
    }

    //If the size is too much, the function copies all string.
    sizeToCopy = 20;

    if (!StringPortable::CopyN(buffer, stringPartial, sizeToCopy)) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, "HelloWorld")) {
        return false;
    }

    //copy 0 chars
    sizeToCopy = 0;
    if (!StringPortable::CopyN(buffer, stringPartial, sizeToCopy)) {
        return false;
    }

    //it creates an empty string
    if (!StringTestHelper::Compare(buffer, "")) {
        return false;
    }

    sizeToCopy = 1;

    //checks the null return.
    return (!StringPortable::CopyN(NULL, stringPartial, sizeToCopy) && !StringPortable::CopyN(buffer, NULL, sizeToCopy)
            && !StringPortable::CopyN(NULL, NULL, sizeToCopy));
}

bool StringPortableTest::TestConcatenateNoResult() {

    char8 bufferTry[32];

    StringPortable::Copy(bufferTry, "Hello");
    const char8 *buffer = "World";

    if (!StringPortable::Concatenate(bufferTry, buffer)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return false;
    }

    if (!StringPortable::Concatenate(bufferTry, "")) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return false;
    }

    return (!StringPortable::Concatenate(bufferTry, NULL) && !StringPortable::Concatenate(NULL, "HelloWorld") && !StringPortable::Concatenate(NULL, NULL));
}

bool StringPortableTest::TestConcatenateNNoResult() {

    char8 bufferTry[32];

    StringPortable::Copy(bufferTry, "Hello");
    const char8 *buffer = "World";

    //Test the conConcatenateenate function with a max number of chars to Concatenate.
    uint32 sizeToConcatenate = 3;

    if (!StringPortable::ConcatenateN(bufferTry, buffer, sizeToConcatenate)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWor")) {
        return false;
    }

    //If the number of chars to Concatenate is too much, all second string is Concatenateed and size is modified.
    sizeToConcatenate = 10;

    if (!StringPortable::ConcatenateN(bufferTry, buffer, sizeToConcatenate)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorWorld")) {
        return false;
    }

    //size=0
    sizeToConcatenate = 0;

    if (!StringPortable::ConcatenateN(bufferTry, buffer, sizeToConcatenate)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorWorld")) {
        return false;
    }

    sizeToConcatenate = 1;
    return (!StringPortable::ConcatenateN(bufferTry, NULL, sizeToConcatenate) && !StringPortable::ConcatenateN(NULL, buffer, sizeToConcatenate)
            && !StringPortable::ConcatenateN(NULL, NULL, sizeToConcatenate));
}

bool StringPortableTest::TestConcatenateWithResult() {

    const char8 *string1 = "Hello ";
    const char8 *string2 = "World";

    char8 buffer[32];

    //Concatenate string2 to string1. In this case since the result parameter (bufferTry) is not a NULL pointer, the first argument
    //can be a const char8*.
    if (!StringPortable::Concatenate(string1, string2, buffer)) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, "Hello World")) {
        return false;
    }

    //Copy string1 to buffer which is a char8* pointer.
    if (!StringPortable::Copy(buffer, string1)) {
        return false;
    }
    //Check if buffer is equal to string1.
    if (!StringTestHelper::Compare(buffer, "Hello ")) {
        return false;
    }

    //Use the same buffer as output
    if (!StringPortable::Concatenate(buffer, string2, buffer)) {
        return false;
    }

    //Check if buffer is the conConcatenateenate string.
    if (!StringTestHelper::Compare(buffer, "Hello World")) {
        return false;
    }

    return (!StringPortable::Concatenate(NULL, string2, buffer) && !StringPortable::Concatenate(string1, NULL, buffer) && !StringPortable::Concatenate(string1, string2, NULL));
}

bool StringPortableTest::TestConcatenateNWithResult() {
    const char8 *string1 = "Hello";
    const char8 *string2 = "World";

    char8 result[32];

    uint32 sizeToConcatenate = 2;

    if (!StringPortable::ConcatenateN(string1, string2, result, sizeToConcatenate)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "HelloWo")) {
        return false;
    }

//If the number of chars to Concatenate is too much, all second string is Concatenateed and size is modified.
    sizeToConcatenate = 20;

    if (!StringPortable::ConcatenateN(string1, string2, result, sizeToConcatenate)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "HelloWorld") || sizeToConcatenate != 5) {
        return false;
    }

    //Test with size = 0

    sizeToConcatenate = 0;

    if (!StringPortable::ConcatenateN(string1, string2, result, sizeToConcatenate)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Hello")) {
        return false;
    }

    sizeToConcatenate = 1;
    return (!StringPortable::ConcatenateN(NULL, string2, result, sizeToConcatenate) && !StringPortable::ConcatenateN(string1, NULL, result, sizeToConcatenate)
            && !StringPortable::ConcatenateN(string1, string2, NULL, sizeToConcatenate));

}

bool StringPortableTest::TestSearchChar() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    char8 toSearch = 'l';

    if ((retPointer = StringPortable::SearchChar(buffer, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return false;
    }

    //search again on the remained
    if ((retPointer = StringPortable::SearchChar(retPointer + 1, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "lo World")) {
        return false;
    }

    //these chars are not in the string
    char8 invalid = 'x';

    if ((retPointer = StringPortable::SearchChar(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringPortable::SearchChar(NULL, toSearch) == NULL);
}

bool StringPortableTest::TestSearchChars() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    const char8 *toSearch = "rdl";

    if ((retPointer = StringPortable::SearchChars(buffer, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return false;
    }

    //search again on the remained
    if ((retPointer = StringPortable::SearchChars(retPointer + 1, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "lo World")) {
        return false;
    }

    //these chars are not in the string
    const char8 *invalid = "xpqg";

    //returns null if the character is not found
    if ((retPointer = StringPortable::SearchChars(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringPortable::SearchChars(NULL, NULL) == NULL && StringPortable::SearchChars(buffer, NULL) == NULL
            && StringPortable::SearchChars(NULL, toSearch) == NULL);
}

bool StringPortableTest::TestSearchLastChar() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    char8 toSearch = 'l';

    if ((retPointer = StringPortable::SearchLastChar(buffer, toSearch)) == NULL) {
        return false;
    }
    if (!StringTestHelper::Compare(retPointer, "ld")) {
        return false;
    }

    char8 invalid = 'x';

    if ((retPointer = StringPortable::SearchLastChar(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringPortable::SearchLastChar(NULL, toSearch) == NULL);

}

bool StringPortableTest::TestSearchString() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    const char8 *toSearch = "orl";

    if ((retPointer = StringPortable::SearchString(buffer, toSearch)) == NULL) {
        return false;
    }
    if (!StringTestHelper::Compare(retPointer, "orld")) {
        return false;
    }

    const char8 *invalid = "Worfd";

    if ((retPointer = StringPortable::SearchString(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringPortable::SearchString(NULL, NULL) == NULL && StringPortable::SearchString(buffer, NULL) == NULL
            && StringPortable::SearchString(NULL, toSearch) == NULL);
}

bool StringPortableTest::TestSearchIndex() {
    const char8 *buffer = "Hello World";

    const char8 *toSearch = "ll";

    if (StringPortable::SearchIndex(buffer, toSearch) != 2 && !StringTestHelper::Compare(buffer + 2, "llo World")) {
        return false;
    }

    //these chars are not in the string
    const char8 *invalid = "xpqg";

    if (StringPortable::SearchIndex(buffer, invalid) != StringPortable::Length(buffer)) {
        return false;
    }

    //NULL pointer arguments
    return (StringPortable::SearchIndex(NULL, NULL) == -1 && StringPortable::SearchIndex(buffer, NULL) == -1
            && StringPortable::SearchIndex(NULL, toSearch) == -1);
}

bool StringPortableTest::TestTokenizeByChars() {

    const char8* string = "Hello:World,I am...Giuseppe";
    char8 result[32];
    const char8* charDelimiters = ":,";

    const char8* retPointer;

    //Test the tokenize giving char8s as delimiters.
    if ((retPointer = StringPortable::TokenizeByChars(string, charDelimiters, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Hello")) {
        return false;
    }

    //Thanks to retPointer is simple tokenize again.
    if ((retPointer = StringPortable::TokenizeByChars(retPointer, charDelimiters, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "World")) {
        return false;
    }

    if ((retPointer = StringPortable::TokenizeByChars(retPointer, charDelimiters, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "I am...Giuseppe")) {
        return false;
    }

    return (StringPortable::TokenizeByChars(retPointer, charDelimiters, NULL) == NULL) && (StringPortable::TokenizeByChars(retPointer, NULL, result) == NULL)
            && StringPortable::TokenizeByChars(NULL, charDelimiters, result) == NULL;

}

bool StringPortableTest::TestTokenizeByString() {

    //Test the tokenize function with string as delimiter.
    const char8 *string = "Hello:World,I am...Giuseppe";
    const char8* stringDelimiter = "...";
    const char8* retPointerConst;

    char8 result[32];

    //This substring is not in the buffer.
    if ((retPointerConst = StringPortable::TokenizeByString(string, "....", result)) != NULL) {
        return false;
    }

    //Check if the result is correct.
    if ((retPointerConst = StringPortable::TokenizeByString(string, stringDelimiter, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Hello:World,I am")) {
        return false;
    }

    //Thanks to retPointer we can tokenize again. Since the string is finished new retPointer become null.
    if ((retPointerConst = StringPortable::TokenizeByString(retPointerConst, stringDelimiter, result)) != NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Giuseppe")) {
        return false;
    }

    return (StringPortable::TokenizeByString(string, stringDelimiter, NULL) == NULL) && (StringPortable::TokenizeByString(string, NULL, result) == NULL)
            && StringPortable::TokenizeByString(NULL, stringDelimiter, result) == NULL;

}

bool StringPortableTest::TestSubstr() {
    const char8* string = "Hello:World,I am...Giuseppe";
    char8 result[32];

    //Test the substr function.
    if (!StringPortable::Substr(3, 6u, string, result)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "lo:W")) {
        return false;
    }

    //Check if it works at the end of the string.
    if (!StringPortable::Substr(25, 26, string, result)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "pe")) {
        return false;
    }

    //If the end index is out of bounds returns false but copies in result the end of the string.
    if (StringPortable::Substr(23, 30, string, result)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "eppe")) {
        return false;
    }

    return !StringPortable::Substr(1, 0, string, result) && !StringPortable::Substr(1, 1, NULL, result) && !StringPortable::Substr(1, 1, string, NULL);

}

bool StringPortableTest::TestSetChar() {

    bool retValue;
    uint32 i = 0u;
    char8 buffer[30] = "length7";
    const uint32 size = 3u;
    retValue = StringPortable::SetChar(buffer, size, 'a');
    while (i < size) {
        retValue &= (*(buffer + i) == 'a');
        i++;
    }
    retValue &= (*(buffer + 3) == 'g');
    retValue &= (*(buffer + 4) == 't');
    retValue &= (*(buffer + 5) == 'h');
    retValue &= (*(buffer + 6) == '7');

    StringPortable::SetChar(buffer, 0, 'b');

    retValue&=buffer[0]=='a';

    return retValue && !StringPortable::SetChar(NULL, size, 'a');

}

