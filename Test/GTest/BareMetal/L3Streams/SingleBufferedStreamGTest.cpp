/**
 * @file SingleBufferedStreamTest.cpp
 * @brief Source file for class SingleBufferedStreamTest
 * @date 17/10/2015
 * @author Andre Neto
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
 * the class BufferedStreamGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SingleBufferedStreamTest.h"
#include "TestSupport.h"
#include "MilliSeconds.h"



TEST(SingleBufferedStreamGTest,TestDefaultConstructor) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestDefaultConstructor());
}

TEST(SingleBufferedStreamGTest,TestConstructor_Timeout) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestConstructor_Timeout());
}

TEST(SingleBufferedStreamGTest,TestGetTimeout) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestGetTimeout());
}

TEST(SingleBufferedStreamGTest,TestSetTimeout) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetTimeout());
}

TEST(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(1));
}

TEST(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(2));
}

TEST(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(8));
}

TEST(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(64));
}

TEST(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(1024));
}

TEST(SingleBufferedStreamGTest,TestRead_Size_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 1));
    ASSERT_TRUE(streamtest.TestRead(2, 1));
    ASSERT_TRUE(streamtest.TestRead(8, 1));
    ASSERT_TRUE(streamtest.TestRead(64, 1));
    ASSERT_TRUE(streamtest.TestRead(1024, 1));
}

TEST(SingleBufferedStreamGTest,TestRead_Size_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 2));
    ASSERT_TRUE(streamtest.TestRead(2, 2));
    ASSERT_TRUE(streamtest.TestRead(8, 2));
    ASSERT_TRUE(streamtest.TestRead(64, 2));
    ASSERT_TRUE(streamtest.TestRead(1024, 2));
}

TEST(SingleBufferedStreamGTest,TestRead_Size_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 8));
    ASSERT_TRUE(streamtest.TestRead(2, 8));
    ASSERT_TRUE(streamtest.TestRead(8, 8));
    ASSERT_TRUE(streamtest.TestRead(64, 8));
    ASSERT_TRUE(streamtest.TestRead(1024, 8));
}

TEST(SingleBufferedStreamGTest,TestRead_Size_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 64));
    ASSERT_TRUE(streamtest.TestRead(2, 64));
    ASSERT_TRUE(streamtest.TestRead(8, 64));
    ASSERT_TRUE(streamtest.TestRead(64, 64));
    ASSERT_TRUE(streamtest.TestRead(1024, 64));
}

TEST(SingleBufferedStreamGTest,TestRead_Size_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 1024));
    ASSERT_TRUE(streamtest.TestRead(2, 1024));
    ASSERT_TRUE(streamtest.TestRead(8, 1024));
    ASSERT_TRUE(streamtest.TestRead(64, 1024));
    ASSERT_TRUE(streamtest.TestRead(1024, 1024));
}

TEST(SingleBufferedStreamGTest,TestRead_Size_8_timeout) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead_Timeout(0, 1, MARTe::MilliSeconds(500,MARTe::Units::ms)));
    ASSERT_TRUE(streamtest.TestRead_Timeout(1, 8, MARTe::MilliSeconds(500,MARTe::Units::ms)));
    ASSERT_TRUE(streamtest.TestRead_Timeout(2, 8, MARTe::MilliSeconds(500,MARTe::Units::ms)));
    ASSERT_TRUE(streamtest.TestRead_Timeout(8, 8, MARTe::MilliSeconds(500,MARTe::Units::ms)));
    ASSERT_TRUE(streamtest.TestRead_Timeout(64, 8, MARTe::MilliSeconds(500,MARTe::Units::ms)));
    ASSERT_TRUE(streamtest.TestRead_Timeout(1024, 8, MARTe::MilliSeconds(500,MARTe::Units::ms)));
}


TEST(SingleBufferedStreamGTest,TestWrite_NotCanWrite) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite_NotCanWrite());
}

TEST(SingleBufferedStreamGTest,TestWrite_Size_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 1));
    ASSERT_TRUE(streamtest.TestWrite(2, 1));
    ASSERT_TRUE(streamtest.TestWrite(8, 1));
    ASSERT_TRUE(streamtest.TestWrite(64, 1));
    ASSERT_TRUE(streamtest.TestWrite(1024, 1));
}

TEST(SingleBufferedStreamGTest,TestWrite_Size_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 2));
    ASSERT_TRUE(streamtest.TestWrite(2, 2));
    ASSERT_TRUE(streamtest.TestWrite(8, 2));
    ASSERT_TRUE(streamtest.TestWrite(64, 2));
    ASSERT_TRUE(streamtest.TestWrite(1024, 2));
}

TEST(SingleBufferedStreamGTest,TestWrite_Size_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 8));
    ASSERT_TRUE(streamtest.TestWrite(2, 8));
    ASSERT_TRUE(streamtest.TestWrite(8, 8));
    ASSERT_TRUE(streamtest.TestWrite(64, 8));
    ASSERT_TRUE(streamtest.TestWrite(1024, 8));
}

TEST(SingleBufferedStreamGTest,TestWrite_Size_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 64));
    ASSERT_TRUE(streamtest.TestWrite(2, 64));
    ASSERT_TRUE(streamtest.TestWrite(8, 64));
    ASSERT_TRUE(streamtest.TestWrite(64, 64));
    ASSERT_TRUE(streamtest.TestWrite(1024, 64));
}

TEST(SingleBufferedStreamGTest,TestWrite_Size_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 1024));
    ASSERT_TRUE(streamtest.TestWrite(2, 1024));
    ASSERT_TRUE(streamtest.TestWrite(8, 1024));
    ASSERT_TRUE(streamtest.TestWrite(64, 1024));
    ASSERT_TRUE(streamtest.TestWrite(1024, 1024));
}

TEST(SingleBufferedStreamGTest,TestWrite_Size_8_timeout) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite_Timeout(8, 64, MARTe::MilliSeconds(500,MARTe::Units::ms)));
}

TEST(SingleBufferedStreamGTest,TestWrite_OverflowInternalBuffer_Size) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite_OverflowInternalBuffer(8, 256));
    ASSERT_TRUE(streamtest.TestWrite_OverflowInternalBuffer(32, 256));
    ASSERT_TRUE(streamtest.TestWrite_OverflowInternalBuffer(64, 256));
}

TEST(SingleBufferedStreamGTest,TestRead_NotCanRead) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead_NotCanRead());
}

TEST(SingleBufferedStreamGTest,TestSize_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(1));
}

TEST(SingleBufferedStreamGTest,TestSize_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(2));
}

TEST(SingleBufferedStreamGTest,TestSize_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(8));
}

TEST(SingleBufferedStreamGTest,TestSize_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(64));
}

TEST(SingleBufferedStreamGTest,TestSize_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(1024));
}

TEST(SingleBufferedStreamGTest,TestSetSize_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(1));
}

TEST(SingleBufferedStreamGTest,TestSetSize_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(2));
}

TEST(SingleBufferedStreamGTest,TestSetSize_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(8));
}

TEST(SingleBufferedStreamGTest,TestSetSize_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(64));
}

TEST(SingleBufferedStreamGTest,TestSetSize_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(1024));
}

TEST(SingleBufferedStreamGTest,TestSeek) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSeek());
}

TEST(SingleBufferedStreamGTest,TestRelativeSeek) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRelativeSeek());
}

TEST(SingleBufferedStreamGTest,TestRelativeSeek_OutOfInt32Range) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRelativeSeek_OutOfInt32Range());
}

TEST(SingleBufferedStreamGTest,TestRelativeSeek_NegativeFinalPosition) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRelativeSeek_NegativeFinalPosition());
}

TEST(SingleBufferedStreamGTest,TestRelativeSeek_OverflowInternalBuffer_Size) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRelativeSeek_OverflowInternalBuffer(8, 256));
    ASSERT_TRUE(streamtest.TestRelativeSeek_OverflowInternalBuffer(32, 256));
    ASSERT_TRUE(streamtest.TestRelativeSeek_OverflowInternalBuffer(64, 256));
}

TEST(SingleBufferedStreamGTest,TestPosition) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestPosition());
}

TEST(SingleBufferedStreamGTest,TestFlushAndResync_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(1));
}

TEST(SingleBufferedStreamGTest,TestFlushAndResync_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(2));
}

TEST(SingleBufferedStreamGTest,TestFlushAndResync_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(8));
}

TEST(SingleBufferedStreamGTest,TestFlushAndResync_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(64));
}

TEST(SingleBufferedStreamGTest,TestFlushAndResync_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(1024));
}
