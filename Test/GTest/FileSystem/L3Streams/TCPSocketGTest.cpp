/**
 * @file TCPSocketGTest.cpp
 * @brief Source file for class TCPSocketGTest
 * @date 16/11/2015
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
 * the class TCPSocketGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TCPSocketGTest, TestConstructor) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(TCPSocketGTest, TestConstructor_Stream) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestConstructor_Stream());
}

TEST(TCPSocketGTest, TestConstructor_BasicTCPSocket) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestConstructor_BasicTCPSocket());
}


TEST(TCPSocketGTest, TestRead) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestRead());
}

TEST(TCPSocketGTest, TestRead_Timeout) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestRead_Timeout());
}

TEST(TCPSocketGTest, TestWrite) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestWrite());
}

TEST(TCPSocketGTest, TestWrite_Timeout) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestWrite_Timeout());
}

TEST(TCPSocketGTest, TestSize) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestSize());
}

TEST(TCPSocketGTest, TestSeek) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestSeek());
}

TEST(TCPSocketGTest, TestRelativeSeek) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestRelativeSeek());
}

TEST(TCPSocketGTest, TestPosition) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestPosition());
}

TEST(TCPSocketGTest, TestSetSize) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestSetSize());
}

TEST(TCPSocketGTest, TestCanWrite) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestCanWrite());
}

TEST(TCPSocketGTest, TestCanSeek) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestCanSeek());
}

TEST(TCPSocketGTest, TestCanRead) {
    TCPSocketTest test;
    ASSERT_TRUE(test.TestCanRead());
}

