/**
 * @file HttpDataExportI.cpp
 * @brief Source file for class HttpDataExportI
 * @date Oct 31, 2018
 * @author aneto
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
 * the class HttpDataExportI (public, protected, and private). Be aware that some 
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
#include "HttpDataExportI.h"
#include "HttpDefinition.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

HttpDataExportI::HttpDataExportI() {

}

HttpDataExportI::~HttpDataExportI() {

}

bool HttpDataExportI::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    bool ok = protocol.MoveAbsolute("OutputOptions");
    if (ok) {
        ok = protocol.Write("Transfer-Encoding", "chunked");
    }
    if (ok) {
        ok = protocol.Write("Content-Type", "text/json");
    }
    if (ok) {
        //empty string... go in chunked mode
        StreamString hstream;
        ok = protocol.WriteHeader(false, HttpDefinition::HSHCReplyOK, &hstream, NULL_PTR(const char8*));
    }
    HttpChunkedStream *sstream = NULL_PTR(HttpChunkedStream *);
    if (ok) {
        sstream = dynamic_cast<HttpChunkedStream *>(data.GetStream());
        ok = (sstream != NULL_PTR(HttpChunkedStream *));
    }
    if (ok) {
        sstream->SetChunkMode(true);
    }
    return ok;
}

bool HttpDataExportI::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    bool ok = protocol.MoveAbsolute("OutputOptions");
    if (ok) {
        ok = protocol.Write("Transfer-Encoding", "chunked");
    }
    if (ok) {
        ok = protocol.Write("Content-Type", "text/html");
    }
    if (ok) {
        //empty string... go in chunked mode
        StreamString hstream;
        ok = protocol.WriteHeader(false, HttpDefinition::HSHCReplyOK, &hstream, NULL_PTR(const char8*));
    }
    HttpChunkedStream *sstream = NULL_PTR(HttpChunkedStream *);
    if (ok) {
        sstream = dynamic_cast<HttpChunkedStream *>(&stream);
        ok = (sstream != NULL_PTR(HttpChunkedStream *));
    }
    if (ok) {
        sstream->SetChunkMode(true);
    }
    return ok;
}

}
