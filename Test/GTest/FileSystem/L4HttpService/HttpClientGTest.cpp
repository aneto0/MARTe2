/**
 * @file HttpClientGTest.cpp
 * @brief Source file for class HttpClientGTest
 * @date 05/10/2018
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
 * the class HttpClientGTest (public, protected, and private). Be aware that some
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
#include "HttpClientTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L4HttpService_HttpClientGTest, TestConstructor) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestInitialise) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestInitialise_FalseNoServerAddress) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoServerAddress());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestInitialise_FalseNoServerPort) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoServerPort());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestInitialise_FalseNoServerUri) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoServerUri());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestInitialise_DefaultAuthorization) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultAuthorization());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestGetHttpProtocol) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetHttpProtocol());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange_Authorization_Digest) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange_Authorization_Digest());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange_Authorization_Basic) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange_Authorization_Basic());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange_Authorization_FalseInvalidAuthType) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange_Authorization_FalseInvalidAuthType());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange_Authorization_FalseTimeout) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange_Authorization_FalseTimeout());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange_Authorization_FalseReplyCommand) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange_Authorization_FalseReplyCommand());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestHttpExchange_Authorization_Digest_KeepAlive) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange_Authorization_Digest_KeepAlive());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestSetServerAddress) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetServerAddress());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestSetServerPort) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetServerPort());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestSetServerUri) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetServerUri());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestGetServerAddress) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetServerAddress());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestGetServerPort) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetServerPort());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestGetServerUri) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetServerUri());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestGetAuthorisation) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetAuthorisation());
}

TEST(FileSystem_L4HttpService_HttpClientGTest, TestSetAuthorisation) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetAuthorisation());
}



