/**
 * @file TCPSocketMessageProxyExample.cpp
 * @brief Source file for class TCPSocketMessageProxyExample
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

 * @details This source file contains the definition of all the methods for
 * the class TCPSocketMessageProxyExample (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "Message.h"
#include "MessageI.h"
#include "StandardParser.h"
#include "TCPSocketMessageProxyExample.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {

TCPSocketMessageProxyExample::TCPSocketMessageProxyExample() :
        MARTe::Object(), MARTe::EmbeddedServiceMethodBinderI(), tcpClientService(*this) {
    waitForConnection = true;
    mux.Create();
    timeout = 10;

}

TCPSocketMessageProxyExample::~TCPSocketMessageProxyExample() {
    using namespace MARTe;
    if (tcpClientService.Stop() != ErrorManagement::NoError) {
        if (tcpClientService.Stop() != ErrorManagement::NoError) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not Stop the tcpThreadService");
        }
    }
    mux.UnLock();
    mux.Close();
    if (!socket.Close()) {
        REPORT_ERROR(ErrorManagement::Warning, "Could not Close the socket");
    }
}

bool TCPSocketMessageProxyExample::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    uint32 port;
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not Read the Port parameter");
        }
    }
    if (ok) {
        ok = socket.Open();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not Open server socket");
        }
    }
    if (ok) {
        ok = socket.Listen(port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not Listen on port %d", port);
        }
    }
    if (ok) {
        ok = (tcpClientService.Start() == ErrorManagement::NoError);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not Start tcpClientService");
        }
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Server listening in port %d", port);
    }
    return ok;
}

MARTe::ErrorManagement::ErrorType TCPSocketMessageProxyExample::Execute(MARTe::ExecutionInfo & info) {
    using namespace MARTe;
    ErrorManagement::ErrorType err;

    if (info.GetStageSpecific() == MARTe::ExecutionInfo::WaitRequestStageSpecific) {
        mux.Lock();
        if (waitForConnection) {
            waitForConnection = false;
            mux.UnLock();
            BasicTCPSocket *client = socket.WaitConnection(timeout);
            if (client != NULL_PTR(BasicTCPSocket *)) {
                REPORT_ERROR(ErrorManagement::Information, "Connection accepted!");
                mux.Lock();
                waitForConnection = true;
                info.SetThreadSpecificContext(reinterpret_cast<void *>(client));
                err = ErrorManagement::NoError;
                mux.UnLock();
            }
            else {
                mux.Lock();
                waitForConnection = true;
                err = ErrorManagement::Timeout;
                mux.UnLock();
            }
        }
        else {
            mux.UnLock();
            MARTe::Sleep::MSec(timeout);
            err = ErrorManagement::Timeout;
        }
    }
    if (info.GetStageSpecific() == MARTe::ExecutionInfo::ServiceRequestStageSpecific) {
        BasicTCPSocket *client = reinterpret_cast<BasicTCPSocket *>(info.GetThreadSpecificContext());
        if (client != NULL_PTR(BasicTCPSocket *)) {
            const uint32 BUFFER_SIZE = 1024u;
            char8 buffer[BUFFER_SIZE];
            uint32 readBytes = BUFFER_SIZE;
            MemoryOperationsHelper::Set(&buffer[0], '\0', BUFFER_SIZE);
            if (client->Read(&buffer[0], readBytes)) {
                StreamString configurationCfg = buffer;
                REPORT_ERROR(ErrorManagement::ParametersError, "Received configuration message [size=%d]:%s", readBytes, configurationCfg.Buffer());
                //Try to parse the configuration message
                StreamString err;
                //Force the string to be seeked to the beginning.
                configurationCfg.Seek(0LLU);
                ConfigurationDatabase msgCdb;
                StandardParser parser(configurationCfg, msgCdb, &err);
                ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                ErrorManagement::ErrorType msgError;
                msgError.parametersError = !parser.Parse();
                if (msgError.ErrorsCleared()) {
                    //After parsing the tree is pointing at the last leaf
                    msgCdb.MoveToRoot();
                    msgError.parametersError = !msg->Initialise(msgCdb);
                    if (!msgError.ErrorsCleared()) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Failed to initialise message");
                    }
                }
                else {
                    StreamString errPrint;
                    errPrint.Printf("Failed to parse %s", err.Buffer());
                    REPORT_ERROR(ErrorManagement::ParametersError, errPrint.Buffer());
                }
                if (msgError.ErrorsCleared()) {
                    msgError = MessageI::SendMessage(msg, this);
                    if (!msgError.ErrorsCleared()) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Error while sending message to destination %s with function %s",
                                     msg->GetDestination().GetList(), msg->GetFunction().GetList());
                    }
                }
                readBytes = BUFFER_SIZE;
            }
            if (!client->Close()) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Failed to Close client connection");
            }
            delete client;
        }
        return MARTe::ErrorManagement::Completed;
    }
    return err;
}

CLASS_REGISTER(TCPSocketMessageProxyExample, "")
}

