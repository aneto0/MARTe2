/**
 * @file StreamStringIOBufferGTest.cpp
 * @brief Source file for class StreamStringIOBufferGTest
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
 * the class StreamStringIOBufferGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamStringIOBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestDefaultConstructor) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_1) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(1, 1));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_2) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(2, 2));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_3) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(3, 2));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_5) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(5, 4));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_10) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(10, 8));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_16) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(16, 16));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_20) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(20, 16));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_33) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(33, 32));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_127) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(127, 64));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_0) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(0, 1));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestGranularityConstructor_MAX) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestGranularityConstructor(0xffffffff, 0x80000000));
}


TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestSetBufferAllocationSize_size_in_granularity) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestSetBufferAllocationSize(10, 32));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestSetBufferAllocationSize_granularity_in_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestSetBufferAllocationSize(10, 16));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestSetBufferAllocationSize_zero_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestSetBufferAllocationSize(0, 16));
}


TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestWrite_minor_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestWrite(5, "HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestWrite_equal_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestWrite(11, "HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestTerminate_minor_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestTerminate(3, "HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestTerminate_equal_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestTerminate(11, "HelloWorld"));
}


TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestWriteAll_minor_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestWriteAll(5, "HelloWorld"));
}

TEST(BareMetal_L3Streams_StreamStringIOBufferGTest, TestWriteAll_equal_size) {
    StreamStringIOBufferTest myStreamStringIOBufferTest;
    ASSERT_TRUE(myStreamStringIOBufferTest.TestWriteAll(11, "HelloWorld"));
}

	
