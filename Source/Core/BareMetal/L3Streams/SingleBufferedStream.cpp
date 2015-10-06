/**
 * @file SingleBufferedStream.cpp
 * @brief Source file for class SingleBufferedStream
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
 * the class SingleBufferedStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SingleBufferedStream.h"
#include "ErrorManagement.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

SingleBufferedStream::SingleBufferedStream() :
        readBuffer(unbufferedStream),
        writeBuffer(unbufferedStream) {
    operatingModes.mutexReadMode = false;
    operatingModes.mutexWriteMode = false;
    timeout = TTDefault;
}

SingleBufferedStream::SingleBufferedStream(TimeoutType msecTimeout) :
        readBuffer(unbufferedStream, msecTimeout),
        writeBuffer(unbufferedStream, msecTimeout) {
    operatingModes.mutexReadMode = false;
    operatingModes.mutexWriteMode = false;
    timeout = msecTimeout;
}

/// default destructor
SingleBufferedStream::~SingleBufferedStream() {

//writeBuffer.Flush();
}

bool SingleBufferedStream::CanSeek() const {
    return unbufferedStream->CanSeek();
}

/** whether it can be written into */
bool SingleBufferedStream::CanWrite() const {
    return unbufferedStream->CanWrite();
}

/** whether it can be  read */
bool SingleBufferedStream::CanRead() const {
    return unbufferedStream->CanRead();
}

bool SingleBufferedStream::SetBufferSize(uint32 readBufferSize,
                                         uint32 writeBufferSize) {

    // mutex mode is enabled if CanSeek and both can Read and Write
// in that case the stream is single and bidirectional
    if (CanSeek() && CanWrite() && CanRead()) {
        operatingModes.mutexWriteMode = true;
        operatingModes.mutexReadMode = false;
    }

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
    if (!FlushAndResync())
        return false;

    // adjust readBufferSize
    readBuffer.SetBufferSize(readBufferSize);

    // adjust writeBufferSize
    writeBuffer.SetBufferSize(writeBufferSize);

    return true;
}

///
IOBuffer *SingleBufferedStream::GetInputBuffer() {
    if (operatingModes.mutexWriteMode) {
        if (!SwitchToReadMode())
            return NULL;
        }
        return &readBuffer;
    }

///
IOBuffer *SingleBufferedStream::GetOutputBuffer() {
    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (operatingModes.mutexReadMode) {
        if (!SwitchToWriteMode())
            return NULL;
        }

        return &writeBuffer;
    }

bool SingleBufferedStream::Read(char8 * bufferIn,
                                uint32 & size) {
    // check for mutually exclusive buffering and
    // whether one needs to switch to ReadMode
    if (operatingModes.mutexWriteMode) {
        if (!SwitchToReadMode())
            return false;
    }

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
bool SingleBufferedStream::Write(const char8* bufferIn,
                                 uint32 & size) {

    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (operatingModes.mutexReadMode) {
        if (!SwitchToWriteMode())
            return false;
    }

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
uint64 SingleBufferedStream::Size() {
// just commit all pending changes if any
// so stream size will be updated
    FlushAndResync();
// then call Size from unbuffered stream
    return unbufferedStream->Size();
}

/** Moves within the file to an absolute location */
bool SingleBufferedStream::Seek(uint64 pos) {

    // if write mode on then just flush out data
    // then seek the stream
    if (writeBuffer.UsedSize() > 0) {
        writeBuffer.Flush();
    }
    else {
        // if read buffer has some data, check whether seek can be within buffer
        if (readBuffer.UsedSize() > 0) {
            int64 currentStreamPosition = unbufferedStream->Position();
            int64 bufferStartPosition = currentStreamPosition - readBuffer.UsedSize();

            // if within range just update readBufferAccessPosition
            if ((pos >= bufferStartPosition) && (pos < currentStreamPosition)) {
                readBuffer.Seek(pos - bufferStartPosition);

                return true;
            }
            else { // otherwise mark read buffer empty and proceed with normal seek
                readBuffer.Empty();
                // continues at the end of the function
            }
        }
    }

    return unbufferedStream->Seek(pos);
}

/** Moves within the file relative to current location */
bool SingleBufferedStream::RelativeSeek(int32 deltaPos) {
    if (deltaPos == 0)
        return true;

    // if write mode on then just flush out data
    if (writeBuffer.UsedSize() > 0) {
        // this will move the stream pointer ahead to the correct position
        writeBuffer.Flush();
    }
    else {

//save the current position because in case of out of range
        //the position becomes one of the buffer bounds.
        int64 currentPos = readBuffer.Position();

        // on success it means we are in range
        if (readBuffer.RelativeSeek(deltaPos)) {
            // no need to move stream pointer
            return true;
        }
        // out of buffer range
// adjust stream seek poistion to account for actual read buffer usage
        deltaPos -= (readBuffer.UsedSize() - currentPos);

// empty buffer
        readBuffer.Empty();

    }

    // seek
    return unbufferedStream->Seek(unbufferedStream->Position() + deltaPos);
}

/** Returns current position */
uint64 SingleBufferedStream::Position() {

    // if write mode on then just flush out data
    if (writeBuffer.UsedSize() > 0) {
        return unbufferedStream->Position() + writeBuffer.Position();
    }

    return unbufferedStream->Position() - readBuffer.UsedSize() + readBuffer.Position();
}

/** Clip the stream size to a specified point */
bool SingleBufferedStream::SetSize(uint64 size) {

    // commit all changes
    FlushAndResync();

    return unbufferedStream->SetSize(size);
}

}

