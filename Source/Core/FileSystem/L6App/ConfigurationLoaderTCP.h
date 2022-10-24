/**
 * @file ConfigurationLoaderTCP.h
 * @brief Header file for class ConfigurationLoaderTCP
 * @date 17/02/2022
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

 * @details This header file contains the declaration of the class ConfigurationLoaderTCP
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILESYSTEM_L6APP_CONFIGURATION_LOADER_TCP_H_
#define FILESYSTEM_L6APP_CONFIGURATION_LOADER_TCP_H_ 

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
#include "RealTimeLoader.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {
/**
 * @details A Loader implementation that allows to reconfiguration MARTe applications using a TCP interface.
 * @details The messages sent using TCP shall be encoded as a string, using the bootstrapped Loader parser language,
 * containing the new configuration to be applied.
 *
 * <pre>
 *  +LoaderPostInit = {
 *    Class = ReferenceContainer
 *    +Parameters = {
 *        Class = ConfigurationDatabase
 *        Port = 24680//Compulsory. The port where to listen for the TCP messages.
 *        ReloadLast = true //Optional. Reload old configuration in case of (re)configuration failure? Default = true. If true the FailedConfiguration message will not to be sent.
 *        KeepAlive = "MyObj1 MyObj2 MyObj3" //Optional. Array with name of the objects (at the root level) that shall not be purged and will always (i.e. even in case of error) be readded to the ObjectRegistryDatabase after a reconfiguration is triggered.
 *    }
 *    +Messages = {
 *      Class = ReferenceContainer
 *      +PreConfiguration = { //Optional message to send before the configuration is applied.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *      +PostConfiguration = { //Optional message to send if the configuration was successfully applied.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *      +FailedConfiguration = { //Optional message to send if the configuration failed. Note that given that the ObjectRegistryDatabase will have been purged, it is likely that this message will not reach its destination...see KeepAlive.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *      +ReloadedConfiguration = { //Optional message to send if the configuration was reloaded as a consequence of a failed reconfiguration attempt.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }

 *    }
 *    +Hash = { //Optional. If set the configuration will be verified against the Hash function.
 *      Class = ConfigurationLoaderHashI
 *    }
 *  }
 * </pre>
 */
class ConfigurationLoaderTCP : public RealTimeLoader, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
ConfigurationLoaderTCP();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConfigurationLoaderTCP();

    /**
     * @brief Reads the Port from the configuration file.
     * @details see Loader::PostInit. The parameter Port shall exist the socket will be opened to listen on this port.
     * @return true if the Port parameter can be read, socket opened and the TCP listening service started.
     */
    virtual ErrorManagement::ErrorType PostInit();

    /**
     * @brief Callback function for the EmbeddedThread that waits for TCP messages .
     * @param[in] info see EmbeddedServiceMethodBinderI
     * @return ErrorManagement::NoError.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);


private:

    /**
     * Listens for new messages on this socket.
     */
    TCPSocket socket;

    /**
     * The service that listens for new messages on the socket.
     */
    MultiClientService tcpClientService;

    /**
     * A mux to guarantee locked access to the sockets.
     */
    MutexSem mux;

    /**
     * True if the thread shall wait for a connection.
     */
    bool waitForConnection;

    /**
     * The timeout in ms
     */
    uint32 timeout;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILESYSTEM_L6APP_CONFIGURATION_LOADER_TCP_H_ */
