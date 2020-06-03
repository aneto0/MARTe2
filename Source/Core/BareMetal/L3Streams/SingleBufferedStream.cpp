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
#define DLL_API
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
        BufferedStreamI(),
        internalBuffer(this) {
    mutexReadMode = true;
    mutexWriteMode = false;
    bufferSizeSet = true;
    if (!internalBuffer.SetBufferSize(32u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
        bufferSizeSet = false;
    }
}

SingleBufferedStream::SingleBufferedStream(const TimeoutType &timeoutIn) :
        BufferedStreamI(),
        internalBuffer(this) {
    mutexReadMode = true;
    mutexWriteMode = false;
    bufferSizeSet = true;
    SetTimeout(timeoutIn);
    if (!internalBuffer.SetBufferSize(32u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
        bufferSizeSet = false;
    }
}

SingleBufferedStream::~SingleBufferedStream() {
}

bool SingleBufferedStream::SetBufferSize(uint32 bufferSize) {
    // minimum size = 8
    if (bufferSize < 8u) {
        bufferSize = 8u;
    }
    bufferSizeSet = false;
    // dump any data in the write Queue
    if (FlushAndResync()) {
        bufferSizeSet = internalBuffer.SetBufferSize(bufferSize);
    }

    return bufferSizeSet;
}

uint32 SingleBufferedStream::GetBufferSize() const {
    return internalBuffer.GetBufferSize();
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer *SingleBufferedStream::GetReadBuffer() {
    IOBuffer *ret = &internalBuffer;
    if (mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = static_cast<IOBuffer *>(NULL);
        }
    }

    return ret;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer *SingleBufferedStream::GetWriteBuffer() {
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

    bool ret = CanRead() && bufferSizeSet;

    // check for mutually exclusive buffering and
    // whether one needs to switch to ReadMode
    if (ret && mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = false;
        }
    }
    if (ret) {
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
            if ((toRead * calibReadParam) < internalBuffer.MaxUsableAmount()) {
                if (!Refill()) {
                    ret = false;
                }

                uint32 readBytes = size;
                while ((toRead > 0u) && (ret)) {
                    uint32 nReads = toRead;
                    if (!internalBuffer.Read(&output[readBytes], nReads)) {
                        ret = false;
                    }
                    if (nReads != toRead) {
                        if (!Refill()) {
                            ret = false;
                        }

                    }
                    if (ret) {
                        toRead -= nReads;
                        readBytes += nReads;
                    }

                }

                size = readBytes;

            }
            else {
                // if needed read directly from stream
                if (!OSRead(&output[size], toRead)) {
                    ret = false;
                }
                else {
                    size += toRead;
                }
            }
        }
    }

    return ret;
}

bool SingleBufferedStream::Read(char8 * const output,
                                uint32 & size,
                                const TimeoutType &timeout) {
    TimeoutType prevTimeout = GetTimeout();
    SetTimeout(timeout);
    bool ret = Read(output, size);
    SetTimeout(prevTimeout);
    return ret;
}

bool SingleBufferedStream::Write(const char8 * const input,
                                 uint32 & size) {

    bool ret = CanWrite() && bufferSizeSet;
    // check for mutually exclusive buffering and
    // whether one needs to switch to WriteMode
    if (ret && mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = false;
        }
    }
    if (ret) {

        uint32 toWrite = size;
        // check available buffer size versus write size
        // if size is comparable to buffer size there
        // is no reason to use the buffering mechanism
        if (internalBuffer.MaxUsableAmount() > (calibWriteParam * size)) {
            uint32 writed = 0u;
            //put a while here
            while ((toWrite > 0u) && (ret)) {
                uint32 nWrites = toWrite;
                // try writing the buffer
                if (!internalBuffer.Write(&input[writed], nWrites)) {
                    ret = false;
                }
                if (nWrites != toWrite) {
                    // make space
                    if (!Flush()) {
                        ret = false;
                    }
                }
                if (ret) {
                    writed += nWrites;
                    toWrite -= nWrites;
                }
            }

            size = writed;

        }
        else {
            // write the buffer so far
            if (!Flush()) {
                ret = false;
            }
            else {
                ret = OSWrite(&input[0], size);
            }
        }
    }
    return ret;

}

bool SingleBufferedStream::Write(const char8 * const input,
                                 uint32 & size,
                                 const TimeoutType &timeout) {
    TimeoutType prevTimeout = GetTimeout();
    SetTimeout(timeout);
    bool ret = Write(input, size);
    SetTimeout(prevTimeout);
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
        size = OSSize();
    }
    return size;
}

