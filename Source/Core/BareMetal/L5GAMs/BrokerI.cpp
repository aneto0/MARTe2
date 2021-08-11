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
    copyTableInfo = NULL_PTR(memoryInfo*);
}

BrokerI::~BrokerI() {
    if (copyTableInfo != NULL_PTR(memoryInfo*)) {
        delete[] copyTableInfo;
        copyTableInfo = NULL_PTR(memoryInfo*);
    }
}

uint32 BrokerI::GetNumberOfCopies() const {
    return numberOfCopies;
}

uint32 BrokerI::GetCopyByteSize(const uint32 copyIdx) const {
    uint32 ret = 0u;
    if (copyIdx < numberOfCopies) {
        if (copyTableInfo != NULL_PTR(memoryInfo*)) {
            ret = copyTableInfo[copyIdx].copyByteSize;
        }
    }
    return ret;
}

uint32 BrokerI::GetCopyOffset(const uint32 copyIdx) const {
    uint32 ret = 0u;
    if (copyIdx < numberOfCopies) {
        if (copyTableInfo != NULL_PTR(memoryInfo*)) {
            ret = copyTableInfo[copyIdx].copyOffset;
        }
    }
    return ret;
}

uint32 BrokerI::GetDSCopySignalIndex(const uint32 copyIdx) const {
    uint32 ret = 0u;
    if (copyIdx < numberOfCopies) {
        if (copyTableInfo != NULL_PTR(memoryInfo*)) {
            ret = copyTableInfo[copyIdx].copySignalIndex;
        }
    }
    return ret;
}

void* BrokerI::GetFunctionPointer(const uint32 copyIdx) const {
    void *ret = NULL_PTR(void*);
    if (copyIdx < numberOfCopies) {
        if (copyTableInfo != NULL_PTR(memoryInfo*)) {
            if (copyTableInfo[copyIdx].functionSignalPointers != NULL_PTR(void*)) {
                ret = copyTableInfo[copyIdx].functionSignalPointers;
            }
        }
    }
    return ret;
}

bool BrokerI::InitFunctionPointers(const SignalDirection direction,
                                   DataSourceI &dataSource,
                                   const char8 *const functionName,
                                   void *const gamMemoryAddress) {

    ownerDataSourceName = dataSource.GetName();
    ownerFunctionName = functionName;
    //Need to check the broker class name. This function loops through all the signals of the
    //functionName and should only react to the signals which are related to this BrokerI instance.
    //Get the class name from the ClassProperties
    const ClassProperties *properties = GetClassProperties();
    bool ret = (properties != NULL);
    const char8 *brokerClassName = NULL_PTR(const char8*);
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
    uint32 samples = 0u;
    uint32 i;
    for (i = 0u; (i < functionNumberOfSignals) && (ret); i++) {
        if (dataSource.IsSupportedBroker(direction, functionIdx, i, brokerClassName)) {
            ret = dataSource.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, i, numberOfByteOffsets);

            if (ret) {
                ret = dataSource.GetFunctionSignalSamples(direction, functionIdx, i, samples);
            }

            if (ret) {
                //One copy for each signal but each signal might want to copy several pieces (i.e. offsets)
                if (numberOfByteOffsets > 1u) {
                    numberOfCopies += (numberOfByteOffsets * samples);
                }
                else {
                    numberOfCopies++;
                }
            }
        }
    }
    if (numberOfCopies > 0u) {
        copyTableInfo = new memoryInfo[numberOfCopies];

        samples = 0u;
        uint32 memoryOffset = 0u;
        uint32 byteSize = 0u;
        uint32 c = 0u;
        //The same signal can be copied from different ranges. An entry is added for each signal range.
        // The pointer of the gam memory for this DataSource!
        ret = (gamMemoryAddress != NULL_PTR(void*));

        for (i = 0u; (i < functionNumberOfSignals) && (ret); i++) {
            if (dataSource.IsSupportedBroker(direction, functionIdx, i, brokerClassName)) {

                ret = dataSource.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, i, numberOfByteOffsets);

                if (ret) {
                    ret = dataSource.GetFunctionSignalGAMMemoryOffset(direction, functionIdx, i, memoryOffset);
                }
                StreamString functionSignalName;
                if (ret) {
                    ret = dataSource.GetFunctionSignalAlias(direction, functionIdx, i, functionSignalName);
                }
                uint32 signalIdx = 0u;
                if (ret) {
                    ret = dataSource.GetSignalIndex(signalIdx, functionSignalName.Buffer());
                }
                if (ret) {
                    ret = dataSource.GetSignalByteSize(signalIdx, byteSize);
                }
                if (ret) {
                    ret = dataSource.GetFunctionSignalSamples(direction, functionIdx, i, samples);
                }
                if (ret) {
                    if (samples == 0u) {
                        samples = 1u;
                    }
                }
                bool noRanges = true;
                uint32 offsetStart = 0u;
                uint32 copySize = 0u;
                if (ret) {
                    ret = dataSource.GetFunctionSignalByteOffsetInfo(direction, functionIdx, i, 0u, offsetStart, copySize);
                    if (ret) {
                        noRanges = ((numberOfByteOffsets == 1u) && (copySize == byteSize));
                    }
                }
                if (noRanges) {
                    ret = dataSource.GetFunctionSignalByteOffsetInfo(direction, functionIdx, i, 0u, offsetStart, copySize);
                    if (ret) {
                        char8 *gamMemoryCharAddress = reinterpret_cast<char8*>(gamMemoryAddress);
                        //gamMemoryCharAddress += memoryOffset;
                        copyTableInfo[c].copyByteSize = copySize * samples;
                        copyTableInfo[c].copyOffset = offsetStart;
                        copyTableInfo[c].functionSignalPointers = reinterpret_cast<void*>(&gamMemoryCharAddress[memoryOffset]);
                        copyTableInfo[c].copySignalIndex = signalIdx;
                        //in the gam shift the sub-block size
                        c++;
                    }
                }
                else {
                    //Take into account  different ranges for the same signal
                    for (uint32 j = 0u; (j < numberOfByteOffsets) && (ret); j++) {
                        if (ret) {
                            char8 *gamMemoryCharAddress = reinterpret_cast<char8*>(gamMemoryAddress);
                            ret = dataSource.GetFunctionSignalByteOffsetInfo(direction, functionIdx, i, j, offsetStart, copySize);
                            //gamMemoryCharAddress += memoryOffset;
                            for (uint32 h = 0u; h < samples; h++) {
                                copyTableInfo[c].copyByteSize = copySize;
                                copyTableInfo[c].copyOffset = offsetStart;
                                copyTableInfo[c].functionSignalPointers = reinterpret_cast<void*>(&gamMemoryCharAddress[memoryOffset]);
                                copyTableInfo[c].copySignalIndex = signalIdx;
                                c++;
                                //skip the whole sample
                                offsetStart += byteSize;
                                //in the gam shift the sub-block siFze
                                memoryOffset += copySize;
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool BrokerI::InitFunctionPointersOptim(const SignalDirection direction,
                                        DataSourceI &dataSource,
                                        const char8 *const functionName,
                                        void *const gamMemoryAddress) {

    ownerDataSourceName = dataSource.GetName();
    ownerFunctionName = functionName;
    //Need to check the broker class name. This function loops through all the signals of the
    //functionName and should only react to the signals which are related to this BrokerI instance.
    //Get the class name from the ClassProperties
    const ClassProperties *properties = GetClassProperties();
    bool ret = (properties != NULL);
    const char8 *brokerClassName = NULL_PTR(const char8*);
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
    //Elements to be copies
    uint32 auxNumberOfCopies;
    basicCopyTable *bcp = NULL_PTR(basicCopyTable*);
    if (ret) {
        auxNumberOfCopies = GetNumberOfRawCopies(direction, functionIdx, functionNumberOfSignals, dataSource, brokerClassName);
        if (auxNumberOfCopies > 0u) {
            bcp = new basicCopyTable[auxNumberOfCopies];
        }
    }
    if (bcp != NULL_PTR(basicCopyTable*)) {
        if (ret) {
            ret = FillCopyTable(direction, functionIdx, functionNumberOfSignals, dataSource, brokerClassName, gamMemoryAddress, bcp);
        }
        if (ret) {
            //lint -e{644} Variable 'auxNumberOfCopies may not have been initialized. Justification bcp != NULL_PTR(basicCopyTable*)--> auxNumberOfCopies initialised
            SortByGAMAddress(bcp, auxNumberOfCopies);
        }
        if (ret) {
            numberOfCopies = 1u;
            for (uint32 i = 1u; i < auxNumberOfCopies; i++) {
                bool isGAMMemoryContinuous = &(reinterpret_cast<char8*>(bcp[i - 1u].gamPointer)[bcp[i - 1u].copySize])
                        == reinterpret_cast<char8*>(bcp[i].gamPointer);
                bool isDSMemoryContinuous = &(reinterpret_cast<char8*>(bcp[i - 1u].dataSourcePointer)[bcp[i - 1u].copySize])
                        == reinterpret_cast<char8*>(bcp[i].dataSourcePointer);
                if (!(isGAMMemoryContinuous && isDSMemoryContinuous)) {
                    numberOfCopies++;
                }
            }

            if (numberOfCopies > 0u) {
                ret = copyTableInfo == NULL_PTR(memoryInfo*);
                if (ret) {
                    //lint -e{423} Creation of memory leak in assignment to 'MARTe::BrokerI::copyTableInfo'. Justification: not possible since copyTableInfo == NULL_PTR(memoryInfo*) shall be true to execute this code.
                    copyTableInfo = new memoryInfo[numberOfCopies];
                    uint32 accumCopyByteSize;
                    copyTableInfo[0].functionSignalPointers = bcp[0].gamPointer;
                    copyTableInfo[0].copyOffset = bcp[0].signalDSByteOffsets;
                    copyTableInfo[0].copySignalIndex = bcp[0].signalDSIdx;
                    accumCopyByteSize = bcp[0].copySize;
                    uint32 auxIdx = 1u;
                    for (uint32 i = 1u; i < auxNumberOfCopies; i++) {
                        //lint -e{661}  Possible access of out-of-bounds pointer (1 beyond end of data) by operator '[' [MISRA C++ Rule 5-0-16]. Justification: i is at least 1. the index goes up to auxNumberOfCopies-1u and bcp has allocated memory for auxNumberOfCopies
                        bool isGAMMemoryContinuous = &(reinterpret_cast<char8*>(bcp[i - 1u].gamPointer)[bcp[i - 1u].copySize])
                                == reinterpret_cast<char8*>(bcp[i].gamPointer);
                        //lint -e{661}  Possible access of out-of-bounds pointer (1 beyond end of data) by operator '[' [MISRA C++ Rule 5-0-16]. Justification: i is at least 1. the index goes up to auxNumberOfCopies-1u and bcp has allocated memory for auxNumberOfCopies
                        bool isDSMemoryContinuous = &(reinterpret_cast<char8*>(bcp[i - 1u].dataSourcePointer)[bcp[i - 1u].copySize])
                                == reinterpret_cast<char8*>(bcp[i].dataSourcePointer);
                        //lint -e{661}  Possible access of out-of-bounds pointer (1 beyond end of data) by operator '[' [MISRA C++ Rule 5-0-16]. Justification: i is at least 1. the index goes up to auxNumberOfCopies-1u and bcp has allocated memory for auxNumberOfCopies
                        if (!(isGAMMemoryContinuous && isDSMemoryContinuous)) {
                            copyTableInfo[auxIdx].functionSignalPointers = bcp[i].gamPointer;
                            copyTableInfo[auxIdx].copyOffset = bcp[i].signalDSByteOffsets;
                            copyTableInfo[auxIdx].copySignalIndex = bcp[i].signalDSIdx;
                            copyTableInfo[auxIdx - 1u].copyByteSize = accumCopyByteSize;
                            accumCopyByteSize = 0u;
                            auxIdx++;
                        }
                        //lint -e{661}  Possible access of out-of-bounds pointer (1 beyond end of data) by operator '[' [MISRA C++ Rule 5-0-16]. Justification: i is at least 1. the index goes up to auxNumberOfCopies-1u and bcp has allocated memory for auxNumberOfCopies
                        accumCopyByteSize += bcp[i].copySize;
                    }
                    copyTableInfo[numberOfCopies - 1u].copyByteSize = accumCopyByteSize;
                }
            }
        }
    }
    if (bcp != NULL_PTR(basicCopyTable*)) {
        delete[] bcp;
    }
    return ret;
}

StreamString BrokerI::GetOwnerFunctionName() const {
    return ownerFunctionName;
}

StreamString BrokerI::GetOwnerDataSourceName() const {
    return ownerDataSourceName;
}

uint32 BrokerI::GetNumberOfRawCopies(const SignalDirection direction,
                                     const uint32 functionIdx,
                                     const uint32 functionNumberOfSignals,
                                     DataSourceI &dataSource,
                                     const char8 *const brokerClassName) const {
    bool ret = true;
    uint32 numberOfByteOffsets;
    uint32 samples = 0u;
    uint32 copies = 0u;
    for (uint32 i = 0u; (i < functionNumberOfSignals) && (ret); i++) {
        if (dataSource.IsSupportedBroker(direction, functionIdx, i, brokerClassName)) {
            ret = dataSource.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, i, numberOfByteOffsets);

            if (ret) {
                ret = dataSource.GetFunctionSignalSamples(direction, functionIdx, i, samples);
            }

            if (ret) {
                //One copy for each signal but each signal might want to copy several pieces (i.e. offsets)
//                if (numberOfByteOffsets > 1u) {
                copies += (numberOfByteOffsets * samples);
//                }
//                else {
//                    copies++;
//                }
            }
        }
    }
    return copies;
}

//lint -e{715} Symbol not referenced. Justification: virtual function prototype to be used via inheritance.
//lint -e{952} Parameter could be declared const [MISRA C++ Rule 7-1-1]. Justification: virtual function prototype to be used via inheritance.
bool BrokerI::Init(SignalDirection direction,
                   DataSourceI &dataSourceIn,
                   const char8 *const functionName,
                   void *gamMemoryAddress,
                   const bool optim) {
    return false;
}

bool BrokerI::FillCopyTable(const SignalDirection direction,
                            const uint32 functionIdx,
                            const uint32 functionNumberOfSignals,
                            DataSourceI &dataSource,
                            const char8 *const brokerClassName,
                            void *const gamMemoryAddress,
                            basicCopyTable *const bcp) const {
    uint32 samples = 0u;
    uint32 memoryOffset = 0u;
    uint32 byteSize = 0u;
    uint32 c = 0u;
    uint32 numberOfByteOffsets;
    //The same signal can be copied from different ranges. An entry is added for each signal range.
    // The pointer of the gam memory for this DataSource!
    bool ret = (gamMemoryAddress != NULL_PTR(void*));
    char8 *gamMemoryCharAddress = reinterpret_cast<char8*>(gamMemoryAddress);
    for (uint32 i = 0u; (i < functionNumberOfSignals) && (ret); i++) {
        if (dataSource.IsSupportedBroker(direction, functionIdx, i, brokerClassName)) {

            ret = dataSource.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, i, numberOfByteOffsets);

            if (ret) {
                ret = dataSource.GetFunctionSignalGAMMemoryOffset(direction, functionIdx, i, memoryOffset);
            }
            StreamString functionSignalName;
            if (ret) {
                //Get's the name  of the singla index i (i = 0 is the first signal of the functionIdx)
                ret = dataSource.GetFunctionSignalAlias(direction, functionIdx, i, functionSignalName);
            }
            uint32 signalIdx = 0u;
            if (ret) {
                //Gets the signalIdx with respect to the beginning of all signals of the dataSource. In general i != signalIdx.
                ret = dataSource.GetSignalIndex(signalIdx, functionSignalName.Buffer());
            }
            if (ret) {
                ret = dataSource.GetSignalByteSize(signalIdx, byteSize);
            }
            if (ret) {
                ret = dataSource.GetFunctionSignalSamples(direction, functionIdx, i, samples);
            }
            if (ret) {
                if (samples == 0u) {
                    samples = 1u;
                }
            }
            uint32 offsetStart = 0u;
            uint32 copySize = 0u;
            void *dataSourceSignalAddress;
            ret = dataSource.GetSignalMemoryBuffer(signalIdx, 0u, dataSourceSignalAddress);
            char8 *dataSourceSignalAddressChar = reinterpret_cast<char8*>(dataSourceSignalAddress);
            //Take into account  different ranges for the same signal
            for (uint32 j = 0u; (j < numberOfByteOffsets) && (ret); j++) {
                if (ret) {
                    ret = dataSource.GetFunctionSignalByteOffsetInfo(direction, functionIdx, i, j, offsetStart, copySize);
                    //gamMemoryCharAddress += memoryOffset;
                    for (uint32 h = 0u; (h < samples) && ret; h++) {
                        bcp[c].copySize = copySize;
                        bcp[c].signalDSByteOffsets = offsetStart;
                        bcp[c].signalDSIdx = signalIdx;
                        bcp[c].dataSourcePointer = &dataSourceSignalAddressChar[offsetStart];
                        bcp[c].gamPointer = reinterpret_cast<void*>(&gamMemoryCharAddress[memoryOffset]);
                        c++;
                        //skip the whole sample
//                        uint32 samplesOffset;
//                        if (samples > 1) {
//                            ret = dataSource.GetInputOffset(signalIdx, h, samplesOffset);
//                        }
//                        offsetStart += samplesOffset;
                        //Assumes that the memory is: signal1_Sample1, signal1_sample2, signal2_sample1, signal2_sample2
                        offsetStart += byteSize;
                        //in the gam shift the sub-block siFze
                        memoryOffset += copySize;
                    }
                }
            }
        }
    }
    return ret;
    //lint -e{429} Custodial pointer 'bcp' (line 382) has not been freed or returned. Justification: it is removed in InitFunctionPointersOptim() when is no longer used.
}

void BrokerI::SortByGAMAddress(basicCopyTable *const bcp,
                               const uint32 elements) const {
    basicCopyTable *sbcp = new basicCopyTable[elements];
    void *candidate = bcp[0u].gamPointer;
    uint32 candidateIdx = 0u;
    void *min = NULL_PTR(void*);
    void *maxAddress = NULL_PTR(void*);
    //lint -e{613} Possible use of null pointer 'maxAddress'. Justification: only comparing addresses, there is no risk of accessing NULL pointers
    for (uint32 bcpIdx = 0u; bcpIdx < elements; bcpIdx++) {
        //lint -e{946} Relational or subtract operator applied to pointers [MISRA C++ Rule 5-0-15], [MISRA C++ Rule 5-0-17], [MISRA C++ Rule 5-0-18]. Justification: comparing address of the same object (GAM memory), therefore compliant by exception
        if (bcp[bcpIdx].gamPointer > maxAddress) {
            maxAddress = bcp[bcpIdx].gamPointer;
        }
    }
    //lint -e{613} Possible use of null pointer 'maxAddress'. Justification: only comparing addresses, there is no risk of accessing NULL pointers
    for (uint32 sbcpIdx = 0u; sbcpIdx < elements; sbcpIdx++) {
        for (uint32 bcpIdx = 0u; bcpIdx < elements; bcpIdx++) {
            //lint -e{946} Relational or subtract operator applied to pointers [MISRA C++ Rule 5-0-15], [MISRA C++ Rule 5-0-17], [MISRA C++ Rule 5-0-18]. Justification: comparing address of the same object (GAM memory), therefore compliant by exception
            if ((bcp[bcpIdx].gamPointer <= candidate) && (bcp[bcpIdx].gamPointer > min)) {
                candidate = bcp[bcpIdx].gamPointer;
                candidateIdx = bcpIdx;
            }
        }
        //lint -e{534} Ignoring return value of function. Justification: the return value has no impact on the behaviour of the function.
        MemoryOperationsHelper::Copy(&sbcp[sbcpIdx], &bcp[candidateIdx], static_cast<uint32>(sizeof(basicCopyTable)));
        min = candidate;
        candidate = maxAddress;
    }
    //lint -e{534} Ignoring return value. Justification: return value has no impact on the code. The function return void.
    //lint -e{593} Custodial pointer 'destination' possibly not freed or returned. Justification: it is removed in InitFunctionPointersOptim() when is no longer used.
    MemoryOperationsHelper::Copy(bcp, sbcp, static_cast<uint32>(sizeof(basicCopyTable)) * elements);
    if (sbcp != NULL_PTR(basicCopyTable*)) {
        delete[] sbcp;
    }
//lint -e{593} Custodial pointer 'bcp' possibly not freed or returned. Justification: bcp freed in InitFunctionPointersOptim() when no longer needed.
}

}
