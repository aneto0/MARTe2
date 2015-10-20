/**
 * @file SingleBufferedStreamTest.cpp
 * @brief Source file for class SingleBufferedStreamTest
 * @date 17/10/2015
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
 * the class SingleBufferedStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SingleBufferedStreamTest.h"
#include "SingleBufferedStream.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

bool SingleBufferedStreamTest::TestDefaultConstructor() {
    DummySingleBufferedStream stream;
    bool ok = (stream.GetTimeout() == TTInfiniteWait);
    ok &= (stream.GetBufferSize() == 32u);
    return ok;
}

bool SingleBufferedStreamTest::TestConstructor_Timeout() {
    DummySingleBufferedStream stream(1u);
    bool ok = (stream.GetTimeout() == 1);
    ok &= (stream.GetBufferSize() == 32u);
    return ok;
}

bool SingleBufferedStreamTest::TestSetBufferSize(uint32 bufferSize) {
    DummySingleBufferedStream stream;
    stream.SetBufferSize(bufferSize);
    if (bufferSize < 8) {
        bufferSize = 8;
    }
    return (stream.GetBufferSize() == bufferSize);
}

bool SingleBufferedStreamTest::TestRead(uint32 bufferSize,
                                        uint32 readSize) {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(bufferSize);

    bool ok = true;
    //Do the test twice to force switching from read to write mode and vice-versa
    uint32 n = 0;
    while (n < 2) {
        stream.Seek(0);
        uint32 size = readSize;
        char8 *bufferWrite = (char8 *) malloc(readSize);
        uint32 i = 0;
        for (i = 0; i < readSize; i++) {
            bufferWrite[i] = i * i;
        }

        uint32 start = 0;
        while ((size > 0) && (stream.Write(bufferWrite + start, size))) {
            start += size;
            if (size > 0) {
                size = (readSize - start);
            }
        }
        ok &= (stream.Position() == readSize);
        stream.Seek(0);

        char8 *bufferRead = (char8 *) malloc(readSize);
        size = readSize;

        start = 0;
        while ((size > 0) && (stream.Read(bufferRead + start, size))) {
            start += size;
            if (size > 0) {
                size = (readSize - start);
            }
        }

        bufferRead[readSize - 1] = '\0';
        bufferWrite[readSize - 1] = '\0';
        ok &= (StringHelper::Compare(bufferRead, bufferWrite) == 0);
        ok &= (stream.Position() == readSize);

        free(bufferRead);
        free(bufferWrite);
        n++;
    }
    return ok;
}

bool SingleBufferedStreamTest::TestWrite(uint32 bufferSize,
                                         uint32 writeSize) {
    return TestRead(bufferSize, writeSize);
}

bool SingleBufferedStreamTest::TestWrite_OverflowInternalBuffer(uint32 bufferSize,
                                                                uint32 writeSize) {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(bufferSize);

    char8 *bufferWrite = (char8 *) malloc(writeSize);
    uint32 i = 0;
    for (i = 0; i < writeSize; i++) {
        bufferWrite[i] = i * i;
    }

    uint32 start = 0;
    uint32 size = bufferSize / 4 - 1;
    while ((size > 0) && (stream.Write(bufferWrite + start, size))) {
        start += size;
        if (size > 0) {
            size = bufferSize / 4 - 1;
        }
        if ((start + size) > writeSize) {
            size = writeSize - start;
        }
    }

    bool ok = (stream.Position() == writeSize);
    stream.Seek(0);
    char8 *bufferRead = (char8 *) malloc(writeSize);
    size = writeSize;
    start = 0;
    while ((size > 0) && (stream.Read(bufferRead + start, size))) {
        start += size;
        if (size > 0) {
            size = (writeSize - start);
        }
    }

    bufferRead[writeSize - 1] = '\0';
    bufferWrite[writeSize - 1] = '\0';
    ok &= (StringHelper::Compare(bufferRead, bufferWrite) == 0);
    ok &= (stream.Position() == writeSize);

    free(bufferRead);
    free(bufferWrite);
    return ok;
}

bool SingleBufferedStreamTest::TestSize(uint32 size) {
    DummySingleBufferedStream stream(true);
    bool ok = stream.SetSize(size);
    return ok && (size == stream.Size());
}

bool SingleBufferedStreamTest::TestSeek() {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(64);

    uint32 size = 32;
    char buffer[size];
    uint32 i = 0;
    for (i = 0; i < size; i++) {
        buffer[i] = i;
    }
    bool ok = stream.Write(buffer, size);
    for (i = 0; i < size; i++) {
        buffer[i] = 0;
    }

    uint32 seekAmount;
    for (seekAmount = 0; seekAmount < 30; seekAmount += 3) {
        ok &= stream.Seek(seekAmount);
        uint32 readSize = size - seekAmount;
        ok &= stream.Read(buffer, readSize);
        for (i = 0; i < readSize; i++) {
            ok &= (buffer[i] == static_cast<char>((i + seekAmount)));
        }
    }
    return ok;
}

bool SingleBufferedStreamTest::TestRelativeSeek() {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(64);

    uint32 size = 32;
    char buffer[size];
    uint32 i = 0;
    for (i = 0; i < size; i++) {
        buffer[i] = i;
    }
    bool ok = stream.Write(buffer, size);
    for (i = 0; i < size; i++) {
        buffer[i] = 0;
    }

    uint32 seekStart = 5;
    uint32 seekAmount;
    for (seekAmount = 1; seekAmount < 25; seekAmount++) {
        ok &= stream.Seek(seekStart);
        ok &= stream.RelativeSeek(seekAmount);
        uint32 readSize = size - seekAmount - seekStart;
        ok &= stream.Read(buffer, readSize);
        for (i = 0; i < readSize; i++) {
            ok &= (buffer[i] == static_cast<char>((i + seekAmount + seekStart)));
        }
    }
    return ok;
}

bool SingleBufferedStreamTest::TestPosition() {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(64);
    uint32 i = 0;
    bool ok = true;
    for (i = 0; i < 32; i++) {
        stream.Seek(i);
        ok &= (stream.Position() == i);
    }

    return ok;
}

bool SingleBufferedStreamTest::TestSetSize(uint32 size) {
    DummySingleBufferedStream stream(true);
    bool ok = stream.SetSize(size);
    return ok && (size == stream.Size());
}

bool SingleBufferedStreamTest::TestFlushAndResync(uint32 bufferSize) {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(bufferSize);
    stream.Clear();
    stream.Printf("%d", 8);
    bool ok = (StringHelper::Compare("8", stream.Buffer()) != 0);
    ok &= stream.FlushAndResync();
    ok &= (StringHelper::Compare("8", stream.Buffer()) == 0);
    return ok;
}

bool SingleBufferedStreamTest::TestRead_NotCanRead() {
    DummySingleBufferedStream stream(true, false, true);
    uint32 size = 8;
    char buffer[size];
    return !stream.Read(buffer, size);
}

bool SingleBufferedStreamTest::TestWrite_NotCanWrite() {
    DummySingleBufferedStream stream(true, true, false);
    uint32 size = 8;
    char buffer[size];
    return !stream.Write(buffer, size);
}

bool SingleBufferedStreamTest::TestRelativeSeek_OverflowInternalBuffer(uint32 bufferSize,
                                                                       uint32 writeSize) {
    DummySingleBufferedStream stream(true);
    stream.SetBufferSize(bufferSize);

    char8 *bufferWrite = (char8 *) malloc(writeSize);
    uint32 i = 0;
    for (i = 0; i < writeSize; i++) {
        bufferWrite[i] = i * i;
    }

    uint32 start = 0;
    uint32 size = bufferSize / 4 - 1;
    while ((size > 0) && (stream.Write(bufferWrite + start, size))) {
        start += size;
        if (size > 0) {
            size = bufferSize / 4 - 1;
        }
        if ((start + size) > writeSize) {
            size = writeSize - start;
        }
        stream.RelativeSeek(1);
        stream.RelativeSeek(-1);
    }

    free(bufferWrite);
    return (stream.Position() == writeSize);
}
