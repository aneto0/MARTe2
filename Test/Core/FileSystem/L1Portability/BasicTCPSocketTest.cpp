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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicTCPSocketTest.h"
#include "StringHelper.h"
#include "stdio.h"
#include "InternetService.h"
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
    noError = true;
    createSocketOnHeap = true;
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

void StartServer_Listen(BasicTCPSocketTest &param) {
    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
    }
    else {
        uint32 nClients = param.alives;

        if (!serverSocket.Listen(param.server.GetPort(), param.maxConnections)) {
            param.noError = false;
            return;
        }
        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();

        while (Threads::NumberOfThreads() < (nClients + 1)) {
            Sleep::MSec(10);
        }

    }
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_Listen(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();
    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort(), param.timeout)) {
            param.sem.FastLock();
            param.retVal = false;
            param.sem.FastUnLock();
        }

    }

    param.eventSem.Wait();
    // printf("\nEnd Client : %d\n", nClient);

}

bool BasicTCPSocketTest::TestListen(const ConnectListenTestTable *table) {

    uint32 i = 0;

    while (table[i].serviceNameIn != NULL) {

        retVal = true;
        nClients = 0;
        exitCondition = 0;
        eventSem.Reset();

        isBlocking = table[i].isBlockingIn;
        server.SetAddress("127.0.0.1");
        server.SetPort(table[i].port);
        maxConnections = table[i].maxConnectionsIn;
        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;

        if (table[i].isServer) {
            Threads::BeginThread((ThreadFunctionType) StartServer_Listen, this);

            while (exitCondition < 1) {
                Sleep::MSec(10);
            }

            if (!noError) {
                return false;
            }
        }
        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Listen, this);
            nClients++;
        }

        if (!table[i].isServer) {
            while (Threads::NumberOfThreads() < (nClients)) {
                Sleep::MSec(10);
            }

            eventSem.Post();
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }

        if ((retVal != table[i].expected) || (!noError)) {
            return false;
        }

        i++;
    }
    return true;
}

void StartServer_ListenByServiceName(BasicTCPSocketTest &param) {
    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
    }
    else {

        uint32 nClients = param.alives;

        if (!serverSocket.Listen(param.serviceName, param.maxConnections)) {
            InternetService service;
            if (service.SearchByName(param.serviceName)) {
                param.noError = false;
            }
        }

        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();

        while (Threads::NumberOfThreads() < (nClients + 1)) {
            Sleep::MSec(10);
        }

    }
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_ListenByServiceName(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort(), param.timeout)) {
            //     printf("\nConnect failed!!\n");
            param.sem.FastLock();
            param.retVal = false;
            param.sem.FastUnLock();
        }

    }

    param.eventSem.Wait();
    // printf("\nEnd Client : %d\n", nClient);

}

bool BasicTCPSocketTest::TestListenByServiceName(const ConnectListenTestTable *table) {

    uint32 i = 0;

    while (table[i].serviceNameIn != NULL) {

        retVal = true;
        nClients = 0;
        exitCondition = 0;
        eventSem.Reset();

        isBlocking = table[i].isBlockingIn;

        server.SetAddress("127.0.0.1");
        InternetService service;
        service.SearchByName(table[i].serviceNameIn);
        server.SetPort(service.Port());
        maxConnections = table[i].maxConnectionsIn;
        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;

        serviceName = table[i].serviceNameIn;

        if (table[i].isServer) {
            Threads::BeginThread((ThreadFunctionType) StartServer_ListenByServiceName, this);

            while (exitCondition < 1) {
                Sleep::MSec(10);
            }

            if (!noError) {
                return false;
            }
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_ListenByServiceName, this);
            nClients++;
        }

        if (!table[i].isServer) {

            while (Threads::NumberOfThreads() < (nClients)) {
                Sleep::MSec(10);
            }
            eventSem.Post();
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!noError)) {

            return false;
        }
        i++;

    }
    return true;
}

bool BasicTCPSocketTest::TestConnect(const ConnectListenTestTable *table) {

    return TestListen(table);

}

bool BasicTCPSocketTest::TestConnectByServiceName(const ConnectListenTestTable *table) {
    return TestListenByServiceName(table);

}

struct ServerParam {
    BasicTCPSocketTest *testObj;
    BasicTCPSocket *socket;
};

