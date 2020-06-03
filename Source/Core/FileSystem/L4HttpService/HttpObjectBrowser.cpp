/**
 * @file HttpObjectBrowser.cpp
 * @brief Source file for class HttpObjectBrowser
 * @date 29/10/2018
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
 * the class HttpObjectBrowser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "HttpChunkedStream.h"

#include "HttpDirectoryResource.h"
#include "HttpObjectBrowser.h"
#include "HttpProtocol.h"
#include "JsonPrinter.h"
#include "ObjectRegistryDatabase.h"
#include "StreamStructuredData.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

HttpObjectBrowser::HttpObjectBrowser() :
        ReferenceContainer(), HttpDataExportI() {
    closeOnAuthFail = 1u;
    root = NULL_PTR(ReferenceContainer *);
}

HttpObjectBrowser::~HttpObjectBrowser() {
    /*lint -e{1540} root is freed by the parent Reference*/
}

void HttpObjectBrowser::Purge(ReferenceContainer &purgeList) {
    ReferenceContainer::Purge(purgeList);
}

bool HttpObjectBrowser::Initialise(StructuredDataI &data) {
    bool ok = ReferenceContainer::Initialise(data);

    StreamString rootName;
    if (ok) {
        ok = data.Read("Root", rootName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "A Root shall be specified");
        }
    }
    if (ok) {
        if (rootName.Size() == 1u) {
            if (rootName[0u] == '/') {
                root = ObjectRegistryDatabase::Instance();
            }
            else if (rootName[0u] == '.') {
                root = this;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Unknown Root [%c]", rootName[0u]);
            }
        }
        else {
            ReferenceT<ReferenceContainer> rootT = ObjectRegistryDatabase::Instance()->Find(rootName.Buffer());
            ok = rootT.IsValid();
            if (ok) {
                root = rootT.operator ->();
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid Root [%s]", rootName.Buffer());
            }
        }

    }
    if (ok) {
        StreamString realmStr;
        if (data.Read("Realm", realmStr)) {
            realm = Find(realmStr.Buffer());
            ok = realm.IsValid();
            if (!ok) {
                realm = ObjectRegistryDatabase::Instance()->Find(realmStr.Buffer());
                ok = realm.IsValid();
            }
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "The specified Realm reference is not valid");
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "No Realm specified");
        }
    }
    if (ok) {
        if (!data.Read("CloseOnAuthFail", closeOnAuthFail)) {
            closeOnAuthFail = 1u;
        }
    }
    return ok;
}

bool HttpObjectBrowser::CheckSecurity(HttpProtocol &protocol) {
    bool securityOK = true;
    if (realm.IsValid()) {
        HttpProtocol *hprotocol = dynamic_cast<HttpProtocol *>(&protocol);
        if (hprotocol != NULL_PTR(HttpProtocol *)) {
            securityOK = hprotocol->SecurityCheck(realm);
            if (!securityOK) {
                bool ok = true;
                if (!hprotocol->MoveAbsolute("OutputOptions")) {
                    ok = (hprotocol->CreateAbsolute("OutputOptions"));
                }
                if (ok) {
                    StreamString contentType = "text/html";
                    ok = hprotocol->Write("Content-Type", contentType.Buffer());
                }
                StreamString realmMsg;
                if (ok) {
                    ok = (realm->GetAuthenticationRequest(realmMsg));
                }
                if (ok) {
                    ok = hprotocol->Write("WWW-Authenticate", realmMsg.Buffer());
                }
                StreamString hstream;
                if (ok) {
                    ok = hstream.Printf("%s", "<HTML><HEAD>\n"
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
                }
                if (ok) {
                    ok = hstream.Seek(0ULL);
                }
                if (ok) {
                    if (closeOnAuthFail > 0u) {
                        // force reissuing of a new thread
                        hprotocol->SetKeepAlive(false);
                    }
                    ok = hprotocol->WriteHeader(true, HttpDefinition::HSHCReplyAUTH, &hstream, NULL_PTR(const char8*));
                }
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed to write the header");
                }
            }
        }
    }
    return securityOK;
}

Reference HttpObjectBrowser::FindTarget(HttpProtocol &protocol) {
    StreamString unmatchedPath;
    protocol.GetUnmatchedId(unmatchedPath);
    StreamString reparsedPath;

    Reference target;
    bool tryAgain = true;
    while (tryAgain) {
        target = FindReference(unmatchedPath.Buffer());
        tryAgain = !target.IsValid();
        if (!target.IsValid()) {
            //Could not find. Try to remove the last part and try again.
            const char8 * lastDotToken = StringHelper::SearchLastChar(unmatchedPath.Buffer(), '.');
            if (lastDotToken == NULL_PTR(const char8 * const)) {
                tryAgain = false;
            }
            else {
                StreamString toAppend;
                lastDotToken = reinterpret_cast<const char8 *>(&lastDotToken[1]);
                if (reparsedPath.Size() == 0u) {
                    toAppend = lastDotToken;
                }
                else {
                    toAppend = lastDotToken;
                    toAppend += ".";
                    toAppend += reparsedPath.Buffer();
                }
                reparsedPath = toAppend;
                /*lint -e{9125,946,947} allow for pointers to be subtracted and cast to uint32*/
                uint32 pathMatchSize = static_cast<uint32>(lastDotToken - unmatchedPath.Buffer());
                //Remove the .
                pathMatchSize -= 1u;
                (void) unmatchedPath.SetSize(static_cast<uint64>(pathMatchSize));
            }
        }
    }
    if (reparsedPath.Size() > 0u) {
        protocol.SetUnmatchedId(reparsedPath.Buffer());
    }
    else {
        protocol.SetUnmatchedId("");
    }
    return target;
}

