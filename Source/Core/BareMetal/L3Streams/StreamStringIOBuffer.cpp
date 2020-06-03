/**
 * @file StreamStringIOBuffer.cpp
 * @brief Source file for class StreamStringIOBuffer
 * @date 26/10/2015
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

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <StreamStringIOBuffer.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamStringIOBuffer::StreamStringIOBuffer() :
        IOBuffer(64u, 0u) {

}

StreamStringIOBuffer::StreamStringIOBuffer(const uint32 granularity) :
        IOBuffer(granularity, 0u) {

}

StreamStringIOBuffer::~StreamStringIOBuffer() {

}

bool StreamStringIOBuffer::SetBufferAllocationSize(const uint32 desiredSize) {

    bool ret;

    //add one to desired size for the terminator character.
    ret = SetBufferHeapMemory(desiredSize + 1U, 1U);

    if (ret) {
        if (desiredSize < UsedSize()) {
            SetUsedSize(desiredSize);
        }

        Terminate();
    }

    return ret;
}

bool StreamStringIOBuffer::Write(const char8 * const buffer,
                                 uint32 &size) {

    bool ret = true;

    if (size > AmountLeft()) {
        ret = SetBufferAllocationSize(Position() + size);
    }

    if (ret) {
        ret = IOBuffer::Write(buffer, size);
    }

    return ret;
}

bool StreamStringIOBuffer::NoMoreSpaceToWrite() {

    bool ret;

    // reallocate buffer
    // uses safe version of the function
    // implemented in this class
    ret = SetBufferAllocationSize(GetBufferSize() + 1u);

    return ret;
}

bool StreamStringIOBuffer::NoMoreSpaceToWrite(const uint32 neededSize) {

    bool ret;

    // reallocate buffer
    // uses safe version of the function
    // implemented in this class
    ret = SetBufferAllocationSize(GetBufferSize() + neededSize);

    return ret;
}

void StreamStringIOBuffer::Terminate() {
    if (BufferReference() != NULL) {
        BufferReference()[UsedSize()] = '\0';
    }
}

}
