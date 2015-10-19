/**
 * @file StreamStringGTest.cpp
 * @brief Source file for class StreamStringGTest
 * @date 19/10/2015
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
#include "gtest/gtest.h"
#include "StreamStringTest.h"

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

/*
 TEST(StreamStringGTest,TestUnbufferedWrite) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedWrite());
 }

 TEST(StreamStringGTest,TestUnbufferedRead) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedRead());
 }
 TEST(StreamStringGTest,TestUnbufferedSize) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedSize());
 }
 TEST(StreamStringGTest,TestUnbufferedSeek) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedSeek());
 }
 TEST(StreamStringGTest,TestUnbufferedRelativeSeek) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedRelativeSeek());
 }
 TEST(StreamStringGTest,TestUnbufferedPosition) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedPosition());
 }
 TEST(StreamStringGTest,TestUnbufferedSetSize) {
 StreamStringTest stringtest;
 ASSERT_TRUE(stringtest.TestUnbufferedSetSize());
 }

 */

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

TEST(StreamStringGTest, TestCopyOperator_StreamString) {
    StreamStringTest stringtest;
    ASSERT_TRUE(stringtest.TestCopyOperator_StreamString("HelloWorld"));
}