void StartServer_ReadWrite(BasicTCPSocketTest &param) {

    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
    }
    else {
        uint32 acceptedConnections = 0;

        uint32 nClients = param.alives;

        ServerParam serverParam[256];

        if (!serverSocket.Listen(param.server.GetPort(), 30)) {
            param.noError = false;
        }
        else {
            param.sem.FastLock();
            param.exitCondition = 1;
            param.sem.FastUnLock();

            while (acceptedConnections < nClients) {

                BasicTCPSocket *newConnection = serverSocket.WaitConnection();
                if (newConnection != NULL) {
                    serverParam[acceptedConnections].socket = newConnection;
                    serverParam[acceptedConnections].testObj = &param;
                    Threads::BeginThread((ThreadFunctionType) param.serverJob, &serverParam[acceptedConnections]);
                    acceptedConnections++;
                }
                else {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                    param.alives = 0;
                }

                Sleep::MSec(5);
            }
        }

        while (Threads::NumberOfThreads() < (nClients + acceptedConnections + 1)) {
            Sleep::MSec(10);
        }
    }
    param.eventSem.Post();

    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void WriteJob(ServerParam &param) {

    uint32 sizeResponse = 64;
    if (param.testObj->isServer) {
        if (!param.socket->Write(param.testObj->string, sizeResponse)) {
            param.testObj->sem.FastLock();
            param.testObj->retVal = false;
            param.testObj->sem.FastUnLock();
        }
    }

    param.testObj->eventSem.Wait();
    param.testObj->sem.FastLock();
    delete param.socket;
    param.testObj->sem.FastUnLock();

}

void ClientJob_Read(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {

        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }
        else {
            clientSocket.SetBlocking(param.isBlocking);

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
                    param.noError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (StringHelper::CompareN(output, param.result, param.expectedSize) != 0) {
                        output[(size % 65)] = 0;
                        param.sem.FastLock();
                        param.noError = false;
                        param.sem.FastUnLock();
                    }
                }
            }
        }

    }

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

        server.SetAddress("127.0.0.1");
        server.SetPort(4444);

        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) WriteJob;

        // printf("\nisB = %d, maxConn= %d, sName= %s, alives= %d timeout= %d\n", isBlocking, maxConnections, serviceName, alives, timeout);
        Threads::BeginThread((ThreadFunctionType) StartServer_ReadWrite, this);

        while (exitCondition < 1) {
            Sleep::MSec(10);
        }

        if (!noError) {
            return false;
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Read, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!noError)) {

            return false;
        }
        i++;

    }
    return true;
}

void ClientJob_Peek(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {

        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }
        else {
            clientSocket.SetBlocking(param.isBlocking);

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
                        param.noError = false;
                        param.sem.FastUnLock();
                    }
                    else {
                        if (StringHelper::CompareN(output, param.result, param.expectedSize) != 0) {
                            output[(size % 65)] = 0;
                            param.sem.FastLock();
                            param.noError = false;
                            param.sem.FastUnLock();
                        }
                    }
                }
            }
        }

    }

    param.eventSem.Wait();

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

        server.SetAddress("127.0.0.1");
        server.SetPort(4444);

        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) WriteJob;

        // printf("\nisB = %d, maxConn= %d, sName= %s, alives= %d timeout= %d\n", isBlocking, maxConnections, serviceName, alives, timeout);
        Threads::BeginThread((ThreadFunctionType) StartServer_ReadWrite, this);

        while (exitCondition < 1) {
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Peek, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!noError)) {

            return false;
        }
        i++;

    }
    return true;

}

void ReadJob(ServerParam &param) {

    uint32 size = 64;
    if (param.testObj->isServer) {
        char8 output[64];
        if (!param.socket->Read(output, size)) {
            param.testObj->sem.FastLock();
            param.testObj->retVal = false;
            param.testObj->sem.FastUnLock();
        }
        else {
            if (size != param.testObj->expectedSize) {
                param.testObj->sem.FastLock();
                param.testObj->noError = false;
                param.testObj->sem.FastUnLock();
            }
            else {
                output[(size % 65)] = 0;
                if (StringHelper::CompareN(output, param.testObj->string, size) != 0) {
                    param.testObj->sem.FastLock();
                    param.testObj->noError = false;
                    param.testObj->sem.FastUnLock();
                }
            }
        }
    }

    param.testObj->eventSem.Wait();
    param.testObj->sem.FastLock();
    delete param.socket;
    param.testObj->sem.FastUnLock();

}

