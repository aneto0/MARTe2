/**
 * @file HttpClientTest.h
 * @brief Source file for class HttpClientTest
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
 * the class HttpClientTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the HttpClient public methods.
 */
class HttpClientTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    HttpClientTest();

    /**
     * @brief Destructor. NOOP.
     */
    ~HttpClientTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method without specifying the ServerAddress.
     */
    bool TestInitialise_FalseNoServerAddress();

    /**
     * @brief Tests the Initialise method without specifying the Port.
     */
    bool TestInitialise_FalseNoServerPort();

    /**
     * @brief Tests the Initialise method without specifying URI.
     */
    bool TestInitialise_FalseNoServerUri();

    /**
     * @brief Tests the Initialise method with the default authorisation.
     */
    bool TestInitialise_DefaultAuthorization();

    /**
     * @brief Tests the GetHttpProtocol method.
     */
    bool TestGetHttpProtocol();

    /**
     * @brief Tests the HttpExchange method.
     */
    bool TestHttpExchange();

    /**
     * @brief Tests the HttpExchange method with a digest.
     */
    bool TestHttpExchange_Authorization_Digest();

    /**
     * @brief Tests the HttpExchange with a basic authentication method.
     */
    bool TestHttpExchange_Authorization_Basic();

    /**
     * @brief Tests the HttpExchange method with an invalidation authentication.
     */
    bool TestHttpExchange_Authorization_FalseInvalidAuthType();

    /**
     * @brief Tests the HttpExchange method with an authorisation timeout.
     */
    bool TestHttpExchange_Authorization_FalseTimeout();

    /**
     * @brief Tests the HttpExchange method with a false reply command.
     */
    bool TestHttpExchange_Authorization_FalseReplyCommand();

    /**
     * @brief Tests the HttpExchange method with a digest keep alive.
     */
    bool TestHttpExchange_Authorization_Digest_KeepAlive();

    /**
     * @brief Tests the SetServerAddress method.
     */
    bool TestSetServerAddress();

    /**
     * @brief Tests the SetServerPort method.
     */
    bool TestSetServerPort();

    /**
     * @brief Tests the SetServerUri method.
     */
    bool TestSetServerUri();

    /**
     * @brief Tests the GetServerAddress method.
     */
    bool TestGetServerAddress();

    /**
     * @brief Tests the GetServerPort method.
     */
    bool TestGetServerPort();

    /**
     * @brief Tests the GetServerUri method.
     */
    bool TestGetServerUri();

    /**
     * @brief Tests the GetAuthorisation method.
     */
    bool TestGetAuthorisation();

    /**
     * @brief Tests the SetAuthorisation method.
     */
    bool TestSetAuthorisation();
};

