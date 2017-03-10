/**
 * @file StreamStringGTest.cpp
 * @brief Source file for class StreamStringGTest
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
 * the class StreamStringGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */



/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include <StreamStringTest.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StreamStringGTest,TestDefaultConstructor) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestDefaultConstructor());
}

TEST(StreamStringGTest,TestCopyConstructor) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyConstructor("HelloWorld"));
}

TEST(StreamStringGTest,TestConstructor_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConstructor_CCString("HelloWorld"));
}

TEST(StreamStringGTest,TestDestructor) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestDestructor());
}

TEST(StreamStringGTest,TestAnyTypeOperator) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestAnyTypeOperator("HelloWorld"));
}

TEST(StreamStringGTest,TestRead_minor_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 5));
}

TEST(StreamStringGTest,TestRead_equal_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 10));
}

TEST(StreamStringGTest,TestRead_bigger_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 20));
}

TEST(StreamStringGTest,TestRead_minor_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 5));
}

TEST(StreamStringGTest,TestRead_equal_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 10));
}

TEST(StreamStringGTest,TestRead_bigger_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 20));
}

TEST(StreamStringGTest,TestWrite_minor_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 5));
}

TEST(StreamStringGTest,TestWrite_equal_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 10));
}

TEST(StreamStringGTest,TestWrite_bigger_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 20));
}

TEST(StreamStringGTest,TestWrite_minor_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 5));
}

TEST(StreamStringGTest,TestWrite_equal_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 10));
}

TEST(StreamStringGTest,TestWrite_bigger_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 20));
}

TEST(StreamStringGTest,TestCanWrite) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanWrite());
}

TEST(StreamStringGTest,TestCanRead) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanRead());
}

TEST(StreamStringGTest,TestCanSeek) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanSeek());
}

TEST(StreamStringGTest,TestPosition) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestPosition());
}

TEST(StreamStringGTest,TestSize) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSize("HelloWorld"));
}

TEST(StreamStringGTest,TestSize_NULL) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSize(NULL));
}

TEST(StreamStringGTest,TestSetSize) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSetSize(32));
}

TEST(StreamStringGTest,TestSetSize_0) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSetSize(0));
}

TEST(StreamStringGTest,TestBuffer) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestBuffer("HelloWorld"));
}

TEST(StreamStringGTest,TestBufferReference) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestBufferReference("HelloWorld"));
}

TEST(StreamStringGTest,TestTail_0) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 0));
}

TEST(StreamStringGTest,TestTail_3) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 3));
}

TEST(StreamStringGTest,TestTail_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 6));
}

TEST(StreamStringGTest,TestTail_out_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 8));
}

TEST(StreamStringGTest, TestSeek_in_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 16, true));
}

TEST(StreamStringGTest, TestSeek_out_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 33, false));
}

TEST(StreamStringGTest, TestRelativeSeek_in_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, 8, true));
}

TEST(StreamStringGTest, TestRelativeSeek_left_boundary) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, -17, false));
}

TEST(StreamStringGTest, TestRelativeSeek_OutOfInt32Range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek_OutOfInt32Range());
}

TEST(StreamStringGTest, TestRelativeSeek_right_boundary) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, 17, false));
}

TEST(StreamStringGTest, TestCopyOperator_Char) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_Char(' '));
}

TEST(StreamStringGTest, TestCopyOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_CCString("HelloWorld"));
}

TEST(StreamStringGTest, TestCopyOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_String("HelloWorld"));
}

TEST(StreamStringGTest, TestConcatenateOperator_Char) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_Char('a'));
}

TEST(StreamStringGTest, TestConcatenateOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_CCString("HelloWorld"));
}

TEST(StreamStringGTest, TestConcatenateOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_String("HelloWorld"));
}

TEST(StreamStringGTest, TestIsEqualOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_CCString("HelloWorld"));
}

TEST(StreamStringGTest, TestIsEqualOperator_EmptyCCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_CCString(""));
}

TEST(StreamStringGTest, TestIsEqualOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_String("HelloWorld"));
}

TEST(StreamStringGTest, TestIsEqualOperator_EmptyString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_String(""));
}

TEST(StreamStringGTest, TestIsDifferentOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsDifferentOperator_CCString("HelloWorld"));
}

TEST(StreamStringGTest, TestIsDifferentOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsDifferentOperator_String("HelloWorld"));
}

TEST(StreamStringGTest, TestGetCharacterOperator) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetCharacterOperator("HelloWorld", 5));
}

TEST(StreamStringGTest, TestLocate_Char_True) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 'W', 5));
}

TEST(StreamStringGTest, TestLocate_Char_More_Than_one) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 'l', 2));
}

TEST(StreamStringGTest, TestLocate_Char_False) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 's', -1));
}

TEST(StreamStringGTest, TestLocate_Char_Terminator) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", '\0', -1));
}


TEST(StreamStringGTest, TestLocate_String_True) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "Worl", 5));
}

TEST(StreamStringGTest, TestLocate_String_OneChar) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "r", 7));
}

TEST(StreamStringGTest, TestLocate_String_Equal) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "HelloWorld", 0));
}

TEST(StreamStringGTest, TestLocate_String_Empty) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "", -1));
}

TEST(StreamStringGTest, TestLocate_String_False) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "elol", -1));
}

TEST(StreamStringGTest, TestPrintFormatted) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestPrintFormatted(GeneratePrintFormattedGenericTable()));
}

TEST(StreamStringGTest, TestGetToken) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetToken(TokenTestTableTerminators));
}
