/**
 * @file HttpService.cpp
 * @brief Source file for class HttpService
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

 * @details This source file contains the definition of all the methods for
 * the class HttpService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "HttpProtocol.h"
#include "HttpRealmI.h"
#include "HttpService.h"
#include "JsonPrinter.h"
#include "ObjectRegistryDatabase.h"
#include "Select.h"
#include "StreamStructuredData.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HttpService::HttpService() :
        /*lint -e{9153} the specialised contructor will be called.*/
        MultiClientService(embeddedMethod), MessageI(), embeddedMethod(*this, &HttpService::ServerCycle) {
    port = 0u;
    listenMaxConnections = 0;
    textMode = 1u;
    chunkSize = 0u;
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551, 1506} the MultiThreadService must be stopped at destruction time.*/
HttpService::~HttpService() {
    (void) server.Close();
    if (Stop() != ErrorManagement::NoError) {
        REPORT_ERROR(ErrorManagement::Warning, "Could not Stop. Going to kill the service");
        (void) Stop();
    }
}

bool HttpService::Initialise(StructuredDataI &data) {
    //Cannot have more than one thread listening for the request.
    bool ret = data.Write("MinNumberOfThreads", 1);
    if (ret) {
        ret = MultiClientService::Initialise(data);
    }
    if (ret) {
        uint32 acceptTimeoutTemp;
        if (!data.Read("AcceptTimeout", acceptTimeoutTemp)) {
            acceptTimeoutTemp = 1000u;
            REPORT_ERROR(ErrorManagement::Information, "AcceptTimeout not specified, using defauld %d ms", acceptTimeoutTemp);
        }
        acceptTimeout = acceptTimeoutTemp;

        if (!data.Read("Port", port)) {
            port = 80u;
            REPORT_ERROR(ErrorManagement::Information, "Port not specified: using default %d", port);
        }
        if (!data.Read("ListenMaxConnections", listenMaxConnections)) {
            listenMaxConnections = 255;
            REPORT_ERROR(ErrorManagement::Information, "ListenMaxConnections not specified: using default %d", listenMaxConnections);
        }
        if (!data.Read("IsTextMode", textMode)) {
            textMode = 1u;
            REPORT_ERROR(ErrorManagement::Information, "IsTextMode unspecified: using default %d", textMode);
        }
        if (!data.Read("ChunkSize", chunkSize)) {
            chunkSize = 32u;
            REPORT_ERROR(ErrorManagement::Information, "ChunkSize not specified: using default %d", chunkSize);
        }
        Reference ref = this->Find("WebRoot");
        if (ref.IsValid()) {
            webRoot = ref;
            ret = webRoot.IsValid();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The WebRoot is not a DataExportI");
            }
        }
        else {
            ret = data.Read("WebRoot", webRootPath);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::ParametersError, "WebRoot path shall be specified or added to the container");
            }
        }
    }

    return ret;
}

ErrorManagement::ErrorType HttpService::Start() {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (!webRoot.IsValid()) {
        webRoot = ObjectRegistryDatabase::Instance()->Find(webRootPath.Buffer());
        err.parametersError = !(webRoot.IsValid());
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Invalid WebRoot path %s or WebRoot is not a DataExportI", webRootPath.Buffer());
        }
    }
    if (err.ErrorsCleared()) {
        err = !(server.Open());

        if (err.ErrorsCleared()) {
            err = !(server.Listen(port, listenMaxConnections));

            if (err.ErrorsCleared()) {
                err = MultiClientService::Start();
            }
        }
    }
    return err;
}

