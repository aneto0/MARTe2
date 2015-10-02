/**
 * @file StreamWrapperIOBuffer.cpp
 * @brief Source file for class StreamWrapperIOBuffer
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
 * the class StreamWrapperIOBuffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamWrapperIOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamWrapperIOBuffer::StreamWrapperIOBuffer(StreamInterface *s,
                      uint32 size) {
    stream = s;
    SetBufferHeapMemory(size);
}

StreamWrapperIOBuffer::StreamWrapperIOBuffer(StreamInterface *s,
                      char8 *buffer,
                      uint32 size) {
    stream = s;
    SetBufferReferencedMemory(buffer, size);
}

bool StreamWrapperIOBuffer::Resync(TimeoutType msecTimeout) {
// empty!
    if (MaxUsableAmount() == 0) {
        return true;
    }

    // distance to end
    uint32 deltaToEnd = UsedAmountLeft();

    // adjust seek position
    // in read mode the actual stream
    // position is to the character after the buffer end
    if (!stream->Seek(stream->Position() - deltaToEnd)) {
        Empty();
        return false;
    }

    // mark it as empty
    Empty();
    return true;
}


bool StreamWrapperIOBuffer::NoMoreDataToRead(TimeoutType msecTimeout) {
// can we write on it?
    if (BufferReference() == NULL) {
        return false;
    }

    Empty();

    uint32 readSize = MaxUsableAmount();

    if (stream->Read(BufferReference(),readSize)) {
        SetUsedSize(readSize);
        return true;
    }

    Empty();
    return false;

}


bool StreamWrapperIOBuffer::NoMoreSpaceToWrite(uint32 neededSize,
                                               TimeoutType msecTimeout) {
    // no buffering!
    if (Buffer() == NULL)return true;

            // how much was written?
            uint32 writeSize = UsedSize();

            // write
            if (!stream->Write(Buffer(),writeSize,msecTimeout,true)) {
                return false;
            }

            Empty();
            return true;
        }
    }

