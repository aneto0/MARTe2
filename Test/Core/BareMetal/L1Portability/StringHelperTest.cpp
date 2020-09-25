/**
 * @file StringHelperTest.cpp
 * @brief Source file for class StringHelperTest
 * @date 30/06/2015
 * @author Giuseppe Ferro
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

bool StringHelperTest::TestLength(const char8 *string) {
    //Calculate the length of the string.
    int32 tryLength = StringHelper::Length(string);
    int32 length = StringTestHelper::Size(string);

    //Check if the length is correct.
    if (tryLength != length) {
        return false;
    }

    //Check if the function returns 0 in case of NULL argument.
    tryLength = StringHelper::Length(NULL);

    return (tryLength == 0);

}

bool StringHelperTest::TestCompare(const char8 *string,
                                   const char8 *stringEqual) {

    //Check if strings are equal and if the result is correct.
    bool tryEqual = StringHelper::Compare(string, stringEqual) == 0;
    bool equal = StringTestHelper::Compare(string, stringEqual);

    if (tryEqual != equal) {
        return false;
    }

    const char8 *string1 = "HelloWorld";
    const char8 *string2 = "Hello";

    //Check if it returns 1 since string2 is minor than string1.
    if (StringHelper::Compare(string2, string1) != 1) {
        return false;
    }

    //Check if it returns 2 since string1 is minor than string2.
    if (StringHelper::Compare(string1, string2) != 2) {
        return false;
    }

    //Check the return value in case of NULL arguments.
    return (StringHelper::Compare(NULL, NULL) == -1 && StringHelper::Compare(string1, NULL) == -1 && StringHelper::Compare(NULL, string2) == -1);
}

bool StringHelperTest::TestCompareN() {

    const char8 *string1 = "HelloWorld";
    const char8 *string2 = "HelloD";

    uint32 size = 5;

    //Check the equality until size chars.
    if (StringHelper::CompareN(string1, string2, size) != 0) {
        return false;
    }

    //Check if it returns 2 since string1 is greater than string2.
    if (StringHelper::CompareN(string1, string2, size + 1) != 2) {
        return false;
    }

    //Check if it returns 1 since string2 is greater than string1.
    if (StringHelper::CompareN(string2, string1, size + 1) != 1) {
        return false;
    }

    size = 0;
    if (StringHelper::CompareN(string1, string2, size)) {
        return false;
    }

    //Check the return value in case of NULL arguments.
    size = 1;
    return (StringHelper::CompareN(NULL, NULL, size) == -1 && StringHelper::CompareN(string1, NULL, size) == -1
            && StringHelper::CompareN(NULL, string2, size) == -1);
}

bool StringHelperTest::TestToUpper() {
	
	// Check conversion to uppercase
	char8 string1[] = "helloworld";
	const char8* result1 = "HELLOWORLD";
	
	StringHelper::ToUpper(string1);
	
	if (StringHelper::Compare(string1, result1) != 0)
		return false;
	
	// Check if uppercase and special characters are left as they are
	char8 string2[] = "HelloWorld123 !£$%&()=?^\n";
	const char8* result2 = "HELLOWORLD123 !£$%&()=?^\n";
	
	StringHelper::ToUpper(string2);
	
	if (StringHelper::Compare(string2, result2) != 0)
		return false;
	
	// Check return false in case of NULL argument
	if (StringHelper::ToUpper(NULL) != false)
		return false;
	
	return true;

}

bool StringHelperTest::TestToLower() {
	
	// Check conversion to uppercase
	char8 string1[] = "HELLOWORLD";
	const char8* result1 = "helloworld";
	
	StringHelper::ToLower(string1);
	
	if (StringHelper::Compare(string1, result1) != 0)
		return false;
	
	// Check if lowercase and special characters are left as they are
	char8 string2[] = "hELLOwORLD123 !£$%&()=?^\n";
	const char8* result2 = "helloworld123 !£$%&()=?^\n";
	
	StringHelper::ToLower(string2);
	
	if (StringHelper::Compare(string2, result2) != 0)
		return false;
	
	// Check return false in case of NULL argument
	if (StringHelper::ToLower(NULL) != false)
		return false;
	
	return true;

}

bool StringHelperTest::TestCompareNoCaseSensN() {

    const char8 *string1 = "HelloWorld";
    const char8 *string2 = "hElLOD";

    uint32 size = 5;

    //Check the equality until size chars.
    if (StringHelper::CompareNoCaseSensN(string1, string2, size) != 0) {
        return false;
    }

    //Check if it returns 2 since string1 is greater than string2.
    if (StringHelper::CompareNoCaseSensN(string1, string2, size + 1) != 2) {
        return false;
    }

    //Check if it returns 1 since string2 is greater than string1.
    if (StringHelper::CompareNoCaseSensN(string2, string1, size + 1) != 1) {
        return false;
    }

    size = 0;
    if (StringHelper::CompareNoCaseSensN(string1, string2, size)) {
        return false;
    }

    //Check the return value in case of NULL arguments.
    size = 1;
    return (StringHelper::CompareNoCaseSensN(NULL, NULL, size) == -1 && StringHelper::CompareNoCaseSensN(string1, NULL, size) == -1
            && StringHelper::CompareNoCaseSensN(NULL, string2, size) == -1);
}

bool StringHelperTest::TestCopy(const char8 *string) {

    //Copy string in the buffer.
    char8 buffer[32];

    if (!StringHelper::Copy(buffer, string)) {
        return false;
    }

    //Check if string and buffer are equals.
    if (!StringTestHelper::Compare(buffer, string)) {
        return false;
    }

    //checks the null return.
    return (!StringHelper::Copy(NULL, string) && !StringHelper::Copy(buffer, NULL) && !StringHelper::Copy(NULL, NULL));
}

bool StringHelperTest::TestCopyN() {
    //Copy string in the buffer.
    char8 buffer[32];
    StringHelper::SetChar(buffer, 32, 0);

    //Test the copy function until sizeToCopy chars.
    const char8 *stringPartial = "HelloWorld";
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

    //copy 0 chars
    sizeToCopy = 0;
    if (!StringHelper::CopyN(buffer, stringPartial, sizeToCopy)) {
        return false;
    }

    //nothing should be happen
    if (!StringTestHelper::Compare(buffer, "HelloWorld")) {
        return false;
    }

    sizeToCopy = 1;

    //checks the null return.
    return (!StringHelper::CopyN(NULL, stringPartial, sizeToCopy) && !StringHelper::CopyN(buffer, NULL, sizeToCopy)
            && !StringHelper::CopyN(NULL, NULL, sizeToCopy));
}

bool StringHelperTest::TestConcatenate() {

    char8 bufferTry[32];

    StringHelper::Copy(bufferTry, "Hello");
    const char8 *buffer = "World";

    if (!StringHelper::Concatenate(bufferTry, buffer)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return false;
    }

    if (!StringHelper::Concatenate(bufferTry, "")) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorld")) {
        return false;
    }

    return (!StringHelper::Concatenate(bufferTry, NULL) && !StringHelper::Concatenate(NULL, "HelloWorld") && !StringHelper::Concatenate(NULL, NULL));
}

bool StringHelperTest::TestConcatenateN() {

    char8 bufferTry[32];

    StringHelper::Copy(bufferTry, "Hello");
    const char8 *buffer = "World";

    //Test the concatenate function with a max number of chars to append.
    uint32 sizeToAppend = 3;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWor")) {
        return false;
    }

    //If the number of chars to append is too much, all second string is appended and size is modified.
    sizeToAppend = 10;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorWorld")) {
        return false;
    }

    //size=0
    sizeToAppend = 0;

    if (!StringHelper::ConcatenateN(bufferTry, buffer, sizeToAppend)) {
        return false;
    }

    if (!StringTestHelper::Compare(bufferTry, "HelloWorWorld")) {
        return false;
    }

    sizeToAppend = 1;
    return (!StringHelper::ConcatenateN(bufferTry, NULL, sizeToAppend) && !StringHelper::ConcatenateN(NULL, buffer, sizeToAppend)
            && !StringHelper::ConcatenateN(NULL, NULL, sizeToAppend));
}

bool StringHelperTest::TestSearchChar() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    char8 toSearch = 'l';

    if ((retPointer = StringHelper::SearchChar(buffer, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return false;
    }

    //search again on the remained
    if ((retPointer = StringHelper::SearchChar(retPointer + 1, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "lo World")) {
        return false;
    }

    //these chars are not in the string
    char8 invalid = 'x';

    if ((retPointer = StringHelper::SearchChar(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringHelper::SearchChar(NULL, toSearch) == NULL);
}

bool StringHelperTest::TestSearchChars() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    const char8 *toSearch = "rdl";

    if ((retPointer = StringHelper::SearchChars(buffer, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "llo World")) {
        return false;
    }

    //search again on the remained
    if ((retPointer = StringHelper::SearchChars(retPointer + 1, toSearch)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "lo World")) {
        return false;
    }

    //these chars are not in the string
    const char8 *invalid = "xpqg";

    //returns null if the character is not found
    if ((retPointer = StringHelper::SearchChars(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringHelper::SearchChars(NULL, NULL) == NULL && StringHelper::SearchChars(buffer, NULL) == NULL
            && StringHelper::SearchChars(NULL, toSearch) == NULL);
}

bool StringHelperTest::TestSearchLastChar() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    char8 toSearch = 'l';

    if ((retPointer = StringHelper::SearchLastChar(buffer, toSearch)) == NULL) {
        return false;
    }
    if (!StringTestHelper::Compare(retPointer, "ld")) {
        return false;
    }

    char8 invalid = 'x';

    if ((retPointer = StringHelper::SearchLastChar(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringHelper::SearchLastChar(NULL, toSearch) == NULL);

}

bool StringHelperTest::TestSearchString() {
    const char8 *buffer = "Hello World";
    const char8 *retPointer;

    const char8 *toSearch = "orl";

    if ((retPointer = StringHelper::SearchString(buffer, toSearch)) == NULL) {
        return false;
    }
    if (!StringTestHelper::Compare(retPointer, "orld")) {
        return false;
    }

    const char8 *invalid = "Worfd";

    if ((retPointer = StringHelper::SearchString(buffer, invalid)) != NULL) {
        return false;
    }

    //NULL pointer arguments
    return (StringHelper::SearchString(NULL, NULL) == NULL && StringHelper::SearchString(buffer, NULL) == NULL
            && StringHelper::SearchString(NULL, toSearch) == NULL);
}

bool StringHelperTest::TestSearchIndex() {
    const char8 *buffer = "Hello World";

    const char8 *toSearch = "ll";

    if (StringHelper::SearchIndex(buffer, toSearch) != 2 && !StringTestHelper::Compare(buffer + 2, "llo World")) {
        return false;
    }

    //these chars are not in the string
    const char8 *invalid = "xpqg";

    if ((uint32)StringHelper::SearchIndex(buffer, invalid) != StringHelper::Length(buffer)) {
        return false;
    }

    //NULL pointer arguments
    return (StringHelper::SearchIndex(NULL, NULL) == -1 && StringHelper::SearchIndex(buffer, NULL) == -1 && StringHelper::SearchIndex(NULL, toSearch) == -1);
}


bool StringHelperTest::TestTokenizeByChars() {

    const char8* string = "Hello:World,I am...Giuseppe";
    char8 result[32];
    const char8* charDelimiters = ":,";

    const char8* retPointer;

    //Test the tokenize giving char8s as delimiters.
    if ((retPointer = StringHelper::TokenizeByChars(string, charDelimiters, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "World,I am...Giuseppe")) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Hello")) {
        return false;
    }

    //Thanks to retPointer is simple tokenize again.
    if ((retPointer = StringHelper::TokenizeByChars(retPointer, charDelimiters, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointer, "I am...Giuseppe")) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "World")) {
        return false;
    }

    if ((retPointer = StringHelper::TokenizeByChars(retPointer, charDelimiters, result)) == NULL) {
        return false;
    }

    if ((retPointer[0] != '\0') || *(retPointer - 1) != 'e') {
        return false;
    }

    if (!StringTestHelper::Compare(result, "I am...Giuseppe")) {
        return false;
    }

    return (StringHelper::TokenizeByChars(retPointer, charDelimiters, NULL) == NULL) && (StringHelper::TokenizeByChars(retPointer, NULL, result) == NULL)
            && StringHelper::TokenizeByChars(NULL, charDelimiters, result) == NULL;

}

bool StringHelperTest::TestTokenizeByString() {

    //Test the tokenize function with string as delimiter.
    const char8 *string = "Hello:World,I am...Giuseppe";
    const char8* stringDelimiter = "...";
    const char8* retPointerConst;

    char8 result[32];

    //This substring is not in the buffer.
    if ((retPointerConst = StringHelper::TokenizeByString(string, "....", result)) != NULL) {
        return false;
    }

    //Check if the result is correct.
    if ((retPointerConst = StringHelper::TokenizeByString(string, stringDelimiter, result)) == NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(retPointerConst, "Giuseppe")) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Hello:World,I am")) {
        return false;
    }

    //Thanks to retPointer we can tokenize again. Since the string is finished new retPointer become null.
    if ((retPointerConst = StringHelper::TokenizeByString(retPointerConst, stringDelimiter, result)) != NULL) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "Giuseppe")) {
        return false;
    }

    return (StringHelper::TokenizeByString(string, stringDelimiter, NULL) == NULL) && (StringHelper::TokenizeByString(string, NULL, result) == NULL)
            && StringHelper::TokenizeByString(NULL, stringDelimiter, result) == NULL;

}

bool StringHelperTest::TestSubstr() {
    const char8* string = "Hello:World,I am...Giuseppe";
    char8 result[32];

    //Test the substr function.
    if (!StringHelper::Substr(3, 6u, string, result)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "lo:W")) {
        return false;
    }

    //Check if it works at the end of the string.
    if (!StringHelper::Substr(25, 26, string, result)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "pe")) {
        return false;
    }

    //If the end index is out of bounds returns false but copies in result the end of the string.
    if (StringHelper::Substr(23, 30, string, result)) {
        return false;
    }

    if (!StringTestHelper::Compare(result, "eppe")) {
        return false;
    }

    return !StringHelper::Substr(1, 0, string, result) && !StringHelper::Substr(1, 1, NULL, result) && !StringHelper::Substr(1, 1, string, NULL);

}

bool StringHelperTest::TestSetChar() {
    bool retValue;
    uint32 i = 0u;
    char8 buffer[30] = "length7";
    const uint32 size = 3u;
    retValue = StringHelper::SetChar(buffer, size, 'a');
    while (i < size) {
        retValue &= (*(buffer + i) == 'a');
        i++;
    }
    retValue &= (*(buffer + 3) == 'g');
    retValue &= (*(buffer + 4) == 't');
    retValue &= (*(buffer + 5) == 'h');
    retValue &= (*(buffer + 6) == '7');

    return retValue;
}

bool StringHelperTest::TestSetChar0Size() {
    bool retValue;

    char8 buffer[30] = "length7";
    const uint32 size = 0u;
    retValue = !StringHelper::SetChar(buffer, size, 'a');
    retValue &= (*(buffer + 0) == 'l');
    retValue &= (*(buffer + 1) == 'e');
    retValue &= (*(buffer + 2) == 'n');
    retValue &= (*(buffer + 3) == 'g');
    retValue &= (*(buffer + 4) == 't');
    retValue &= (*(buffer + 5) == 'h');
    retValue &= (*(buffer + 6) == '7');
    return retValue;
}

bool StringHelperTest::TestSetCharNull() {
    bool retValue;
    char8 *buffer = NULL;
    const uint32 size = 3u;
    retValue = !StringHelper::SetChar(buffer, size, 'a');
    return retValue;
}
