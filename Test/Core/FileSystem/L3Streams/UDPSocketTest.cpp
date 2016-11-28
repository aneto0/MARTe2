/**
 * @file UDPSocketTest.cpp
 * @brief Source file for class UDPSocketTest
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
 * the class UDPSocketTest (public, protected, and private). Be aware that some
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
#include "UDPSocket.h"
#include "UDPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Verifies if the template can solve the stream interface.
static bool CheckStream(MARTe::StreamI &stream) {
    return true;
}

static bool CheckUDPSocket(MARTe::BasicUDPSocket &file) {
    return true;
}

static bool readWithTimeout = false;
static bool serverOK = false;
static bool serverDone = false;
static const MARTe::uint32 udpServerExpectReadSize = 255;
static MARTe::uint8 udpServerBufferRead[udpServerExpectReadSize];
static MARTe::uint32 udpServerPort = 44488;
static void UDPSocketTestServerThread(void *params) {
    using namespace MARTe;
    UDPSocket server;
    serverOK = server.Open();
    InternetHost thisHost(udpServerPort, "127.0.0.1");
    serverOK &= server.Listen(thisHost.GetPort());
    uint32 udpServerReadSize = udpServerExpectReadSize;
    if (serverOK) {
        if (readWithTimeout) {
            serverOK &= server.Read((char8 *) udpServerBufferRead, udpServerReadSize, TTInfiniteWait);
        }
        else {
            serverOK &= server.Read((char8 *) udpServerBufferRead, udpServerReadSize);
        }
    }
    serverOK &= (udpServerReadSize == udpServerExpectReadSize);
    uint8 i;
    for (i = 0; (i < udpServerExpectReadSize) && (serverOK); i++) {
        serverOK &= (udpServerBufferRead[i] == i);
    }
    server.Close();
    serverDone = true;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool UDPSocketTest::TestConstructor() {
    MARTe::UDPSocket f;
    return true;
}

bool UDPSocketTest::TestConstructor_Stream() {
    MARTe::UDPSocket f;
    return CheckStream(f);
}

bool UDPSocketTest::TestConstructor_BasicUDPSocket() {
    MARTe::UDPSocket f;
    return CheckUDPSocket(f);
}

bool UDPSocketTest::TestRead() {
    using namespace MARTe;
    serverDone = false;
    serverOK = false;
    readWithTimeout = false;
    uint8 i;
    for (i = 0; (i < udpServerExpectReadSize); i++) {
        udpServerBufferRead[i] = 0;
    }
    MARTe::uint8 udpServerWriteBuffer[udpServerExpectReadSize];
    for (i = 0; (i < udpServerExpectReadSize); i++) {
        udpServerWriteBuffer[i] = i;
    }
    Threads::BeginThread((ThreadFunctionType) UDPSocketTestServerThread, NULL);
    uint32 counter = 0;
    UDPSocket client;
    client.Open();
    client.Connect("127.0.0.1", udpServerPort);
    while (!serverDone) {
        Sleep::Sec(0.1);
        counter++;
        if (counter > 50) {
            break;
        }
        uint32 bytesSent = udpServerExpectReadSize;
        client.Write((char8 *) udpServerWriteBuffer, bytesSent);
    }
    client.Close();
    return serverOK;
}

bool UDPSocketTest::TestRead_Timeout() {
    using namespace MARTe;
    serverDone = false;
    serverOK = false;
    readWithTimeout = true;
    uint8 i;
    for (i = 0; (i < udpServerExpectReadSize); i++) {
        udpServerBufferRead[i] = 0;
    }
    MARTe::uint8 udpServerWriteBuffer[udpServerExpectReadSize];
    for (i = 0; (i < udpServerExpectReadSize); i++) {
        udpServerWriteBuffer[i] = i;
    }
    Threads::BeginThread((ThreadFunctionType) UDPSocketTestServerThread, NULL);
    uint32 counter = 0;
    UDPSocket client;
    client.Open();
    client.Connect("127.0.0.1", udpServerPort);
    while (!serverDone) {
        Sleep::Sec(0.1);
        counter++;
        if (counter > 50) {
            break;
        }
        uint32 bytesSent = udpServerExpectReadSize;
        client.Write((char8 *) udpServerWriteBuffer, bytesSent);
    }
    client.Close();
    return serverOK;
}

bool UDPSocketTest::TestWrite() {
    return TestRead();
}

bool UDPSocketTest::TestWrite_Timeout() {
    return TestRead_Timeout();
}

bool UDPSocketTest::TestSize() {
    using namespace MARTe;
    UDPSocket socket;
    return (socket.Size() == 0xffffffffffffffffu);
}

bool UDPSocketTest::TestSeek() {
    using namespace MARTe;
    UDPSocket socket;
    return !socket.Seek(0u);
}

bool UDPSocketTest::TestRelativeSeek() {
    using namespace MARTe;
    UDPSocket socket;
    return !socket.RelativeSeek(0u);
}

bool UDPSocketTest::TestPosition() {
    using namespace MARTe;
    UDPSocket socket;
    return (socket.Position() == 0xffffffffffffffffu);
}

bool UDPSocketTest::TestSetSize() {
    using namespace MARTe;
    UDPSocket socket;
    return !socket.SetSize(10u);
}

bool UDPSocketTest::TestCanWrite() {
    using namespace MARTe;
    UDPSocket socket;
    return socket.CanWrite();
}

bool UDPSocketTest::TestCanSeek() {
    using namespace MARTe;
    UDPSocket socket;
    return !socket.CanSeek();
}

bool UDPSocketTest::TestCanRead() {
    using namespace MARTe;
    UDPSocket socket;
    return socket.CanRead();
}
