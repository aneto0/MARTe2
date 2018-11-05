/**
 * @file HttpProtocolTest.h
 * @brief Header file for class HttpProtocolTest
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

 * @details This header file contains the declaration of the class HttpProtocolTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPPROTOCOLTEST_H_
#define HTTPPROTOCOLTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventSem.h"
#include "HttpProtocol.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the HttpProtocol methods
 */
class HttpProtocolTest {
public:

    /**
     * @brief Constructor
     */
    HttpProtocolTest();

    /**
     * @brief Destructor
     */
    virtual ~HttpProtocolTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the ReadHeaderMethod with GET request
     */
    bool TestReadHeader_Get1();

    /**
     * @brief Tests the ReadHeaderMethod with GET request passing commands
     */
    bool TestReadHeader_Get2_Commands();

    /**
     * @brief Tests the ReadHeaderMethod with PUT request
     */
    bool TestReadHeader_Put1();

    /**
     * @brief Tests the ReadHeaderMethod with POST request
     */
    bool TestReadHeader_Post1();

    /**
     * @brief Tests the ReadHeaderMethod with POST request
     * and multipart/form-data content type
     */
    bool TestReadHeader_Post2_Multiform();

    /**
     * @brief Tests the ReadHeaderMethod with POST request
     * and multipart/form-data content type. The boundary is wrapped by quotes
     */
    bool TestReadHeader_Post2_Multiform_WrappedBoundary();

    /**
     * @brief Tests the ReadHeaderMethod with POST request
     * and multipart/form-data content type and consecutive data variables.
     */
    bool TestReadHeader_Post2_MultiformConsecutiveData();

    /**
     * @brief Tests the ReadHeaderMethod with POST request
     * and multipart/form-data content type that fails if the boundary
     * is not defined in the header.
     */
    bool TestReadHeader_Post2_FalseMultiformNoParsedBoundary();

    /**
     * @brief Tests the ReadHeaderMethod with HEAD request
     */
    bool TestReadHeader_Head();

    /**
     * @brief Tests the ReadHeaderMethod with HTTP reply
     */
    bool TestReadHeader_Reply();

    /**
     * @brief Tests the ReadHeaderMethod with HTTP reply
     * but invalid HTTP version
     */
    bool TestReadHeader_IncompatibleHTTPVersion();

    /**
     * @brief Tests the ReadHeaderMethod with GET request
     * but invalid HTTP version
     */
    bool TestReadHeader_IncompatibleHTTPVersionNoReply();

    /**
     * @brief Tests the ReadHeaderMethod with HTTP reply
     * but invalid reply code
     */
    bool TestReadHeader_FalseInvalidReplyCode();

    /**
     * @brief Tests the ReadHeaderMethod with nvalid
     * request command.
     */
    bool TestReadHeader_FalseInvalidCommand();

    /**
     * @brief Tests the CompleteReadOperation method.
     */
    bool TestCompleteReadOperation();

    /**
     * @brief Tests the CompleteReadOperation with a body size greater than 1024
     */
    bool TestCompleteReadOperation_ClipSize();

    /**
     * @brief Used to tests the WriteHeaderMethods with different
     * request commands and isMessageComplete=true
     */
    bool TestWriteHeader();

    /**
     * @brief Used to tests the WriteHeaderMethods with different
     * request commands and isMessageComplete=false
     */
    bool TestWriteHeader2();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=true
     * and GET command
     */
    bool TestWriteHeader_Get1();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=false
     * and GET command
     */
    bool TestWriteHeader_Get2();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=true
     * and PUT command
     */
    bool TestWriteHeader_Put1();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=false
     * and PUT command
     */
    bool TestWriteHeader_Put2();


    /**
     * @brief Tests the WriteHeader method with isMessageComplete=true
     * and HEAD command
     */
    bool TestWriteHeader_Head1();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=false
     * and HEAD command
     */
    bool TestWriteHeader_Head2();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=true
     * and POST command
     */
    bool TestWriteHeader_Post();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=true
     * and HTTP reply
     */
    bool TestWriteHeader_Reply();

    /**
     * @brief Tests the WriteHeader method with isMessageComplete=false
     * and HTTP reply
     */
    bool TestWriteHeader_Reply2();


    /**
     * @brief Tests the WriteHeader method passing a payload written using
     * a StreamStructuredData with isMessageComplete=true.
     */
    bool TestWriteHeader_StrucuredDataStored();

    /**
     * @brief Tests the WriteHeader method passing a payload written using
     * a StreamStructuredData with isMessageComplete=false.
     */
    bool TestWriteHeader_StrucuredDataOnline();

    /**
     * @brief Tests the WriteHeader that fails if an invalid HTTP command
     * is given as input
     */
    bool TestWriteHeader_False_InvalidCommand();

    /**
     * @brief Tests the SecurityCheck method
     */
    bool TestSecurityCheck();

    /**
     * @brief Tests the KeepAlive method that returns
     * true if the client does not specify the conection mode.
     */
    bool TestKeepAliveDefault();

    /**
     * @brief Tests the KeepAlive method
     */
    bool TestKeepAlive(bool isKeepAliveIn);

    /**
     * @brief Tests the SetKeepAlive method.
     */
    bool TestSetKeepAlive(bool isKeepAlive);

    /**
     * @brief Tests the GetHttpCommand method.
     */
    bool TestGetHttpCommand(int32 commandIn);

    /**
     * @brief Tests the SetUnmatchedId method.
     */
    bool TestSetUnmatchedId();

    /**
     * @brief Tests the GetUnmatchedId method.
     */
    bool TestGetUnmatchedId();

    /**
     * @brief Tests the GetPath method.
     */
    bool TestGetPath();

    /**
     * @brief Tests the GetId method.
     */
    bool TestGetId();

    /**
     * @brief Tests the TextMode method.
     */
    bool TestTextMode(int8 textMode);

    /**
     * @brief Tests the ReadHeader method that fails if
     * no data is sent by the host.
     */
    bool TestReadHeader_False_FailGetLine();

    /**
     * @brief Tests the ReadHeader method that fails if
     * it is a POST request but without the content-type.
     */
    bool TestReadHeader_False_PostNoContentType();

    /**
     * @brief Tests the ReadHeader method that fails if
     * it is a POST request but without the content-length.
     */
    bool TestReadHeader_False_PostNoContentLength();

    /**
     * @brief tests the GetInputCommand method.
     */
    bool TestGetInputCommand();

    EventSem eventSem;

    bool retVal;

    int32 command;

    bool isKeepAlive;

    int8 textMode;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPPROTOCOLTEST_H_ */

