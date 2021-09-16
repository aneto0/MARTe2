/**
 * @file BufferedStreamIOBufferGTest.cpp
 * @brief Source file for class BufferedStreamIOBufferGTest
 * @date 14/10/2015
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
 * the class BufferedStreamIOBufferGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BufferedStreamIOBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestDefaultConstructor) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestFullConstructor) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestFullConstructor());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestGetTimeout) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestGetTimeout());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestSetTimeout) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestSetTimeout());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestRefill) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestRefill());
}
TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestRefill_NULL_Stream) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestRefill_NULL_Stream());
}
TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestRefill_NULL_Buffer) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestRefill_NULL_Buffer());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestFlush) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestFlush());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestFlush_NULL_Stream) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestFlush_NULL_Stream());
}
TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestFlush_NULL_Buffer) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestFlush_NULL_Buffer());
}
TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestResync) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestResync());
}
TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestResync_NULL_Stream) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestResync_NULL_Stream());
}
TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestResync_NULL_Buffer) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestResync_NULL_Buffer());
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestSetBufferSize) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestSetBufferSize(32));
}

TEST(BareMetal_L3Streams_BufferedStreamIOBufferGTest, TestSetBufferSize_zero_size) {
    BufferedStreamIOBufferTest myBufferedStreamIOBufferTest;
    ASSERT_TRUE(myBufferedStreamIOBufferTest.TestSetBufferSize(0));
}
