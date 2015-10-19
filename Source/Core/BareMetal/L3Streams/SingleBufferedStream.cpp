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
        StreamI(),
        internalBuffer(this) {
    mutexReadMode = true;
    mutexWriteMode = false;
    if (!internalBuffer.SetBufferSize(32u)) {
        REPORT_ERROR(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
    }
}

SingleBufferedStream::SingleBufferedStream(const TimeoutType &timeoutIn) :
        StreamI(),
        internalBuffer(this) {
    mutexReadMode = true;
    mutexWriteMode = false;
    SetTimeout(timeoutIn);
    if (!internalBuffer.SetBufferSize(32u)) {
        REPORT_ERROR(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
    }
}

SingleBufferedStream::~SingleBufferedStream() {
}

bool SingleBufferedStream::SetBufferSize(uint32 bufferSize) {

    bool ret = true;
    // mutex mode is enabled if CanSeek and both can Read and Write
// in that case the stream is single and bidirectional
    bool canSeek = CanSeek();
    bool canRead = CanRead();
    bool canWrite = CanWrite();
    if (canSeek && canWrite && canRead) {
        mutexWriteMode = true;
        mutexReadMode = false;
    }

    // minimum size = 8
    if (bufferSize < 8u) {
        bufferSize = 8u;
    }

    // dump any data in the write Queue
    if (FlushAndResync()) {
        if (!internalBuffer.SetBufferSize(bufferSize)) {
            ret = false;
        }
    }
    else {
        ret = false;
    }
    return ret;
}

uint32 SingleBufferedStream::GetBufferSize() const {
    return internalBuffer.GetBufferSize();
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer *SingleBufferedStream::GetInputBuffer() {
    IOBuffer *ret = &internalBuffer;
    if (mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = static_cast<IOBuffer *>(NULL);
        }
    }

    return ret;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer *SingleBufferedStream::GetOutputBuffer() {
    IOBuffer *ret = &internalBuffer;

    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = static_cast<IOBuffer *>(NULL);
        }
    }
    return ret;
}

bool SingleBufferedStream::Read(char8 * const output,
                                uint32 & size) {

    bool ret = true;

    // check for mutually exclusive buffering and
    // whether one needs to switch to ReadMode
    if (mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = false;
        }
    }
    if (ret) {
        // check whether we have a buffer
        if (internalBuffer.GetBufferSize() > 0u) {

            // read from buffer first
            uint32 toRead = size;

            // try once
            if (!internalBuffer.Read(&output[0], size)) {
                ret = false;
            }

            if (ret && (size != toRead)) {
                // partial only so continue

                // adjust toRead
                toRead -= size;

                // decide whether to use the buffer again or just to read directly
                if ((toRead * 4u) < internalBuffer.MaxUsableAmount()) {
                    if (!internalBuffer.Refill()) {
                        ret = false;
                    }

                    else {

                        if (!internalBuffer.Read(&output[size], toRead)) {
                            ret = false;
                        }
                        size += toRead;

                        // should have completed
                        // as our buffer is at least 4x the need
                    }

                }
                else {
                    // if needed read directly from stream
                    if (!UnbufferedRead(&output[size], toRead)) {
                        ret = false;
                    }
                    else {
                        size += toRead;
                    }
                }
            }
        }
        else {
            // if needed read directly from stream
            ret = UnbufferedRead(&output[0], size);
        }

    }

    return ret;
}