ErrorManagement::ErrorType HttpService::ClientService(HttpChunkedStream * const commClient) const {
    ErrorManagement::ErrorType err = !(commClient == NULL);

    if (err.ErrorsCleared()) {
        err = !(commClient->SetBlocking(true));
    }
    Select sel;
    if (err.ErrorsCleared()) {
        commClient->SetChunkMode(false);
        err = !(sel.AddReadHandle(*commClient));
    }
    //give the possibility to stop the thread
    if (err.ErrorsCleared()) {
        uint8 requestedTextMode = textMode;
        HttpProtocol hprotocol(*commClient);
        if (sel.WaitUntil(1000u) > 0) {
            //you want plain text or data
            if (!hprotocol.ReadHeader()) {
                err = ErrorManagement::CommunicationError;
                REPORT_ERROR(ErrorManagement::CommunicationError, "Error while reading HTTP header");
            }
            bool pagePrepared = false;

            if (err.ErrorsCleared()) {
                if (hprotocol.TextMode() >= 0) {
                    requestedTextMode = static_cast<uint8>(hprotocol.TextMode());
                }
            }
            if (err.ErrorsCleared()) {
                if (!hprotocol.MoveAbsolute("OutputOptions")) {
                    err = !(hprotocol.CreateAbsolute("OutputOptions"));
                }
                if (requestedTextMode > 0u) {
                    pagePrepared = webRoot->GetAsText(*commClient, hprotocol);
                }
                else {
                    StreamStructuredData<JsonPrinter> sdata;
                    sdata.SetStream(*commClient);
                    pagePrepared = webRoot->GetAsStructuredData(sdata, hprotocol);
                }
                if (err.ErrorsCleared()) {
                    err = !(commClient->Flush());
                }
                if (err.ErrorsCleared()) {
                    if (commClient->IsChunkMode()) {
                        err = !(commClient->FinalChunk());
                    }
                }
            }
            if (err.ErrorsCleared()) {
                if (!pagePrepared) {
                    //TODO??
                }
            }
        }
        if (err.ErrorsCleared()) {
            if (!hprotocol.KeepAlive()) {
                REPORT_ERROR(ErrorManagement::Information, "Connection closed");
                err = !(commClient->Close());
                if (err.ErrorsCleared()) {
                    err = ErrorManagement::Completed;
                }
                delete commClient;
            }

        }
        else {
            (void) (commClient->Close());
            delete commClient;
            REPORT_ERROR(ErrorManagement::Information, "client deleted");
        }

    }

    return err;

}

ErrorManagement::ErrorType HttpService::ServerCycle(MARTe::ExecutionInfo &information) {
    ErrorManagement::ErrorType err;
    if (information.GetStage() == MARTe::ExecutionInfo::StartupStage) {
    }
    if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {

        /*lint -e{593} -e{429} the newClient pointer will be freed within the thread*/
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::WaitRequestStageSpecific) {
            /*lint -e{429} the newClient pointer will be freed within the thread*/
            HttpChunkedStream *newClient = new HttpChunkedStream();
            newClient->SetChunkMode(false);
            newClient->SetCalibWriteParam(0u);
            err = !(newClient->SetBufferSize(32u, chunkSize));
            if (err.ErrorsCleared()) {
                if (server.WaitConnection(acceptTimeout, newClient) == NULL) {
                    err = MARTe::ErrorManagement::Timeout;
                    delete newClient;
                }
                else {
                    if (GetNumberOfActiveThreads() == GetMaximumNumberOfPoolThreads()) {
                        err = MARTe::ErrorManagement::Timeout;
                        HttpProtocol hprotocol(*newClient);
                        StreamString s;
                        (void) s.SetSize(0LLU);
                        if (!hprotocol.WriteHeader(false, HttpDefinition::HSHCReplyTooManyRequests, &s, NULL_PTR(const char8*))) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Too many connections");
                        }
                        REPORT_ERROR(ErrorManagement::Warning, "Too many connections");
                        (void) newClient->Close();
                        delete newClient;
                    }
                    else {
                        information.SetThreadSpecificContext(reinterpret_cast<void*>(newClient));
                        err = MARTe::ErrorManagement::NoError;
                    }
                }
            }
        }
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::ServiceRequestStageSpecific) {
            HttpChunkedStream *newClient = reinterpret_cast<HttpChunkedStream *>(information.GetThreadSpecificContext());
            err = ClientService(newClient);
        }
    }

    return err;
}

uint16 HttpService::GetPort() const {
    return port;
}

int32 HttpService::GetMaxConnections() const {
    return listenMaxConnections;
}

ReferenceT<HttpDataExportI> HttpService::GetWebRoot() const {
    return webRoot;
}

CLASS_REGISTER(HttpService, "1.0")
CLASS_METHOD_REGISTER(HttpService, Start)
}

