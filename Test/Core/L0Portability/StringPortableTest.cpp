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

bool StringPortableTest::TestLength(const char* string) {
    //Calculate the length of the string.
    int32 tryLength = StringPortable::Length(string);
    int32 length = StringTestHelper::Size(string);

    //Check if the length is correct.
    if (tryLength != length) {
        return False;
    }

    //Check if the function returns a negative number (-1) in case of NULL argument.
    tryLength = StringPortable::Length(NULL);

    if (tryLength > 0) {
        return False;
    }

    return True;
}

bool StringPortableTest::TestEqual(const char* string,
                                   const char* stringEqual) {

    //Check if strings are equal and if the result is correct.	
    bool tryEqual = StringPortable::Equal(string, stringEqual);
    bool equal = StringTestHelper::Compare(string, stringEqual);

    if (tryEqual != equal) {
        return False;
    }

    const char* string1 = "HelloWorld";
    const char* string2 = "Hello";

    uint32 size = 5;

    //Check the equality until size chars.
    if (!StringPortable::EqualN(string1, string2, size)) {
        return False;
    }

    //Check if it returns false if size is greater than the size of one string.
    if (StringPortable::EqualN(string1, string2, size + 1)) {
        return False;
    }

    //Check the return value in case of NULL arguments.
    if (StringPortable::Equal(NULL, NULL)) {
        return False;
    }

    return True;
}

bool StringPortableTest::TestCopy(const char* string) {

    //Copy string in the buffer.
    char buffer[32];
    if (!StringPortable::Copy(buffer, string)) {
        return False;
    }

    //Check if string and buffer are equals.
    if (!StringTestHelper::Compare(buffer, string)) {
        return False;
    }

    //Test the copy function until sizeToCopy chars.
    const char* stringPartial = "HelloWorld";
    uint32 sizeToCopy = 5;
    if (!StringPortable::CopyN(buffer, stringPartial, sizeToCopy)) {
        return False;
    }

    if (!StringTestHelper::Compare(buffer, "Hello")) {
        return False;
    }

    //If the size is too much, the function copies all string and modifies size.
    sizeToCopy = 20;

    if (!StringPortable::CopyN(buffer, stringPartial, sizeToCopy)) {
        return False;
    }

    if (!StringTestHelper::Compare(buffer, "HelloWorld") || sizeToCopy != 10) {
        return False;
    }

    return True;
}

bool StringPortableTest::TestAppend(const char* string1, const char* string2) {

    char bufferTry[32];
    char buffer[32];

    //Append string2 to string1. In this case since the result parameter (bufferTry) is not a NULL pointer, the first argument
    //can be a const char*.
    if (!StringPortable::Append((char*) string1, string2, bufferTry)) {
        return False;
    }

    StringTestHelper::Append(string1, string2, buffer);

    if (!StringTestHelper::Compare(bufferTry, buffer)) {
        return False;
    }

    //Copy string1 to bufferTry which is a char* pointer.
    if (!StringPortable::Copy(bufferTry, string1)) {
        return False;
    }

    //Since in this case the result argument is NULL, bufferTry is modified.
    if (!StringPortable::Append(bufferTry, string2, NULL)) {
        return False;
    }

    //Check if bufferTry is the concatenate string.
    if (!StringTestHelper::Compare(bufferTry, buffer)) {
        return False;
    }

    char result[32];

    //Test the append with a max number of chars to append.
    if (!StringPortable::Copy(bufferTry, "Hello")) {
        return False;
    }

    if (!StringPortable::Copy(buffer, "World")) {
        return False;
    }

    uint32 sizeToAppend = 2;

    if (!StringPortable::AppendN(bufferTry, buffer, result, sizeToAppend)) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "HelloWo")) {
        return False;
    }

    //If the number of chars to append is too much, all second string is appended and size is modified.
    sizeToAppend = 20;

    if (!StringPortable::AppendN(bufferTry, buffer, result, sizeToAppend)) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "HelloWorld") || sizeToAppend != 5) {
        return False;
    }

    //Test the Cat function.
    if (!StringPortable::Cat(bufferTry, buffer)) {
        return False;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return False;
    }

    //Test the cat function with a max number of chars to append.
    sizeToAppend = 3;

    if (!StringPortable::CatN(bufferTry, buffer, sizeToAppend)) {
        return False;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorldWor")) {
        return False;
    }

    //If the number of chars to append is too much, all second string is appended and size is modified.
    sizeToAppend = 10;

    if (!StringPortable::CatN(bufferTry, buffer, sizeToAppend)) {
        return False;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorldWorWorld")
            || sizeToAppend != 5) {
        return False;
    }

    return True;
}

