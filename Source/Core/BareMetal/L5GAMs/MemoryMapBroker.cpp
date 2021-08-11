/**
 * @file MemoryMapBroker.cpp
 * @brief Source file for class MemoryMapBroker
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
 * the class MemoryMapBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

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
    copyTable = NULL_PTR(MemoryMapBrokerCopyTableEntry*);
    dataSource = NULL_PTR(DataSourceI*);
    numberOfCopies = 0u;
}

MemoryMapBroker::~MemoryMapBroker() {
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*)) {
        delete[] copyTable;
    }
    /*lint -e{1740} dataSource contains a copy of a pointer. No need to be freed.*/
}

bool MemoryMapBroker::Init(const SignalDirection direction,
                           DataSourceI &dataSourceIn,
                           const char8 *const functionName,
                           void *const gamMemoryAddress) {

    bool ret = Init(direction, dataSourceIn, functionName, gamMemoryAddress, false);
    return ret;
}

bool MemoryMapBroker::Init(const SignalDirection direction,
                           DataSourceI &dataSourceIn,
                           const char8 *const functionName,
                           void *const gamMemoryAddress,
                           const bool optim) {

    dataSource = &dataSourceIn;

    bool ret;
    if (optim) {
        ret = InitFunctionPointersOptim(direction, dataSourceIn, functionName, gamMemoryAddress);
    }
    else {
        ret = InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);
    }

    const ClassProperties *properties = GetClassProperties();
    if (ret) {
        ret = (properties != NULL);
    }
    const char8 *brokerClassName = NULL_PTR(const char8*);
    if (ret) {
        brokerClassName = properties->GetName();
        ret = (brokerClassName != NULL);
    }

    if (ret) {
        ret = (numberOfCopies > 0u);
    }
    uint32 numberOfBuffers = dataSource->GetNumberOfStatefulMemoryBuffers();
    if (ret) {
        uint32 totalNumberOfElements = (numberOfCopies * numberOfBuffers);
        copyTable = new MemoryMapBrokerCopyTableEntry[totalNumberOfElements];
        ret = (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*));

    }
    uint32 functionIdx = 0u;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
//    uint32 functionNumberOfSignals = 0u;
//    if (ret) {
//        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
//    }
    //The same signal can be copied from different ranges. A MemoryMapBrokerCopyTableEntry is added for each signal range.
    uint32 c = 0u;
    for (uint32 c0 = 0u; (c0 < numberOfBuffers) && (ret); c0++) {
        //lint -e{613} Possible use of null pointer. Justification: if ret == true --> copyTable != NULL
        for (uint32 numberOfCopiesIdx = 0u; (numberOfCopiesIdx < numberOfCopies) && (ret); numberOfCopiesIdx++) {
            //if (dataSource->IsSupportedBroker(direction, functionIdx, n, brokerClassName)) {
            copyTable[c].copySize = GetCopyByteSize(numberOfCopiesIdx);
            copyTable[c].gamPointer = GetFunctionPointer(numberOfCopiesIdx);
            //To maintain compatibility with previous code. Currently, signal type make no sense since we are copying several signals (of several types) at once. Nevertheless MemoryMapInputBroker uses it.
            copyTable[c].type = dataSource->GetSignalType(GetDSCopySignalIndex(numberOfCopiesIdx));
            uint32 dataSourceOffset = GetCopyOffset(numberOfCopiesIdx);

            void *dataSourceSignalAddress;
            ret = dataSource->GetSignalMemoryBuffer(GetDSCopySignalIndex(numberOfCopiesIdx), c0, dataSourceSignalAddress);
            char8 *dataSourceSignalAddressChar = reinterpret_cast<char8*>(dataSourceSignalAddress);
            if (ret) {
                dataSourceSignalAddressChar = &dataSourceSignalAddressChar[dataSourceOffset];
                copyTable[c].dataSourcePointer = reinterpret_cast<void*>(dataSourceSignalAddressChar);
            }
            c++;
        }
//        }
    }
    return ret;
}

}
