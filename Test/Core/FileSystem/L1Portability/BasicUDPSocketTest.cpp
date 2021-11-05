/**
 * @file BasicUDPSocketTest.cpp
 * @brief Source file for class BasicUDPSocketTest
 * @date 02/11/2015
 * @author Giuseppe Ferrò
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
 * the class BasicUDPSocketTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocketTest.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

BasicUDPSocketTest::BasicUDPSocketTest() {

    retVal = true;
    NoError = true;
    eventSem.Create();
    exitCondition = 0;
    isBlocking = true;
    alives = 0;
    string = NULL;
    result = NULL;
    size = 0;
    expectedSize = 0;
    isServer = true;
    timeout = TTInfiniteWait;
    isTimeout = false;
    isValidServer = true;
    isValidClient = true;

}

bool BasicUDPSocketTest::TestDefaultConstructor() {
    BasicUDPSocket socket;

    if (socket.IsValid()) {
        return false;
    }

    if (socket.GetDestination().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (socket.GetDestination().GetPort() != 0) {
        return false;
    }

    if (socket.GetSource().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (socket.GetSource().GetPort() != 0) {
        return false;
    }

    return socket.IsBlocking();
}

bool BasicUDPSocketTest::TestSeek() {
    BasicUDPSocket socket;
    return !socket.Seek(0);
}

bool BasicUDPSocketTest::TestSize() {
    BasicUDPSocket socket;
    return socket.Size() == 0xffffffffffffffff;
}

bool BasicUDPSocketTest::TestRelativeSeek() {
    BasicUDPSocket socket;
    return !socket.RelativeSeek(0);
}

bool BasicUDPSocketTest::TestPosition() {
    BasicUDPSocket socket;
    return socket.Position() == 0xffffffffffffffff;
}

bool BasicUDPSocketTest::TestSetSize() {
    BasicUDPSocket socket;
    return !socket.SetSize(1);
}

bool BasicUDPSocketTest::TestCanWrite() {
    BasicUDPSocket socket;
    return socket.CanWrite();
}

bool BasicUDPSocketTest::TestCanRead() {

    BasicUDPSocket socket;
    return socket.CanRead();
}

bool BasicUDPSocketTest::TestCanSeek() {
    BasicUDPSocket socket;
    return !socket.CanSeek();
}

bool BasicUDPSocketTest::TestIsValid() {

    BasicUDPSocket socket;

    if (socket.IsValid()) {
        return false;
    }

    socket.Open();

    if (!socket.IsValid()) {
        return false;
    }

    socket.Close();

    return !socket.IsValid();
}

bool BasicUDPSocketTest::TestOpen() {
    return TestIsValid();
}

bool BasicUDPSocketTest::TestClose() {
    return TestIsValid();
}

void StartServer_Listen(BasicUDPSocketTest &param) {
    BasicUDPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }

    if (!param.isValidServer) {
        serverSocket.Close();
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.retVal = false;
        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    if (param.isValidClient) {
        for (uint32 i = 0; i < param.alives; i++) {
            char8 output[32] = { 0 };
            uint32 sizeRead = 32;
            if (!serverSocket.Read(output, sizeRead)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
            else {
                if (StringHelper::Compare(output, "HelloServer") != 0) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
            }

            if (param.NoError) {
                serverSocket.SetDestination(serverSocket.GetSource());
                const char8 *toWrite = "HelloClient";
                uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
                if (!serverSocket.Write(toWrite, sizeWrite)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
            }

        }
    }
    while (Threads::NumberOfThreads() < (param.alives + 1)) {
        Sleep::MSec(10);
    }

    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }
    serverSocket.Close();
}

void ClientJob_Listen(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    bool go = clientSocket.Open();
    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!param.isValidClient) {
            clientSocket.Close();
        }

        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.retVal = false;
            param.sem.FastUnLock();
        }

        else {
            const char8* toWrite = "HelloServer";
            uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
            if (!clientSocket.Write(toWrite, sizeWrite)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
            else {
                char8 output[32] = { 0 };
                uint32 sizeRead = 32;
                if (!clientSocket.Read(output, sizeRead)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (StringHelper::Compare(output, "HelloClient") != 0) {
                        param.sem.FastLock();
                        param.NoError = false;
                        param.sem.FastUnLock();
                    }
                }
            }
        }

    }

    param.eventSem.Wait();
    clientSocket.Close();
}

static bool ListenConnectTest(BasicUDPSocketTest &param,
                              const ConnectListenUDPTestTable* table,
                              bool isServer) {

    uint32 i = 0;
    while (table[i].port != 0) {
        param.server.SetPort(table[i].port);
        param.server.SetAddress("127.0.0.1");

        param.eventSem.Reset();
        param.exitCondition = 0;

        param.alives = table[i].nClients;

        if (isServer) {
            param.isValidServer = table[i].isValid;
        }
        else {
            param.isValidClient = table[i].isValid;
        }

        Threads::BeginThread((ThreadFunctionType) StartServer_Listen, &param);

        while (param.exitCondition < 1) {
            if (!param.NoError) {
                param.alives = 0;
                while (Threads::NumberOfThreads() > 0) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        if (!param.retVal) {
            return (!table[i].expected);
        }

        for (uint32 k = 0; k < table[i].nClients; k++) {
            ThreadIdentifier tId = Threads::BeginThread((ThreadFunctionType) ClientJob_Listen, &param);
            if(tId == InvalidThreadIdentifier) {
            	REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Failure creating ClientJob listen thread");
            	return false;
            }
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }

        if ((param.retVal != table[i].expected) || (!param.NoError)) {
            return false;
        }

        i++;

    }

    return true;

}

void StartServer_Join(BasicUDPSocketTest &param) {
    BasicUDPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }

    bool error = serverSocket.SetBlocking(param.isBlocking);

    if (!error) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::StartServer_Join() Cannot put the socket in blocking mode");
        return;
    }

    if (!param.isValidServer) {
        serverSocket.Close();
    }

    if(!serverSocket.Join(param.server.GetAddress().Buffer())) {
        param.retVal = false;
        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::StartServer_Join() Cannot join");
        return;
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.retVal = false;
        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::StartServer_Join() Cannot listen");
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    if (param.isValidClient) {
        for (uint32 i = 0; i < param.alives; i++) {
            char8 output[32] = { 0 };
            uint32 sizeRead = 32;
            if (!serverSocket.Read(output, sizeRead)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
            else {
                if (StringHelper::Compare(output, "HelloServer") != 0) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
            }

            if (param.NoError) {
                serverSocket.SetDestination(serverSocket.GetSource());
                const char8 *toWrite = "HelloClient";
                uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
                if (!serverSocket.Write(toWrite, sizeWrite)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
            }

        }
    }
    while (Threads::NumberOfThreads() < (param.alives + 1)) {
        Sleep::MSec(10);
    }

    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }
    serverSocket.Close();
}

void ClientJob_Join(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    bool go = clientSocket.Open();
    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::ClientJob_Join() Cannot put the socket in blocking mode");
    }
    else {
        if (!param.isValidClient) {
            clientSocket.Close();
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::ClientJob_Join() Closing client socket, !isValidClient");
        }

        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.retVal = false;
            param.sem.FastUnLock();
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::ClientJob_Join() Cannot connect");
        }

        else {
            const char8* toWrite = "HelloServer";
            uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
            if (!clientSocket.Write(toWrite, sizeWrite)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::ClientJob_Join() Cannot write");
            }
            else {
                char8 output[32] = { 0 };
                uint32 sizeRead = 32;
                if (!clientSocket.Read(output, sizeRead)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::ClientJob_Join() Cannot read");
                }
                else {
                    if (StringHelper::Compare(output, "HelloClient") != 0) {
                        param.sem.FastLock();
                        param.NoError = false;
                        param.sem.FastUnLock();
                        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicUDPSocketTest::ClientJob_Join() Read data is not matching written data");
                    }
                }
            }
        }

    }

    param.eventSem.Wait();
    clientSocket.Close();
}

static bool JoinConnectTest(BasicUDPSocketTest &param,
                              const ConnectListenUDPTestTable* table,
                              bool isServer) {

    uint32 i = 0;
    while (table[i].port != 0) {
        param.server.SetPort(table[i].port);
        //234.0.0.0-234.255.255.255	Unicast-Prefix-based IPv4 Multicast Addresses [RFC6034] Reg. 2010-08-11
        param.server.SetAddress("234.0.5.1");

        param.eventSem.Reset();
        param.exitCondition = 0;

        param.alives = table[i].nClients;

        if (isServer) {
            param.isValidServer = table[i].isValid;
        }
        else {
            param.isValidClient = table[i].isValid;
        }

        uint32 numOfThreadsBefore = Threads::NumberOfThreads();

        Threads::BeginThread((ThreadFunctionType) StartServer_Join, &param);

        while (param.exitCondition < 1) {
            if (!param.NoError) {
                param.alives = 0;
                while (Threads::NumberOfThreads() > numOfThreadsBefore) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        if (!param.retVal) {
            return (!table[i].expected);
        }

        for (uint32 k = 0; k < table[i].nClients; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Join, &param);
        }

        uint32 currentNumOfThreads = Threads::NumberOfThreads();

        while (currentNumOfThreads > numOfThreadsBefore) {
            Sleep::MSec(10);
            currentNumOfThreads = Threads::NumberOfThreads();
        }

        if ((param.retVal != table[i].expected) || (!param.NoError)) {
            return false;
        }

        i++;

    }

    return true;

}

bool BasicUDPSocketTest::TestListen(const ConnectListenUDPTestTable* table) {
    return ListenConnectTest(*this, table, true);
}

bool BasicUDPSocketTest::TestJoin(const ConnectListenUDPTestTable* table) {
    return JoinConnectTest(*this, table, true);
}

bool BasicUDPSocketTest::TestConnect(const ConnectListenUDPTestTable* table) {

    return ListenConnectTest(*this, table, false);
}

void StartServer_Read(BasicUDPSocketTest &param) {
    BasicUDPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.NoError = false;
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    for (uint32 i = 0; i < param.alives; i++) {
        char8 output[32] = { 0 };
        uint32 sizeRead = 32;
        if (!serverSocket.Read(output, sizeRead)) {
            param.sem.FastLock();
            param.NoError = false;
            param.sem.FastUnLock();
        }
        else {
            if (StringHelper::Compare(output, "HelloServer") != 0) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
        }

        if (param.NoError && param.isServer && param.isValidClient) {
            serverSocket.SetDestination(serverSocket.GetSource());

            //uint32 sizeWrite = 64;
            char8 input[64] = { 0 };
            StringHelper::Copy(input, param.string);
            uint32 sizeWrite = param.size;
            if (!serverSocket.Write(input, sizeWrite)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
        }
    }

    while (Threads::NumberOfThreads() < (param.alives + 1)) {
        Sleep::MSec(10);
    }
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }
    serverSocket.Close();
}

void ClientJob_Read(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;

    if (!clientSocket.Open()) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.NoError = false;
            param.sem.FastUnLock();
        }

        else {

            const char8* toWrite = "HelloServer";
            uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
            if (!clientSocket.Write(toWrite, sizeWrite)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
            else {

                bool ret = clientSocket.SetBlocking(param.isBlocking);
                if (!ret) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (!param.isValidClient) {
                        clientSocket.Close();
                    }

                    char8 output[64] = { 0 };
                    uint32 sizeRead = 64; //param.size;
                    if (param.isTimeout) {
                        ret = clientSocket.Read(output, sizeRead, param.timeout);
                    }
                    else {
                        ret = clientSocket.Read(output, sizeRead);
                    }
                    if (!ret) {
                        param.sem.FastLock();
                        param.retVal = false;
                        param.sem.FastUnLock();
                    }
                    else {

                        if (sizeRead != param.expectedSize) {
                            param.sem.FastLock();
                            param.retVal = false;
                            param.sem.FastUnLock();
                        }
                        else {
                            output[(sizeRead > 63) ? (63) : (sizeRead)] = 0;
                            if (StringHelper::Compare(output, param.result) != 0) {
                                param.sem.FastLock();
                                param.retVal = false;
                                param.sem.FastUnLock();
                            }
                        }
                    }
                }
            }
        }
    }
    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicUDPSocketTest::TestRead(const ReadWriteUDPTestTable* table) {

    uint32 i = 0;
    while (table[i].port != 0) {
        server.SetPort(table[i].port);
        server.SetAddress("127.0.0.1");

        eventSem.Reset();
        exitCondition = 0;
        string = table[i].string;
        result = table[i].result;
        alives = table[i].nClients;
        timeout = table[i].timeout;
        isServer = table[i].isServer;
        size = table[i].size;
        expectedSize = table[i].expectedSize;
        isTimeout = table[i].isTimeout;
        isBlocking = table[i].isBlocking;
        isValidClient = table[i].isValid;

        Threads::BeginThread((ThreadFunctionType) StartServer_Read, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                while (Threads::NumberOfThreads() > 0) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);

        }

        for (uint32 k = 0; k < table[i].nClients; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Read, this);
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(50);
        }

        if ((retVal != table[i].expected) || (!NoError)) {
            return false;
        }

        i++;
    }
    return true;
}

void ClientJob_Peek(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    if (!clientSocket.Open()) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.NoError = false;
            param.sem.FastUnLock();
        }

        else {

            const char8* toWrite = "HelloServer";
            uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
            if (!clientSocket.Write(toWrite, sizeWrite)) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
            else {

                bool ret = clientSocket.SetBlocking(param.isBlocking);

                if (!ret) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (!param.isValidClient) {
                        clientSocket.Close();
                    }

                    char8 output[64] = { 0 };
                    uint32 sizeRead = 64; //param.size;

                    for (uint32 i = 0; i < 4; i++) {
                        ret = clientSocket.Peek(output, sizeRead);

                        if (!ret) {
                            param.sem.FastLock();
                            param.retVal = false;
                            param.sem.FastUnLock();
                            break;
                        }
                        else {

                            if (sizeRead != param.expectedSize) {
                                param.sem.FastLock();
                                param.retVal = false;
                                param.sem.FastUnLock();
                                break;
                            }
                            else {
                                output[(sizeRead > 63) ? (63) : (sizeRead)] = 0;
                                if (StringHelper::Compare(output, param.result) != 0) {
                                    param.sem.FastLock();
                                    param.retVal = false;
                                    param.sem.FastUnLock();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicUDPSocketTest::TestPeek(const ReadWriteUDPTestTable* table) {

    uint32 i = 0;
    while (table[i].port != 0) {
        server.SetPort(table[i].port);
        server.SetAddress("127.0.0.1");

        eventSem.Reset();
        exitCondition = 0;
        string = table[i].string;
        result = table[i].result;
        alives = table[i].nClients;
        timeout = table[i].timeout;
        isServer = table[i].isServer;
        size = table[i].size;
        expectedSize = table[i].expectedSize;
        isTimeout = table[i].isTimeout;
        isBlocking = table[i].isBlocking;
        isValidClient = table[i].isValid;

        Threads::BeginThread((ThreadFunctionType) StartServer_Read, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                while (Threads::NumberOfThreads() > 0) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClients; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Peek, this);
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }

        if ((retVal != table[i].expected) || (!NoError)) {
            return false;
        }

        i++;
    }
    return true;
}

void StartServer_Write(BasicUDPSocketTest &param) {
    BasicUDPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.NoError = false;
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    if (param.isValidClient) {
        for (uint32 i = 0; i < param.alives; i++) {

            if (param.isServer) {

                uint32 sizeRead = 64;
                char8 output[64];
                if (!serverSocket.Read(output, sizeRead)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (sizeRead != param.expectedSize) {
                        param.sem.FastLock();
                        param.retVal = false;
                        param.sem.FastUnLock();
                    }
                    else {
                        output[(sizeRead > 63) ? (63) : (sizeRead)] = 0;
                        if (StringHelper::Compare(output, param.result) != 0) {
                            param.sem.FastLock();
                            param.retVal = false;
                            param.sem.FastUnLock();
                        }
                    }
                }
            }
        }
    }

    while (Threads::NumberOfThreads() < (param.alives + 1)) {
        Sleep::MSec(10);
    }
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }
    serverSocket.Close();
}

void ClientJob_Write(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    if (!clientSocket.Open()) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.NoError = false;
            param.sem.FastUnLock();
        }

        else {
            bool ret = clientSocket.SetBlocking(param.isBlocking);

            if (!ret) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
            else {

                if (!param.isValidClient) {
                    clientSocket.Close();
                }

                char8 input[128];
                MemoryOperationsHelper::Set(input, 0, 128);
                StringHelper::Copy(input, param.string);
                uint32 sizeWrite = param.size;
                if (param.isTimeout) {
                    ret = clientSocket.Write(input, sizeWrite, param.timeout);
                }
                else {
                    ret = clientSocket.Write(input, sizeWrite);
                }
                if (!ret) {
                    param.sem.FastLock();
                    param.retVal = false;
                    param.sem.FastUnLock();
                }

            }
        }
    }
    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicUDPSocketTest::TestWrite(const ReadWriteUDPTestTable* table) {
    uint32 i = 0;
    while (table[i].string != NULL) {
        server.SetPort(table[i].port);
        server.SetAddress("127.0.0.1");

        eventSem.Reset();
        exitCondition = 0;
        string = table[i].string;
        result = table[i].result;
        alives = table[i].nClients;
        timeout = table[i].timeout;
        isServer = table[i].isServer;
        size = table[i].size;
        expectedSize = table[i].expectedSize;
        isTimeout = table[i].isTimeout;
        isBlocking = table[i].isBlocking;
        isValidClient = table[i].isValid;

        Threads::BeginThread((ThreadFunctionType) StartServer_Write, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                while (Threads::NumberOfThreads() > 0) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClients; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Write, this);
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }

        if ((retVal != table[i].expected) || (!NoError)) {
            return false;
        }

        i++;
    }
    return true;
}
