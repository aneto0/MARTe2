/**
 * @file BufferedStreamIOBufferTest.cpp
 * @brief Source file for class BufferedStreamIOBufferTest
 * @date 14/10/2015
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
 * the class BufferedStreamIOBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BufferedStreamIOBufferTest.h"
#include "StreamTestHelper.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool BufferedStreamIOBufferTest::TestDefaultConstructor() {

    BufferedStreamIOBuffer buffered;
    if (buffered.AmountLeft() != 0) {
        return false;
    }

    if (buffered.UsedAmountLeft() != 0) {
        return false;
    }

    if (buffered.MaxUsableAmount() != 0) {
        return false;
    }

    if (buffered.Buffer() != NULL) {
        return false;
    }

    if (buffered.GetStream() != NULL) {
        return false;
    }

    if (buffered.GetTimeout() != TTInfiniteWait) {
        return false;
    }
    return buffered.UndoLevel() == 0;

}

bool BufferedStreamIOBufferTest::TestFullConstructor() {

    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer buffered(&stream);

    if (buffered.AmountLeft() != 0) {
        return false;
    }

    if (buffered.UsedAmountLeft() != 0) {
        return false;
    }

    if (buffered.MaxUsableAmount() != 0) {
        return false;
    }

    if (buffered.Buffer() != NULL) {
        return false;
    }

    if (buffered.GetStream() != (&stream)) {
        return false;
    }

    if (buffered.GetTimeout() != TTInfiniteWait) {
        return false;
    }

    return buffered.UndoLevel() == 0;

}

bool BufferedStreamIOBufferTest::TestGetTimeout() {
    TimeoutType tt = 1;
    BufferedStreamIOBuffer buffered;
    buffered.SetTimeout(tt);
    return (buffered.GetTimeout() == tt);
}

bool BufferedStreamIOBufferTest::TestSetTimeout() {
    TimeoutType tt = 1;
    BufferedStreamIOBuffer buffered;
    buffered.SetTimeout(tt);
    return (buffered.GetTimeout() == tt);
}

bool BufferedStreamIOBufferTest::TestRefill() {

    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer *buffered=stream.GetReadBufferX();
    uint32 bufferSize = 10;
    buffered->SetBufferSize(bufferSize);


    const char8* toWrite = "HelloWorldThisIsATest";

    uint32 writeSize = StringHelper::Length(toWrite);
    buffered->Flush();
    buffered->WriteAll(toWrite, writeSize);

    stream.Seek(0);

    //try to trick with a non empty buffer
    const char8 *trick = "trick";
    uint32 trickSize = StringHelper::Length(trick);
    buffered->Write(trick, trickSize);

    buffered->Refill();

    if (buffered->UsedAmountLeft() != buffered->MaxUsableAmount()) {
        return false;
    }

    return StringHelper::CompareN(buffered->Buffer(), toWrite+trickSize, bufferSize) == 0;
}

bool BufferedStreamIOBufferTest::TestRefill_NULL_Stream() {
    BufferedStreamIOBuffer buffered;

    uint32 bufferSize = 10;
    buffered.SetBufferSize(bufferSize);

    return !buffered.Refill();
}

bool BufferedStreamIOBufferTest::TestRefill_NULL_Buffer() {

    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer *buffered=stream.GetReadBufferX();
    buffered->SetBufferSize(0);

    return !buffered->Refill();
}

bool BufferedStreamIOBufferTest::TestFlush() {
    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer *buffered=stream.GetWriteBufferX();

    uint32 bufferSize = 10;
    buffered->SetBufferSize(bufferSize);

    const char8* toWrite = "Hello";
    uint32 writeSize = StringHelper::Length(toWrite);

    buffered->Write(toWrite, writeSize);

    buffered->Flush();
    //the buffer now is empty
    if ((buffered->Position() != 0) || (buffered->UsedSize() != 0)) {
        return false;
    }

    return StringHelper::Compare(stream.Buffer(), toWrite) == 0;
}

bool BufferedStreamIOBufferTest::TestFlush_NULL_Stream() {
    BufferedStreamIOBuffer buffered;

    uint32 bufferSize = 10;
    buffered.SetBufferSize(bufferSize);

    return !buffered.Flush();
}

bool BufferedStreamIOBufferTest::TestFlush_NULL_Buffer() {
    DummySingleBufferedStream stream;

    BufferedStreamIOBuffer *buffered=stream.GetWriteBufferX();
    buffered->SetBufferSize(0);

    return !buffered->Flush();
}

bool BufferedStreamIOBufferTest::TestResync() {

    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer *buffered=stream.GetWriteBufferX();

    buffered->Flush();
    uint32 bufferSize = 10;
    buffered->SetBufferSize(bufferSize);

    const char8* toWrite = "HelloWorldThisIsATest";

    uint32 writeSize = StringHelper::Length(toWrite);

    buffered->WriteAll(toWrite, writeSize);

    buffered->Flush();

    stream.Seek(0);

    //something is written on the stream.

    char8 bufferOut[32];
    uint32 toRead = 5;
    // the buffer is refilled until 10 chars
    buffered->Refill();
    buffered->Read(bufferOut, toRead);

    if (stream.OSPosition() != bufferSize) {
        return false;
    }

    buffered->Resync();

    return stream.OSPosition() == toRead;
}

bool BufferedStreamIOBufferTest::TestResync_NULL_Stream() {
    BufferedStreamIOBuffer buffered;

    uint32 bufferSize = 10;
    buffered.SetBufferSize(bufferSize);

    return !buffered.Resync();
}

bool BufferedStreamIOBufferTest::TestResync_NULL_Buffer() {

    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer buffered(&stream);

    return buffered.Resync();
}

bool BufferedStreamIOBufferTest::TestSetBufferSize(uint32 size) {
    BufferedStreamIOBuffer buffered;

    if (buffered.GetBufferSize() != 0) {
        return false;
    }

    if ((!buffered.SetBufferSize(size)) && (size != 0)) {
        return false;
    }

    return (buffered.GetBufferSize() == size) && (buffered.MaxUsableAmount() == (buffered.GetBufferSize()));
}

bool BufferedStreamIOBufferTest::TestGetStream() {
    BufferedStreamIOBuffer buffered;

    if (buffered.GetStream() != NULL) {
        return false;
    }
    DummySingleBufferedStream stream;
    BufferedStreamIOBuffer buffer2(&stream);
    return buffered.GetStream() == (&stream);

}
