/**
 * @file BasicTCPSocketTest.cpp
 * @brief Source file for class BasicTCPSocketTest
 * @date 29/10/2015
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
 * the class BasicTCPSocketTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicTCPSocketTest.h"
#include "StringHelper.h"
#include "InternetService.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

BasicTCPSocketTest::BasicTCPSocketTest() {
    maxConnections = 0;
    retVal = true;
    nClients = 0;
    alives = 0;
    exitCondition = 0;
    serverJob = NULL;
    timeout = TTInfiniteWait;
    eventSem.Create();
    serviceName = NULL;
    isBlocking = true;
    string = NULL;
    result = NULL;
    size = 0;
    expectedSize = 0;
    isServer = true;
    isTimeout = false;
    NoError = true;
    createSocketOnHeap = true;
    isValidClient = true;
    isValidServer = true;

    numOfThreadsBefore = Threads::NumberOfThreads();
}

bool BasicTCPSocketTest::TestDefaultConstructor() {

    BasicTCPSocket basicTCPSocket;

    if (basicTCPSocket.GetDestination().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (basicTCPSocket.GetDestination().GetPort() != 0) {
        return false;
    }

    if (basicTCPSocket.GetSource().GetAddress() != "0.0.0.0") {
        return false;
    }

    if (basicTCPSocket.GetSource().GetPort() != 0) {
        return false;
    }

    return !basicTCPSocket.IsValid();
}

bool BasicTCPSocketTest::TestSeek() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.Seek(0);
}

bool BasicTCPSocketTest::TestSize() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.Size() == 0xffffffffffffffff;
}

bool BasicTCPSocketTest::TestRelativeSeek() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.RelativeSeek(0);
}

bool BasicTCPSocketTest::TestPosition() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.Position() == 0xffffffffffffffff;
}

bool BasicTCPSocketTest::TestSetSize() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.SetSize(1);
}

bool BasicTCPSocketTest::TestCanWrite() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.CanWrite();
}

bool BasicTCPSocketTest::TestCanRead() {
    BasicTCPSocket basicTCPSocket;
    return basicTCPSocket.CanRead();
}

bool BasicTCPSocketTest::TestCanSeek() {
    BasicTCPSocket basicTCPSocket;
    return !basicTCPSocket.CanSeek();
}

static void StartServer_Listen(BasicTCPSocketTest &param) {
    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }

    if (!param.isValidServer) {
        serverSocket.Close();
    }

    if (!serverSocket.Listen(param.server.GetPort(), param.maxConnections)) {
        param.retVal = false;
        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();
        //printf("\nListen fail");
    }


    if (param.retVal) {
        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();

        while (Threads::NumberOfThreads() < (param.alives + 1)) {
            Sleep::MSec(10);
        }

        param.eventSem.Post();
        while (Threads::NumberOfThreads() > 1) {
            Sleep::MSec(10);
        }
    }
    serverSocket.Close();
    //printf("\nClosed!");
}

static void ClientJob_Listen(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
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

        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort(), param.timeout)) {
            param.sem.FastLock();
            param.retVal = false;
            //printf("\nConnect fail");
            param.sem.FastUnLock();
        }
    }

    param.eventSem.Wait();
    clientSocket.Close();
}

static bool ListenConnectTest(BasicTCPSocketTest &param,
                              const ConnectListenTestTable *table,
                              bool isServer) {

    uint32 i = 0;

    while (table[i].port != 0) {

        param.retVal = true;
        param.nClients = 0;
        param.exitCondition = 0;
        param.eventSem.Reset();

        param.isBlocking = table[i].isBlockingIn;
        param.server.SetAddress("127.0.0.1");
        param.server.SetPort(table[i].port);
        param.maxConnections = table[i].maxConnectionsIn;
        param.alives = table[i].nClientsIn;
        param.timeout = table[i].timeoutIn;

        if (isServer) {
            param.isValidServer = table[i].isValid;
        }
        else {
            param.isValidClient = table[i].isValid;
        }
        
        uint32 numOfThreadsBefore = 0u;
        uint32 numOfSpawnedThreads = 0u;

        if (table[i].isServer) {    
            numOfThreadsBefore = Threads::NumberOfThreads();
            Threads::BeginThread((ThreadFunctionType) StartServer_Listen, &param);
            ++numOfSpawnedThreads;
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
                return (!table[i].expected) && (!param.isValidServer);
            }
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Listen, &param);
            ++numOfSpawnedThreads;
            param.nClients++;
        }

        if (!table[i].isServer) {
            while (Threads::NumberOfThreads() < (param.nClients)) {
                Sleep::MSec(10);
            }

            param.eventSem.Post();
        }

        while (Threads::NumberOfThreads() > numOfThreadsBefore) {
            Sleep::MSec(10);
        }
        if ((param.retVal != table[i].expected) || (!param.NoError)) {
            printf("fallo en %d %d %d %d\n", i, param.retVal, table[i].expected, param.NoError);
            return false;
        }

        i++;
    }
    return true;
}

bool BasicTCPSocketTest::TestListen(const ConnectListenTestTable *table) {

    return ListenConnectTest(*this, table, true);
}

bool BasicTCPSocketTest::TestConnect(const ConnectListenTestTable *table) {

    return ListenConnectTest(*this, table, false);
}

struct ServerParam {
    BasicTCPSocketTest *testObj;
    BasicTCPSocket *socket;
};

static void StartServer_ReadWrite(BasicTCPSocketTest &param) {

    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }
    uint32 acceptedConnections = 0;

    ServerParam serverParam[256];

    if (!serverSocket.Listen(param.server.GetPort(), 64)) {
        param.NoError = false;
        return;
    }
    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    while (acceptedConnections < param.alives) {

        BasicTCPSocket *newConnection = serverSocket.WaitConnection();
        if (newConnection != NULL) {
            serverParam[acceptedConnections].socket = newConnection;
            serverParam[acceptedConnections].testObj = &param;
            Threads::BeginThread((ThreadFunctionType) param.serverJob, &serverParam[acceptedConnections]);
            acceptedConnections++;
        }
        else {
            param.sem.FastLock();
            param.NoError = false;
            param.sem.FastUnLock();
        }

        Sleep::MSec(10);
    }
    while (Threads::NumberOfThreads() < (param.alives + acceptedConnections + 1)) {
        Sleep::MSec(10);
    }
    param.eventSem.Post();

    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

    for (uint32 i = 0; i < acceptedConnections; i++) {
        serverParam[i].socket->Close();
        delete serverParam[i].socket;
    }
    serverSocket.Close();
}

static void WriteJob(ServerParam &param) {

    uint32 sizeResponse = 64;
    if (param.testObj->isServer && param.testObj->isValidClient) {
        char8 input[64]={'\0'};
        StringHelper::Copy(input, param.testObj->string);
        if (!param.socket->Write(input, sizeResponse)) {
            param.testObj->sem.FastLock();
            param.testObj->retVal = false;
            param.testObj->sem.FastUnLock();
        }
    }

    param.testObj->eventSem.Wait();
    param.testObj->sem.FastLock();
    param.testObj->sem.FastUnLock();
    param.socket->Close();
}

static void ClientJob_Read(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
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

            clientSocket.SetBlocking(param.isBlocking);

            if (!param.isValidClient) {
                clientSocket.Close();
            }

            char8 output[64];
            uint32 size = param.size;
            bool ret;
            if (param.isTimeout) {
                ret = clientSocket.Read(output, size, param.timeout);
            }
            else {
                ret = clientSocket.Read(output, size);
            }

            if (!ret) {
                param.sem.FastLock();
                param.retVal = false;
                param.sem.FastUnLock();

            }
            else {
                if ((size != param.expectedSize)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
                else {
                    output[(size > 63) ? (63) : (size)] = 0;
                    if (StringHelper::Compare(output, param.result) != 0) {
                        param.sem.FastLock();
                        param.NoError = false;
                        param.sem.FastUnLock();
                    }
                }
            }
        }
    }
    clientSocket.Close();
    param.eventSem.Wait();
}

bool BasicTCPSocketTest::TestRead(const ReadWriteTestTable *table) {

    uint32 i = 0;

    while (table[i].stringIn != NULL) {

        size = table[i].sizeIn;
        expectedSize = table[i].expectedSizeIn;
        string = table[i].stringIn;
        result = table[i].resultIn;
        isBlocking = table[i].isBlockingIn;

        isServer = table[i].isServer;
        retVal = true;
        nClients = 0;
        exitCondition = 0;
        eventSem.Reset();

        isTimeout = table[i].isTimeoutIn;

        isValidClient = table[i].isValid;

        server.SetAddress("127.0.0.1");
        server.SetPort(4444);

        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) WriteJob;

        Threads::BeginThread((ThreadFunctionType) StartServer_ReadWrite, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                while (Threads::NumberOfThreads() > numOfThreadsBefore) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Read, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > numOfThreadsBefore) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!NoError)) {
            return false;
        }
        i++;

    }
    return true;
}

static void ClientJob_Peek(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
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
            clientSocket.SetBlocking(param.isBlocking);

            if (!param.isValidClient) {
                clientSocket.Close();
            }
            char8 output[64];
            uint32 size = param.size;

            for (uint32 i = 0; i < 4; i++) {
                if (!clientSocket.Peek(output, size)) {
                    param.sem.FastLock();
                    param.retVal = false;
                    param.sem.FastUnLock();
                }
                else {
                    if ((size != param.expectedSize)) {
                        param.sem.FastLock();
                        param.NoError = false;
                        param.sem.FastUnLock();
                    }
                    else {
                        output[(size > 63) ? (63) : (size)] = 0;
                        if (StringHelper::Compare(output, param.result) != 0) {

                            param.sem.FastLock();
                            param.NoError = false;
                            param.sem.FastUnLock();
                        }
                    }
                }
            }
        }

    }

    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicTCPSocketTest::TestPeek(const ReadWriteTestTable *table) {

    uint32 i = 0;

    while (table[i].stringIn != NULL) {

        size = table[i].sizeIn;
        expectedSize = table[i].expectedSizeIn;
        string = table[i].stringIn;
        result = table[i].resultIn;
        isBlocking = table[i].isBlockingIn;

        isServer = table[i].isServer;
        retVal = true;
        nClients = 0;
        exitCondition = 0;
        eventSem.Reset();

        isValidClient = table[i].isValid;

        server.SetAddress("127.0.0.1");
        server.SetPort(4444);

        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) WriteJob;

        Threads::BeginThread((ThreadFunctionType) StartServer_ReadWrite, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                while (Threads::NumberOfThreads() > numOfThreadsBefore) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Peek, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > numOfThreadsBefore) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!NoError)) {

            return false;
        }
        i++;

    }
    return true;

}

static void ReadJob(ServerParam &param) {

    uint32 size = 64;
    if (param.testObj->isServer && param.testObj->isValidClient) {
        char8 output[64];
        if (!param.socket->Read(output, size)) {
            param.testObj->sem.FastLock();
            param.testObj->retVal = false;
            param.testObj->sem.FastUnLock();
        }
        else {
            if (size != param.testObj->expectedSize) {
                param.testObj->sem.FastLock();
                param.testObj->NoError = false;
                param.testObj->sem.FastUnLock();
            }
            else {
                output[(size > 63) ? (63) : (size)] = 0;
                if (StringHelper::Compare(output, param.testObj->result) != 0) {
                    param.testObj->sem.FastLock();
                    param.testObj->NoError = false;
                    param.testObj->sem.FastUnLock();
                }
            }
        }
    }

    param.testObj->eventSem.Wait();
    param.testObj->sem.FastLock();
    param.testObj->sem.FastUnLock();

}

static void ClientJob_Write(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
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
            clientSocket.SetBlocking(param.isBlocking);

            if (!param.isValidClient) {
                clientSocket.Close();
            }

            bool ret = true;
            const uint32 maxSize = (param.size > 1000) ? (512) : (128);
            char8 *input = new char8[maxSize];
            MemoryOperationsHelper::Set(input, 0, maxSize);
            StringHelper::Copy(input, param.string);
            uint32 size = param.size;
            uint32 remainedSize = size;
            //  for (uint32 k = 0; k < iterations; k++) {
            while ((remainedSize > 0) && (ret)) {
                size = (remainedSize > maxSize) ? (maxSize) : (remainedSize);

                if (param.isTimeout) {
                    ret = clientSocket.Write(input, size, param.timeout);
                }
                else {
                    ret = clientSocket.Write(input, size);
                }

                remainedSize -= size;

            }

            //}

            if (!ret) {
                param.sem.FastLock();
                param.retVal = false;
                param.sem.FastUnLock();
            }
            else {
                if ((size != param.size)) {
                    param.sem.FastLock();
                    param.NoError = false;
                    param.sem.FastUnLock();
                }
            }
            delete [] input;

        }

    }
    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicTCPSocketTest::TestWrite(const ReadWriteTestTable *table) {

    uint32 i = 0;

    while (table[i].stringIn != NULL) {

        size = table[i].sizeIn;
        expectedSize = table[i].expectedSizeIn;
        string = table[i].stringIn;
        result = table[i].resultIn;
        isBlocking = table[i].isBlockingIn;

        isTimeout = table[i].isTimeoutIn;

        isServer = table[i].isServer;
        retVal = true;
        nClients = 0;
        exitCondition = 0;
        eventSem.Reset();

        isValidClient = table[i].isValid;

        server.SetAddress("127.0.0.1");
        server.SetPort(4444);

        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) ReadJob;

        Threads::BeginThread((ThreadFunctionType) StartServer_ReadWrite, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                while (Threads::NumberOfThreads() > numOfThreadsBefore) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Write, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > numOfThreadsBefore) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!NoError)) {

            return false;
        }
        i++;

    }
    return true;
}

static void StartServer_WaitConnection(BasicTCPSocketTest &param) {

    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }
    serverSocket.SetBlocking(param.isBlocking);

    uint32 acceptedConnections = 0;

    ServerParam serverParam[256];
    BasicTCPSocket client[256];

    if (!serverSocket.Listen(param.server.GetPort(), 30)) {
        param.NoError = false;
        return;
    }

    if (!param.isValidServer) {
        serverSocket.Close();
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    for (uint32 k = 0; k < param.alives; k++) {

        BasicTCPSocket *newConnection = NULL;
        if (param.createSocketOnHeap) {
            newConnection = serverSocket.WaitConnection(param.timeout);
        }
        else {
            newConnection = serverSocket.WaitConnection(param.timeout, &client[acceptedConnections]);
        }
        if (newConnection != NULL) {
            serverParam[acceptedConnections].socket = newConnection;
            acceptedConnections++;
        }
        else {
            param.sem.FastLock();
            param.retVal = false;
            param.sem.FastUnLock();
        }

        Sleep::MSec(5);
    }

    while (Threads::NumberOfThreads() < (param.nClients + 1)) {
        Sleep::MSec(10);
    }

    param.eventSem.Post();

    if (param.createSocketOnHeap) {
        for (uint32 i = 0; i < acceptedConnections; i++) {
            delete serverParam[i].socket;
        }
    }

    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }
    serverSocket.Close();
}

static void ClientJob_WaitConnection(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();
    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort(), param.timeout)) {

            if (param.isValidServer) {
                param.sem.FastLock();
                param.NoError = false;
                param.sem.FastUnLock();
            }
        }

    }

    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicTCPSocketTest::TestWaitConnection(const WaitConnectionTestTable *table) {
    uint32 i = 0;

    while (table[i].timeoutIn != 0) {

        isBlocking = table[i].isBlockingIn;
        retVal = true;
        nClients = 0;
        exitCondition = 0;
        eventSem.Reset();

        server.SetAddress("127.0.0.1");
        server.SetPort(4490);

        alives = (table[i].nClientsIn == 0) ? (10) : (table[i].nClientsIn);
        nClients = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) ReadJob;
        createSocketOnHeap = table[i].createOnHeapIn;
        isValidServer = table[i].isValid;

        Threads::BeginThread((ThreadFunctionType) StartServer_WaitConnection, this);

        while (exitCondition < 1) {
            if (!NoError) {
                alives = 0;
                nClients = 0;
                while (Threads::NumberOfThreads() > numOfThreadsBefore) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_WaitConnection, this);
        }

        while (Threads::NumberOfThreads() > numOfThreadsBefore) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!NoError)) {
            printf("Row=%d retVal %d==%d expected, NoError=%d",i,retVal,table[i].expected,NoError);
            return false;
        }
        i++;

    }
    return true;
}

bool BasicTCPSocketTest::TestIsValid() {

    BasicTCPSocket socket;

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

bool BasicTCPSocketTest::TestOpen() {
    return TestIsValid();
}

bool BasicTCPSocketTest::TestClose() {
    return TestIsValid();
}

static void StartServer_IsConnected(BasicTCPSocketTest &param) {

    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.NoError = false;
        return;
    }
    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    uint32 acceptedConnections = 0;

    BasicTCPSocket *clients[256];
    if (param.isServer) {

        if (!serverSocket.Listen(param.server.GetPort(), 30)) {
            param.sem.FastLock();
            param.NoError = false;
            param.sem.FastUnLock();
        }
        else {

            while (acceptedConnections < param.alives) {

                BasicTCPSocket * newConnection = serverSocket.WaitConnection();
                if (newConnection != NULL) {
                    clients[acceptedConnections] = newConnection;
                    acceptedConnections++;
                }
                else {
                    param.sem.FastLock();
                    param.retVal = false;
                    param.sem.FastUnLock();
                    param.alives = 0;
                }

                Sleep::MSec(5);
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
    for(uint32 i=0; i<acceptedConnections; i++){
        delete clients[i];
    }

    serverSocket.Close();
}

static void ClientJob_IsConnected(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
        param.sem.FastLock();
        param.NoError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
        }

    }
    uint32 t = 0;
    while (t++ < 100) {
        Sleep::MSec(1);
        if (clientSocket.IsConnected()) {

            break;
        }
    }

    param.sem.FastLock();
    param.retVal = clientSocket.IsConnected();
    param.sem.FastUnLock();

    param.eventSem.Wait();
    clientSocket.Close();
}

bool BasicTCPSocketTest::TestIsConnected(bool connect,
                                         bool expected) {

    isServer = connect;
    alives = 10;
    retVal = true;
    nClients = 0;
    exitCondition = 0;
    eventSem.Reset();

    server.SetAddress("127.0.0.1");
    server.SetPort(4444);

    Threads::BeginThread((ThreadFunctionType) StartServer_IsConnected, this);

    while (exitCondition < 1) {
        if (!NoError) {
            alives = 0;
            while (Threads::NumberOfThreads() > numOfThreadsBefore) {
                Sleep::MSec(10);
            }
            return false;
        }
        Sleep::MSec(10);
    }

    for (uint32 k = 0; k < alives; k++) {
        Threads::BeginThread((ThreadFunctionType) ClientJob_IsConnected, this);
        nClients++;
    }

    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::MSec(10);
    }

    return ((retVal == expected) && (NoError));
}

