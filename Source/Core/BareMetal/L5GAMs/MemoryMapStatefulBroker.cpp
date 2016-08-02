/**
 * @file MemoryMapStatefulBroker.cpp
 * @brief Source file for class MemoryMapStatefulBroker
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
 * the class MemoryMapStatefulBroker (public, protected, and private). Be aware that some
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
#include "MemoryMapStatefulBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapStatefulBroker::MemoryMapStatefulBroker() :
        BrokerI() {
    copyTable = NULL_PTR(MemoryMapStatefulBrokerCopyTableEntry *);
    numberOfCopies = 0u;
    dataSource = NULL_PTR(DataSourceI*);
}

MemoryMapStatefulBroker::~MemoryMapStatefulBroker() {
    if (copyTable != NULL_PTR(MemoryMapStatefulBrokerCopyTableEntry *)) {
        delete[] copyTable;
    }
    /*lint -e{1740} dataSource contains a copy of a pointer. No need to be freed.*/
}

bool MemoryMapStatefulBroker::Init(const SignalDirection direction,
                                   DataSourceI &dataSourceIn,
                                   const char8 * const functionName,
                                   void * const gamMemoryAddress) {
    dataSource = &dataSourceIn;

    bool ret = InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);

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
        copyTable = new MemoryMapStatefulBrokerCopyTableEntry[numberOfCopies];
    }
    uint32 functionIdx = 0u;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    //The same signal can be copied from different ranges. A MemoryMapStatefulBrokerCopyTableEntry is added for each signal range.
    uint32 c = 0u;
    uint32 n;
    for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
        if (dataSource->IsSupportedBroker(direction, functionIdx, n, brokerClassName)) {
            uint32 numberOfByteOffsets = 0u;
            ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);

            StreamString functionSignalName;
            if (ret) {
                ret = dataSource->GetFunctionSignalAlias(direction, functionIdx, n, functionSignalName);
            }
            uint32 signalIdx = 0u;
            if (ret) {
                ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
            }
            //Take into account different ranges for the same signal
            uint32 bo;
            for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                if (copyTable != NULL_PTR(MemoryMapStatefulBrokerCopyTableEntry *)) {
                    copyTable[c].copySize = GetCopyByteSize(c);
                    copyTable[c].gamPointer = GetFunctionPointer(c);
                    copyTable[c].dataSourceOffset = GetCopyOffset(c);
                    uint32 b;
                    for (b = 0u; (b < 2u) && (ret); b++) {
                        void *dataSourceSignalAddress;
                        ret = dataSource->GetSignalMemoryBuffer(signalIdx, b, dataSourceSignalAddress);
                        if (ret) {
                            //lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: two pointer indirection required to access the address of the variable that holds the final address of the double buffer
                            copyTable[c].dataSourcePointer[b] = reinterpret_cast<void **>(dataSourceSignalAddress);
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
