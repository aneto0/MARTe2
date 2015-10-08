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
        BufferedStream(),
        readBuffer(),
        writeBuffer() {
    unbufferedStream = static_cast<RawStream *>(NULL);
    operatingModes.mutexReadMode = false;
    operatingModes.mutexWriteMode = false;
    timeout = TTDefault;
}

SingleBufferedStream::SingleBufferedStream(RawStream* const lowLevelStream) :
        BufferedStream(),
        readBuffer(lowLevelStream),
        writeBuffer(lowLevelStream) {
    unbufferedStream = lowLevelStream;
    operatingModes.mutexReadMode = false;
    operatingModes.mutexWriteMode = false;
    timeout = TTDefault;
}

SingleBufferedStream::SingleBufferedStream(RawStream* const lowLevelStream,
                                           const TimeoutType &msecTimeout) :
        BufferedStream(),
        readBuffer(lowLevelStream, msecTimeout),
        writeBuffer(lowLevelStream, msecTimeout) {
    unbufferedStream = lowLevelStream;
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

    bool ret = true;
    // mutex mode is enabled if CanSeek and both can Read and Write
// in that case the stream is single and bidirectional
    bool canSeek = CanSeek();
    bool canRead = CanRead();
    bool canWrite = CanWrite();
    if (canSeek && canWrite && canRead) {
        operatingModes.mutexWriteMode = true;
        operatingModes.mutexReadMode = false;
    }

    // minimum size = 8
    if (readBufferSize < 8u) {
        readBufferSize = 8u;
    }
    if (writeBufferSize < 8u) {
        writeBufferSize = 8u;
    }

    // do not allocate if not necessary
    if (!CanRead()) {
        readBufferSize = 0u;
    }
    if (!CanWrite()) {
        writeBufferSize = 0u;
    }

    // dump any data in the write Queue
    if (FlushAndResync()) {

        // adjust readBufferSize
        if (!readBuffer.SetBufferSize(readBufferSize)) {
            //TODO
        }

        // adjust writeBufferSize
        if (!writeBuffer.SetBufferSize(writeBufferSize)) {
            //TODO
        }

    }
    else {
        ret = false;
    }
    return ret;
}

///
IOBuffer *SingleBufferedStream::GetInputBuffer() {
    IOBuffer *ret = &readBuffer;
    if (operatingModes.mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = static_cast<IOBuffer *>(NULL);
        }
    }

    return ret;
}

///
IOBuffer *SingleBufferedStream::GetOutputBuffer() {
    IOBuffer *ret = &writeBuffer;

    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (operatingModes.mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = static_cast<IOBuffer *>(NULL);
        }
    }
    return ret;
}

bool SingleBufferedStream::Read(char8 * const bufferIn,
                                uint32 & size) {

    bool ret = true;

    // check for mutually exclusive buffering and
    // whether one needs to switch to ReadMode
    if (operatingModes.mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = false;
        }
    }
    if (ret) {

        // check whether we have a buffer
        if (readBuffer.BufferSize() > 0u) {

            // read from buffer first
            uint32 toRead = size;

            // try once
            readBuffer.Read(&bufferIn[0], size);

            if (size != toRead) {
                // partial only so continue

                // adjust toRead
                toRead -= size;

                // decide whether to use the buffer again or just to read directly
                if ((toRead * 4u) < readBuffer.MaxUsableAmount()) {
                    if (!readBuffer.Refill()) {
                        ret = false;
                    }

                    else {

                        readBuffer.Read(&bufferIn[size], toRead);
                        size += toRead;

                        // should have completed
                        // as our buffer is at least 4x the need
                    }

                }
                else {
                    // if needed read directly from stream
                    if (!unbufferedStream->Read(&bufferIn[size], toRead, timeout)) {
                        ret = false;
                    }
                    else {
                        size += toRead;
                    }
                }
            }
        }
    }

    // if needed read directly from stream
    return (ret) ? (unbufferedStream->Read(&bufferIn[0], size, timeout)) : (false);
}

/** Write data from a buffer to the stream. As much as size byte are written, actual size
 is returned in size. msecTimeout is how much the operation should last.
 timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
 when noWait is used .... */
