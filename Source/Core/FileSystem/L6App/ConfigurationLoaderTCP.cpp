/**
 * @file ConfigurationLoaderTCP.cpp
 * @brief Source file for class ConfigurationLoaderTCP
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

 * @details This source file contains the definition of all the methods for
 * the class ConfigurationLoaderTCP (public, protected, and private). Be aware that some 
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
#include "ConfigurationLoaderTCP.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConfigurationLoaderTCP::ConfigurationLoaderTCP() :
        RealTimeLoader(), EmbeddedServiceMethodBinderI(), tcpClientService(*this) {
    waitForConnection = true;
    (void) mux.Create();
    timeout = 10u;
}

/*lint -e{1551} the service and the semaphore must be closed in destructor*/
ConfigurationLoaderTCP::~ConfigurationLoaderTCP() {
    if (tcpClientService.Stop() != ErrorManagement::NoError) {
        if (tcpClientService.Stop() != ErrorManagement::NoError) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not Stop the tcpThreadService");
        }
    }
    (void) mux.UnLock();
    (void) mux.Close();
    if (!socket.Close()) {
        REPORT_ERROR(ErrorManagement::Warning, "Could not Close the socket");
    }
}

ErrorManagement::ErrorType ConfigurationLoaderTCP::PostInit() {
    uint16 port = 0u;
    bool ok = RealTimeLoader::PostInit();
    if (ok) {
        ok = postInitParameters.IsValid();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the LoaderPostInit parameters.");
        }
    }
    if (ok) {
        ok = postInitParameters->Read("Port", port);
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
        tcpClientService.SetName(GetName());
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

ErrorManagement::ErrorType ConfigurationLoaderTCP::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err;

    if (info.GetStageSpecific() == ExecutionInfo::WaitRequestStageSpecific) {
        if(mux.Lock() == ErrorManagement::NoError) {
            if (waitForConnection) {
                waitForConnection = false;
                (void) mux.UnLock();
                BasicTCPSocket *client = socket.WaitConnection(timeout);
                if (client != NULL_PTR(BasicTCPSocket *)) {
                    REPORT_ERROR(ErrorManagement::Information, "Connection accepted!");
                    if (mux.Lock() == ErrorManagement::NoError) {
                        waitForConnection = true;
                        info.SetThreadSpecificContext(reinterpret_cast<void *>(client));
                        err = ErrorManagement::NoError;
                    }
                    (void) mux.UnLock();
                }
                else {
                    if (mux.Lock() == ErrorManagement::NoError) {
                        waitForConnection = true;
                        err = ErrorManagement::Timeout;
                    }
                    (void) mux.UnLock();
                }
            }
            else {
                (void) mux.UnLock();
                Sleep::MSec(timeout);
                err = ErrorManagement::Timeout;
            }
        }
        else {
            (void) mux.UnLock();
        }
    }
    if (info.GetStageSpecific() == ExecutionInfo::ServiceRequestStageSpecific) {
        BasicTCPSocket *client = reinterpret_cast<BasicTCPSocket *>(info.GetThreadSpecificContext());
        if (client != NULL_PTR(BasicTCPSocket *)) {
            StreamString errStream;
            StreamString newCfg;
            const uint32 BUFFER_SIZE = 1024u;
            char8 *buffer = new char8[BUFFER_SIZE + 1u];
            bool moreToRead = true;
            while (moreToRead) {
                uint32 readBytes = BUFFER_SIZE;
                moreToRead = client->Read(&buffer[0], readBytes);
                if (readBytes <= BUFFER_SIZE) {
                    buffer[readBytes] = '\0'; //Note that +1u was allocated
                }
                if (moreToRead) {
                    (void) newCfg.Printf("%s", buffer);
                    moreToRead = (readBytes == BUFFER_SIZE);
                }
            }
            delete [] buffer;
            err.fatalError = newCfg.Seek(0LLU); 
            ErrorManagement::ErrorType ret = Loader::Reconfigure(newCfg, errStream);
            StreamString retStr;
            if (ret.ErrorsCleared()) {
                (void) retStr.Printf("%s\n", "OK");
            }
            else {
                (void) retStr.Printf("ERR:%s\n", errStream.Buffer());
                REPORT_ERROR(ret, retStr.Buffer());
            }
            uint32 retStrSize = static_cast<uint32>(retStr.Size());
            if (!client->Write(retStr.Buffer(), retStrSize)) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Failed to reply to client");
            }
            if (!client->Close()) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Failed to Close client connection");
            }
            delete client;
        }
        err = ErrorManagement::Completed;
    }
    return err;
}

CLASS_REGISTER(ConfigurationLoaderTCP, "")
}

