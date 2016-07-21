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
    copyTable = NULL_PTR(CopyTableEntry *);
    numberOfCopies = 0u;
    numberOfDataSourceSignalBuffers = 0u;
}

MemoryMapBroker::~MemoryMapBroker() {
    uint32 n;
    for (n = 0u; n < numberOfCopies; n++) {
        delete[] copyTable[n].dataSourcePointers;
    }
    if (copyTable != NULL_PTR(CopyTableEntry *)) {
        delete[] copyTable;
    }
}

bool MemoryMapBroker::Init(SignalDirection direction,
                           DataSourceI *dataSourceIn,
                           const char8 * const functionName) {
    dataSource = dataSourceIn;

    bool ret = InitFunctionPointers(direction, dataSource, functionName);

    if (ret) {
        ret = (numberOfCopies > 0u);
    }
    if (ret) {
        copyTable = new CopyTableEntry[numberOfCopies];
    }
    uint32 functionIdx;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    numberOfDataSourceSignalBuffers = dataSource->GetNumberOfMemoryBuffers();
    //The same signal can be copied from different ranges. A CopyTableEntry is added for each signal range.
    uint32 c = 0u;
    uint32 n;
    for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
        uint32 numberOfByteOffsets = 0u;
        if (ret) {
            ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);
        }
        StreamString functionSignalName;
        if (ret) {
            ret = dataSource->GetFunctionSignalName(direction, functionIdx, n, functionSignalName);
        }
        uint32 signalIdx;
        if (ret) {
            ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
        }
        //Take into account different ranges for the same signal
        uint32 bo;
        for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
            uint32 offsetStart = GetCopyOffset(c);
            if (ret) {
                copyTable[c].copySize = GetCopyByteSize(c);
                copyTable[c].gamPointer = GetFunctionPointer(c);
                copyTable[c].dataSourcePointers = new void*[numberOfDataSourceSignalBuffers];
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
    return ret;
}

bool MemoryMapBroker::Init2(SignalDirection direction,
                            DataSourceI &dataSourceIn,
                            const char8 * const functionName,
                            void *gamMemoryAddress) {
    dataSource = &dataSourceIn;

    bool ret = InitFunctionPointers2(direction, dataSourceIn, functionName, gamMemoryAddress);

    const ClassProperties * properties = GetClassProperties();
    if (ret) {
        ret = (properties != NULL);
    }
    const char8* brokerClassName = NULL_PTR(const char8*);
    if (ret) {
        brokerClassName = properties->GetName();
        ret = (brokerClassName != NULL);
    }

    if (ret) {
        ret = (numberOfCopies > 0u);
    }
    if (ret) {
        copyTable = new CopyTableEntry[numberOfCopies];
    }
    uint32 functionIdx;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    numberOfDataSourceSignalBuffers = dataSource->GetNumberOfMemoryBuffers();
    //The same signal can be copied from different ranges. A CopyTableEntry is added for each signal range.
    uint32 c = 0u;
    for (uint32 n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
        if (dataSource->IsSupportedBroker(direction, functionIdx, n, brokerClassName)) {
            uint32 numberOfByteOffsets = 0u;
            if (ret) {
                ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);
            }
            StreamString functionSignalName;
            if (ret) {
                ret = dataSource->GetFunctionSignalAlias(direction, functionIdx, n, functionSignalName);
            }
            uint32 signalIdx;
            if (ret) {
                ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
            }
            //Take into account different ranges for the same signal
            uint32 bo;
            for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                uint32 offsetStart = GetCopyOffset(c);
                if (ret) {
                    copyTable[c].copySize = GetCopyByteSize(c);
                    copyTable[c].gamPointer = GetFunctionPointer(c);
                    copyTable[c].dataSourcePointers = new void*[numberOfDataSourceSignalBuffers];
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

}