bool SingleBufferedStream::Write(const char8 * const bufferIn,
                                 uint32 & size) {

    bool ret = true;
    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (operatingModes.mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = false;
        }
    }
    if (ret) {
        // buffering active?
        if (writeBuffer.BufferSize() > 0u) {
            // separate input and output size

            uint32 toWrite = size;
            // check available buffer size versus write size
            // if size is comparable to buffer size there
            // is no reason to use the buffering mechanism
            if (writeBuffer.MaxUsableAmount() > (4u * size)) {

                // try writing the buffer
                writeBuffer.Write(&bufferIn[0], size);

                // all done! space available!
                if (size != toWrite) {
                    // make space
                    if (!writeBuffer.Flush()) {
                        ret = false;
                    }
                    else {
                        toWrite -= size;
                        uint32 leftToWrite = toWrite;

                        // try writing the buffer
                        writeBuffer.Write(&bufferIn[size], leftToWrite);

                        size += leftToWrite;

                        // should have been able to fill in it!!!
                        if (leftToWrite != toWrite) {
                            ret = false;
                        }
                    }
                }
            }
            else {
                // write the buffer so far
                if (!writeBuffer.Flush()) {
                    ret = false;
                }
            }

        }
    }
    return (ret) ? (unbufferedStream->Write(&bufferIn[0], size, timeout)) : (false);

}

/** The size of the stream */
uint64 SingleBufferedStream::Size() {
// just commit all pending changes if any
// so stream size will be updated
    if (!FlushAndResync()) {
        //TODO
    }
// then call Size from unbuffered stream
    return unbufferedStream->Size();
}

/** Moves within the file to an absolute location */
bool SingleBufferedStream::Seek(const uint64 pos) {

    bool ubSeek = true;
    // if write mode on then just flush out data
    // then seek the stream
    if (writeBuffer.UsedSize() > 0u) {
        if (!writeBuffer.Flush()) {
            //TODO
        }
    }
    else {
        // if read buffer has some data, check whether seek can be within buffer
        if (readBuffer.UsedSize() > 0u) {
            uint64 currentStreamPosition = unbufferedStream->Position();
            uint64 bufferStartPosition = currentStreamPosition - readBuffer.UsedSize();

            // if within range just update readBufferAccessPosition
            if ((pos >= bufferStartPosition) && (pos < currentStreamPosition)) {
                if (!readBuffer.Seek(static_cast<uint32>(pos - bufferStartPosition))) {
                    //TODO
                }

                ubSeek = false;
            }
            else { // otherwise mark read buffer empty and proceed with normal seek
                readBuffer.Empty();
                // continues at the end of the function
            }
        }
    }

    return (ubSeek) ? (unbufferedStream->Seek(pos)) : (true);
}

/** Moves within the file relative to current location */
bool SingleBufferedStream::RelativeSeek(int32 deltaPos) {
    bool ubSeek = false;

    if (deltaPos != 0) {

        ubSeek = true;
        // if write mode on then just flush out data
        if (writeBuffer.UsedSize() > 0u) {
            // this will move the stream pointer ahead to the correct position
            if (!writeBuffer.Flush()) {
                //TODO
            }
        }
        else {

            //save the current position because in case of out of range
            //the position becomes one of the buffer bounds.
            uint64 currentPos = readBuffer.Position();

            // on success it means we are in range
            if (readBuffer.RelativeSeek(deltaPos)) {
                // no need to move stream pointer
                ubSeek = false;
            }
            if (ubSeek) {
                // out of buffer range
                // adjust stream seek position to account for actual read buffer usage

                /*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: If the number is not in the signed range a warning will be generated*/
                int32 gap = static_cast<int32>(readBuffer.UsedSize() - currentPos);

                // if gap is not in the signed range generate a warning
                if (gap < 0) {
                    //TODO
                }
                deltaPos -= gap;
                // empty buffer
                readBuffer.Empty();
            }

        }
    }

    // seek
    /*lint -e{9117} -e{737} [MISRA C++ Rule 5-0-4]. The input value is always positive so the signed does not change. */
    return (ubSeek) ? (unbufferedStream->Seek(static_cast<uint64>(unbufferedStream->Position() + deltaPos))) : (true);
}

/** Returns current position */
uint64 SingleBufferedStream::Position() {

    uint64 ret = 0u;
    // if write mode on then just flush out data
    if (writeBuffer.UsedSize() > 0u) {
        ret = unbufferedStream->Position() + writeBuffer.Position();
    }
    else {

        ret = (unbufferedStream->Position() - readBuffer.UsedSize()) + readBuffer.Position();
    }
    return ret;
}

/** Clip the stream size to a specified point */
bool SingleBufferedStream::SetSize(const uint64 size) {

    // commit all changes
    if (!FlushAndResync()) {
        //TODO
    }

    return unbufferedStream->SetSize(size);
}

}

