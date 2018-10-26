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

#include "HttpService.h"
#include "ObjectRegistryDatabase.h"
#include "HttpProtocol.h"
#include "Select.h"
#include "HttpRealmI.h"
#include "DataExportI.h"
#include "ReferenceContainerFilterNameAndType.h"
#include "AdvancedErrorManagement.h"
#include "StreamStructuredData.h"
#include "JsonPrinter.h"
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
    closeOnAuthFail = 1u;
}

/*lint -e{1551} no exception will be thrown by the destructor*/
HttpService::~HttpService() {
    (void) server.Close();
}

bool HttpService::Initialise(StructuredDataI &data) {
    bool ret = MultiClientService::Initialise(data);
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
        if (!data.Read("WebRoot", webRootPath)) {
            webRoot = this->Find("WebRoot");
            ret = webRoot.IsValid();
            if (ret) {
                REPORT_ERROR(ErrorManagement::Information, "WebRoot not specified: using default this.WebRoot");
            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Please define a valid WebRoot path or add a WebRoot node in this container");
            }
        }
        if (!data.Read("IsTextMode", textMode)) {
            textMode = 1u;
            REPORT_ERROR(ErrorManagement::Information, "IsTextMode unspecified: using default %d", textMode);
        }

        if (!data.Read("ChunkSize", chunkSize)) {
            chunkSize = 32u;
            REPORT_ERROR(ErrorManagement::Information, "ChunkSize not specified: using default %d", chunkSize);

        }

        if (!data.Read("CloseOnAuthFail", closeOnAuthFail)) {
            closeOnAuthFail = 1u;
        }

    }

    return ret;
}

