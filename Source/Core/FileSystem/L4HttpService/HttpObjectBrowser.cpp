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
#include "HttpObjectBrowser.h"
#include "HttpProtocol.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

HttpObjectBrowser::HttpObjectBrowser() :
        ObjectBrowser() {
    closeOnAuthFail = 1u;
}

HttpObjectBrowser::~HttpObjectBrowser() {

}

bool HttpObjectBrowser::Initialise(StructuredDataI &data) {
    bool ok = ObjectBrowser::Initialise(data);
    if (ok) {
        StreamString realmStr;
        if (data.Read("Realm", realmStr)) {
            if (ok) {
                realm = Find(realmStr.Buffer());
                ok = realm.IsValid();
                if (!ok) {
                    realm = ObjectRegistryDatabase::Instance()->Find(realmStr.Buffer());
                    ok = realm.IsValid();
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The specified Realm reference is not valid");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "A reference to a Realm object shall be specified");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "No Realm specified");

        }
    }
    if (ok) {
        if (!data.Read("CloseOnAuthFail", closeOnAuthFail)) {
            closeOnAuthFail = 1u;
        }
    }
    return ok;
}

bool HttpObjectBrowser::CheckSecurity(ProtocolI &protocol) {
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
            }
        }
    }
    return securityOK;
}

bool HttpObjectBrowser::GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol) {
    bool ok = true;
    bool securityOK = CheckSecurity(protocol);
    if (securityOK) {
        ok = ObjectBrowser::GetAsStructuredData(data, protocol);
    }
    return ok;
}

bool HttpObjectBrowser::GetAsText(StreamI &stream, ProtocolI &protocol) {
    bool ok = true;
    bool securityOK = CheckSecurity(protocol);
    if (securityOK) {
        ok = ObjectBrowser::GetAsText(stream, protocol);
    }
    return ok;
}

CLASS_REGISTER(HttpObjectBrowser, "1.0")
}
