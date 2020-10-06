/**
 * @file HttpChunkedStreamTest.cpp
 * @brief Source file for class HttpChunkedStreamTest
 * @date 01/10/2018
 * @author Giuseppe Ferro
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
 * the class HttpChunkedStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpChunkedStreamTest.h"
#include "Threads.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpChunkedStreamTest::HttpChunkedStreamTest() {
    // Auto-generated constructor stub for HttpChunkedStreamTest
    // TODO Verify if manual additions are needed
    eventSem.Create();
}

HttpChunkedStreamTest::~HttpChunkedStreamTest() {
    // Auto-generated destructor stub for HttpChunkedStreamTest
    // TODO Verify if manual additions are needed
}

bool HttpChunkedStreamTest::TestConstructor() {
    HttpChunkedStream test;
    bool chunkModeCheck = !test.IsChunkMode();
    bool readBufferSizeCheck = (test.GetReadBufferSize() == 32u);
    bool writeBufferSizeCheck = (test.GetWriteBufferSize() == 4096u);

    return chunkModeCheck && readBufferSizeCheck && writeBufferSizeCheck;
}

static void clientWriter(HttpChunkedStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(6665, "127.0.0.1");
    InternetHost destination(6666, "127.0.0.1");

    HttpChunkedStream socket;
    socket.SetChunkMode(true);

    //TODO Verify if this call is licit
    socket.SetBufferSize(8u, 8u);

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 6666);
    if (ret) {
        socket.Printf("%s", "HelloWorld");

        //uint32 sz=StringHelper::Length("HelloWorld");
        //socket.Write("HelloWorld", sz);
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpChunkedStreamTest::TestFlush() {
    InternetHost source(6665, "127.0.0.1");
    InternetHost destination(6666, "127.0.0.1");

    TCPSocket socket;
    socket.SetBlocking(false);
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(6666, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientWriter, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    char8 buff[32];
    MemoryOperationsHelper::Set(buff, 0, 32);
    uint32 size = 32u;

    uint32 beg = 0;
    while (size > 0) {
        size = 32u;
        newSocket.Read(buff + beg, size, 500);
        beg += size;
    }

    eventSem.Wait();

    bool ret = StringHelper::Compare(buff, "8\r\nHelloWor\r\n2\r\nld\r\n") == 0;

    printf("\n%s\n", buff);
    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientWriterFinalChunk(HttpChunkedStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(6665, "127.0.0.1");
    InternetHost destination(6666, "127.0.0.1");

    HttpChunkedStream socket;
    socket.SetChunkMode(true);

    socket.SetBufferSize(8u, 8u);

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 6666);
    if (ret) {
        socket.Printf("%s", "HelloWorld");
        socket.Flush();
        //uint32 sz=StringHelper::Length("HelloWorld");
        //socket.Write("HelloWorld", sz);
        socket.FinalChunk();

    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpChunkedStreamTest::TestFinalChunk() {
    InternetHost source(6665, "127.0.0.1");
    InternetHost destination(6666, "127.0.0.1");

    TCPSocket socket;
    socket.SetBlocking(false);
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(6666, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientWriterFinalChunk, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    char8 buff[32];
    MemoryOperationsHelper::Set(buff, 0, 32);
    uint32 size = 32u;

    uint32 beg = 0;
    while (size > 0) {
        size = 32u;
        newSocket.Read(buff + beg, size, 500);
        beg += size;
    }

    eventSem.Wait();

    bool ret = StringHelper::Compare(buff, "8\r\nHelloWor\r\n2\r\nld\r\n0\r\n\r\n")==0;

    printf("\n%s\n", buff);
    newSocket.Close();
    socket.Close();

    return ret;
}

bool HttpChunkedStreamTest::TestSetChunkMode() {
    HttpChunkedStream socket;
    bool ret=!socket.IsChunkMode();
    socket.SetChunkMode(true);
    ret&=socket.IsChunkMode();

    socket.SetChunkMode(false);
    ret&=!socket.IsChunkMode();

    return ret;
}

bool HttpChunkedStreamTest::TestIsChunkMode() {
    return TestSetChunkMode();
}
