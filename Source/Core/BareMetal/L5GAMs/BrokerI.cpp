/**
 * @file BrokerI.cpp
 * @brief Source file for class BrokerI
 * @date 17/05/2016
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
 * the class BrokerI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
BrokerI::BrokerI() :
        Object(),
        ExecutableI() {

    numberOfCopies = 0u;
    functionSignalPointers = NULL_PTR(void **);
    copyByteSize = NULL_PTR(uint32 *);
    copyOffset = NULL_PTR(uint32 *);
}

BrokerI::~BrokerI() {
    if (functionSignalPointers != NULL_PTR(void **)) {
        delete[] functionSignalPointers;
    }
    if (copyByteSize != NULL_PTR(uint32 *)) {
        delete[] copyByteSize;
    }
    if (copyOffset != NULL_PTR(uint32 *)) {
        delete[] copyOffset;
    }
}

uint32 BrokerI::GetNumberOfCopies() const {
    return numberOfCopies;
}

uint32 BrokerI::GetCopyByteSize(const uint32 copyIdx) const {
    uint32 ret = 0u;
    if (copyIdx < numberOfCopies) {
        if (copyByteSize != NULL_PTR(uint32 *)) {
            ret = copyByteSize[copyIdx];
        }
    }
    return ret;
}

uint32 BrokerI::GetCopyOffset(const uint32 copyIdx) const {
    uint32 ret = 0u;
    if (copyIdx < numberOfCopies) {
        if (copyOffset != NULL_PTR(uint32 *)) {
            ret = copyOffset[copyIdx];
        }
    }
    return ret;
}

void *BrokerI::GetFunctionPointer(const uint32 copyIdx) const {
    void * ret = NULL_PTR(void *);
    if (copyIdx < numberOfCopies) {
        if (functionSignalPointers != NULL_PTR(void **)) {
            ret = functionSignalPointers[copyIdx];
        }
    }
    return ret;
}

bool BrokerI::InitFunctionPointers(const SignalDirection direction,
                                   DataSourceI &dataSource,
                                   const char8 * const functionName,
                                   void * const gamMemoryAddress) {

    //Need to check the broker class name. This function loops through all the signals of the
    //functionName and should only react to the signals which are related to this BrokerI instance.
    //Get the class name from the ClassProperties
    const ClassProperties * properties = GetClassProperties();
    bool ret = (properties != NULL);
    const char8* brokerClassName = NULL_PTR(const char8*);
    if (ret) {
        brokerClassName = properties->GetName();
        ret = (brokerClassName != NULL);
    }
    //Find the function
    uint32 functionIdx = 0u;
    if (ret) {
        ret = dataSource.GetFunctionIndex(functionIdx, functionName);
    }
    //For this function, how many signals in this direction
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource.GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    numberOfCopies = 0u;
    uint32 numberOfByteOffsets = 0u;

    uint32 i;
    for (i = 0u; (i < functionNumberOfSignals) && (ret); i++) {
        if (dataSource.IsSupportedBroker(direction, functionIdx, i, brokerClassName)) {
            ret = dataSource.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, i, numberOfByteOffsets);
            if (ret) {
                //One copy for each signal but each signal might want to copy several pieces (i.e. offsets)
                numberOfCopies += numberOfByteOffsets;
            }
        }
    }
    if (numberOfCopies > 0u) {
        functionSignalPointers = new void*[numberOfCopies];
        copyByteSize = new uint32[numberOfCopies];
        copyOffset = new uint32[numberOfCopies];

        uint32 memoryOffset = 0u;
        uint32 samples = 0u;
        uint32 c = 0u;
        //The same signal can be copied from different ranges. An entry is added for each signal range.
        // The pointer of the gam memory for this DataSource!
        ret = (gamMemoryAddress != NULL_PTR(void *));

        for (i = 0u; (i < functionNumberOfSignals) && (ret); i++) {
            ret = dataSource.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, i, numberOfByteOffsets);

            if (ret) {
                ret = dataSource.GetFunctionSignalGAMMemoryOffset(direction, functionIdx, i, memoryOffset);
            }
            //Take into account  different ranges for the same signal
            for (uint32 j = 0u; (j < numberOfByteOffsets) && (ret); j++) {
                uint32 offsetStart;
                uint32 copySize;
                ret = dataSource.GetFunctionSignalByteOffsetInfo(direction, functionIdx, i, j, offsetStart, copySize);
                if (ret) {
                    ret = dataSource.GetFunctionSignalSamples(direction, functionIdx, i, samples);
                }
                if (samples == 0u) {
                    samples = 1u;
                }
                if (ret) {
                    char8 *gamMemoryCharAddress = reinterpret_cast<char8 *>(gamMemoryAddress);
                    //gamMemoryCharAddress += memoryOffset;

                    if (dataSource.IsSupportedBroker(direction, functionIdx, i, brokerClassName)) {
                        copyByteSize[c] = copySize * samples;
                        copyOffset[c] = offsetStart;
                        functionSignalPointers[c] = reinterpret_cast<void *>(&gamMemoryCharAddress[memoryOffset]);
                        c++;
                    }
                    memoryOffset += copySize;
                }
            }
        }
    }
    return ret;
}

}
