/**
 * @file HttpDataExportITest.cpp
 * @brief Source file for class HttpDataExportITest
 * @date 15/03/2019
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
 * the class HttpDataExportITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpChunkedStream.h"
#include "HttpDataExportI.h"
#include "HttpDataExportITest.h"
#include "HttpProtocol.h"
#include "JsonPrinter.h"
#include "StreamStructuredData.h"
#include "TCPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool HttpDataExportITest::TestGetAsStructuredData() {
    using namespace MARTe;
    bool ok = true;
    TCPSocket server;
    server.Open();
    server.Listen(10002, 10);
    HttpDataExportI httpDataExportI;
    StreamStructuredData<JsonPrinter> sdata;
    HttpChunkedStream httpChunkedStream;
    sdata.SetStream(httpChunkedStream);
    httpChunkedStream.Open();
    httpChunkedStream.Connect("127.0.0.1", 10002);
    HttpProtocol hprotocol(httpChunkedStream);
    hprotocol.CreateAbsolute("OutputOptions");
    ok = httpDataExportI.GetAsStructuredData(sdata, hprotocol);
    httpChunkedStream.Close();
    server.Close();
    StreamString str;
    ok &= hprotocol.MoveAbsolute("OutputOptions");
    ok &= hprotocol.Read("Transfer-Encoding", str);
    ok &= (str == "chunked");
    str = "";
    ok &= hprotocol.Read("Content-Type", str);
    ok &= (str == "text/json");
    return ok;
}

bool HttpDataExportITest::TestGetAsText() {
    using namespace MARTe;
    using namespace MARTe;
    bool ok = true;
    TCPSocket server;
    server.Open();
    server.Listen(10002, 10);
    HttpDataExportI httpDataExportI;
    HttpChunkedStream httpChunkedStream;
    httpChunkedStream.Open();
    httpChunkedStream.Connect("127.0.0.1", 10002);
    HttpProtocol hprotocol(httpChunkedStream);
    hprotocol.CreateAbsolute("OutputOptions");
    ok = httpDataExportI.GetAsText(httpChunkedStream, hprotocol);
    httpChunkedStream.Close();
    server.Close();
    StreamString str;
    ok &= hprotocol.MoveAbsolute("OutputOptions");
    ok &= hprotocol.Read("Transfer-Encoding", str);
    ok &= (str == "chunked");
    str = "";
    ok &= hprotocol.Read("Content-Type", str);
    ok &= (str == "text/html");
    return ok;
}

	
