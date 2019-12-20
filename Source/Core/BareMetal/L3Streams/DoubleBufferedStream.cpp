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

#define DLL_API
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
        BufferedStreamI(), readBuffer(this), writeBuffer(this) {
    bufferSizeSet = true;
    if (!readBuffer.SetBufferSize(32u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
        bufferSizeSet = false;
    }
    if (!writeBuffer.SetBufferSize(32u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
        bufferSizeSet = false;
    }
}

DoubleBufferedStream::DoubleBufferedStream(const TimeoutType &timeoutIn) :
        BufferedStreamI(), readBuffer(this), writeBuffer(this) {
    SetTimeout(timeoutIn);
    bufferSizeSet = true;
    if (!readBuffer.SetBufferSize(32u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
        bufferSizeSet = false;
    }
    if (!writeBuffer.SetBufferSize(32u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SingleBufferedStream: Failed to SetBufferSize(32)");
        bufferSizeSet = false;
    }
}

DoubleBufferedStream::~DoubleBufferedStream() {
}

bool DoubleBufferedStream::SetBufferSize(uint32 readBufferSize, uint32 writeBufferSize) {
    // minimum size = 8
    if (readBufferSize < 8u) {
        readBufferSize = 8u;
    }
    if (writeBufferSize < 8u) {
        writeBufferSize = 8u;
    }

    bufferSizeSet = false;
    // dump any data in the write Queue
    if (Flush()) {
        bufferSizeSet = readBuffer.SetBufferSize(readBufferSize);
        if (bufferSizeSet) {
            bufferSizeSet = writeBuffer.SetBufferSize(writeBufferSize);
        }
    }

    return bufferSizeSet;
}
/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer * DoubleBufferedStream::GetReadBuffer() {
    return &readBuffer;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer * DoubleBufferedStream::GetWriteBuffer() {

    return &writeBuffer;
}

bool DoubleBufferedStream::Read(char8 * const output, uint32 & size) {

    bool ret = CanRead() && bufferSizeSet;
    if (ret) {

        // read from buffer first
        uint32 toRead = size;

        // try once
        if (!readBuffer.Read(&output[0], size)) {
            ret = false;
        }

        if (ret && (size != toRead)) {
            // partial only so continue

            // adjust toRead
            toRead -= size;

            // decide whether to use the buffer again or just to read directly
            if ((toRead * calibReadParam) < readBuffer.MaxUsableAmount()) {
                ret = Refill();

                uint32 readBytes = size;
                while ((toRead > 0u) && (ret)) {
                    uint32 nRead = toRead;
                    ret = readBuffer.Read(&output[readBytes], nRead);
                    if (ret) {
                        if (nRead != toRead) {
                            ret = Refill();
                        }
                    }
                    if (ret) {
                        toRead -= nRead;
                        readBytes += nRead;
                    }

                }

                size = readBytes;
#if 0
                if (!readBuffer.Refill()) {
                    ret = false;
                }

                else {

                    if (!readBuffer.Read(&output[size], toRead)) {
                        ret = false;
                    }
                    size += toRead;

                    // should have completed
                    // as our buffer is at least 4x the need
                }
#endif

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

    // if needed read directly from stream
    return ret;
}

bool DoubleBufferedStream::Read(char8 * const output, uint32 & size, const TimeoutType &timeout) {
    TimeoutType prevTimeout = GetTimeout();
    SetTimeout(timeout);
    bool ret = Read(output, size);
    SetTimeout(prevTimeout);
    return ret;
}

bool DoubleBufferedStream::Write(const char8 * const input, uint32 & size) {

    bool ret = CanWrite() && bufferSizeSet;
    if (ret) {
        // separate input and output size

        uint32 toWrite = size;
        // check available buffer size versus write size
        // if size is comparable to buffer size there
        // is no reason to use the buffering mechanism
        if (writeBuffer.MaxUsableAmount() > (calibWriteParam * size)) {
            uint32 writed = 0u;
            //put a while here
            while ((toWrite > 0u) && (ret)) {
                uint32 nWrites = toWrite;
                // try writing the buffer
                if (!writeBuffer.Write(&input[writed], nWrites)) {
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
#if 0
            // try writing the buffer
            if (!writeBuffer.Write(&input[0], size)) {
                ret = false;
            }

            // all done! space available!
            if (ret && (size != toWrite)) {
                // make space
                if (!writeBuffer.Flush()) {
                    ret = false;
                }
                else {
                    toWrite -= size;
                    uint32 leftToWrite = toWrite;

                    // try writing the buffer
                    if (!writeBuffer.Write(&input[size], leftToWrite)) {
                        ret = false;
                    }

                    size += leftToWrite;

                    // should have been able to fill in it!!!
                    if (leftToWrite != toWrite) {
                        ret = false;
                    }
                }
            }
#endif
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

bool DoubleBufferedStream::Write(const char8 * const input, uint32 & size, const TimeoutType &timeout) {
    TimeoutType prevTimeout = GetTimeout();
    SetTimeout(timeout);
    bool ret = Write(input, size);
    SetTimeout(prevTimeout);
    return ret;
}

uint64 DoubleBufferedStream::Size() {
    return OSSize();
}

bool DoubleBufferedStream::Seek(const uint64 pos) {
    return OSSeek(pos);
}

bool DoubleBufferedStream::RelativeSeek(const int64 deltaPos) {
    return OSRelativeSeek(deltaPos);
}

uint64 DoubleBufferedStream::Position() {
    return OSPosition();
}

bool DoubleBufferedStream::SetSize(const uint64 size) {
    return OSSetSize(size);
}

uint32 DoubleBufferedStream::GetReadBufferSize() const {
    return readBuffer.GetBufferSize();
}

uint32 DoubleBufferedStream::GetWriteBufferSize() const {
    return writeBuffer.GetBufferSize();
}

TimeoutType DoubleBufferedStream::GetTimeout() const {
    return readBuffer.GetTimeout();
}

void DoubleBufferedStream::SetTimeout(const TimeoutType &timeoutIn) {
    readBuffer.SetTimeout(timeoutIn);
    writeBuffer.SetTimeout(timeoutIn);
}

}

