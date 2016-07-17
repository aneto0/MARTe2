/**
 * @file MemoryMapDataSourceBroker.cpp
 * @brief Source file for class MemoryMapDataSourceBroker
 * @date 11/04/2016
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
 * the class MemoryMapDataSourceBroker (public, protected, and private). Be aware that some 
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
#include "GAMGenericSignal.h"
#include "GAMSampledSignal.h"
#include "MemoryMapBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapBroker::MemoryMapBroker() :
        BrokerI() {
    inputCopyTable = NULL_PTR(CopyTableEntry *);
    outputCopyTable = NULL_PTR(CopyTableEntry *);
    numberOfInputCopies = 0u;
    numberOfOutputCopies = 0u;
    numberOfDataSourceSignalBuffers = 0u;
}

MemoryMapBroker::~MemoryMapBroker() {
    uint32 n;
    for (n = 0u; n < numberOfInputCopies; n++) {
        delete[] inputCopyTable[n].dataSourcePointers;
    }
    for (n = 0u; n < numberOfOutputCopies; n++) {
        delete[] outputCopyTable[n].dataSourcePointers;
    }
    if (inputCopyTable != NULL_PTR(CopyTableEntry *)) {
        delete[] inputCopyTable;
    }
    if (outputCopyTable != NULL_PTR(CopyTableEntry *)) {
        delete[] outputCopyTable;
    }
}

bool MemoryMapBroker::Read(const TimeoutType &timeout) {
    uint32 n;
    uint32 dataSourceIdx = dataSource->GetCurrentBufferIndex();
    for (n = 0u; n < numberOfInputCopies; n++) {
        MemoryOperationsHelper::Copy(inputCopyTable[n].gamPointer, inputCopyTable[n].dataSourcePointers[dataSourceIdx], inputCopyTable[n].copySize);
    }
    return true;
}

bool MemoryMapBroker::Write(const TimeoutType &timeout) {
    uint32 n;
    uint32 dataSourceIdx = dataSource->GetCurrentBufferIndex();
    for (n = 0u; n < numberOfOutputCopies; n++) {
        MemoryOperationsHelper::Copy(outputCopyTable[n].dataSourcePointers[dataSourceIdx], outputCopyTable[n].gamPointer, outputCopyTable[n].copySize);
    }
    return true;
}

#if 0
bool MemoryMapBroker::Finalise() {
    bool ok = true;
    uint32 i = 0;
    uint32 numberOfSignals = 0u;
    numberOfSignalCopies = 0u;
    for (i = 0; (i < numberOfSignals) && (ok); i++) {
        uint32 numberOfSamplesBlocks = GetNumberOfSamplesBlocks(i);
        uint32 numberOfIndexBlocks = GetNumberOfIndexBlocks(i);
        numberOfSignalCopies += numberOfSamplesBlocks * numberOfIndexBlocks;
    }

    copyTable = new CopyTableEntry[numberOfSignalCopies];
    uint32 copySignalIndex = 0u;
    for (i = 0; (i < numberOfSignals) && (ok); i++) {
        uint32 numberOfSamplesBlocks = GetNumberOfSamplesBlocks(i);
        uint32 numberOfIndexBlocks = GetNumberOfIndexBlocks(i);
        uint32 s = 0;
        for (s = 0; (s < numberOfSamplesBlocks) && (ok); s++) {
            uint32 b = 0;
            for (b = 0; (b < numberOfIndexBlocks) && (ok); b++) {
                copyTable[copySignalIndex].gamPointer = GetGAMSignalPointer(i, s, b);
                copyTable[copySignalIndex].dataSourcePointer0 = GetDataSourceSignalPointer(i, s, b, 0u);
                copyTable[copySignalIndex].dataSourcePointer1 = GetDataSourceSignalPointer(i, s, b, 1u);
                ok = GetIndexBlockSize(i, b, copyTable[copySignalIndex].copySize);
                copySignalIndex++;
            }
        }
    }
    SetFinalised(ok);
    return ok;
}

bool MemoryMapBroker::Read(const uint8 activeDataSourceBuffer,
        const TimeoutType &timeout) {

    bool ok = IsFinalised();
    for (uint32 i = 0u; (i < numberOfSignalCopies) && (ok); i++) {
        if (activeDataSourceBuffer == 0u) {
            ok = MemoryOperationsHelper::Copy(copyTable[i].gamPointer, copyTable[i].dataSourcePointer0, copyTable[i].copySize);
        }
        else {
            ok = MemoryOperationsHelper::Copy(copyTable[i].gamPointer, copyTable[i].dataSourcePointer1, copyTable[i].copySize);
        }
    }
    return ok;
}

bool MemoryMapBroker::Write(const uint8 activeDataSourceBuffer,
        const TimeoutType &timeout) {

    bool ok = IsFinalised();
    for (uint32 i = 0u; (i < numberOfSignalCopies) && (ok); i++) {
        if (activeDataSourceBuffer == 0u) {
            ok = MemoryOperationsHelper::Copy(copyTable[i].dataSourcePointer0, copyTable[i].gamPointer, copyTable[i].copySize);
        }
        else {
            ok = MemoryOperationsHelper::Copy(copyTable[i].dataSourcePointer1, copyTable[i].gamPointer, copyTable[i].copySize);
        }
    }
    return ok;
}
#endif

bool MemoryMapBroker::Init(SignalDirection direction,
                           ReferenceT<DataSourceI> dataSourceIn,
                           const char8 * const functionName) {

    CopyTableEntry *copyTable = NULL_PTR(CopyTableEntry *);

    dataSource = dataSourceIn;
    //Find the function
    uint32 functionIdx;
    bool ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    //For this function, how many signals in this direction
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    uint32 numberOfCopies = 0u;
    uint32 numberOfByteOffsets = 0u;
    uint32 n;
    for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
        if (ret) {
            ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);
        }
        if (ret) {
            //One copy for each signal but each signal might want to copy several pieces (i.e. offsets)
            numberOfCopies += numberOfByteOffsets;
        }
    }
    if (numberOfCopies > 0u) {
        copyTable = new CopyTableEntry[numberOfCopies];
        if (direction == InputSignals) {
            inputCopyTable = copyTable;
            numberOfInputCopies = numberOfCopies;
        }
        else {
            outputCopyTable = copyTable;
            numberOfOutputCopies = numberOfCopies;
        }

        uint32 memoryOffset = 0u;
        uint32 c = 0u;
        numberOfDataSourceSignalBuffers = dataSource->GetNumberOfMemoryBuffers();
        //The same signal can be copied from different ranges. A CopyTableEntry is added for each signal range.
        for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
            void *gamMemoryAddress = NULL_PTR(void *);
            ret = dataSource->GetFunctionSignalsAddress(direction, functionIdx, gamMemoryAddress);
            if (ret) {
                ret = (gamMemoryAddress != NULL_PTR(void *));
            }
            if (ret) {
                ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);
            }

            uint32 bo;
            //Take into account different ranges for the same signal
            for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                uint32 offsetStart;
                uint32 copySize;
                ret = dataSource->GetFunctionSignalByteOffsetInfo(direction, functionIdx, n, bo, offsetStart, copySize);
                if (ret) {
                    copyTable[c].copySize = copySize;

                    char8 *gamMemoryCharAddress = reinterpret_cast<char8 *>(gamMemoryAddress);
                    gamMemoryCharAddress += memoryOffset;
                    copyTable[c].gamPointer = reinterpret_cast<void *>(gamMemoryCharAddress);
                    memoryOffset += copySize;

                    copyTable[c].dataSourcePointers = new void*[numberOfDataSourceSignalBuffers];
                }
                if (ret) {
                    StreamString functionSignalName;
                    ret = dataSource->GetFunctionSignalName(direction, functionIdx, n, functionSignalName);
                    uint32 signalIdx;
                    if (ret) {
                        ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
                    }
                    uint32 b;
                    for (b = 0u; (b < numberOfDataSourceSignalBuffers) && (ret); b++) {
                        void *dataSourceSignalAddress;
                        ret = dataSource->GetSignalMemoryBuffer(signalIdx, b, dataSourceSignalAddress);
                        if (ret) {
                            char8 *dataSourceSignalCharAddress = reinterpret_cast<char8 *>(dataSourceSignalAddress);
                            dataSourceSignalCharAddress += offsetStart;
                            copyTable[c].dataSourcePointers[b] = reinterpret_cast<void *>(dataSourceSignalCharAddress);
                        }
                    }
                }
                c++;
            }
        }
    }
    return ret;
}

CLASS_REGISTER(MemoryMapBroker, "1.0");
}
