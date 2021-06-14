/**
 * @file UDPSocketGTest.cpp
 * @brief Source file for class UDPSocketGTest
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
 * the class UDPSocketGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UDPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FileSystem_L3Streams_UDPSocketGTest, TestConstructor) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestConstructor_Stream) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestConstructor_Stream());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestConstructor_BasicUDPSocket) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestConstructor_BasicUDPSocket());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestRead) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestRead());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestRead_Timeout) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestRead_Timeout());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestWrite) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestWrite());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestWrite_Timeout) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestWrite_Timeout());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestSize) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestSize());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestSeek) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestSeek());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestRelativeSeek) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestRelativeSeek());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestPosition) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestPosition());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestSetSize) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestSetSize());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestCanWrite) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestCanWrite());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestCanSeek) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestCanSeek());
}

TEST(FileSystem_L3Streams_UDPSocketGTest, TestCanRead) {
    UDPSocketTest test;
    ASSERT_TRUE(test.TestCanRead());
}
