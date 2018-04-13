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
        socket.Open();
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
            if (client != NULL) {
                mux.Lock();
                waitForConnection = true;
                info.SetThreadSpecificContext(reinterpret_cast<void *>(client));
                mux.UnLock();
            }
        }
        else {
            mux.UnLock();
            MARTe::Sleep::Sec(0.1);
            err = ErrorManagement::Timeout;
            tcpClientService.
        }
    }
    if (information.GetStageSpecific() == MARTe::ExecutionInfo::ServiceRequestStageSpecific) {
        MARTe::Sleep::Sec(0.1);
        delete client;

        mux.Lock();
        if (connectIsServed) {
            connectIsServed = false;
            numberConnectionsServing--;
            mux.UnLock();
            return MARTe::ErrorManagement::Completed;
        }
        mux.UnLock();
        return MARTe::ErrorManagement::NoError;
    }
    return err;
}

}

