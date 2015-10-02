/**
 * @file StreamStringIOBuffer.cpp
 * @brief Source file for class StreamStringIOBuffer
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
 * the class StreamStringIOBuffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamStringIOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamStringIOBuffer::StreamStringIOBuffer(){

}

StreamStringIOBuffer::~StreamStringIOBuffer() {

}

bool StreamStringIOBuffer::SetBufferAllocationSize(uint32 desiredSize,
                                                   uint32 allocationGranularityMask) {

    //add one to desired size for the terminator character.
    bool ret = SetBufferHeapMemory(desiredSize + 1, allocationGranularityMask, 1);
    Terminate();
    return ret;

}

void StreamStringIOBuffer::Write(const char8 *buffer,
                                 uint32 &size) {

    if (size > AmountLeft()) {
        SetBufferAllocationSize(Position() + size);
    }

    IOBuffer::Write(buffer, size);

}


bool StreamStringIOBuffer::NoMoreSpaceToWrite(uint32 neededSize,
                                              TimeoutType msecTimeout) {

    // reallocate buffer
    // uses safe version of the function
    // implemented in this class
    if (!SetBufferAllocationSize(BufferSize() + neededSize)) {
        return false;
    }

    return true;
}

void StreamStringIOBuffer::Terminate() {
    if (BufferReference() != NULL)
    BufferReference()[UsedSize()]= 0;
}
}

