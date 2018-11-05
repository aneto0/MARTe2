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


#include <limits.h>
#include "gtest/gtest.h"

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
TEST(HttpProtocolGTest, TestConstructor) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestConstructor());
}


TEST(HttpProtocolGTest, TestReadHeader_Get1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Get1());
}

TEST(HttpProtocolGTest, TestReadHeader_Get2_Commands) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Get2_Commands());
}

TEST(HttpProtocolGTest, TestReadHeader_Put1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Put1());
}

TEST(HttpProtocolGTest, TestReadHeader_Post1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post1());
}

TEST(HttpProtocolGTest, TestReadHeader_Post2_Multiform) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_Multiform());
}

TEST(HttpProtocolGTest, TestReadHeader_Post2_Multiform_WrappedBoundary) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_Multiform_WrappedBoundary());
}

TEST(HttpProtocolGTest, TestReadHeader_Post2_MultiformConsecutiveData) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_MultiformConsecutiveData());
}

TEST(HttpProtocolGTest, TestReadHeader_Post2_FalseMultiformNoParsedBoundary) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_FalseMultiformNoParsedBoundary());
}

TEST(HttpProtocolGTest, TestReadHeader_Head) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Head());
}

TEST(HttpProtocolGTest, TestReadHeader_Reply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_Reply());
}

TEST(HttpProtocolGTest, TestReadHeader_IncompatibleHTTPVersion) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_IncompatibleHTTPVersion());
}

TEST(HttpProtocolGTest, TestReadHeader_IncompatibleHTTPVersionNoReply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_IncompatibleHTTPVersionNoReply());
}

TEST(HttpProtocolGTest, TestCompleteReadOperation) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestCompleteReadOperation());
}

TEST(HttpProtocolGTest, TestCompleteReadOperation_ClipSize) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestCompleteReadOperation_ClipSize());
}

TEST(HttpProtocolGTest, TestWriteHeader_Get1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Get1());
}

TEST(HttpProtocolGTest, TestWriteHeader_Get2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Get2());
}

TEST(HttpProtocolGTest, TestWriteHeader_Put1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Put1());
}

TEST(HttpProtocolGTest, TestWriteHeader_Put2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Put2());
}

TEST(HttpProtocolGTest, TestWriteHeader_Head1) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Head1());
}

TEST(HttpProtocolGTest, TestWriteHeader_Head2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Head2());
}

TEST(HttpProtocolGTest, TestWriteHeader_Post) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Post());
}

TEST(HttpProtocolGTest, TestWriteHeader_Reply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Reply());
}

TEST(HttpProtocolGTest, TestWriteHeader_Reply2) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_Reply2());
}

TEST(HttpProtocolGTest, TestWriteHeader_False_InvalidCommand) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_False_InvalidCommand());
}

TEST(HttpProtocolGTest, TestSecurityCheck) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSecurityCheck());
}

TEST(HttpProtocolGTest, TestKeepAliveDefault) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestKeepAliveDefault());
}

TEST(HttpProtocolGTest, TestKeepAliveTrue) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestKeepAlive(true));
}


TEST(HttpProtocolGTest, TestKeepAliveFalse) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestKeepAlive(false));
}


TEST(HttpProtocolGTest, TestSetKeepAliveTrue) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSetKeepAlive(true));
}

TEST(HttpProtocolGTest, TestSetKeepAliveFalse) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSetKeepAlive(false));
}

TEST(HttpProtocolGTest, TestGetHttpCommandGet) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCGet));
}

TEST(HttpProtocolGTest, TestGetHttpCommandPut) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCPut));
}

TEST(HttpProtocolGTest, TestGetHttpCommandHead) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCHead));
}

TEST(HttpProtocolGTest, TestGetHttpCommandPost) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCPost));
}

TEST(HttpProtocolGTest, TestGetHttpCommandReply) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetHttpCommand(HttpDefinition::HSHCReply));
}

TEST(HttpProtocolGTest, TestSetUnmatchedId) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestSetUnmatchedId());
}

TEST(HttpProtocolGTest, TestGetUnmatchedId) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetUnmatchedId());
}

TEST(HttpProtocolGTest, TestGetPath) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetPath());
}

TEST(HttpProtocolGTest, TestGetId) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetId());
}

TEST(HttpProtocolGTest, TestWriteHeader_StrucuredDataStored) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_StrucuredDataStored());
}

TEST(HttpProtocolGTest, TestWriteHeader_StrucuredDataOnline) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestWriteHeader_StrucuredDataOnline());
}

TEST(HttpProtocolGTest, TestTextMode_True) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestTextMode(0));
}

TEST(HttpProtocolGTest, TestTextMode_False) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestTextMode(1));
}

TEST(HttpProtocolGTest, TestReadHeader_False_FailGetLine) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_False_FailGetLine());
}

TEST(HttpProtocolGTest, TestReadHeader_False_PostNoContentType) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_False_PostNoContentType());
}

TEST(HttpProtocolGTest, TestReadHeader_False_PostNoContentLength) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_False_PostNoContentLength());
}

TEST(HttpProtocolGTest, TestReadHeader_FalseInvalidReplyCode) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_FalseInvalidReplyCode());
}

TEST(HttpProtocolGTest, TestReadHeader_FalseInvalidCommand) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestReadHeader_FalseInvalidCommand());
}

TEST(HttpProtocolGTest, TestGetInputCommand) {
    HttpProtocolTest test;
    ASSERT_TRUE(test.TestGetInputCommand());
}

