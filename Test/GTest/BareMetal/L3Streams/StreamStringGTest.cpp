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
#include <StreamStringTest.h>
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_StreamStringGTest,TestDefaultConstructor) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestCopyConstructor) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyConstructor("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestConstructor_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConstructor_CCString("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestDestructor) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestDestructor());
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestAnyTypeOperator) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestAnyTypeOperator("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestRead_minor_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestRead_equal_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 10));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestRead_bigger_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRead("HelloWorld", 20));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestRead_minor_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestRead_equal_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 10));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestRead_bigger_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestReadTimeout("HelloWorld", 20));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestWrite_minor_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestWrite_equal_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 10));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestWrite_bigger_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWrite("HelloWorld", 20));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestWrite_minor_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestWrite_equal_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 10));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestWrite_bigger_size_timeout) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestWriteTimeout("HelloWorld", 20));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestCanWrite) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanWrite());
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestCanRead) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanRead());
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestCanSeek) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCanSeek());
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestPosition) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestPosition());
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestSize) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSize("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestSize_NULL) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSize(NULL));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestSetSize) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSetSize(32));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestSetSize_0) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSetSize(0));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestBuffer) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestBuffer("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestBufferReference) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestBufferReference("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestTail_0) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 0));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestTail_3) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 3));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestTail_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 6));
}

TEST(BareMetal_L3Streams_StreamStringGTest,TestTail_out_size) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestTail("abcdefg", 8));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestSeek_in_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 16, true));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestSeek_out_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestSeek(32, 33, false));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestRelativeSeek_in_range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, 8, true));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestRelativeSeek_left_boundary) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, -17, false));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestRelativeSeek_OutOfInt32Range) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek_OutOfInt32Range());
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestRelativeSeek_right_boundary) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestRelativeSeek(16, 17, false));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestCopyOperator_Char) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_Char(' '));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestCopyOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_CCString("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestCopyOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_String("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestConcatenateOperator_Char) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_Char('a'));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestConcatenateOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_CCString("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestConcatenateOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestConcatenateOperator_String("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestIsEqualOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_CCString("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestIsEqualOperator_EmptyCCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_CCString(""));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestIsEqualOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_String("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestIsEqualOperator_EmptyString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsEqualOperator_String(""));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestIsDifferentOperator_CCString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsDifferentOperator_CCString("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestIsDifferentOperator_String) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestIsDifferentOperator_String("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestGetCharacterOperator) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetCharacterOperator("HelloWorld", 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_Char_True) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 'W', 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_Char_More_Than_one) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 'l', 2));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_Char_False) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", 's', -1));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_Char_Terminator) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_Char("HelloWorld", '\0', -1));
}


TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_String_True) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "Worl", 5));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_String_OneChar) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "r", 7));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_String_Equal) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "HelloWorld", 0));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_String_Empty) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "", -1));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestLocate_String_False) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestLocate_String("HelloWorld", "elol", -1));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestPrintFormatted) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestPrintFormatted(GeneratePrintFormattedGenericTable()));
}

TEST(BareMetal_L3Streams_StreamStringGTest, TestGetToken) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestGetToken(TokenTestTableTerminators));
}
