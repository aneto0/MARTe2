/**
 * @file CharBuffer.cpp
 * @brief Source file for class CharBuffer
 * @date 05/08/2015
 * @author Filippo Sartori
 * @author Andre' Neto
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
 * the class ClassProperties (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CharBuffer.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

CharBuffer::CharBuffer() {
    bufferSize = 0u;
    buffer = NULL_PTR(char8 *);
    readOnly = true;
    allocated = false;
    allocationGranularityMask = 0xFFFFFFFFu;
}

CharBuffer::CharBuffer(const uint32 allocationGranularity) {
    bufferSize = 0u;
    buffer = NULL_PTR(char8 *);
    readOnly = true;
    allocated = false;

    {
        uint32 granularityTest = 1u;
        // round the granularity mask at the first 2^x up
        for (uint32 i = 0u; i < 31u; i++) {
            if (granularityTest >= allocationGranularity) {
                break;
            }
            granularityTest <<= 1u;
        }
        // sets the allocation granularity mask
        allocationGranularityMask = ~(granularityTest - 1u);
    }
}

void CharBuffer::Reset() {
    if (allocated) {
        if (buffer != NULL) {
            if (HeapManager::Free(reinterpret_cast<void *&>(buffer))) {
                bufferSize = 0u;
            }
        }
    }
    bufferSize = 0u;
    buffer = NULL_PTR(char8 *);
    readOnly = true;
    allocated = false;
}

/*lint -e{1551} The free should not cause a segmentation fault given that buffer != NULL is checked
 * the pointer is properly initialised and is allocated by this class (so it should stay valid).*/
CharBuffer::~CharBuffer() {
    Reset();
}

bool CharBuffer::SetBufferSize(const uint32 desiredSize) {

    /* granularity = ~granularityMask + 1 (2's complement).
     * (e.g. granularityMask=0xfffffff0 ==> 4 zeros ==> granularity: 2^4=16 bytes)
     * If desiredSize < granularity => desiredSize = granularity
     */

    bool ok = true;
    // If memory reference is present remove it otherwise we end up reallocating others's memory!!
    if ((bufferSize > 0u) && (!allocated)) {
        Reset();
    }

    //the mask is the 2 complement of the actual granularity
    uint32 allocationGranularity = ~allocationGranularityMask + 1u;
    uint32 allocationBoundary = allocationGranularityMask;

    // stay within mathematical limits
    if (desiredSize > allocationBoundary) {
        ok = false;
    }

    if (ok) {
        // Increase up to granularity boundaries
        uint32 neededMemory = desiredSize + allocationGranularity;
        neededMemory -= 1u;
        neededMemory &= allocationGranularityMask;

        if (buffer == NULL) {
            buffer = static_cast<char8 *>(HeapManager::Malloc(neededMemory));
        }
        else {
            buffer = static_cast<char8 *>(HeapManager::Realloc(reinterpret_cast<void *&>(buffer), neededMemory));
        }
        // if the pointer is not NULL it means we have been successful
        if (buffer != NULL) {
            bufferSize = neededMemory;
            readOnly = false;
            allocated = true;
        }
        else {
            bufferSize = 0u;
            ok = false;
        }
    }
    return ok;
}

void CharBuffer::SetBufferReference(char8 * const buff,
                                    const uint32 buffSize) {
    Reset();
    buffer = buff;
    bufferSize = buffSize;
    readOnly = false;
}

void CharBuffer::SetBufferReference(const char8 * const buff,
                                    const uint32 buffSize) {
    Reset();
    buffer = const_cast<char8 *>(buff);
    bufferSize = buffSize;
}

}
