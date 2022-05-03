/**
 * @file HttpClient.cpp
 * @brief Source file for class HttpClient
 * @date 22/08/2018
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
 * the class HttpClient (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Base64Encoder.h"
#include "HttpClient.h"
#include "Md5Encrypt.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static bool SearchKey(const char8 * const key, const char8 * const name, StreamString &value) {
    bool ret = value.SetSize(0ULL);
    if (ret) {
        ret = (key != NULL_PTR(const char8 * const));
        if (ret) {
            ret = (name != NULL_PTR(const char8 * const));
        }

        if (ret) {
            const char8 *p = StringHelper::SearchString(key, name);
            ret = (p != NULL_PTR(const char8 *));
            if (ret) {
                uint32 gap = StringHelper::Length(name);
                p = &p[gap];
                ret = ((p[0] == '=') && (p[1] == '"'));
                if (ret) {
                    p = &p[2];
                    while (p[0] != '"') {
                        value += p[0];
                        p = &(p[1]);
                    }
                }
            }
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpClient::HttpClient() :
        Object(), tcpSocket(), protocol(tcpSocket) {
    urlPort = 0u;
    urlHost = "";
    lastOperationId = 0;
    reConnect = true;
}

/*lint -e{1551} no exception will be thrown*/
HttpClient::~HttpClient() {
    (void) tcpSocket.Close();
}

/*lint -e{1536} the pointer is exposed deliberately*/
HttpProtocol *HttpClient::GetHttpProtocol() {
    return &protocol;
}

bool HttpClient::Initialise(StructuredDataI &data) {

    StreamString urlHostTemp;
    bool ret = data.Read("ServerAddress", urlHostTemp);
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Please define the server ip ServerAddress");
    }
    else {
        if (urlHost != urlHostTemp) {
            urlHost = urlHostTemp;
            reConnect = true;
        }
        uint16 urlPortTemp;
        ret = data.Read("ServerPort", urlPortTemp);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define the server port ServerPort");
        }
        else {
            if (urlPort != urlPortTemp) {
                urlPort = urlPortTemp;
                reConnect = true;
            }
        }
    }

    if (ret) {
        StreamString urlUriTemp;
        ret = data.Read("ServerUri", urlUriTemp);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define the server uri ServerUri");
        }
        else {
            if (urlUri != urlUriTemp) {
                urlUri = urlUriTemp;
                reConnect = true;
            }
        }
    }
    if (ret) {
        if (!data.Read("Authorisation", authorisation)) {
            authorisation = "";
        }
    }

    return ret;
}

void HttpClient::SetServerAddress(const char8 * const serverAddressIn) {
    if (urlHost != serverAddressIn) {
        urlHost = serverAddressIn;
        reConnect = true;
    }
}

void HttpClient::SetServerPort(const uint16 serverPortIn) {
    if (urlPort != serverPortIn) {
        urlPort = serverPortIn;
        reConnect = true;
    }
}

void HttpClient::SetServerUri(const char8 * const serverUriIn) {
    if (urlUri != serverUriIn) {
        urlUri = serverUriIn;
    }
}

void HttpClient::SetAuthorisation(const char8 * const authIn) {
    authorisation = authIn;
    authorisationKey = authIn;
}

void HttpClient::GetServerAddress(StreamString &serverAddrOut) const {
    serverAddrOut = urlHost;
}

uint32 HttpClient::GetServerPort() const {
    return urlPort;
}

void HttpClient::GetServerUri(StreamString &serverUriOut) const {
    serverUriOut = urlUri;
}

void HttpClient::GetAuthorisation(StreamString &authOut) const {
    authOut = authorisation;
}

bool HttpClient::AutenticationProcedure(const int32 command, const TimeoutType &msecTimeout, const int32 operationId) {

    // discard bodyF
    StreamString nullStream;
    bool ret = protocol.CompleteReadOperation(&nullStream, msecTimeout);
    StreamString auth;
    if (ret) {
        ret = protocol.MoveAbsolute("InputOptions");
        if (ret) {
            ret = protocol.Read("WWW-Authenticate", auth);
        }
    }

    StreamString newAuthorisationKey;
    if (ret) {
        StreamString authRequest;
        ret = auth.Seek(0ULL);
        if (ret) {
            char8 terminator;
            (void) auth.GetToken(authRequest, "\n\r", terminator);

            // identify basic or digest and call appropriate function
            // to generate newAuthorisationKey
            if (StringHelper::CompareNoCaseSensN("Basic ", authRequest.Buffer(), 6u) == 0) {
                newAuthorisationKey = authorisation;
            }
            else if (StringHelper::CompareNoCaseSensN("Digest ", authRequest.Buffer(), 7u) == 0) {
                ret = GenerateDigestKey(newAuthorisationKey, &(authRequest.Buffer()[7]), HttpDefinition::GetErrorCodeString(command), operationId);
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Authorisation request of unknown type: %s", authRequest.Buffer());
                ret = false;
            }
        }
        if (ret) {
            authorisationKey = newAuthorisationKey;
        }

    }
    return ret;
}