ErrorManagement::ErrorType HttpService::Start() {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;

    if (!webRoot.IsValid()) {
        webRoot = ObjectRegistryDatabase::Instance()->Find(webRootPath.Buffer());
        err = !(webRoot.IsValid());
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::Information, "Invalid WebRoot path %s", webRootPath.Buffer());
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

ErrorManagement::ErrorType HttpService::ClientService(HttpChunkedStream * const commClient) {
    ErrorManagement::ErrorType err = !(commClient == NULL);

    if (err.ErrorsCleared()) {
        err = !(commClient->SetBlocking(true));
        if (err.ErrorsCleared()) {
            commClient->SetChunkMode(false);
            HttpProtocol hprotocol(*commClient);

            Select sel;
            err = !(sel.AddReadHandle(*commClient));
            //give the possibility to stop the thread
            if (err.ErrorsCleared()) {

                if (sel.WaitUntil(1000u) > 0) {

                    //todo from here is possible to understand if
                    //you want plain text or data
                    if (!hprotocol.ReadHeader()) {
                        err = ErrorManagement::CommunicationError;
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Error while reading HTTP header");
                    }
                    ReferenceT<DataExportI> hi = webRoot;
                    bool pagePrepared = false;

                    if (err.ErrorsCleared()) {
                        if (hprotocol.TextMode() >= 0) {
                            textMode = static_cast<uint8>(hprotocol.TextMode());
                        }
                        StreamString path;
                        hprotocol.GetPath(path);
                        if (path.Size() > 0ull) {
                            // search for destination
                            int32 occurrences = 1;
                            uint32 mode = ReferenceContainerFilterMode::PATH;
                            ReferenceContainerFilterNameAndType<DataExportI> filter(occurrences, mode, path.Buffer());
                            ReferenceContainer results;
                            webRoot->Find(results, filter);
                            if (results.Size() > 0ull) {
                                uint32 last = static_cast<uint32>(results.Size()) - 1u;
                                hi = results.Get(last);
                            }
                            // save remainder of address
                            uint32 remAddrIndex = filter.GetRemainedAddrIndex();
                            StreamString urlTemp;
                            hprotocol.GetId(urlTemp);
                            StreamString unmatchedUrl = &urlTemp.Buffer()[remAddrIndex];
                            if (unmatchedUrl.Size() > 0ull) {
                                uint32 newUrlLastCharIdx = (static_cast<uint32>(unmatchedUrl.Size()) - 1u);
                                if (unmatchedUrl.Buffer()[newUrlLastCharIdx] == '/') {
                                    err = !(unmatchedUrl.SetSize(static_cast<uint64>(newUrlLastCharIdx)));
                                }
                            }

                            hprotocol.SetUnmatchedId(unmatchedUrl.BufferReference());
                        }

                        if (err.ErrorsCleared()) {
                            ReferenceT<HttpRealmI> hiRealm = hi;
                            if (hiRealm.IsValid()) {
                                //check security
                                //GCRTemplate<HttpRealm> realm = searchFilter.GetRealm();
                                //TODO security stuff
                                if (!hprotocol.SecurityCheck(hiRealm)) {
                                    if (!hprotocol.MoveAbsolute("OutputOptions")) {
                                        err = !(hprotocol.CreateAbsolute("OutputOptions"));
                                    }
                                    if (err.ErrorsCleared()) {
                                        StreamString contentType = "text/html";
                                        err = !hprotocol.Write("Content-Type", contentType.Buffer());
                                        if (err.ErrorsCleared()) {
                                            StreamString realmMsg;
                                            err = !(hiRealm->GetAuthenticationRequest(realmMsg));
                                            if (err.ErrorsCleared()) {
                                                err = !hprotocol.Write("WWW-Authenticate", realmMsg.Buffer());
                                            }
                                            StreamString hstream;
                                            if (err.ErrorsCleared()) {
                                                err = !hstream.Printf("%s", "<HTML><HEAD>\n"
                                                                      "<TITLE>401 Authorization Required</TITLE>\n"
                                                                      "</HEAD><BODY>\n"
                                                                      "<H1>Authorization Required</H1>\n"
                                                                      "This server could not verify that you\n"
                                                                      "are authorized to access the document you\n"
                                                                      "requested.  Either you supplied the wrong\n"
                                                                      "credentials (e.g., bad password), or your\n"
                                                                      "browser doesn't understand how to supply\n"
                                                                      "the credentials required.<P>\n"
                                                                      "</BODY></HTML>\n");
                                                if (err.ErrorsCleared()) {
                                                    err = !hstream.Seek(0ULL);
                                                }
                                            }
                                            if (err.ErrorsCleared()) {
                                                if (closeOnAuthFail > 0u) {
                                                    // force reissuing of a new thread
                                                    hprotocol.SetKeepAlive(false);
                                                }
                                                err = !hprotocol.WriteHeader(true, HttpDefinition::HSHCReplyAUTH, &hstream, NULL_PTR(const char8*));
                                                pagePrepared = err.ErrorsCleared();
                                            }

                                        }
                                    }
                                }
                            }

                            if (err.ErrorsCleared()) {
                                if (!pagePrepared) {

                                    int32 replyCode = hi->GetReplyCode(hprotocol);

                                    if (!hprotocol.MoveAbsolute("OutputOptions")) {
                                        err = !(hprotocol.CreateAbsolute("OutputOptions"));
                                    }
                                    if (err.ErrorsCleared()) {
                                        err = !(hprotocol.Write("Transfer-Encoding", "chunked"));
                                    }
                                    if (err.ErrorsCleared()) {
                                        err = !(hprotocol.Write("Content-Type", "text/html"));
                                    }
                                    if (err.ErrorsCleared()) {

                                        //empty string... go in chunked mode
                                        StreamString hstream;
                                        err = !(hprotocol.WriteHeader(false, replyCode, &hstream, NULL_PTR(const char8*)));
                                        if (err.ErrorsCleared()) {

                                            commClient->SetChunkMode(true);
                                            if (textMode > 0u) {
                                                pagePrepared = hi->GetAsText(*commClient, hprotocol);
                                            }
                                            else {
                                                StreamStructuredData<JsonPrinter> sdata;
                                                sdata.SetStream(*commClient);
                                                pagePrepared = hi->GetAsStructuredData(sdata, hprotocol);
                                            }

                                            err = !(commClient->Flush());
                                            if (err.ErrorsCleared()) {

                                                err = !(commClient->FinalChunk());
                                            }
                                            //hprotocol.SetKeepAlive(false);
                                        }
                                    }
                                }
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
                    information.SetThreadSpecificContext(reinterpret_cast<void*>(newClient));
                    err = MARTe::ErrorManagement::NoError;
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

CLASS_REGISTER(HttpService, "1.0")

}