void ClientJob_Write(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {

        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }
        else {
            clientSocket.SetBlocking(param.isBlocking);

            uint32 size = param.size;
            bool ret;
            if (param.isTimeout) {
                ret = clientSocket.Write(param.string, size, param.timeout);
            }
            else {
                ret = clientSocket.Write(param.string, size);
            }

            if (!ret) {
                param.sem.FastLock();
                param.retVal = false;
                param.sem.FastUnLock();
            }
            else {
                if ((size != param.size)) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
            }
        }

    }

    param.eventSem.Wait();

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

        server.SetAddress("127.0.0.1");
        server.SetPort(4444);

        alives = table[i].nClientsIn;
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) ReadJob;

        // printf("\nisB = %d, maxConn= %d, sName= %s, alives= %d timeout= %d\n", isBlocking, maxConnections, serviceName, alives, timeout);
        Threads::BeginThread((ThreadFunctionType) StartServer_ReadWrite, this);

        while (exitCondition < 1) {
            Sleep::MSec(10);
        }

        if (!noError) {
            return false;
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Write, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!noError)) {

            return false;
        }
        i++;

    }
    return true;
}

void StartServer_WaitConnection(BasicTCPSocketTest &param) {

    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
    }
    else {
        serverSocket.SetBlocking(param.isBlocking);

        uint32 acceptedConnections = 0;

        uint32 nClients = param.alives;

        ServerParam serverParam[256];
        BasicTCPSocket client[256];

        if (!serverSocket.Listen(param.server.GetPort(), 30)) {
            param.noError = false;

        }
        else {
            param.sem.FastLock();
            param.exitCondition = 1;
            param.sem.FastUnLock();

            while (acceptedConnections < nClients) {

                BasicTCPSocket *newConnection;
                if (param.createSocketOnHeap) {
                    newConnection = serverSocket.WaitConnection(param.timeout);
                }
                else {
                    newConnection = serverSocket.WaitConnection(param.timeout, &client[acceptedConnections]);
                }
                if (newConnection != NULL) {
                    serverParam[acceptedConnections].socket = newConnection;
                    serverParam[acceptedConnections].testObj = &param;
                }
                else {
                    param.sem.FastLock();
                    param.retVal = false;
                    param.sem.FastUnLock();
                    param.alives = 0;
                }
                acceptedConnections++;

                Sleep::MSec(5);
            }
        }

        while (Threads::NumberOfThreads() < (param.nClients + 1)) {
            Sleep::MSec(10);
        }
        /*
         while (param.exitCondition < (2 * param.nClients + 1)) {
         printf("<-");
         Sleep::MSec(10);
         }*/
    }
    param.eventSem.Post();

    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_WaitConnection(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();
    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort(), param.timeout)) {
            // printf("\nConnect failed!!\n");
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }

    }

    param.eventSem.Wait();
    // printf("\nEnd Client : %d\n", nClient);

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
        server.SetPort(4444);

        alives = (table[i].nClientsIn == 0) ? (10) : (table[i].nClientsIn);
        timeout = table[i].timeoutIn;
        serverJob = (ThreadFunctionType) ReadJob;

        // printf("\nisB = %d, maxConn= %d, sName= %s, alives= %d timeout= %d\n", isBlocking, maxConnections, serviceName, alives, timeout);
        Threads::BeginThread((ThreadFunctionType) StartServer_WaitConnection, this);

        while (exitCondition < 1) {
            Sleep::MSec(10);
        }

        if (!noError) {
            return false;
        }

        for (uint32 k = 0; k < table[i].nClientsIn; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_WaitConnection, this);
            nClients++;
        }

        while (Threads::NumberOfThreads() > 0) {
            Sleep::MSec(10);
        }
        if ((retVal != table[i].expected) || (!noError)) {

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

void StartServer_IsConnected(BasicTCPSocketTest &param) {

    BasicTCPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
    }
    else {

        param.sem.FastLock();
        param.exitCondition = 1;
        param.sem.FastUnLock();

        uint32 acceptedConnections = 0;

        uint32 nClients = param.alives;

        BasicTCPSocket clients[256];
        if (param.isServer) {

            if (!serverSocket.Listen(param.server.GetPort(), 30)) {
                param.sem.FastLock();
                param.noError = false;
                param.sem.FastUnLock();
            }
            else {

                while (acceptedConnections < nClients) {

                    BasicTCPSocket * newConnection = serverSocket.WaitConnection();
                    if (newConnection != NULL) {
                        clients[acceptedConnections] = *newConnection;
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
        while (Threads::NumberOfThreads() < (nClients + 1)) {
            Sleep::MSec(10);
        }

    }

    param.eventSem.Post();

    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_IsConnected(BasicTCPSocketTest &param) {

    BasicTCPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {
        if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
            // printf("\nConnect failed!!\n");
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
    // printf("\nEnd Client : %d\n", nClient);

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
        Sleep::MSec(10);
    }

    if (!noError) {
        return false;
    }

    for (uint32 k = 0; k < alives; k++) {
        Threads::BeginThread((ThreadFunctionType) ClientJob_IsConnected, this);
        nClients++;
    }

    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }

    return ((retVal == expected) && (noError));
}