bool SingleBufferedStream::Seek(const uint64 pos) {

    bool ubSeek = true;
    bool ok = CanSeek() && bufferSizeSet;
    // if write mode on then just flush out data
    // then seek the stream
    if (ok && mutexWriteMode) {
        if (internalBuffer.UsedSize() > 0u) {
            if (!internalBuffer.Flush()) {
                ok = false;
                ubSeek = false;
            }
        }
    }
    if (ok && mutexReadMode) {
        // if read buffer has some data, check whether seek can be within buffer
        if (internalBuffer.UsedSize() > 0u) {
            uint64 currentStreamPosition = OSPosition();
            uint64 bufferStartPosition = currentStreamPosition - internalBuffer.UsedSize();

            // if within range just update readBufferAccessPosition
            if ((pos >= bufferStartPosition) && (pos < currentStreamPosition)) {
                if (!internalBuffer.Seek(static_cast<uint32>(pos - bufferStartPosition))) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to seek the internal buffer");
                }

                ubSeek = false;

            }
            else { // otherwise mark read buffer empty and proceed with normal seek
                internalBuffer.Empty();
                // continues at the end of the function
            }
        }
    }
    if (ok && ubSeek) {
        ok = OSSeek(pos);
    }
    return ok;
}

bool SingleBufferedStream::RelativeSeek(const int64 deltaPos) {
    bool ubSeek = false;
    bool ok = CanSeek() && bufferSizeSet;

    bool moveBack = (deltaPos < 0);
    /*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: The sign will not change because before the variable is negative for sure. */
    uint64 absDelta = (moveBack) ? static_cast<uint64>(-deltaPos) : static_cast<uint64>(deltaPos);

    if (ok && (deltaPos != 0)) {

        ubSeek = true;
        // if write mode on then just flush out data
        if (mutexWriteMode) {
            if (internalBuffer.UsedSize() > 0u) {
                // this will move the stream pointer ahead to the correct position
                if (!internalBuffer.Flush()) {
                    ok = false;
                    ubSeek = false;
                }
            }
        }

        if (mutexReadMode) {
            // IOBuffer uses int32
            if ((deltaPos <= MAX_INT32) && (deltaPos >= MIN_INT32)) {

                //save the current position because in case of out of range
                //the position becomes one of the buffer bounds.
                uint32 currentPos = internalBuffer.Position();

                // on success it means we are in range
                if (internalBuffer.RelativeSeek(static_cast<int32>(deltaPos))) {
                    // no need to move stream pointer
                    ubSeek = false;
                }
                if (ubSeek) {
                    // out of buffer range
                    // adjust stream seek position to account for actual read buffer usage

                    uint32 gap = internalBuffer.UsedSize() - currentPos;

                    // if relative seek of IOBuffer fails then absDelta-gap is positive
                    (moveBack) ? (absDelta -= gap) : (absDelta += gap);
                    //deltaPos -= gap;
                    // empty read buffer
                    internalBuffer.Empty();
                }

            }
            else {
                if (!internalBuffer.Resync()) {
                    ok = false;
                }
            }
        }
    }

    if (ok && ubSeek) {
        if ((absDelta > OSPosition()) && (moveBack)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "RelativeSeek: Desired position negative: moved to the begin");
            ok = false;
        }
        else {
            // if the final position is greater than OSSize should be managed by OSSeek
            /*lint -e{9117} -e{737} [MISRA C++ Rule 5-0-4]. The input value is always positive so the signed does not change. */
            ok = (moveBack) ? (OSSeek(OSPosition() - absDelta)) : (OSSeek(OSPosition() + absDelta));
        }
    }
    return ok;
}

uint64 SingleBufferedStream::Position() {

    uint64 ret = 0u;
    // if write mode on then just flush out data
    if (mutexWriteMode) {
        ret = OSPosition() + internalBuffer.Position();
    }
    if (mutexReadMode) {

        ret = OSPosition() - internalBuffer.UsedAmountLeft();
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
        ret = OSSetSize(size);
    }
    return ret;
}

TimeoutType SingleBufferedStream::GetTimeout() const {
    return internalBuffer.GetTimeout();
}

void SingleBufferedStream::SetTimeout(const TimeoutType &timeoutIn) {
    internalBuffer.SetTimeout(timeoutIn);
}

}

