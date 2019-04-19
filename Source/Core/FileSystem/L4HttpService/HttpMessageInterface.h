/**
 * @file HttpMessageInterface.h
 * @brief Header file for class HttpMessageInterface
 * @date 25/03/2019
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

 * @details This header file contains the declaration of the class HttpMessageInterface
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPMESSAGEINTERFACE_H_
#define SOURCE_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPMESSAGEINTERFACE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpDataExportI.h"
#include "MessageI.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Sends Messages identified by a given HTTP command.
 *
 * @details Extracts from the HttpProtocol the command "msg" and if the value of the command corresponds to the name of any of its child objects,
 * the corresponding message is sent.
 *
 * @details The configuration syntax is (names are only given as an example):
 * <pre>
 * +HttpMessageInterface1 = {
 *     Class = HttpMessageInterface
 *     +Message1 = {
 *         Class = Message //See Message documentation.
 *         Destination = SomeObject
 *     }
 *     +MessageN = {
 *         Class = Message
 *         Destination = SomeObject
 *     }
 *     ...
 * }
 * </pre>
 */
class HttpMessageInterface: public ReferenceContainer, public HttpDataExportI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
HttpMessageInterface    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~HttpMessageInterface();

    /**
     * @brief Calls ReferenceContainer::Initialise and verifies that all child objects inherit from Message.
     * @param[in] data see ReferenceContainer::Initialise.
     * @return true if the parameters are correctly specified and all the child elements inherit from Message.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Gets the available messages (see ReferenceContainer::GetAsStructuredData) and if the HTTP command "msg" exists, send the corresponding data.
     * @param[out] data if the HTTP msg command exists writes OK=1 or 0 if the message is successfully sent, otherwise outputs the list of available messages.
     * @param[in] protocol expected to contain a \a msg parameter where to read the name of the message to send.
     * @return true if the message can be sent (only if the msg HTTP command was set).
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    /**
     * @brief If the HTTP command "msg" exists, send the corresponding data.
     * @param[out] stream not used.
     * @param[in] protocol expected to contain a \a msg parameter where to read the name of the message to send.
     * @return true if the message can be sent (only if the "msg" HTTP command was set).
     */
    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

private:

    /**
     * @brief Sends the message encoded in the "msg" HTTP command.
     * @param[in] protocol expected to contain a \a msg parameter where to read the name of the message to send.
     * @return true if the message can be sent (only if the "msg" HTTP command was set).
     */
    bool SendMessageFromHttp(HttpProtocol &protocol);
};
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPMESSAGEINTERFACE_H_ */
