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
#define DLL_API

#include "IOBuffer.h"
#include "AdvancedErrorManagement.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


bool IOBuffer::Seek(const uint32 position) {
    bool retval = (position <= UsedSize());

    if (retval) {
        amountLeft = MaxUsableAmount() - position;
        positionPtr = &((BufferReference())[position]);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Position in input greater than the buffer size");
    }
    return retval;
}

//position is set relative to current position
/*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: the sign does not change because delta is positive (see the inline code).*/
bool IOBuffer::RelativeSeek(const int32 delta) {
    bool ret = true;
    if (delta >= 0) {
        uint32 gap = static_cast<uint32>(delta);
        uint32 actualLeft = amountLeft - fillLeft;
        //cannot seek beyond fillLeft
        if (gap > actualLeft) {
            //  saturate at the end
            gap = actualLeft;
            ret = false;
            REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Final position greater than the buffer used size: move to the end");
        }
        amountLeft -= gap;
        positionPtr = &positionPtr[gap];
    }
    else {

        uint32 gap = static_cast<uint32>(-delta);
        // cannot seek below 0
        if (gap > Position()) {
            //  saturate at the beginning
            ret = false;
            gap = Position();
            REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Final position less than zero: move to the beginning");
        }
        amountLeft += gap;
        positionPtr = &((BufferReference())[Position() - gap]);
    }

    return ret;
}

void IOBuffer::SetUsedSize(const uint32 desiredSize) {
    uint32 size;

    if (desiredSize > maxUsableAmount) {
        size = maxUsableAmount;
    }
    else {
        size = desiredSize;
    }

    fillLeft = maxUsableAmount - size;
}

IOBuffer::~IOBuffer() {
    positionPtr = static_cast<char8 *>(NULL);
}

bool IOBuffer::SetBufferHeapMemory(const uint32 desiredSize,
                                   const uint32 reservedSpaceAtEnd) {
    // save position
    uint32 position = Position();
    uint32 usedSize = UsedSize();

    // if the buffer was set on a memory reference begin again
    if ((!internalBuffer.IsAllocated()) && (positionPtr != static_cast<char8 *>(NULL))) {
        position = 0u;
        usedSize = 0u;
    }

    //special case: if we consider the difference
    //between two unsigned integers we can obtain bigger numbers (overflow).
    if (desiredSize < reservedSpaceAtEnd) {
        usedSize = 0u;
        REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: The reserved space at end is greater than the size to be allocated: set the used size to zero");
    }

    // truncating
    if ((desiredSize - reservedSpaceAtEnd) < usedSize) {
        usedSize = desiredSize - reservedSpaceAtEnd;
    }

    // saturate index
    if (position > usedSize) {
        position = usedSize;
    }

    bool ret = internalBuffer.SetBufferSize(desiredSize);

    positionPtr = BufferReference();

    maxUsableAmount = GetBufferSize();

    if (maxUsableAmount <= reservedSpaceAtEnd) {
        maxUsableAmount = 0u;
    }
    else {
        maxUsableAmount = GetBufferSize() - reservedSpaceAtEnd;
    }

    amountLeft = maxUsableAmount - position;
    fillLeft = maxUsableAmount - usedSize;

    positionPtr = &positionPtr[position];
    return ret;
}

void IOBuffer::SetBufferReferencedMemory(char8 * const buffer,
                                         const uint32 bufferSize,
                                         const uint32 reservedSpaceAtEnd) {
    internalBuffer.SetBufferReference(buffer, bufferSize);
    positionPtr = BufferReference();
    maxUsableAmount = GetBufferSize() - reservedSpaceAtEnd;
    Empty();
}

void IOBuffer::SetBufferReadOnlyReferencedMemory(const char8 * const buffer,
                                                 const uint32 bufferSize,
                                                 const uint32 reservedSpaceAtEnd) {

    internalBuffer.SetBufferReference(buffer, bufferSize);
    positionPtr = const_cast<char8*>(Buffer());
    maxUsableAmount = GetBufferSize() - reservedSpaceAtEnd;
    Empty();
}

bool IOBuffer::NoMoreSpaceToWrite() {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11]. Justification: The default behavior does not need the input argument.*/
bool IOBuffer::NoMoreSpaceToWrite(const uint32 neededSize) {
    return NoMoreSpaceToWrite();
}

bool IOBuffer::NoMoreDataToRead() {
    return false;
}

bool IOBuffer::Resync() {
    return false;
}

bool IOBuffer::Write(const char8 * const buffer,uint32 &size) {
    bool retval = internalBuffer.CanWrite();
    if (retval) {

        // clip to spaceLeft
        if (size > amountLeft) {
            size = amountLeft;
        }

        // fill the buffer with the remainder
        if (size > 0u) {
            if (!MemoryOperationsHelper::Copy(positionPtr, buffer, size)) {
                REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Failed MemoryOperationsHelper::Copy()");
                retval = false;
            }

            if (retval) {
                positionPtr = &positionPtr[size];
                amountLeft -= size;
                if (fillLeft > amountLeft) {
                    fillLeft = amountLeft;
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Failed CharBuffer::CanWrite()");
    }

    return retval;
}

bool IOBuffer::WriteAll(const char8 * buffer,
                        const uint32 &size) {

    bool retval = true;
    //size to be copied.
    uint32 leftSize = size;
    while (retval && (leftSize > 0u)) {
        // if the cursor is at the end call NoMoreSpaceToWrite
        // flushes the buffer or allocates new memory.
        if (amountLeft == 0u) {
            if (!NoMoreSpaceToWrite(leftSize)) {
                retval = false;
            }
            if (retval) {
                //Something wrong, no more available space, return false.
                if (amountLeft == 0u) {
                    retval = false;
                }
            }
        }

        if (retval) {
            uint32 toDo = leftSize;
            retval = Write(buffer, toDo);
            if (retval) {
                buffer = &buffer[toDo];
                //if all the size is copied leftSize becomes 0 and return true
                leftSize -= toDo;
            }
        }
    }
    return retval;
}

bool IOBuffer::Read(char8 * const buffer,
                    uint32 &size) {

    bool retval = true;
    uint32 maxSize = UsedAmountLeft();
    // clip to available space
    if (size > maxSize) {
        size = maxSize;
    }

    // fill the buffer with the remainder
    if (size > 0u) {
        if (!MemoryOperationsHelper::Copy(buffer, positionPtr, size)) {
            retval = false;
            REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Failed MemoryOperationsHelper::Copy()");
        }
        if (retval) {
            amountLeft -= size;
            positionPtr = &positionPtr[size];
        }
    } else {
    	retval = false;
    }

    return retval;
}

}
