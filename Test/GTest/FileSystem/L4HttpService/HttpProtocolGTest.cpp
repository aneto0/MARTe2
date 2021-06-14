/**
 * @file HttpProtocolGTest.cpp
 * @brief Source file for class HttpProtocolGTest
 * @date 17/09/2018
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
 * the class HttpProtocolGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpProtocolTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestConstructor) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Get1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Get1());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Get2_Commands) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Get2_Commands());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Put1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Put1());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Post1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post1());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Post2_Multiform) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_Multiform());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Post2_Multiform_WrappedBoundary) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_Multiform_WrappedBoundary());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Post2_MultiformConsecutiveData) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_MultiformConsecutiveData());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Post2_FalseMultiformNoParsedBoundary) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_FalseMultiformNoParsedBoundary());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Head) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Head());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_Reply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Reply());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_IncompatibleHTTPVersion) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_IncompatibleHTTPVersion());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_IncompatibleHTTPVersionNoReply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_IncompatibleHTTPVersionNoReply());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestCompleteReadOperation) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestCompleteReadOperation());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestCompleteReadOperation_ClipSize) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestCompleteReadOperation_ClipSize());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Get1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Get1());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Get2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Get2());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Put1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Put1());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Put2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Put2());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Head1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Head1());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Head2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Head2());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Post) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Post());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Reply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Reply());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_Reply2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Reply2());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_False_InvalidCommand) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_False_InvalidCommand());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestSecurityCheck) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSecurityCheck());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestKeepAliveDefault) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestKeepAliveDefault());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestKeepAliveTrue) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestKeepAlive(true));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestKeepAliveFalse) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestKeepAlive(false));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestSetKeepAliveTrue) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSetKeepAlive(true));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestSetKeepAliveFalse) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSetKeepAlive(false));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetHttpCommandGet) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCGet));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetHttpCommandPut) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCPut));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetHttpCommandHead) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCHead));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetHttpCommandPost) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCPost));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetHttpCommandReply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCReply));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestSetUnmatchedId) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSetUnmatchedId());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetUnmatchedId) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetUnmatchedId());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetPath) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetPath());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetId) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetId());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_StrucuredDataStored) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_StrucuredDataStored());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestWriteHeader_StrucuredDataOnline) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_StrucuredDataOnline());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestTextMode_True) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestTextMode(0));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestTextMode_False) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestTextMode(1));
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_False_FailGetLine) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_False_FailGetLine());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_False_PostNoContentType) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_False_PostNoContentType());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_False_PostNoContentLength) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_False_PostNoContentLength());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_FalseInvalidReplyCode) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_FalseInvalidReplyCode());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestReadHeader_FalseInvalidCommand) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_FalseInvalidCommand());
}

TEST(FileSystem_L4HttpService_HttpProtocolGTest, TestGetInputCommand) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetInputCommand());
}

