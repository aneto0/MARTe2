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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

BasicUDPSocketTest::BasicUDPSocketTest() {

    retVal = true;
    noError = true;
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
        param.noError = false;
        return;
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.noError = false;
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    if (param.noError) {
        for (uint32 i = 0; i < param.alives; i++) {
            char8 output[32] = { 0 };
            uint32 sizeRead = 32;
            if (!serverSocket.Read(output, sizeRead)) {
                param.sem.FastLock();
                param.noError = false;
                param.sem.FastUnLock();
            }
            else {
                if (StringHelper::Compare(output, "HelloServer") != 0) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
            }

            if (param.noError) {
                serverSocket.SetDestination(serverSocket.GetSource());
                const char8 *toWrite = "HelloClient";
                uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
                if (!serverSocket.Write(toWrite, sizeWrite)) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
            }

        }
    }
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_Listen(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    bool go = clientSocket.Open();
    go &= clientSocket.SetBlocking(param.isBlocking);

    if (!go) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }
    else {
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
                param.noError = false;
                param.sem.FastUnLock();
            }
            else {
                char8 output[32] = { 0 };
                uint32 sizeRead = 32;
                if (!clientSocket.Read(output, sizeRead)) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (StringHelper::Compare(output, "HelloClient") != 0) {
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

bool BasicUDPSocketTest::TestListen(const ConnectListenUDPTestTable* table) {

    uint32 i = 0;
    while (table[i].port != 0) {
        server.SetPort(table[i].port);
        server.SetAddress("127.0.0.1");

        eventSem.Reset();
        exitCondition = 0;

        alives = table[i].nClients;

        Threads::BeginThread((ThreadFunctionType) StartServer_Listen, this);

        while (exitCondition < 1) {
            if (!noError) {
                alives = 0;
                while (Threads::NumberOfThreads() > 0) {
                    Sleep::MSec(10);
                }
                return false;
            }
            Sleep::MSec(10);
        }

        for (uint32 k = 0; k < table[i].nClients; k++) {
            Threads::BeginThread((ThreadFunctionType) ClientJob_Listen, this);
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

bool BasicUDPSocketTest::TestConnect(const ConnectListenUDPTestTable* table) {

    return TestListen(table);
}

void StartServer_Read(BasicUDPSocketTest &param) {
    BasicUDPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
        return;
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.noError = false;
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    if (param.noError) {
        for (uint32 i = 0; i < param.alives; i++) {
            char8 output[32] = { 0 };
            uint32 sizeRead = 32;
            if (!serverSocket.Read(output, sizeRead)) {
                param.sem.FastLock();
                param.noError = false;
                param.sem.FastUnLock();
            }
            else {
                if (StringHelper::Compare(output, "HelloServer") != 0) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
            }

            if (param.noError && param.isServer) {
                serverSocket.SetDestination(serverSocket.GetSource());

                uint32 sizeWrite = 64;
                char8 input[64];
                StringHelper::Copy(input, param.string);
                if (!serverSocket.Write(input, sizeWrite)) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
            }
        }

    }
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_Read(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }

    else {

        const char8* toWrite = "HelloServer";
        uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
        if (!clientSocket.Write(toWrite, sizeWrite)) {
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }
        else {

            bool ret = clientSocket.SetBlocking(param.isBlocking);

            if (!ret) {
                param.sem.FastLock();
                param.noError = false;
                param.sem.FastUnLock();
            }
            else {
                char8 output[64] = { 0 };
                uint32 sizeRead = param.size;
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

    param.eventSem.Wait();
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

        Threads::BeginThread((ThreadFunctionType) StartServer_Read, this);

        while (exitCondition < 1) {
            if (!noError) {
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
            Sleep::MSec(10);
        }

        if ((retVal != table[i].expected) || (!noError)) {
            printf("\n%d %d %d %d\n", retVal, table[i].expected, noError, i);
            return false;
        }

        i++;

    }

    return true;

}

void ClientJob_Peek(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }

    else {

        const char8* toWrite = "HelloServer";
        uint32 sizeWrite = StringHelper::Length(toWrite) + 1;
        if (!clientSocket.Write(toWrite, sizeWrite)) {
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }
        else {

            bool ret = clientSocket.SetBlocking(param.isBlocking);

            if (!ret) {
                param.sem.FastLock();
                param.noError = false;
                param.sem.FastUnLock();
            }
            else {
                char8 output[64] = { 0 };
                uint32 sizeRead = param.size;

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

    param.eventSem.Wait();
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

        Threads::BeginThread((ThreadFunctionType) StartServer_Read, this);

        while (exitCondition < 1) {
            if (!noError) {
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

        if ((retVal != table[i].expected) || (!noError)) {
            printf("\n%d %d %d %d\n", retVal, table[i].expected, noError, i);
            return false;
        }

        i++;

    }

    return true;

}

void StartServer_Write(BasicUDPSocketTest &param) {
    BasicUDPSocket serverSocket;

    if (!serverSocket.Open()) {
        param.noError = false;
        return;
    }

    if (!serverSocket.Listen(param.server.GetPort())) {
        param.noError = false;
        return;
    }

    param.sem.FastLock();
    param.exitCondition = 1;
    param.sem.FastUnLock();

    if (param.noError) {
        for (uint32 i = 0; i < param.alives; i++) {

            if (param.isServer) {

                uint32 sizeRead = 64;
                char8 output[64];
                if (!serverSocket.Read(output, sizeRead)) {
                    param.sem.FastLock();
                    param.noError = false;
                    param.sem.FastUnLock();
                }
                else {
                    if (sizeRead != param.expectedSize) {
                        param.sem.FastLock();
                        param.retVal = false;
                        printf("\n%d %d\n", sizeRead, param.expectedSize);
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
    param.eventSem.Post();
    while (Threads::NumberOfThreads() > 1) {
        Sleep::MSec(10);
    }

}

void ClientJob_Write(BasicUDPSocketTest &param) {

    BasicUDPSocket clientSocket;
    bool go = clientSocket.Open();

    if (!clientSocket.Connect(param.server.GetAddress().Buffer(), param.server.GetPort())) {
        param.sem.FastLock();
        param.noError = false;
        param.sem.FastUnLock();
    }

    else {
        bool ret = clientSocket.SetBlocking(param.isBlocking);

        if (!ret) {
            param.sem.FastLock();
            param.noError = false;
            param.sem.FastUnLock();
        }
        else {
            char8 input[128];
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

    param.eventSem.Wait();
}

bool BasicUDPSocketTest::TestWrite(const ReadWriteUDPTestTable* table) {
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

        Threads::BeginThread((ThreadFunctionType) StartServer_Write, this);

        while (exitCondition < 1) {
            if (!noError) {
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

        if ((retVal != table[i].expected) || (!noError)) {
            printf("\n%d %d %d %d\n", retVal, table[i].expected, noError, i);
            return false;
        }

        i++;

    }

    return true;
}
