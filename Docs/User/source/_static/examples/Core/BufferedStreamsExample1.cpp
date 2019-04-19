/**
 * @file BufferedStreamsExample1.cpp
 * @brief Source file for class BufferedStreamsExample1
 * @date 30/04/2018
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
 * the class BufferedStreamsExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BufferedStreamI.h"
#include "Directory.h"
#include "ErrorLoggerExample.h"
#include "File.h"
#include "UDPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Parses tokens from a given stream
 */
static void GetTokensFromStream(MARTe::BufferedStreamI &stream) {
    using namespace MARTe;
    StreamString line = "";
    bool end = false;
    while (!end) {
        stream.GetLine(line);
        StreamString token = "";
        char8 term;
        line.Seek(0u);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Line: %s", line.Buffer());
        while (line.GetToken(token, ":", term)) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Token: %s", token.Buffer());
            end = (token == "END");
            token = "";
        }
        line = "";
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    UDPSocket sock;
    bool ok = sock.Open();
    uint16 port = 24680;
    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to listen in port = %d", port);
        ok = sock.Listen(port);
    }
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to listen in port = %d", port);
    }

    if (ok) {
        GetTokensFromStream(sock);
        sock.Close();
    }

    const char8 * const TEST_FILENAME = "BufferedStreamTestExample1.out";
    if (ok) {
        File f;
        ok = f.Open(TEST_FILENAME, BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
        if (ok) {
            f.Printf("%s", "FILE\nA1:B1:C1\nD2:E2:F2\nEND");
            f.Flush();
            f.Close();
        }
    }
    if (ok) {
        File f;
        ok = f.Open(TEST_FILENAME, BasicFile::ACCESS_MODE_R);
        if (ok) {
            GetTokensFromStream(f);
            f.Close();
            Directory d(TEST_FILENAME);
            d.Delete();
        }
    }
    return 0;
}

