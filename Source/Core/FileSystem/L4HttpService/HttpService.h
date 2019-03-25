/**
 * @file HttpService.h
 * @brief Header file for class HttpService
 * @date 24/08/2018
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

 * @details This header file contains the declaration of the class HttpService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPSERVICE_H_
#define HTTPSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderT.h"
#include "FastPollingMutexSem.h"
#include "HttpChunkedStream.h"
#include "HttpDataExportI.h"
#include "MessageI.h"
#include "MultiClientService.h"
#include "ReferenceT.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Implementation of an HTTP server.
 * @see MultiClientService.
 *
 * @details Performs the following tasks:\n
 *   - Waits on the principal socket for a new client connection.\n
 *   - Once a new client is connected, the new socket is created to handle the
 *     specific connection and a new thread is launched with the ServerCycle task to
 *     manage new client connections.\n
 *   -The current thread with the new created socket calls the ClientService function to
 *    manage the client HTTP requests.\n
 *   - Once the client closes the connection (or in case of error) the thread terminates.\n
 * @details The client can send a specific HTTP command called TextMode to express that it wants to receive data
 * in text mode (TextMode=1) or in database mode (TextMode=0). If TextMode=1, the HttpService will call the
 * DataExportI::GetAsText of the object found at the path received from the client, or DatExportI::GetAsStructuredData if
 * TextMode=0. In this second case a StructuredDataStream<JsonPrinter> is passed in input to the DatExportI::GetAsStructuredData
 * in order to send to the client data written in json language.
 *
 * @details The HttpService replies to the client always using the HTTP chunked transfer encoding. This allows to stream out
 * data to the socket without knowing a priori the full length of the HTTP message body. This allows to avoid having to store the
 * whole body in memory before sending it.
 *
 * @details The configuration syntax is (names are only given as an example):
 * <pre>
 * +HttpService1 = {
 *     Class = HttpService
 *     AcceptTimeout = 1000 //Optional (default = 1000).  Timeout in the TCPSocket::WaitConnection to wait for new client connections.
 *     Port = 8084 //Optional (default = 80). The HTTP server port.
 *     Timeout = 0 //Compulsory. See MultiClientService::Initialise
 *     MaxNumberOfThreads = 8 //Compulsory. See MultiClientService::Initialise
 *     ListenMaxConnections = 255 //Compulsory. The maximum number of HTTP connections.
 *     WebRoot = ARoot //Compulsory. Path in the ObjectConfigurationDatabase of the object that acts as the root for the service. This object shall inherit from HttpDataExportI.
 *     IsTextMode = 1 //Optional (default = 1). If the GET option TextMode is not set, the reply is either sent as text/html (IsTextMode = 1) or as text/json (IsTextMode = 0). With the former GetAsText is called on the web root object, while with the latter GetAsStructuredData is called instead.
 *     ChunkSize = 32 //Optional (default = 32). The maximum size of the chunks in which the reply bode is divided to perform the chunked transfer encoding mode.
 * }
 * </pre>
 */
class HttpService: public MultiClientService, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    HttpService();

    /**
     * @brief Destructor.
     */
    virtual ~HttpService();

    /**
     * @see MultiClientService::Initialise.
     * @details The following parameters can be specified:\n
     *   AcceptTimeout: the timeout in the TCPSocket::WaitConnection that waits for new client connections. This allows to not
     *     block infinitely a thread so that can be stopped using MultiClientService::Stop(). Default=1000.
     *   Port: The port number where to accept new client connections. Default=80
     *   ListenMaxConnections: The maximum number of connection to be given in input to the TCPSocket::Listen function.
     *     Default=255.
     *   WebRoot: The path of in the ObjectConfigurationDatabase of the object that acts as the root of the paths received
     *     by the clients.
     *   IsTextMode: The default data sending mode. A client can change this mode by sending the HTTP command called TextMode=[0(false), 1(true)].
     *     Default=1 (text mode).
     *   ChunkSize: the maximum size of the chunks in which the reply bode is divided to perform the chunked transfer encoding mode. Default = 32
     * @return true if all the parameters are set and valid.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see MultiClientService::Start
     * @details Before starting the thread, it finds the root object specified in the \a WebRoot configuration parameter.
     * If the path is wrong, ErrorManagement::FatalError is returned.
     */
    virtual ErrorManagement::ErrorType Start();


    /**
     * @brief The server task that has to be performed by one thread at once.
     * @brief Once a new connection has been created the thread calls ClientService to handle the
     * connection task with the client, while a new thread calls ServerCycle to wait for new connections.
     * @param[in] information specifies the thread status managed in MultiClientEmbeddedThread::ThreadLoop.
     */
    ErrorManagement::ErrorType ServerCycle(MARTe::ExecutionInfo &information);

    /**
     * @brief Handles the connection with a specific client.
     * @details Until the connection is keep alive by the client, the HttpService receives HTTP messages
     * and calls the objects at the paths specified by the client (that must implement a DataExportI interface)
     * to get the required data to be sent as a HTTP reply.
     * @param[in] commClient is the socket to communicate with the client.
     */
    ErrorManagement::ErrorType ClientService(HttpChunkedStream * const commClient) const;

    /**
     * @brief Gets the configured port.
     * @return the configured port.
     */
    uint16 GetPort() const;

    /**
     * @brief Gets the maximum number of connections.
     * @return the maximum number of connections.
     */
    int32 GetMaxConnections() const;

    /**
     * @brief Gets the root reference.
     * @return the maximum number of connections.
     */
    ReferenceT<HttpDataExportI> GetWebRoot() const;

private:
    /**
     * The server socket
     */
    TCPSocket server;

    /**
     * The port this server is listening to
     */
    uint16 port;

    /**
     * The maximum number of connections
     * that Listen can handle.
     */
    int32 listenMaxConnections;

    /**
     * Where the web pages are contained.
     * It will use the URL to search in the container
     */
    ReferenceT<HttpDataExportI> webRoot;

    /**
     * The ServerCycle method to be executed in the thread loop.
     */
    EmbeddedServiceMethodBinderT<HttpService> embeddedMethod;

    /**
     * The web root path in the ObjectRegistryDatabase
     */
    StreamString webRootPath;

    /**
     * The text mode flag.
     */
    uint8 textMode;

    /**
     * The timeout in the WaitConnection function
     */
    TimeoutType acceptTimeout;

    /**
     * The HTTP reply body chunk size.
     */
    uint32 chunkSize;

    /**
     * Filter to receive the RPC
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPSERVICE_H_ */

