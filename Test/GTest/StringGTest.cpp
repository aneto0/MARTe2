/**
 * @file StringGTest.cpp
 * @brief Source file for class StringGTest
 * @date 26/10/2015
 * @author Giuseppe Ferrò
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
 * the class StringGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */



/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
#include "StringTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StringGTest,TestDefaultConstructor) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestDefaultConstructor());
}

TEST(StringGTest,TestCopyConstructor) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyConstructor("HelloWorld"));
}

TEST(StringGTest,TestDestructor) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestDestructor());
}

TEST(StringGTest,TestAnyTypeOperator) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestAnyTypeOperator("HelloWorld"));
}

TEST(StringGTest,TestRead_minor_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 5));
}

TEST(StringGTest,TestRead_equal_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 10));
}

TEST(StringGTest,TestRead_bigger_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 20));
}

TEST(StringGTest,TestRead_minor_size_timeout) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 5));
}

TEST(StringGTest,TestRead_equal_size_timeout) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 10));
}

TEST(StringGTest,TestRead_bigger_size_timeout) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 20));
}



TEST(StringGTest,TestWrite_minor_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 5));
}

TEST(StringGTest,TestWrite_equal_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 10));
}

TEST(StringGTest,TestWrite_bigger_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 20));
}

TEST(StringGTest,TestWrite_minor_size_timeout) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 5));
}

TEST(StringGTest,TestWrite_equal_size_timeout) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 10));
}

TEST(StringGTest,TestWrite_bigger_size_timeout) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 20));
}

TEST(StringGTest,TestCanWrite) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanWrite());
}

TEST(StringGTest,TestCanRead) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanRead());
}

TEST(StringGTest,TestCanSeek) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanSeek());
}

TEST(StringGTest,TestPosition) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestPosition());
}

TEST(StringGTest,TestSize) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestSize("HelloWorld"));
}

TEST(StringGTest,TestSize_NULL) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestSize(NULL));
}

TEST(StringGTest,TestSetSize) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestSetSize(32));
}

TEST(StringGTest,TestSetSize_0) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestSetSize(0));
}

TEST(StringGTest,TestBuffer) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestBuffer("HelloWorld"));
}

TEST(StringGTest,TestBufferReference) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestBufferReference("HelloWorld"));
}

TEST(StringGTest,TestTail_0) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 0));
}

TEST(StringGTest,TestTail_3) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 3));
}

TEST(StringGTest,TestTail_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 6));
}

TEST(StringGTest,TestTail_out_size) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 8));
}


TEST(StringGTest, TestSeek_in_range) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 16, true));
}

TEST(StringGTest, TestSeek_out_range) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 33, false));
}

TEST(StringGTest, TestRelativeSeek_in_range) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, 8, true));
}

TEST(StringGTest, TestRelativeSeek_left_boundary) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, -17, false));
}

TEST(StringGTest, TestRelativeSeek_right_boundary) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, 17, false));
}

TEST(StringGTest, TestCopyOperator_Char) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_Char(' '));
}

TEST(StringGTest, TestCopyOperator_CCString) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_CCString("HelloWorld"));
}

TEST(StringGTest, TestCopyOperator_String) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_String("HelloWorld"));
}

TEST(StringGTest, TestConcatenateOperator_Char) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_Char('a'));
}

TEST(StringGTest, TestConcatenateOperator_CCString) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_CCString("HelloWorld"));
}

TEST(StringGTest, TestConcatenateOperator_String) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_String("HelloWorld"));
}

TEST(StringGTest, TestIsEqualOperator_CCString) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_CCString("HelloWorld"));
}

TEST(StringGTest, TestIsEqualOperator_String) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_String("HelloWorld"));
}

TEST(StringGTest, TestIsDifferentOperator_CCString) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsDifferentOperator_CCString("HelloWorld"));
}

TEST(StringGTest, TestIsDifferentOperator_String) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsDifferentOperator_String("HelloWorld"));
}

TEST(StringGTest, TestGetCharacterOperator) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetCharacterOperator("HelloWorld", 5));
}

TEST(StringGTest, TestLocate_Char_True) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 'W', 5));
}

TEST(StringGTest, TestLocate_Char_More_Than_one) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 'l', 2));
}

TEST(StringGTest, TestLocate_Char_False) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 's', -1));
}

TEST(StringGTest, TestLocate_Char_Terminator) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", '\0', -1));
}


TEST(StringGTest, TestLocate_String_True) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "Worl", 5));
}

TEST(StringGTest, TestLocate_String_OneChar) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "r", 7));
}

TEST(StringGTest, TestLocate_String_Equal) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "HelloWorld", 0));
}

TEST(StringGTest, TestLocate_String_Empty) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "", -1));
}

TEST(StringGTest, TestLocate_String_False) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "elol", -1));
}

TEST(StringGTest, TestPrintFormatted) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestPrintFormatted(GeneratePrintFormattedGenericTable()));
}

TEST(StringGTest, TestGetToken) {
    StringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetToken(TokenTestTableTerminators));
}