bool HttpClient::HttpExchange(BufferedStreamI &streamDataRead, const int32 command, BufferedStreamI * const payload, TimeoutType msecTimeout, int32 operationId) {

    // absolute time for timeout !
    uint64 startCounter = HighResolutionTimer::Counter();

    // a counter of the transactions
    if (operationId == -1) {
        lastOperationId++;
        operationId = lastOperationId;
    }

    if (!reConnect) {
        reConnect = (!tcpSocket.IsConnected());
    }

    int32 errorCode;
    bool ret = !HttpDefinition::IsReplyCode(command, errorCode);
    if (ret) {
        /* connect to remote host */
        if (reConnect) {
            ret = Connect(msecTimeout);
            reConnect = false;
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The command cannot be a reply code");
    }

    if (ret) {
        /* create and send request */
        protocol.SetKeepAlive(true);
        if (authorisationKey.Size() > 0ULL) {
            //switch and write
            if (!protocol.MoveAbsolute("OutputOptions")) {
                ret = protocol.CreateAbsolute("OutputOptions");
            }

            if (ret) {
                AnyType at = protocol.GetType("Authorization");
                if (!at.IsVoid()) {
                    ret = protocol.Delete("Authorization");
                }
                if (ret) {
                    ret = protocol.Write("Authorization", authorisationKey.Buffer());
                }
            }
        }

        if (ret) {
            ret = protocol.WriteHeader(true, command, payload, urlUri.Buffer());
        }

    }
    if (ret) {
        /* read reply */
        ret = protocol.ReadHeader();
    }

    if (ret) {
        if (msecTimeout.IsFinite()) {
            uint64 elapsed = (HighResolutionTimer::Counter() - startCounter);
            uint64 delta = msecTimeout.HighResolutionTimerTicks();
            ret = elapsed < delta;
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::Timeout, "Timeout on completion");
            }
            else {
                uint64 ticksLeft = (delta - elapsed);
                msecTimeout.SetTimeoutHighResolutionTimerTicks(ticksLeft);
            }
        }
    }

    if (ret) {
        // check reply for 200 (authorisation request)
        if (protocol.GetHttpCommand() == HttpDefinition::HSHCReplyAUTH) {

            ret = AutenticationProcedure(command, msecTimeout, operationId);
            if (ret) {
                if (protocol.KeepAlive()) {
                    //try again with new authorization
                    ret = HttpExchange(streamDataRead, command, payload, msecTimeout, operationId);
                }
            }

        }
        else {
            // read body
            //TODO here we can read the body inside a structured data?
            ret = protocol.CompleteReadOperation(&streamDataRead, msecTimeout);
        }

        // close if the server says so...
        if (!protocol.KeepAlive()) {
            (void) tcpSocket.Close();
        }
    }

    return ret;

}

bool HttpClient::Connect(const TimeoutType &msecTimeout) {
    (void) tcpSocket.Close();
    bool ret = tcpSocket.Open();
    if (ret) {
        ret = tcpSocket.SetBlocking(true);
    }
    if (ret) {
        ret = tcpSocket.Connect(urlHost.Buffer(), urlPort, msecTimeout);
    }
    return ret;
}

