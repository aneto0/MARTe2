/**
 * @file TCPSocketTest.cpp
 * @brief Source file for class TCPSocketTest
 * @date 16/11/2016
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
 * the class TCPSocketTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "TCPSocket.h"
#include "TCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Verifies if the template can solve the stream interface.
static bool CheckStream(MARTe::StreamI &stream) {
    return true;
}

static bool CheckTCPSocket(MARTe::BasicTCPSocket &file) {
    return true;
}

static bool readWithTimeout = false;
static bool serverOK = false;
static bool serverDone = false;
static const MARTe::uint32 tcpServerExpectReadSize = 255;
static MARTe::uint8 tcpServerBufferRead[tcpServerExpectReadSize];
static MARTe::uint32 tcpServerPort = 44488;
static void TCPSocketTestServerThread(void *params) {
    using namespace MARTe;
    TCPSocket server;
    serverOK = server.Open();
    InternetHost thisHost(tcpServerPort, "127.0.0.1");
    serverOK &= server.Listen(thisHost.GetPort());
    uint32 tcpServerReadSize = tcpServerExpectReadSize;
    if (serverOK) {
        BasicTCPSocket *client;
        client = server.WaitConnection(TTInfiniteWait);
        if (client != NULL) {
            if (readWithTimeout) {
                serverOK &= client->Read((char8 *) tcpServerBufferRead, tcpServerReadSize, TTInfiniteWait);
            }
            else {
                serverOK &= client->Read((char8 *) tcpServerBufferRead, tcpServerReadSize);
            }
            client->Close();
            delete client;
        }
        else {
            serverOK = false;
        }
    }
    serverOK &= (tcpServerReadSize == tcpServerExpectReadSize);
    uint8 i;
    for (i = 0; (i < tcpServerExpectReadSize) && (serverOK); i++) {
        serverOK &= (tcpServerBufferRead[i] == i);
    }
    server.Close();
    serverDone = true;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool TCPSocketTest::TestConstructor() {
    MARTe::TCPSocket f;
    return true;
}

bool TCPSocketTest::TestConstructor_Stream() {
    MARTe::TCPSocket f;
    return CheckStream(f);
}

bool TCPSocketTest::TestConstructor_BasicTCPSocket() {
    MARTe::TCPSocket f;
    return CheckTCPSocket(f);
}


bool TCPSocketTest::TestRead() {
    using namespace MARTe;
    serverDone = false;
    serverOK = false;
    readWithTimeout = false;
    uint8 i;
    for (i = 0; (i < tcpServerExpectReadSize); i++) {
        tcpServerBufferRead[i] = 0;
    }
    MARTe::uint8 tcpServerWriteBuffer[tcpServerExpectReadSize];
    for (i = 0; (i < tcpServerExpectReadSize); i++) {
        tcpServerWriteBuffer[i] = i;
    }
    Threads::BeginThread((ThreadFunctionType) TCPSocketTestServerThread, NULL);
    uint32 counter = 0;
    TCPSocket client;

    while (!serverDone) {
        Sleep::Sec(0.1);
        counter++;
        if (counter > 50) {
            break;
        }
        uint32 bytesSent = tcpServerExpectReadSize;
        client.Open();
        client.Connect("127.0.0.1", tcpServerPort);
        client.Write((char8 *) tcpServerWriteBuffer, bytesSent);
        client.Close();
    }
    return serverOK;
}

bool TCPSocketTest::TestRead_Timeout() {
    using namespace MARTe;
    serverDone = false;
    serverOK = false;
    readWithTimeout = true;
    uint8 i;
    for (i = 0; (i < tcpServerExpectReadSize); i++) {
        tcpServerBufferRead[i] = 0;
    }
    MARTe::uint8 tcpServerWriteBuffer[tcpServerExpectReadSize];
    for (i = 0; (i < tcpServerExpectReadSize); i++) {
        tcpServerWriteBuffer[i] = i;
    }
    Threads::BeginThread((ThreadFunctionType) TCPSocketTestServerThread, NULL);
    uint32 counter = 0;
    TCPSocket client;
    while (!serverDone) {
        Sleep::Sec(0.1);
        counter++;
        if (counter > 50) {
            break;
        }
        uint32 bytesSent = tcpServerExpectReadSize;
        client.Open();
        client.Connect("127.0.0.1", tcpServerPort);
        client.Write((char8 *) tcpServerWriteBuffer, bytesSent);
        client.Close();
    }
    return serverOK;
}

bool TCPSocketTest::TestWrite() {
    return TestRead();
}

bool TCPSocketTest::TestWrite_Timeout() {
    return TestRead_Timeout();
}

bool TCPSocketTest::TestSize() {
    using namespace MARTe;
    TCPSocket socket;
    return (socket.Size() == 0xffffffffffffffffu);
}

bool TCPSocketTest::TestSeek() {
    using namespace MARTe;
    TCPSocket socket;
    return !socket.Seek(0u);
}

bool TCPSocketTest::TestRelativeSeek() {
    using namespace MARTe;
    TCPSocket socket;
    return !socket.RelativeSeek(0u);
}

bool TCPSocketTest::TestPosition() {
    using namespace MARTe;
    TCPSocket socket;
    return (socket.Position() == 0xffffffffffffffffu);
}

bool TCPSocketTest::TestSetSize() {
    using namespace MARTe;
    TCPSocket socket;
    return !socket.SetSize(10u);
}

bool TCPSocketTest::TestCanWrite() {
    using namespace MARTe;
    TCPSocket socket;
    return socket.CanWrite();
}

bool TCPSocketTest::TestCanSeek() {
    using namespace MARTe;
    TCPSocket socket;
    return !socket.CanSeek();
}

bool TCPSocketTest::TestCanRead() {
    using namespace MARTe;
    TCPSocket socket;
    return socket.CanRead();
}
