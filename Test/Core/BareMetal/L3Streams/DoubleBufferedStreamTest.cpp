/**
 * @file DoubleBufferedStreamTest.cpp
 * @brief Source file for class DoubleBufferedStreamTest
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
 * the class DoubleBufferedStreamTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DoubleBufferedStreamTest.h"
#include "StreamI.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

bool DoubleBufferedStreamTest::TestDefaultConstructor() {
    DummyDoubleBufferedStream stream;
    bool ok = (stream.GetTimeout() == TTInfiniteWait);
    ok &= (stream.GetReadBufferSize() == 32u);
    ok &= (stream.GetWriteBufferSize() == 32u);
    return ok;
}

bool DoubleBufferedStreamTest::TestConstructor_Timeout() {
    DummyDoubleBufferedStream stream(1u);
    bool ok = (stream.GetTimeout() == 1);
    ok &= (stream.GetReadBufferSize() == 32u);
    ok &= (stream.GetWriteBufferSize() == 32u);
    return ok;
}

bool DoubleBufferedStreamTest::TestSetBufferSize(uint32 bufferSize) {
    DummyDoubleBufferedStream stream;
    stream.SetBufferSize(bufferSize, bufferSize);
    if (bufferSize < 8) {
        bufferSize = 8;
    }
    bool ok = (stream.GetReadBufferSize() == bufferSize);
    ok &= (stream.GetWriteBufferSize() == bufferSize);
    return ok;
}

bool DoubleBufferedStreamTest::TestRead(uint32 bufferSize,
                                        uint32 readSize) {
    DummyDoubleBufferedStream stream(true);
    stream.SetBufferSize(bufferSize, bufferSize);

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
            size = (readSize - size);
        }
    }
    bool ok = (stream.Position() == readSize);
    stream.Seek(0);

    char8 *bufferRead = (char8 *) malloc(readSize);
    size = readSize;

    start = 0;
    while ((size > 0) && (stream.Read(bufferRead + start, size))) {
        start += size;
        if (size > 0) {
            size = (readSize - size);
        }
    }

    bufferRead[readSize - 1] = '\0';
    bufferWrite[readSize - 1] = '\0';
    ok &= (StringHelper::Compare(bufferRead, bufferWrite) == 0);
    ok &= (stream.Position() == readSize);

    free(bufferRead);
    free(bufferWrite);
    return ok;
}

bool DoubleBufferedStreamTest::TestWrite(uint32 bufferSize,
                                         uint32 readSize) {
    return TestRead(bufferSize, readSize);
}

bool DoubleBufferedStreamTest::TestSize(uint32 size) {
    DummyDoubleBufferedStream stream(true);
    bool ok = stream.SetSize(size);
    return ok && (size == stream.Size());
}

bool DoubleBufferedStreamTest::TestSeek() {
    DummyDoubleBufferedStream stream(true);
    stream.SetBufferSize(64, 64);

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
            ok &= static_cast<char>((buffer[i] == (i + seekAmount)));
        }
    }
    return ok;
}

bool DoubleBufferedStreamTest::TestRelativeSeek() {
    DummyDoubleBufferedStream stream(true);
    stream.SetBufferSize(64, 64);

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
    uint32 seekAmount = 0;
    for (seekAmount = 0; seekAmount < 25; seekAmount++) {
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

bool DoubleBufferedStreamTest::TestPosition() {
    DummyDoubleBufferedStream stream(true);
    stream.SetBufferSize(64, 64);
    uint32 i = 0;
    bool ok = true;
    for (i = 0; i < 32; i++) {
        stream.Seek(i);
        ok &= (stream.Position() == i);
    }

    return ok;
}

bool DoubleBufferedStreamTest::TestSetSize(uint32 size) {
    DummyDoubleBufferedStream stream(true);
    bool ok = stream.SetSize(size);
    return ok && (size == stream.Size());
}

bool DoubleBufferedStreamTest::TestFlush(uint32 bufferSize) {
    DummyDoubleBufferedStream stream(true);
    stream.SetBufferSize(bufferSize, bufferSize);
    stream.Clear();
    stream.Printf("%d", 8);
    bool ok = (StringHelper::Compare("8", stream.Buffer()) != 0);
    ok &= stream.Flush();
    ok &= (StringHelper::Compare("8", stream.Buffer()) == 0);
    return ok;
}
