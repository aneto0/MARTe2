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
#include "gtest/gtest.h"

class SingleBufferedStreamGTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test
        // (right before the destructor).
    }
};

TEST_F(SingleBufferedStreamGTest,TestDefaultConstructor) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestDefaultConstructor());
}

TEST_F(SingleBufferedStreamGTest,TestConstructor_Timeout) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestConstructor_Timeout());
}

TEST_F(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(1));
}

TEST_F(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(2));
}

TEST_F(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(8));
}

TEST_F(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(64));
}

TEST_F(SingleBufferedStreamGTest,TestSetBufferSize_Buffer_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetBufferSize(1024));
}

TEST_F(SingleBufferedStreamGTest,TestRead_Size_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 1));
    ASSERT_TRUE(streamtest.TestRead(2, 1));
    ASSERT_TRUE(streamtest.TestRead(8, 1));
    ASSERT_TRUE(streamtest.TestRead(64, 1));
    ASSERT_TRUE(streamtest.TestRead(1024, 1));
}

TEST_F(SingleBufferedStreamGTest,TestRead_Size_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 2));
    ASSERT_TRUE(streamtest.TestRead(2, 2));
    ASSERT_TRUE(streamtest.TestRead(8, 2));
    ASSERT_TRUE(streamtest.TestRead(64, 2));
    ASSERT_TRUE(streamtest.TestRead(1024, 2));
}

TEST_F(SingleBufferedStreamGTest,TestRead_Size_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 8));
    ASSERT_TRUE(streamtest.TestRead(2, 8));
    ASSERT_TRUE(streamtest.TestRead(8, 8));
    ASSERT_TRUE(streamtest.TestRead(64, 8));
    ASSERT_TRUE(streamtest.TestRead(1024, 8));
}

TEST_F(SingleBufferedStreamGTest,TestRead_Size_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 64));
    ASSERT_TRUE(streamtest.TestRead(2, 64));
    ASSERT_TRUE(streamtest.TestRead(8, 64));
    ASSERT_TRUE(streamtest.TestRead(64, 64));
    ASSERT_TRUE(streamtest.TestRead(1024, 64));
}

TEST_F(SingleBufferedStreamGTest,TestRead_Size_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead(0, 1));
    ASSERT_TRUE(streamtest.TestRead(1, 1024));
    ASSERT_TRUE(streamtest.TestRead(2, 1024));
    ASSERT_TRUE(streamtest.TestRead(8, 1024));
    ASSERT_TRUE(streamtest.TestRead(64, 1024));
    ASSERT_TRUE(streamtest.TestRead(1024, 1024));
}

TEST_F(SingleBufferedStreamGTest,TestWrite_NotCanWrite) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite_NotCanWrite());
}

TEST_F(SingleBufferedStreamGTest,TestWrite_Size_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 1));
    ASSERT_TRUE(streamtest.TestWrite(2, 1));
    ASSERT_TRUE(streamtest.TestWrite(8, 1));
    ASSERT_TRUE(streamtest.TestWrite(64, 1));
    ASSERT_TRUE(streamtest.TestWrite(1024, 1));
}

TEST_F(SingleBufferedStreamGTest,TestWrite_Size_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 2));
    ASSERT_TRUE(streamtest.TestWrite(2, 2));
    ASSERT_TRUE(streamtest.TestWrite(8, 2));
    ASSERT_TRUE(streamtest.TestWrite(64, 2));
    ASSERT_TRUE(streamtest.TestWrite(1024, 2));
}

TEST_F(SingleBufferedStreamGTest,TestWrite_Size_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 8));
    ASSERT_TRUE(streamtest.TestWrite(2, 8));
    ASSERT_TRUE(streamtest.TestWrite(8, 8));
    ASSERT_TRUE(streamtest.TestWrite(64, 8));
    ASSERT_TRUE(streamtest.TestWrite(1024, 8));
}

TEST_F(SingleBufferedStreamGTest,TestWrite_Size_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 64));
    ASSERT_TRUE(streamtest.TestWrite(2, 64));
    ASSERT_TRUE(streamtest.TestWrite(8, 64));
    ASSERT_TRUE(streamtest.TestWrite(64, 64));
    ASSERT_TRUE(streamtest.TestWrite(1024, 64));
}

TEST_F(SingleBufferedStreamGTest,TestWrite_Size_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite(0, 1));
    ASSERT_TRUE(streamtest.TestWrite(1, 1024));
    ASSERT_TRUE(streamtest.TestWrite(2, 1024));
    ASSERT_TRUE(streamtest.TestWrite(8, 1024));
    ASSERT_TRUE(streamtest.TestWrite(64, 1024));
    ASSERT_TRUE(streamtest.TestWrite(1024, 1024));
}

TEST_F(SingleBufferedStreamGTest,TestWrite_OverflowInternalBuffer_Size) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestWrite_OverflowInternalBuffer(8, 256));
    ASSERT_TRUE(streamtest.TestWrite_OverflowInternalBuffer(32, 256));
    ASSERT_TRUE(streamtest.TestWrite_OverflowInternalBuffer(64, 256));
}

TEST_F(SingleBufferedStreamGTest,TestRead_NotCanRead) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRead_NotCanRead());
}

TEST_F(SingleBufferedStreamGTest,TestSize_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(1));
}

TEST_F(SingleBufferedStreamGTest,TestSize_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(2));
}

TEST_F(SingleBufferedStreamGTest,TestSize_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(8));
}

TEST_F(SingleBufferedStreamGTest,TestSize_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(64));
}

TEST_F(SingleBufferedStreamGTest,TestSize_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSize(1024));
}

TEST_F(SingleBufferedStreamGTest,TestSetSize_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(1));
}

TEST_F(SingleBufferedStreamGTest,TestSetSize_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(2));
}

TEST_F(SingleBufferedStreamGTest,TestSetSize_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(8));
}

TEST_F(SingleBufferedStreamGTest,TestSetSize_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(64));
}

TEST_F(SingleBufferedStreamGTest,TestSetSize_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSetSize(1024));
}

TEST_F(SingleBufferedStreamGTest,TestSeek) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestSeek());
}

TEST_F(SingleBufferedStreamGTest,TestRelativeSeek) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRelativeSeek());
}

TEST_F(SingleBufferedStreamGTest,TestRelativeSeek_OverflowInternalBuffer_Size) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestRelativeSeek_OverflowInternalBuffer(8, 256));
    ASSERT_TRUE(streamtest.TestRelativeSeek_OverflowInternalBuffer(32, 256));
    ASSERT_TRUE(streamtest.TestRelativeSeek_OverflowInternalBuffer(64, 256));
}

TEST_F(SingleBufferedStreamGTest,TestPosition) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestPosition());
}

TEST_F(SingleBufferedStreamGTest,TestFlushAndResync_1) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(1));
}

TEST_F(SingleBufferedStreamGTest,TestFlushAndResync_2) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(2));
}

TEST_F(SingleBufferedStreamGTest,TestFlushAndResync_8) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(8));
}

TEST_F(SingleBufferedStreamGTest,TestFlushAndResync_64) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(64));
}

TEST_F(SingleBufferedStreamGTest,TestFlushAndResync_1024) {
    SingleBufferedStreamTest streamtest;
    ASSERT_TRUE(streamtest.TestFlushAndResync(1024));
}
