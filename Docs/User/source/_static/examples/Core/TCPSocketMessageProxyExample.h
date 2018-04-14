/**
 * @file TCPSocketMessageProxyExample.h
 * @brief Header file for class TCPSocketMessageProxyExample
 * @date 13/04/2018
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

 * @details This header file contains the declaration of the class TCPSocketMessageProxyExample
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_TCPSOCKETMESSAGEPROXYEXAMPLE_H_
#define EXAMPLES_CORE_TCPSOCKETMESSAGEPROXYEXAMPLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceMethodBinderI.h"
#include "Object.h"
#include "ReferenceContainer.h"
#include "MultiClientService.h"
#include "MutexSem.h"
#include "TCPSocket.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe2Tutorial {
/**
 * @brief An example of an Object which relays TCP encoded messages to MARTe messages.
 *
 * @details The messages send using TCP shall be a CDB encoded string containing the Message configuration (as prescribed in Message.h)
 *
 * <pre>
 * +TCPMessageProxy = {
 *     Class = TCPSocketMessageProxyExample
 *     Port = 24680//Compulsory. The port where to listen for the TCP messages.
 * }
 * </pre>
 */
class TCPSocketMessageProxyExample : public MARTe::Object, public MARTe::EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
TCPSocketMessageProxyExample();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TCPSocketMessageProxyExample();

    /**
     * @brief Reads the Port from the configuration file.
     * @param[in] data see Object::Initialise. The parameter Port shall exist the socket will be opened to listen on this port.
     * @return true if the Port parameter can be read, socket opened and the TCP listening service started.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Callback function for the EmbeddedThread that waits for TCP messages .
     * @param[in] info see EmbeddedServiceMethodBinderI
     * @return ErrorManagement::NoError.
     */
    virtual MARTe::ErrorManagement::ErrorType Execute(MARTe::ExecutionInfo & info);


private:

    /**
     * Listens for new messages on this socket.
     */
    MARTe::TCPSocket socket;

    /**
     * The service that listens for new messages on the socket.
     */
    MARTe::MultiClientService tcpClientService;

    /**
     * A mux to guarantee locked access to the sockets.
     */
    MARTe::MutexSem mux;

    /**
     * True if the thread shall wait for a connection.
     */
    bool waitForConnection;

    /**
     * The timeout in ms
     */
    MARTe::int32 timeout;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXAMPLES_CORE_TCPSOCKETMESSAGEPROXYEXAMPLE_H_ */
