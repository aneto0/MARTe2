/**
 * @file HttpClient.h
 * @brief Header file for class HttpClient
 * @date 22/08/2018
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

 * @details This header file contains the declaration of the class HttpClient
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamI.h"
#include "HttpProtocol.h"
#include "TCPSocket.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of an HttpClient that allows to build and send HTTP requests and to receive replies from the server.
 *
 * @details The request may contain an authorization digest that allows the called object on the server side
 * to check the authenticity of the client caller. This mechanism can be of two types:
 *   Basic: the Authorization field of the HTTP message is filled with the authorisation code set by the client.
 *   Digest: assuming that the authorisation code set by the client is the base64 encoded result of user:passwd,
 *     the Authorization field of the HTTP message will be:
 *     "Digest "
 *     "username=[username],"
 *     "realm=[realm received by the object on the server]","
 *     "nonce=[nonce received by the object on the server],"
 *     "uri=[address of the object on the server to be called],"
 *     "qop=auth,"
 *     "nc=[operation id],"
 *     "cnonce=MD5(threadId, idLength, random),"
 *     "response=MD5(MD5(user, realm, passwd), nonce, nc, cnonce, qop, MD5(command, uri)),"
 *     "opaque=[opaque received by the object on the server]"
 *   So, basically, the object on the server sends the [realm], the [nonce] and the [opaque], the client computes the response and sends back
 *   all the inputs and the [response] to the server object. The server object can compute the response by its own using these inputs
 *   and check if the result is equal to [response]. The object on the server should implement the HttpRealmI interface; more informations
 *   can be found by looking at its documentation.
 *
 *   @see the Initialise method for the available configuration options.
 */
class HttpClient: public Object {
public:

    /**
     * @brief Default constructor
     */
    HttpClient();

    /**
     * @brief Destructor
     */
    virtual ~HttpClient();

    /**
     * @see Object::Initialise
     * @details The following parameters can be configured.
     *   ServerAddress: the server ip address
     *   ServerPort: the server port
     *   ServerUri: the server uri that identifies the object that has to be called on the server side.
     *   Authorisation: the initial message to be insert in the Authorization field of the HTTP request.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Retrieves the internal HttpProtocol
     * @details this function can be used to set other HTTP header fields from outside this class implementation.
     * @return a pointer to the internal HttpProtocol.
     */
    HttpProtocol *GetHttpProtocol();

    /**
     * @brief Sends the HTTP request and waits for reply. If the request has not been authorised,
     * then it updates the Authorization field depending on what has been sent from the server and
     * tries again (if the connection has not been closed by the server itself)
     * @param[out] streamDataRead contains the server reply body in output.
     * @param[in] command the HTTP command code. It can be one of the following:
     *   HttpDefinition::HSHCGet, HttpDefinition::HSHCPut, HttpDefinition::HSHCPost, HttpDefinition::HSHCHead.
     * @param[in] payload is the body to be sent with the HTTP request.
     * @param[in] msecTimeout the operation timeout.
     * @param[in] operationId the operation id.
     * @return true if the HTTP request has been sent correctly and a reply has been received. False if timeout of
     * errors occur.
     */
    bool HttpExchange(BufferedStreamI &streamDataRead, const int32 command, BufferedStreamI * const payload = NULL_PTR(BufferedStreamI *), TimeoutType msecTimeout = TTInfiniteWait, int32 operationId = -1);

    /**
     * @brief Sets the server ip address.
     * @param[in] serverAddressIn the server ip address to be set.
     */
    void SetServerAddress(const char8 * const serverAddressIn);

    /**
     * @brief Sets the server port.
     * @param[in] serverPortIn the server port to be set.
     */
    void SetServerPort(const uint16 serverPortIn);

    /**
     * @brief Sets the server uri.
     * @param[in] serverUriIn the server uri to be set.
     */
    void SetServerUri(const char8 * const serverUriIn);

    /**
     * @brief Sets the authorisation text to be sent along with the HTTP request.
     * @param[in] authIn the authorisation text to be sent along with the HTTP request.
     */
    void SetAuthorisation(const char8 * const authIn);

    /**
     * @brief Retrieves the server ip address.
     * @param[out] serverAddrOut the server ip address in output.
     *
     */
    void GetServerAddress(StreamString &serverAddrOut) const;

    /**
     * @brief Retrieves the server port.
     * @return the server port number.
     */
    uint32 GetServerPort() const;

    /**
     * @brief Retrieves the server uri.
     * @param[out] serverUriOut the server uri in output.
     */
    void GetServerUri(StreamString &serverUriOut) const;

    /**
     * @brief Retrieves the original set authorisation message.
     * @param[out] authOut the authorisation message in output.
     */
    void GetAuthorisation(StreamString &authOut) const;
protected:

    /**
     * @brief Connects to the remote server.
     * @param[in] msecTimeout the connection timeout.
     */
    bool Connect(const TimeoutType &msecTimeout);

    /**
     * @brief Computes the local nonce as MD5(threadId, threadId size, random)
     * @param[out] nonce the local nonce in output,
     */
    bool CalculateNonce(StreamString & nonce);

    /**
     * @brief Generates the digest key.
     * @details Generates the digest key by appending all the inputs to the MD5 function that generates the key response, and the key response itself. In this way, the object on the server can check the client authenticity.
     * @param[out] key the authorisation digest key in output.
     * @param[in] data the data sent by the server object containing the [realm], the server object [nonce] and the [opaque].
     * @param[in] command the HTTP request command code.
     * @param[in] nc the operation id.
     * @return the key.
     */
    bool GenerateDigestKey(StreamString & key, const char8 * const data, const char8 * const command, int32 nc);

    /**
     * @brief Performs the Digest or Basic authentication procedure.
     * @param[in] command the HTTP request command code.
     * @param[in] msecTimeout the function timeout.
     * @param[in] operationId the operation id.
     */
    bool AutenticationProcedure(const int32 command, const TimeoutType &msecTimeout, const int32 operationId);

    /**
     * True if the client should attempt to reconnect.
     */
    bool reConnect;

    /**
     * The socket being used in the connection
     */
    TCPSocket tcpSocket;

    /**
     * Used to send the HTTP request and receive the reply
     */
    HttpProtocol protocol;

    /**
     * The uri to be sent to the server
     */
    StreamString urlUri;

    /**
     * The server ip address.
     */
    StreamString urlHost;

    /**
     * The server port.
     */
    uint16 urlPort;

    /**
     * Authorisation is <BASE64 of User:Password>
     */
    StreamString authorisation;

    /**
     * Digest or basic cooked user:password
     */
    StreamString authorisationKey;

    /**
     * A number counting the number of operations performed
     */
    int32 lastOperationId;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPCLIENT_H_ */
