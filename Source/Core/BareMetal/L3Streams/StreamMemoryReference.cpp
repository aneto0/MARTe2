/**
 * @file StreamMemoryReference.cpp
 * @brief Source file for class StreamMemoryReference
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
 * the class StreamMemoryReference (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamMemoryReference.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamMemoryReference::StreamMemoryReference() :
        BufferedStreamI(),
        buffer() {

}

StreamMemoryReference::StreamMemoryReference(char8 * const bufferIn,
                                             const uint32 bufferSize) :
        BufferedStreamI() {
    buffer.SetBufferReferencedMemory(bufferIn, bufferSize, 0u);
}

StreamMemoryReference::StreamMemoryReference(const char8 * const bufferIn,
                                             const uint32 bufferSize) :
        BufferedStreamI() {
    buffer.SetBufferReadOnlyReferencedMemory(bufferIn, bufferSize, 0u);
    buffer.SetUsedSize(bufferSize);
}

StreamMemoryReference::~StreamMemoryReference() {
}

StreamMemoryReference::operator AnyType() const {
    AnyType at(Buffer());
    return at;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer *StreamMemoryReference::GetReadBuffer() {
    return &buffer;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: StreamI must have the access to the final buffers.*/
IOBuffer *StreamMemoryReference::GetWriteBuffer() {
    return &buffer;
}

bool StreamMemoryReference::Read(char8* const output,
                                 uint32 & size) {
    return this->buffer.Read(&output[0], size);
}

bool StreamMemoryReference::Write(const char8* const input,
                                  uint32 & size) {
    return this->buffer.Write(&input[0], size);

}

bool StreamMemoryReference::CanWrite() const {
    return ((buffer.Buffer() != NULL) && (buffer.CanWrite()));
};

bool StreamMemoryReference::CanRead() const {
    return (buffer.Buffer() != NULL);
};

uint64 StreamMemoryReference::Size() {
    return buffer.UsedSize();
}

bool StreamMemoryReference::SetSize(const uint64 size) {

    buffer.SetUsedSize(static_cast<uint32>(size));
    return (buffer.UsedSize() == static_cast<uint32>(size));
}

bool StreamMemoryReference::Seek(const uint64 pos) {
    uint32 usedSize = buffer.UsedSize();
    bool ret = true;
    if (pos > usedSize) {
        if (!buffer.Seek(usedSize)) {
            REPORT_ERROR(ErrorManagement::FatalError, "StreamMemoryReference: Failed IOBuffer::Seek");
        }
        ret = false;
        REPORT_ERROR(ErrorManagement::FatalError, "StreamString: Desired Position greater than current size: moved to end");
    }

    return (ret) ? (buffer.Seek(static_cast<uint32>(pos))) : (false);
}

bool StreamMemoryReference::RelativeSeek(const int64 deltaPos) {
    bool ret = true;
    if ((deltaPos > MAX_INT32) || (deltaPos < MIN_INT32)) {
        REPORT_ERROR(ErrorManagement::FatalError, "RelativeSeek: The seek offset should be in the int32 range");
        ret = false;
    }
    else {
        ret = buffer.RelativeSeek(static_cast<int32>(deltaPos));
    }
    return ret;
}

uint64 StreamMemoryReference::Position() {
    return buffer.Position();
}

bool StreamMemoryReference::CanSeek() const {
    return true;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the timeout parameter is not used here but it is
 * used by other buffered streams. */
bool StreamMemoryReference::Read(char8 * const output,
                                 uint32 & size,
                                 const TimeoutType &timeout) {
    return Read(output, size);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the timeout parameter is not used here but it is
 * used by other buffered streams. */
bool StreamMemoryReference::Write(const char8 * const input,
                                  uint32 & size,
                                  const TimeoutType &timeout) {
    return Write(input, size);
}

}

