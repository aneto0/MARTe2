/**
 * @file IOBuffer.cpp
 * @brief Source file for class IOBuffer
 * @date 01/10/2015
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
 * the class IOBuffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IOBuffer.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

// position is set relative to start of buffer
bool IOBuffer::Seek(const uint32 position) {
    bool retval = (position <= UsedSize());

    if (retval) {
        amountLeft = MaxUsableAmount() - position;
        bufferPtr = const_cast<char8 *>(&((Buffer())[position]));
    }
    return retval;
}

//position is set relative to current position
bool IOBuffer::RelativeSeek(int32 delta) {
    bool ret = true;
    if (delta >= 0) {
        uint32 actualLeft = amountLeft - fillLeft;
        //cannot seek beyond fillLeft
        if (static_cast<uint32>(delta) > actualLeft) {
            delta = static_cast<int32>(actualLeft);
            //  saturate at the end
            ret = false;
//REPORT_ERROR_PARAMETERS(ErrorType::ParametersError,"delta=%i at position %i moves out of range %i, moving to end of stream",delta,Position(),MaxUsableAmount())
        }
    }
    else {
        // cannot seek below 0
        if (static_cast<uint32>(-delta) > Position()) {
            //  saturate at the beginning
            ret = false;
            delta = -static_cast<int32>(Position());
//REPORT_ERROR_PARAMETERS(ParametersError,"delta=%i at position %i moves out of range 0, moving to beginning of stream",delta,Position())
        }
    }
    amountLeft -= delta;
    bufferPtr += delta;
    return ret;
}

bool IOBuffer::SetUsedSize(uint32 size) {
    if (size > maxUsableAmount) {
        size = maxUsableAmount;
    }

    fillLeft = maxUsableAmount - size;

    return true;
}

IOBuffer::~IOBuffer() {
}

/*
 allocate or reallocate memory to the desired size
 content is preserved by copy, if contiguus memory is not available, as long as it fits the newsize
 allocationGranularityMask defines how many bits to consider
 for the buffer size. round up the others
 */
bool IOBuffer::SetBufferHeapMemory(const uint32 desiredSize,
                                   const uint32 allocationGranularityMask,
                                   const uint32 reservedSpaceAtEnd) {
    // save position
    uint32 position = Position();
    uint32 usedSize = UsedSize();

    //special case: if we consider the difference
    //between two unsigned integers we can obtain bigger numbers (overflow).
    if (desiredSize < reservedSpaceAtEnd) {
        usedSize = 0u;
    }

    // truncating
    if ((desiredSize - reservedSpaceAtEnd) < usedSize) {
        usedSize = desiredSize - reservedSpaceAtEnd;
    }

    // saturate index
    if (position > usedSize) {
        position = usedSize;
    }

    bool ret = CharBuffer::SetBufferSize(desiredSize, allocationGranularityMask);

    bufferPtr = BufferReference();

    maxUsableAmount = BufferSize();

    if (maxUsableAmount <= reservedSpaceAtEnd) {
        maxUsableAmount = 0u;
    }
    else {
        maxUsableAmount = BufferSize() - reservedSpaceAtEnd;
    }

    amountLeft = maxUsableAmount - position;
    fillLeft = maxUsableAmount - usedSize;

    bufferPtr = &bufferPtr[position];
    return ret;
}

// wipes all content and replaces the used buffer
bool IOBuffer::SetBufferReferencedMemory(char8 * const buffer,
                                         const uint32 bufferSize,
                                         const uint32 reservedSpaceAtEnd) {
    CharBuffer::SetBufferReference(buffer, bufferSize);
    bufferPtr = BufferReference();
    maxUsableAmount = BufferSize() - reservedSpaceAtEnd;
    Empty();
    return true;
}


bool IOBuffer::SetBufferReadOnlyReferencedMemory(const char8 * const buffer,
                                                 const uint32 bufferSize,
                                                 const uint32 reservedSpaceAtEnd) {
    CharBuffer::SetBufferReference(buffer, bufferSize);
    bufferPtr = BufferReference();
    maxUsableAmount = BufferSize() - reservedSpaceAtEnd;
    Empty();
    return true;
}

bool IOBuffer::NoMoreSpaceToWrite(const uint32 neededSize,
                                  const TimeoutType msecTimeout) {
    return false;
}

/*
 * deals with the case when we do not have any more data to read
 * it might reset accessPosition and fill the buffer with more data
 * or it might fail
 * READ OPERATIONS
 * */
bool IOBuffer::NoMoreDataToRead(const TimeoutType msecTimeout) {
    return false;
}

/**
 sets amountLeft to 0
 adjust the seek position of the stream to reflect the bytes read from the buffer
 * READ OPERATIONS
 */
bool IOBuffer::Resync(const TimeoutType msecTimeout) {
    return false;
}

//void IOBuffer::Terminate(){
//}

/*
 *  copies buffer of size size at the end of writeBuffer
 * before calling check that bufferPtr is not NULL
 * can be overridden to allow resizeable buffers
 */
void IOBuffer::Write(const char8 * const buffer,
                     uint32 &size) {
    if (CanWrite()) {

        // clip to spaceLeft
        if (size > amountLeft) {
            size = amountLeft;
        }

        // fill the buffer with the remainder
        if (size > 0u) {
            MemoryOperationsHelper::Copy(bufferPtr, buffer, size);

            bufferPtr = &bufferPtr[size];
            amountLeft -= size;
            if (fillLeft > amountLeft) {
                fillLeft = amountLeft;
            }
        }
    }
}

bool IOBuffer::WriteAll(const char8 * buffer,
                        const uint32 &size) {

    bool retval = true;
    //size to be copied.
    uint32 leftSize = size;
    while (leftSize > 0u) {
        //if the cursor is at the end call NoMoreSpaceToWrite
        // flushes the buffer or allocates new memory.
        if (amountLeft == 0u) {
            if (!NoMoreSpaceToWrite(leftSize, TTDefault)) {
                retval = false;
            }
            //Something wrong, no more avaiable space, return false.
            if (amountLeft == 0u) {
                retval = false;
            }
        }
        if(!retval){
            break;
        }

        uint32 toDo = leftSize;
        Write(buffer, toDo);
        buffer = &buffer[toDo];
        //if all the size is copied leftSize becomes 0 and return true
        leftSize -= toDo;
    }
    return retval;
}

void IOBuffer::Read(char8 * const buffer,
                    uint32 &size) {

    uint32 maxSize = UsedAmountLeft();
    // clip to available space
    if (size > maxSize) {
        size = maxSize;
    }

    // fill the buffer with the remainder
    if (size > 0u) {
        MemoryOperationsHelper::Copy(buffer, bufferPtr, size);

        amountLeft -= size;
        bufferPtr = &bufferPtr[size];
    }
}

}