bool StringPortableTest::TestSearch() {
    const char* buffer = "Hello World";
    const char* retPointer;

    //Since u is not in the string this must be return false
    if (StringPortable::SearchChar(buffer, 'u') != NULL) {
        return False;
    }

    //Test the return pointer (search first occurrence).
    if ((retPointer = StringPortable::SearchChar(buffer, 'l')) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return False;
    }

    //Test the return pointer (search last occurrence).
    if ((retPointer = StringPortable::SearchLastChar(buffer, 'l')) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "ld")) {
        return False;
    }

    if ((retPointer = StringPortable::SearchChars(buffer, " e")) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "ello World")) {
        return False;
    }

    if ((retPointer = StringPortable::SearchChars(retPointer + 1, " e"))
            == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, " World")) {
        return False;
    }

    //Test the search with a list of chars as argument and index as return (return index to the first occurrence).
    const char* charsToSearch = "dow";

    if (StringPortable::SearchIndex(buffer, charsToSearch) != 4) {
        return False;
    }

    //Test if the index is the length if chars are not found.
    charsToSearch = "zug";

    if (StringPortable::SearchIndex(buffer, charsToSearch)
            != StringPortable::Length(buffer)) {
        return False;
    }

    //This substring is not in the buffer.
    const char* substring = "lloW";

    if (StringPortable::SearchSubstr(buffer, substring) != NULL) {
        return False;
    }

    //Test if the return pointer is correct.
    substring = "llo W";

    if ((retPointer = StringPortable::SearchSubstr(buffer, substring))
            == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return False;
    }

    return True;
}

bool StringPortableTest::TestToken() {

    char* buffer1 = (char*) "Hello:World,I am...Giuseppe";
    char result[32];
    const char* charDelimiters = ":,";

    char* retPointer;

    //Test the tokenize giving chars as delimiters.
    if ((retPointer = StringPortable::TokenizeByChars(buffer1, charDelimiters,
                                                      result)) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "Hello")) {
        return False;
    }

    //Thanks to retPointer is simple tokenize again.
    if ((retPointer = StringPortable::TokenizeByChars(retPointer,
                                                      charDelimiters, result))
            == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "World")) {
        return False;
    }

    if ((retPointer = StringPortable::TokenizeByChars(retPointer,
                                                      charDelimiters, result))
            == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "I am...Giuseppe")) {
        return False;
    }

    //Copy buffer1 in a char* to test the destructive tokenize function (the same with NULL in the result argument).
    if (!StringPortable::Copy(result, buffer1)) {
        return False;
    }

    if ((retPointer = StringPortable::TokenizeByChars(result, charDelimiters,
                                                      NULL)) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "Hello")
            || !StringTestHelper::Compare(retPointer,
                                          "World,I am...Giuseppe")) {
        return False;
    }

    //Test the tokenize function with string as delimiter.
    buffer1 = (char*) "Hello:World,I am...Giuseppe";
    const char* stringDelimiter = "...";
    const char* retPointerConst;

    //This substring is not in the buffer.
    if ((retPointerConst = StringPortable::TokenizeByString(buffer1, "....",
                                                            result)) != NULL) {
        return False;
    }

    //Check if the result is correct.
    if ((retPointerConst = StringPortable::TokenizeByString(buffer1,
                                                            stringDelimiter,
                                                            result)) == NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "Hello:World,I am")) {
        return False;
    }

    //Thanks to retPointer we can tokenize again. Since the string is finisced new retPointer become null.
    if ((retPointerConst = StringPortable::TokenizeByString(retPointerConst,
                                                            stringDelimiter,
                                                            result)) != NULL) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "Giuseppe")) {
        return False;
    }

    //Test the substr function.
    if (!StringPortable::Substr(3, 6, buffer1, result)) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "lo:W")) {
        return False;
    }

    //Check if it works at the end of the string.
    if (!StringPortable::Substr(25, 26, buffer1, result)) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "pe")) {
        return False;
    }

    //If the end index is out of bounds returns False but copies in result the end of the string.
    if (StringPortable::Substr(23, 30, buffer1, result)) {
        return False;
    }

    if (!StringTestHelper::Compare(result, "eppe")) {
        return False;
    }

    return True;

}

bool StringPortableTest::TestNull() {

    uint32 size = 2;

    if (StringPortable::Length(NULL) != -1) {
        return False;
    }

    if (StringPortable::Equal(NULL, "ciao")) {
        return False;
    }

    if (StringPortable::EqualN("ciao", NULL, size)) {
        return False;
    }

    char buffer[32];

    if (StringPortable::Append((char*) "Hello", NULL, buffer)) {
        return False;
    }

    if (StringPortable::AppendN(NULL, "World", NULL, size)) {
        return False;
    }

    if (StringPortable::Cat(NULL, "Hello")) {
        return False;
    }

    if (StringPortable::CatN(buffer, NULL, size)) {
        return False;
    }

    if (StringPortable::SearchChar(NULL, 'H') != NULL) {
        return False;
    }

    if (StringPortable::Copy(NULL, "Hello")) {
        return False;
    }

    if (StringPortable::CopyN(buffer, NULL, size)) {
        return False;
    }

    if (StringPortable::SearchIndex(NULL, "hey") != -1) {
        return False;
    }

    if (StringPortable::SearchChars(NULL, "hey") != NULL) {
        return False;
    }

    if (StringPortable::SearchLastChar(NULL, 'c') != NULL) {
        return False;
    }

    if (StringPortable::SearchSubstr("Hello", NULL) != NULL) {
        return False;
    }

    if (StringPortable::TokenizeByChars(NULL, ".,", buffer) != NULL) {
        return False;
    }

    if (StringPortable::TokenizeByString("Hello", NULL, buffer) != NULL) {
        return False;
    }

    if (StringPortable::Substr(-1, 3, "Hello", buffer)) {
        return False;
    }

    return True;
}