/*lint -e{613} sdata cannot be NULL as otherwise ok would be false*/
bool HttpObjectBrowser::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    bool ok = CheckSecurity(protocol);
    if (ok) {
        Reference target = FindTarget(protocol);
        ok = target.IsValid();
        StreamStructuredData<JsonPrinter> *sdata;
        if (ok) {
            sdata = dynamic_cast<StreamStructuredData<JsonPrinter> *>(&data);
            /*lint -e{665} StreamStructuredData<JsonPrinter> is only used to define the pointer type of the NULL_PTR*/
            ok = (sdata != NULL_PTR(StreamStructuredData<JsonPrinter> *));
        }
        if (ok) {
            bool isThis = (target == this);
            //If we are printing ourselves list all the elements belonging to the root (note that the root might be pointing elsewhere).
            if (isThis) {
                ok = HttpDataExportI::GetAsStructuredData(data, protocol);
                //Print the opening {
                if (ok) {
                    //lint -e{644} sdata initialised otherwise ok would be false*/
                    ok = sdata->GetPrinter()->PrintBegin();
                }
                if (ok) {
                    //Export the data.
                    ok = Object::ExportData(data);
                }
                //List the elements that belong to the root (cannot point directly to the RC implementation as otherwise it would print the wrong class name).
                uint32 numberOfChildren = root->Size();
                for (uint32 i = 0u; (i < numberOfChildren) && (ok); i++) {
                    StreamString nname;
                    uint32 ii = i;
                    ok = nname.Printf("%d", ii);
                    if (ok) {
                        ok = data.CreateRelative(nname.Buffer());
                    }
                    Reference child;
                    if (ok) {
                        child = root->Get(i);
                        ok = child.IsValid();
                    }
                    if (ok) {
                        ReferenceT<ReferenceContainer> childRC = child;
                        //Do not go recursive
                        if (childRC.IsValid()) {
                            ok = child->Object::ExportData(data);
                            if (ok) {
                                ok = data.Write("IsContainer", 1);
                            }
                        }
                        else {
                            ok = child->ExportData(data);
                        }
                    }
                    if (ok) {
                        ok = data.MoveToAncestor(1u);
                    }
                }
                //Print the closing {
                if (ok) {
                    ok = sdata->GetPrinter()->PrintEnd();
                }
            }
            else {
                //Not pointing at ourselves. It can be a HttpDataExportI in which case we forward the work.
                ReferenceT<HttpDataExportI> httpDataExportI = target;
                if (httpDataExportI.IsValid()) {
                    ok = httpDataExportI->GetAsStructuredData(data, protocol);
                }
                else {
                    //Otherwise dump the object values.
                    ok = HttpDataExportI::GetAsStructuredData(data, protocol);
                    //Print the opening {
                    if (ok) {
                        ok = sdata->GetPrinter()->PrintBegin();
                    }
                    if (ok) {
                        ok = target->ExportData(data);
                    }
                    //Print the closing }
                    if (ok) {
                        ok = sdata->GetPrinter()->PrintEnd();
                    }
                }
            }
        }
        else {
            ok = HttpDataExportI::ReplyNotFound(protocol);
        }
    }
    return ok;
}

bool HttpObjectBrowser::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    bool ok = CheckSecurity(protocol);
    if (ok) {
        Reference target = FindTarget(protocol);
        ok = target.IsValid();
        if (ok) {
            bool isThis = (target == this);
            if (isThis) {
                //Check if there are any HttpDirectoryResources that could handle the request
                uint32 n;
                uint32 nObjs = Size();
                bool handled = false;
                for (n = 0u; (n < nObjs) && (!handled); n++) {
                    ReferenceT<HttpDirectoryResource> httpDirectoryResource = Get(n);
                    handled = httpDirectoryResource.IsValid();
                    if (handled) {
                        httpDirectoryResource->SetReplyNotFound(false);
                        handled = httpDirectoryResource->GetAsText(stream, protocol);
                    }
                }
                if (!handled) {
                    ok = HttpDataExportI::ReplyNotFound(protocol);
                }
            }
            else {
                ReferenceT<HttpDataExportI> httpDataExportI = target;
                if (httpDataExportI.IsValid()) {
                    ok = httpDataExportI->GetAsText(stream, protocol);
                }
                else {
                    ok = HttpDataExportI::ReplyNotFound(protocol);
                }
            }
        }
        else {
            ok = HttpDataExportI::ReplyNotFound(protocol);
        }
    }
    return ok;
}

Reference HttpObjectBrowser::FindReference(const char8 * const unmatchedPath) {
    Reference target;
    if (StringHelper::Length(unmatchedPath) > 0u) {
        // search for destination
        int32 occurrences = 1;
        uint32 mode = ReferenceContainerFilterMode::SHALLOW;
        ReferenceContainerFilterObjectName filter(occurrences, mode, unmatchedPath);
        ReferenceContainer results;
        if (root != NULL_PTR(ReferenceContainer *)) {
            root->Find(results, filter);
            if (results.Size() > 0ull) {
                uint32 last = static_cast<uint32>(results.Size()) - 1u;
                target = results.Get(last);
            }
        }
    }
    else {
        target = this;
    }
    return target;
}

CLASS_REGISTER(HttpObjectBrowser, "1.0")
}
