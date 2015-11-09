/**
 * @file BufferedStreamIOBuffer.cpp
 * @brief Source file for class BufferedStreamIOBuffer
 * @date 02/10/2015
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
 * the class BufferedStreamIOBuffer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DoubleBufferedStream.h"
#include "BufferedStreamIOBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BufferedStreamIOBuffer::BufferedStreamIOBuffer() :
        IOBuffer() {
    stream = static_cast<OperatingSystemCallbacksI *>(NULL);
    timeout = TTInfiniteWait;
}

BufferedStreamIOBuffer::BufferedStreamIOBuffer(OperatingSystemCallbacksI * const s) :
        IOBuffer() {
    stream = s;
    timeout = TTInfiniteWait;
}

bool BufferedStreamIOBuffer::Resync() {
    bool retval = (stream != NULL);

    if (retval) {
        // empty!
        if (UsedSize() > 0u) {

            // distance to end
            uint32 deltaToEnd = UsedAmountLeft();

            // adjust seek position
            // in read mode the actual stream
            // position is to the character after the buffer end
            if (!stream->OSSeek(stream->OSPosition() - deltaToEnd)) {
                REPORT_ERROR(ErrorManagement::FatalError, "BufferedStreamIOBuffer: Failed OSSeek");
                retval = false;
            }

            // mark it as empty
            Empty();

        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BufferedStreamIOBuffer: Invalid stream");
    }
    return retval;
}

/*lint -e{534} [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2]. Justification: IOBuffer::SetUsedSize always returns true.*/
bool BufferedStreamIOBuffer::NoMoreDataToRead() {

    bool retval = false;
    if (stream != NULL) {
// can we write on it?
        if (BufferReference() != NULL) {

            // move all pointers and indexes to empty status
            Empty();

            uint32 readSize = MaxUsableAmount();

            if (stream->OSRead(BufferReference(), readSize)) {
                IOBuffer::SetUsedSize(readSize);
                retval = true;
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "BufferedStreamIOBuffer: Failed OSRead");
                Empty();
            }

        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BufferedStreamIOBuffer: Invalid stream");
    }
    return retval;
}

bool BufferedStreamIOBuffer::NoMoreSpaceToWrite() {
    bool retval = false;

    if (stream != NULL) {
        // no buffering!
        if (Buffer() != NULL) {

            // how much was written?
            uint32 writeSize = UsedSize();
            if (writeSize == 0u) {
                retval = true;
            }
            // write
            else {
                if (stream->OSWrite(Buffer(), writeSize)) {
                    retval = true;
                    Empty();
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "BufferedStreamIOBuffer: Failed OSWrite");
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "BufferedStreamIOBuffer: Invalid stream");
    }
    return retval;
}

bool BufferedStreamIOBuffer::SetBufferSize(const uint32 size) {
    return IOBuffer::SetBufferHeapMemory(size, 0u);
}

}
