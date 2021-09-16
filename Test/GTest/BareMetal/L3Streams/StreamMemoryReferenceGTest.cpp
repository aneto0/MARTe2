/**
 * @file StreamMemoryReferenceGTest.cpp
 * @brief Source file for class StreamMemoryReferenceGTest
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
 * the class StreamMemoryReferenceGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"
#include "StreamMemoryReferenceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestDefaultConstructor) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestConstructor_CharPointer) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestConstructor_CharPointer());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestConstructor_ConstCharPointer) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestConstructor_ConstCharPointer());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestAnyTypeOperator) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestAnyTypeOperator("HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestDestructor) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestDestructor());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestRead_Same_BufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestRead("HelloWorld", bufferSize, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestRead_greater_than_BufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestRead("HelloWorld", 10, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestRead_minor_than_BufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestRead("HelloWorld", 5, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestRead_Same_BufferSize_timeout) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestReadTimeout("HelloWorld", bufferSize, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestRead_greater_than_BufferSize_timeout) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestReadTimeout("HelloWorld", 10, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestRead_minor_than_BufferSize_timeout) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestReadTimeout("HelloWorld", 5, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_Same_BufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestWrite("HelloWorld", bufferSize, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_greater_than_BufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestWrite("HelloWorld", 10, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_minor_than_BufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestWrite("HelloWorld", 5, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_Same_BufferSize_timeout) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestWriteTimeout("HelloWorld", bufferSize, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_greater_than_BufferSize_timeout) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestWriteTimeout("HelloWorld", 10, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_minor_than_BufferSize_timeout) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestWriteTimeout("HelloWorld", 5, bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestWrite_RO) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestWrite_RO());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSize_Const) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestSize_Const("HelloWorld", bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSize_size_in_bufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestSize("HelloWorld", bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSize_bufferSize_in_size) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestSize("HelloWorld", bufferSize));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSeek_pos_in_bufferSize_in_size) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestSeek(bufferSize, 10, 5, true));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSeek_pos_in_size_in_bufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestSeek(bufferSize, 10, 5, true));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSeek_size_in_pos_in_bufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestSeek(bufferSize, 5, 6, false));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest,TestSeek_size_in_bufferSize_in_pos) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 8;
    ASSERT_TRUE(smrTest.TestSeek(bufferSize, 5, 16, false));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestRelativeSeek_pos_in_size_in_bufferSize) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 32;
    ASSERT_TRUE(smrTest.TestRelativeSeek(bufferSize, 14, 7, true));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestRelativeSeek_pos_in_bufferSize_in_size) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestRelativeSeek(bufferSize, 14, 2, true));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestRelativeSeek_bufferSize_in_pos_in_size) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestRelativeSeek(bufferSize, 14, 5, false));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestRelativeSeek_bufferSize_in_size_in_pos) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestRelativeSeek(bufferSize, 14, 28, false));
    ASSERT_TRUE(smrTest.TestRelativeSeek(bufferSize, 14, -15, false));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestRelativeSeek_OutOfInt32Range) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestRelativeSeek_OutOfInt32Range());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestPosition_in_range) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestPosition(bufferSize, 15));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestPosition_out_range) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestPosition(bufferSize, 17));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestSetSize_in_range) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestSetSize(bufferSize, 15, true));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestSetSize_out_range) {
    StreamMemoryReferenceTest smrTest;
    const char8 bufferSize = 16;
    ASSERT_TRUE(smrTest.TestSetSize(bufferSize, 17, false));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestCanSeek) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestCanSeek());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestCanWrite) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestCanWrite());
}
TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestCanRead) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestCanRead());
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestPrintFormatted) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestPrintFormatted(32, GeneratePrintFormattedGenericTable()));
}

TEST(BareMetal_L3Streams_StreamMemoryReferenceGTest, TestGetToken) {
    StreamMemoryReferenceTest smrTest;
    ASSERT_TRUE(smrTest.TestGetToken(32, TokenTestTableTerminators));
}
