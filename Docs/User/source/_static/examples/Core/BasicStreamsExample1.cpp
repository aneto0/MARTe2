/**
 * @file BasicStreamsExample1.cpp
 * @brief Source file for class BasicStreamsExample1
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
 * the class BasicStreamsExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "ErrorLoggerExample.h"
#include "BasicConsole.h"
#include "BasicFile.h"
#include "BasicUDPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Writes to the specified stream
 */
static void WriteToBasicStream(MARTe::StreamI &stream, MARTe::StreamString &str) {
    MARTe::uint32 writeSize = str.Size();
    stream.Write(str.Buffer(), writeSize);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    BasicUDPSocket sock;
    bool ok = sock.Open();
    uint16 port = 24680;
    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to listen in port = %d", port);
        ok = sock.Listen(port);
    }
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to listen in port = %d", port);
    }

    StreamString recStream;
    if (ok) {
        const uint32 MAX_BUFFER_SIZE = 512;
        char8 buffer[MAX_BUFFER_SIZE];
        uint32 bufferSize = MAX_BUFFER_SIZE;
        ok = sock.Read(buffer, bufferSize);
        if (bufferSize == MAX_BUFFER_SIZE) {
            bufferSize = (MAX_BUFFER_SIZE - 1);
        }
        buffer[bufferSize] = '\0';
        if (ok) {
            recStream = buffer;
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Read from UDP: %s", recStream.Buffer());
        }
        ok = sock.Close();
    }
    if (ok) {
        BasicConsole bc;
        uint32 ncols = 24;
        uint32 nrows = 8;
        bc.Open(BasicConsoleMode::EnablePaging);
        bc.SetSceneSize(ncols, nrows);
        StreamString str;
        str.Printf("BasicConsole: %s", recStream.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Writing into console: %s", str.Buffer());
        WriteToBasicStream(bc, str);
        bc.Close();
    }

    const char8 * const TEST_FILENAME = "BasicStreamTestExample1.out";
    if (ok) {
        BasicFile f;
        ok = f.Open(TEST_FILENAME, BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
        if (ok) {
            StreamString str;
            str.Printf("BasicFile: %s", recStream.Buffer());
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Writing into file: %s", str.Buffer());
            WriteToBasicStream(f, str);
            f.Close();
        }
    }
    if (ok) {
        const uint32 MAX_BUFFER_SIZE = 512;
        char8 buffer[MAX_BUFFER_SIZE];
        uint32 bufferSize = MAX_BUFFER_SIZE;
        BasicFile f;
        ok = f.Open(TEST_FILENAME, BasicFile::ACCESS_MODE_R);
        if (ok) {
            ok = f.Read(&buffer[0], bufferSize);
            f.Close();
        }
        if (ok) {
            StreamString readStr = buffer;
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Read from File: %s", readStr.Buffer());
            Directory d(TEST_FILENAME);
            d.Delete();
        }
    }
    return 0;
}

