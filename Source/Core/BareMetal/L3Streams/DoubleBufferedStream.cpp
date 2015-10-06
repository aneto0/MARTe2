/**
 * @file DoubleBufferedStream.cpp
 * @brief Source file for class DoubleBufferedStream
 * @date 06/10/2015
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
 * the class DoubleBufferedStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DoubleBufferedStream.h"
#include "ErrorManagement.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DoubleBufferedStream::DoubleBufferedStream() :
        readBuffer(unbufferedStream),
        writeBuffer(unbufferedStream) {
    timeout = TTDefault;
}

DoubleBufferedStream::DoubleBufferedStream(TimeoutType msecTimeout) :
        readBuffer(unbufferedStream, msecTimeout),
        writeBuffer(unbufferedStream, msecTimeout) {
    timeout = msecTimeout;
}

/// default destructor
DoubleBufferedStream::~DoubleBufferedStream() {

//writeBuffer.Flush();
}

bool DoubleBufferedStream::CanSeek() const {
    return unbufferedStream->CanSeek();
}

/** whether it can be written into */
bool DoubleBufferedStream::CanWrite() const {
    return unbufferedStream->CanWrite();
}

/** whether it can be  read */
bool DoubleBufferedStream::CanRead() const {
    return unbufferedStream->CanRead();
}

bool DoubleBufferedStream::SetBufferSize(uint32 readBufferSize,
                                         uint32 writeBufferSize) {

    // minimum size = 8
    if (readBufferSize < 8)
        readBufferSize = 8;
    if (writeBufferSize < 8)
        writeBufferSize = 8;

    // do not allocate if not necessary
    if (!CanRead())
        readBufferSize = 0;
    if (!CanWrite())
        writeBufferSize = 0;

    // dump any data in the write Queue
    if (!Flush())
        return false;

    // adjust readBufferSize
    readBuffer.SetBufferSize(readBufferSize);

    // adjust writeBufferSize
    writeBuffer.SetBufferSize(writeBufferSize);

    return true;
}

///
IOBuffer *DoubleBufferedStream::GetInputBuffer() {

    return &readBuffer;
}

///
IOBuffer *DoubleBufferedStream::GetOutputBuffer() {

    return &writeBuffer;
}

bool DoubleBufferedStream::Read(char8 * bufferIn,
                                uint32 & size) {

    // check whether we have a buffer
    if (readBuffer.BufferSize() > 0) {

        // read from buffer first
        uint32 toRead = size;

        // try once
        readBuffer.Read(bufferIn, size);

        if (size == toRead) {
            return true;
        }
        else {  // partial only so continue

            // adjust toRead
            toRead -= size;

            // decide whether to use the buffer again or just to read directly
            if ((toRead * 4) < readBuffer.MaxUsableAmount()) {
                if (!readBuffer.Refill())
                    return false;

                readBuffer.Read(bufferIn + size, toRead);
                size += toRead;

                // should have completed
                // as our buffer is at least 4x the need
                return true;

            }
            else {
                // if needed read directly from stream
                if (!unbufferedStream->Read(bufferIn + size, toRead, timeout))
                    return false;
                size += toRead;
                return true;
            }
        }
    }

    // if needed read directly from stream
    return unbufferedStream->Read(bufferIn, size, timeout);
}

/** Write data from a buffer to the stream. As much as size byte are written, actual size
 is returned in size. msecTimeout is how much the operation should last.
 timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
 when noWait is used .... */
bool DoubleBufferedStream::Write(const char8* bufferIn,
                                 uint32 & size) {

    // buffering active?
    if (writeBuffer.BufferSize() > 0) {
        // separate input and output size

        uint32 toWrite = size;
        // check available buffer size versus write size
        // if size is comparable to buffer size there
        // is no reason to use the buffering mechanism
        if (writeBuffer.MaxUsableAmount() > (4 * size)) {

            // try writing the buffer
            writeBuffer.Write(bufferIn, size);

            // all done! space available!
            if (size == toWrite)
                return true;

            // make space
            if (!writeBuffer.Flush())
                return false;

            toWrite -= size;
            uint32 leftToWrite = toWrite;

            // try writing the buffer
            writeBuffer.Write(bufferIn + size, leftToWrite);

            size += leftToWrite;

            // should have been able to fill in it!!!
            if (leftToWrite != toWrite)
                return false;
            return true;
        }
        else {
            // write the buffer so far
            if (!writeBuffer.Flush())
                return false;
        }

    }
    return unbufferedStream->Write(bufferIn, size, timeout);

}

/** The size of the stream */
uint64 DoubleBufferedStream::Size() {
    return unbufferedStream->Size();
}

/** Moves within the file to an absolute location */
bool DoubleBufferedStream::Seek(uint64 pos) {
    return unbufferedStream->RelativeSeek(pos);
}

/** Moves within the file relative to current location */
bool DoubleBufferedStream::RelativeSeek(int32 deltaPos) {
    return unbufferedStream->RelativeSeek(deltaPos);
}

/** Returns current position */
uint64 DoubleBufferedStream::Position() {
    return unbufferedStream->Position();
}

/** Clip the stream size to a specified point */
bool DoubleBufferedStream::SetSize(uint64 size) {
    return unbufferedStream->SetSize(size);
}

}