bool SingleBufferedStream::Write(const char8 * const input,
                                 uint32 & size) {

    bool ret = true;
    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = false;
        }
    }
    if (ret) {

        // buffering active?
        if (internalBuffer.GetBufferSize() > 0u) {
            // separate input and output size

            uint32 toWrite = size;
            // check available buffer size versus write size
            // if size is comparable to buffer size there
            // is no reason to use the buffering mechanism
            if (internalBuffer.MaxUsableAmount() > (4u * size)) {

                // try writing the buffer
                if (!internalBuffer.Write(&input[0], size)) {
                    ret = false;
                }

                // all done! space available!
                if (ret && (size != toWrite)) {
                    // make space
                    if (!internalBuffer.Flush()) {
                        ret = false;
                    }
                    else {
                        toWrite -= size;
                        uint32 leftToWrite = toWrite;

                        // try writing the buffer
                        if (!internalBuffer.Write(&input[size], leftToWrite)) {
                            ret = false;
                        }

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
                if (!internalBuffer.Flush()) {
                    ret = false;
                }
                else {
                    ret = UnbufferedWrite(&input[0], size);
                }
            }

        }
        else {
            ret = UnbufferedWrite(&input[0], size);
        }

    }
    return ret;

}

uint64 SingleBufferedStream::Size() {
// commit all pending changes if any
// so stream size will be updated
    bool ok = true;
    if (!FlushAndResync()) {
        ok = false;
    }
    uint64 size = 0u;
    if (ok) {
        size = UnbufferedSize();
    }
    return size;
}

bool SingleBufferedStream::Seek(const uint64 pos) {

    bool ubSeek = true;
    // if write mode on then just flush out data
    // then seek the stream
    if (mutexWriteMode) {
        if (internalBuffer.UsedSize() > 0u) {
            if (!internalBuffer.Flush()) {
                ubSeek = false;
            }
        }
    }
    if (mutexReadMode) {
        // if read buffer has some data, check whether seek can be within buffer
        if (internalBuffer.UsedSize() > 0u) {
            uint64 currentStreamPosition = UnbufferedPosition();
            uint64 bufferStartPosition = currentStreamPosition - internalBuffer.UsedSize();

            // if within range just update readBufferAccessPosition
            if ((pos >= bufferStartPosition) && (pos < currentStreamPosition)) {
                if (!internalBuffer.Seek(static_cast<uint32>(pos - bufferStartPosition))) {
                    REPORT_ERROR(ErrorManagement::FatalError, "SingleBufferedStream: Failed to seek the internal buffer");
                }

                ubSeek = false;

            }
            else { // otherwise mark read buffer empty and proceed with normal seek
                internalBuffer.Empty();
                // continues at the end of the function
            }
        }
    }

    return (ubSeek) ? (UnbufferedSeek(pos)) : (true);
}

bool SingleBufferedStream::RelativeSeek(int32 deltaPos) {
    bool ubSeek = false;

    if (deltaPos != 0) {

        ubSeek = true;
        // if write mode on then just flush out data
        if (mutexWriteMode) {
            if (internalBuffer.UsedSize() > 0u) {
                // this will move the stream pointer ahead to the correct position
                if (!internalBuffer.Flush()) {
                    ubSeek = false;
                }
            }
        }
        if (mutexReadMode) {

            //save the current position because in case of out of range
            //the position becomes one of the buffer bounds.
            uint64 currentPos = internalBuffer.Position();

            // on success it means we are in range
            if (internalBuffer.RelativeSeek(deltaPos)) {
                // no need to move stream pointer
                ubSeek = false;
            }
            if (ubSeek) {
                // out of buffer range
                // adjust stream seek position to account for actual read buffer usage

                /*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: If the number is not in the signed range a warning will be generated*/
                int32 gap = static_cast<int32>(internalBuffer.UsedSize() - currentPos);

                // if gap is not in the signed range generate a warning
                if (gap < 0) {
                    ubSeek = false;
                }
                deltaPos -= gap;
                // empty buffer
                internalBuffer.Empty();
            }

        }
    }

    // seek
    /*lint -e{9117} -e{737} [MISRA C++ Rule 5-0-4]. The input value is always positive so the signed does not change. */
    return (ubSeek) ? (UnbufferedSeek(static_cast<uint64>(UnbufferedPosition() + deltaPos))) : (true);
}

uint64 SingleBufferedStream::Position() {

    uint64 ret = 0u;
    // if write mode on then just flush out data
    if (mutexWriteMode) {
        ret = UnbufferedPosition() + internalBuffer.Position();
    }
    if (mutexReadMode) {

        ret = UnbufferedPosition() - internalBuffer.UsedAmountLeft();
    }
    return ret;
}

bool SingleBufferedStream::SetSize(const uint64 size) {
    bool ret = true;
    // commit all changes
    if (!FlushAndResync()) {
        ret = false;
    }

    if (ret) {
        ret = UnbufferedSetSize(size);
    }
    return ret;
}

}