bool HttpClient::GenerateDigestKey(StreamString &key, const char8 * const data, const char8 * const command, int32 nc) {
    bool ret = key.SetSize(0ULL);

    StreamString qop = "auth";

    StreamString userPasswd;
    if (ret) {
        ret = Base64Encoder::Decode(authorisation, userPasswd);
        StreamString user;
        StreamString passwd;
        StreamString realm;
        if (ret) {

            char8 terminator;

            ret = userPasswd.Seek(0ULL);
            if (ret) {
                if (!userPasswd.GetToken(user, ":", terminator)) {
                    //TODO warning?
                }
                if (!userPasswd.GetToken(passwd, "\n\t ", terminator)) {
                    //TODO warning?
                }
            }

            ret = SearchKey(data, "realm", realm);
        }

        REPORT_ERROR(ErrorManagement::Information, "user=%s, pass=%s, realm=%s", user.Buffer(), passwd.Buffer(), realm.Buffer());

        StreamString HA1;
        StreamString HA2;
        StreamString response;
        StreamString ncStr;
        StreamString cnonce;
        StreamString nonce;
        StreamString opaque;

        if (ret) {
            ret = HA1.SetSize(0ULL);
            if (ret) {
                uint8 buffer[16];
                StreamString toEncode;
                ret = toEncode.Printf("%s:%s:%s", user.Buffer(), realm.Buffer(), passwd.Buffer());
                if (ret) {
                    uint32 toEncodeLen = static_cast<uint32>(toEncode.Size());
                    Md5Encrypt::Md5(reinterpret_cast<uint8 *>(toEncode.BufferReference()), toEncodeLen, &buffer[0]);
                    for (uint32 i = 0u; (i < 16u) && (ret); i++) {
                        ret = HA1.Printf("%02x", buffer[i]);
                    }
                    if (ret) {
                        ret = MemoryOperationsHelper::Set(&buffer[0], '\0', 16u);
                    }
                    if (ret) {
                        ret = HA2.SetSize(0ULL);
                    }
                }
                if (ret) {
                    ret = toEncode.SetSize(0ULL);
                }
                if (ret) {
                    ret = toEncode.Printf("%s:%s", command, urlUri);
                    if (ret) {
                        uint32 toEncodeLen = static_cast<uint32>(toEncode.Size());
                        Md5Encrypt::Md5(reinterpret_cast<uint8*>(toEncode.BufferReference()), toEncodeLen, &buffer[0]);
                        for (uint32 i = 0u; (i < 16u) && (ret); i++) {
                            ret = HA2.Printf("%02x", buffer[i]);
                        }
                        if (ret) {
                            ret = ncStr.Printf("%08x", nc);
                        }
                        if (ret) {
                            ret = CalculateNonce(cnonce);
                        }
                        if (ret) {
                            ret = SearchKey(data, "nonce", nonce);
                        }
                    }
                }
            }
        }
        if (ret) {
            uint8 buffer[16];

            StreamString toEncode;
            ret = toEncode.SetSize(0ULL);
            if (ret) {
                ret = toEncode.Printf("%s:%s:%s:%s:%s:%s", HA1.Buffer(), nonce.Buffer(), ncStr.Buffer(), cnonce.Buffer(), qop.Buffer(), HA2.Buffer());
                if (ret) {
                    uint32 toEncodeLen = static_cast<uint32>(toEncode.Size());
                    Md5Encrypt::Md5(reinterpret_cast<uint8*>(toEncode.BufferReference()), toEncodeLen, &buffer[0]);
                    for (uint32 i = 0u; (i < 16u) && (ret); i++) {
                        ret = response.Printf("%02x", buffer[i]);
                    }
                    if (ret) {
                        ret = SearchKey(data, "opaque", opaque);
                    }
                }
            }
        }

        if (ret) {

            ret = key.Printf("Digest "
                             "username=\"%s\","
                             "realm=\"%s\","
                             "nonce=\"%s\","
                             "uri=\"%s\","
                             "qop=%s,"
                             "nc=%s,"
                             "cnonce=\"%s\","
                             "response=\"%s\","
                             "opaque=\"%s\"",
                             user.Buffer(), realm.Buffer(), nonce.Buffer(), urlUri.Buffer(), qop.Buffer(), ncStr.Buffer(), cnonce.Buffer(), response.Buffer(), opaque.Buffer());
        }
    }
    return ret;

}

bool HttpClient::CalculateNonce(StreamString &nonce) {
    bool ret = nonce.SetSize(0ULL);
    StreamString tid;
    if (ret) {
        //ThreadIdentifier threadId = Threads::Id();
        ret = tid.Printf("%08x%08x", static_cast<uint32>(HighResolutionTimer::Counter()), this);

        //Original line
        //ret = tid.Printf("%08x%08x", static_cast<uint32>(Threads::Id()), this);
    }
    if (ret) {
        uint8 buffer[16];
        uint32 tidSize = static_cast<uint32>(tid.Size());
        Md5Encrypt::Md5(reinterpret_cast<uint8 *>(tid.BufferReference()), tidSize, &buffer[0]);
        for (uint32 i = 0u; (i < 16u) && (ret); i++) {
            ret = nonce.Printf("%02x", buffer[i]);
        }
    }

    return ret;
}

}

