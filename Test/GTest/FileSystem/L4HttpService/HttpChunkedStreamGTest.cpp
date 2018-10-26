/**
 * @file HttpChunkedStreamGTest.cpp
 * @brief Source file for class HttpChunkedStreamGTest
 * @date 01/10/2018
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
 * the class HttpChunkedStreamGTest (public, protected, and private). Be aware that some 
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpChunkedStreamTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(HttpChunkedStreamGTest, TestConstructor) {
    HttpChunkedStreamTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(HttpChunkedStreamGTest, TestFlush) {
    HttpChunkedStreamTest test;
    ASSERT_TRUE(test.TestFlush());
}

TEST(HttpChunkedStreamGTest, TestFinalChunk) {
    HttpChunkedStreamTest test;
    ASSERT_TRUE(test.TestFinalChunk());
}

TEST(HttpChunkedStreamGTest, TestSetChunkMode) {
    HttpChunkedStreamTest test;
    ASSERT_TRUE(test.TestSetChunkMode());
}

TEST(HttpChunkedStreamGTest, TestIsChunkMode) {
    HttpChunkedStreamTest test;
    ASSERT_TRUE(test.TestIsChunkMode());
}

