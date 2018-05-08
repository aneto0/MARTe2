/**
 * @file MemoryDataSourceI.cpp
 * @brief Source file for class MemoryDataSourceI
 * @date 07/05/2018
 * @author Andre Neto
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
 * the class MemoryDataSourceI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryDataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryDataSourceI::MemoryDataSourceI() {
    numberOfBuffers = 0u;
    stateMemorySize = 0u;
    totalMemorySize = 0u;
    memory = NULL_PTR(uint8 *);
    signalOffsets = NULL_PTR(uint32 *);
    memoryHeap = NULL_PTR(HeapI *);
    signalSize = NULL_PTR(uint32 *);
}

MemoryDataSourceI::~MemoryDataSourceI() {
    if (memoryHeap != NULL_PTR(HeapI *)) {
        if (memory != NULL_PTR(uint8 *)) {
            /*lint -e{1551} HeapManager::Free is expected to be exception free*/
            memoryHeap->Free(reinterpret_cast<void *&>(memory));
            memory = NULL_PTR(uint8 *);
        }
        memoryHeap = NULL_PTR(HeapI *);
    }
    if (signalOffsets != NULL_PTR(uint32 *)) {
        delete[] signalOffsets;
        signalOffsets = NULL_PTR(uint32 *);
    }

    if (signalSize != NULL_PTR(uint32 *)) {
        delete[] signalSize;
        signalSize = NULL_PTR(uint32 *);
    }
}

bool MemoryDataSourceI::AllocateMemory() {

    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = (memory == NULL_PTR(uint8 *));
    if (ret) {
        if (nOfSignals > 0u) {
            signalOffsets = new uint32[nOfSignals];
            ret = (signalOffsets != NULL_PTR(uint32*));
            if (ret) {
                signalSize = new uint32[nOfSignals];
                ret = (signalSize != NULL_PTR(uint32*));
            }
        }
    }

    stateMemorySize = 0u;
    for (uint32 s = 0u; (s < nOfSignals) && (ret); s++) {
        uint32 thisSignalMemorySize;
        ret = GetSignalByteSize(s, thisSignalMemorySize);

        if (ret) {
            if (signalOffsets != NULL_PTR(uint32 *)) {
                signalOffsets[s] = stateMemorySize;
            }
        }
        if (ret) {
            ret = (thisSignalMemorySize > 0u);
        }
        if (ret) {
            stateMemorySize += (thisSignalMemorySize * numberOfBuffers);
            /*lint -e{613} null pointer checked before.*/
            signalSize[s] = thisSignalMemorySize;
        }
    }
    uint32 numberOfStateBuffers = GetNumberOfStatefulMemoryBuffers();
    if (ret) {
        ret = (numberOfStateBuffers > 0u);
    }
    if (ret) {
        totalMemorySize = stateMemorySize * numberOfStateBuffers;
        if (memoryHeap != NULL_PTR(HeapI *)) {
            memory = reinterpret_cast<uint8 *>(memoryHeap->Malloc(totalMemorySize));
        }
        ret = MemoryOperationsHelper::Set(memory, '\0', totalMemorySize);
    }
    return ret;

}

uint32 MemoryDataSourceI::GetNumberOfMemoryBuffers() {
    return numberOfBuffers;
}

bool MemoryDataSourceI::Initialise(StructuredDataI & data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        ret = data.Read("NumberOfBuffers", numberOfBuffers);
        if (!ret) {
            ret = true;
            numberOfBuffers = 1u;
            REPORT_ERROR(ErrorManagement::Warning, "NumberOfBuffers was not specified. Using default: %d", numberOfBuffers);
        }
    }

    if (ret) {
        StreamString heapName;
        if (data.Read("HeapName", heapName)) {
            memoryHeap = HeapManager::FindHeap(heapName.Buffer());
            if (memoryHeap == NULL_PTR(HeapI *)) {
                ret = false;
                REPORT_ERROR(ErrorManagement::Warning, "Could not instantiate an memoryHeap with the name: %s", heapName.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "Using the standard heap");
            memoryHeap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
        }
    }
    return ret;
}

bool MemoryDataSourceI::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ret = (bufferIdx < GetNumberOfStatefulMemoryBuffers());
    uint32 nOfSignals = GetNumberOfSignals();
    if (ret) {
        ret = (signalIdx < nOfSignals);
    }

    if (ret) {
        char8 *signalAddressChar = reinterpret_cast<char8 *>(memory);
        //Move to the correct state buffer
        uint32 stateBufferOffset = stateMemorySize * bufferIdx;
        signalAddressChar = &signalAddressChar[stateBufferOffset];

        uint32 offset = 0u;
        if (signalOffsets != NULL_PTR(uint32 *)) {
            offset = signalOffsets[signalIdx];
        }
        if (signalAddressChar != NULL_PTR(char8 *)) {
            signalAddressChar = &signalAddressChar[offset];
            signalAddress = reinterpret_cast<void *&>(signalAddressChar);
        }
    }

    return ret;
}

}
