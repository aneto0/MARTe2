/**
 * @file MemoryMapMultiBufferBroker.cpp
 * @brief Source file for class MemoryMapMultiBufferBroker
 * @date 03/05/2018
 * @author Giuseppe Ferro
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
 * the class MemoryMapMultiBufferBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MemoryMapMultiBufferBroker.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapMultiBufferBroker::MemoryMapMultiBufferBroker() :
        MemoryMapBroker() {

    signalIdxArr = NULL_PTR(uint32 *);
    samples = NULL_PTR(uint32 *);
    maxOffset = NULL_PTR(uint32 *);
}

MemoryMapMultiBufferBroker::~MemoryMapMultiBufferBroker() {
    if (signalIdxArr != NULL_PTR(uint32 *)) {
        delete[] signalIdxArr;
        signalIdxArr = NULL_PTR(uint32 *);
    }
    if (samples != NULL_PTR(uint32 *)) {
        delete[] samples;
        samples = NULL_PTR(uint32 *);
    }
    if (maxOffset != NULL_PTR(uint32 *)) {
        delete[] maxOffset;
        maxOffset = NULL_PTR(uint32 *);
    }
}

/*lint -e{613} no NULL pointers expected at this stage as this would violate the pre-conditions to use this function (i.e. to have had a valid Init).*/
bool MemoryMapMultiBufferBroker::CopyInputs() {
    uint32 n;
    bool ret = true;
    uint32 currentBuffer = dataSource->GetCurrentStateBuffer();

    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            uint32 ioffset = 0u;
            ret = dataSource->GetInputOffset(signalIdxArr[n], samples[n], ioffset);
            if (ret) {
                uint32 dataSourceIndex = ((currentBuffer * numberOfCopies) + n);
                uint32 allowedSize = copyTable[n].copySize;
                uint32 gamOffset = 0u;
                uint32 copyOffsetN = GetCopyOffset(n);
                ioffset %= maxOffset[n];

                //Trap the circular buffer exceptions
                //Note that the offset is allowed to be negative, meaning that it has to copy N bytes before the current position of the dataSourcePointer
                int32 offset = static_cast<int32>(ioffset);
                //copyOffsetN will take into account the number of samples to be copied. There will be one copy for every sample and some of the samples might be requesting ranges that require the circular buffer to restart
                //Copy would go over boundary? This is only possible if the number of samples is > than the number of memory buffers and assumes that a circular buffer is required.
                //Only do (at most) one of the following whiles
                bool offsetChanged = false;
                while ((copyOffsetN + offset) >= maxOffset[n]) {
                    offset = (offset - maxOffset[n]);
                    offsetChanged = true;
                }
                if (!offsetChanged) {
                    //Circular buffer where one might need to copy the a sub-buffer from the end, N buffers in the middle and the reminder at the end again
                    if (allowedSize > (maxOffset[n] - (copyOffsetN + offset))) {
                        //allowedSize is the number of bytes that are to be copied from the data source: N samples x size of the type to be copied
                        //maxOffset is the maximum number of bytes that can be copied from the data source at a time: Number of buffers x size of the type to be copied. If the allowedSize is greater than the maxOffset, the copy must restart from the beginning of the memory
                        //overSize if how much bigger is the allowedSize w.r.t. to the overSize (e.g. copy 3 samples of an uint32 => allowedSize = 12, dual buffer in the data source => 2 x 4 = 8) => overSize = 4
                        uint32 overSize = allowedSize % maxOffset[n];

                        //The allowedSize may be many times bigger than the maxOffset, so that the copy of all the data source memory may need to be performed many times
                        uint32 numberOfFullCopies = ((allowedSize - overSize) / maxOffset[n]);
                        bool isMultiple = (((allowedSize - overSize) % maxOffset[n]) == 0);

                        //If the numberOfFullCopies is a multiple do one less copy, because at the end
                        if (isMultiple) {
                            overSize += maxOffset[n];
                            numberOfFullCopies--;
                        }
                        uint32 allowedSizePhase = 0u;
                        if (overSize > (maxOffset[n] - (copyOffsetN + offset))) {
                            allowedSizePhase = (maxOffset[n] - offset);

                            (void) MemoryOperationsHelper::Copy(&(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]),
                                                                &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]), allowedSizePhase);
                            gamOffset = allowedSizePhase;
                            offset = 0;
                        }

                        for (uint32 z = 0u; z < numberOfFullCopies; z++) {
                            (void) MemoryOperationsHelper::Copy(&(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]),
                                                                &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                                maxOffset[n]);

                            gamOffset += maxOffset[n];
                        }
                        allowedSize = allowedSize - (maxOffset[n] * numberOfIterations) - allowedSizePhase;
                    }
                }
                (void) MemoryOperationsHelper::Copy(&(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]),
                                                    &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]), allowedSize);
                /*lint -e{613} null pointer checked before.*/
                ret = dataSource->TerminateInputCopy(signalIdxArr[n], ioffset, samples[n]);
            }
        }
    }
    return ret;
}

/*lint -e{613} no NULL pointers expected at this stage as this would violate the pre-conditions to use this function (i.e. to have had a valid Init).*/
bool MemoryMapMultiBufferBroker::CopyOutputs() {

    uint32 n;
    bool ret = true;
    /*lint -e{613} null pointer checked before.*/
    uint32 currentBuffer = dataSource->GetCurrentStateBuffer();
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            uint32 ioffset = 0u;
            /*lint -e{613} null pointer checked before.*/
            ret = dataSource->GetOutputOffset(signalIdxArr[n], samples[n], ioffset);
            if (ret) {
                uint32 dataSourceIndex = ((currentBuffer * numberOfCopies) + n);
                uint32 allowedSize = copyTable[n].copySize;
                uint32 gamOffset = 0u;
                uint32 copyOffsetN = GetCopyOffset(n);
                ioffset %= maxOffset[n];
                //Trap the circular buffer exceptions
                //Note that the offset is allowed to be negative, meaning that it has to copy N bytes before the current position of the dataSourcePointer
                int32 offset = static_cast<int32>(ioffset);
                //Only do (at most) one of the following whiles
                bool offsetChanged = false;

                while ((copyOffsetN + offset) >= maxOffset[n]) {
                    offset = (offset - maxOffset[n]);
                    offsetChanged = true;
                }
                if (!offsetChanged) {
                    while (allowedSize > (maxOffset[n] - (copyOffsetN + offset))) {
                        allowedSize = (maxOffset[n] - offset);
                        (void) MemoryOperationsHelper::Copy(&((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                            &(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]), allowedSize);
                        gamOffset = allowedSize;
                        offset = (offset - maxOffset[n]) + allowedSize;
                        allowedSize = (copyTable[n].copySize - allowedSize);
                    }
                }

                (void) MemoryOperationsHelper::Copy(&((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                    &(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]), allowedSize);
                /*lint -e{613} null pointer checked before.*/
                ret = dataSource->TerminateOutputCopy(signalIdxArr[n], ioffset, samples[n]);
            }
        }
    }
    return ret;
}

bool MemoryMapMultiBufferBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
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
    uint32 numberOfStatefulBuffers = dataSource->GetNumberOfStatefulMemoryBuffers();
    if (ret) {
        uint32 totalNumberOfElements = (numberOfCopies * numberOfStatefulBuffers);
        /*lint -e{423} copyTable is freed in MemoryMapBroker destructor.*/
        copyTable = new MemoryMapBrokerCopyTableEntry[totalNumberOfElements];
        ret = (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*));
        if (ret) {
            signalIdxArr = new uint32[numberOfCopies];
            ret = (signalIdxArr != NULL_PTR(uint32*));
        }
        if (ret) {
            samples = new uint32[numberOfCopies];
            ret = (samples != NULL_PTR(uint32*));
        }

        if (ret) {
            maxOffset = new uint32[numberOfCopies];
            ret = (maxOffset != NULL_PTR(uint32*));
        }
    }
    uint32 functionIdx = 0u;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    //The same signal can be copied from different ranges. A MemoryMapBrokerCopyTableEntry is added for each signal range.
    uint32 c = 0u;
    uint32 n;
    for (uint32 c0 = 0u; c0 < numberOfStatefulBuffers; c0++) {
        for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
            if (dataSource->IsSupportedBroker(direction, functionIdx, n, brokerClassName)) {
                uint32 numberOfByteOffsets = 0u;
                ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);

                StreamString functionSignalName;
                TypeDescriptor signalType;
                if (ret) {
                    ret = dataSource->GetFunctionSignalAlias(direction, functionIdx, n, functionSignalName);
                }
                uint32 signalIdx = 0u;
                if (ret) {
                    ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
                    signalType = dataSource->GetSignalType(signalIdx);
                }
                uint32 nSamples = 0u;
                if (ret) {
                    ret = dataSource->GetFunctionSignalSamples(direction, functionIdx, n, nSamples);
                }
                uint32 maxSignalOffset = 0u;
                if (ret) {
                    maxSignalOffset = dataSource->GetNumberOfMemoryBuffers();
                }
                uint32 byteSize = 0u;
                if (ret) {
                    ret = dataSource->GetSignalByteSize(signalIdx, byteSize);
                    maxSignalOffset *= byteSize;
                }

                bool noRanges = true;
                if (ret) {
                    uint32 offsetStart;
                    uint32 copySize;
                    ret = dataSource->GetFunctionSignalByteOffsetInfo(direction, functionIdx, n, 0u, offsetStart, copySize);
                    if (ret) {
                        noRanges = ((numberOfByteOffsets == 1u) && (copySize == byteSize));
                    }
                }
                //get the number of buffers
                //Take into account different ranges for the same signal
                uint32 bo;
                for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                    //in this case only one node with the whole size
                    uint32 nFakeSamples = nSamples;
                    if (noRanges) {
                        nFakeSamples = 1u;
                    }
                    /*lint -e{613} copyTable cannot be NULL as otherwise ret would be false*/
                    for (uint32 h = 0u; (h < nFakeSamples) && (ret); h++) {
                        copyTable[c].copySize = GetCopyByteSize(c % (numberOfCopies));
                        copyTable[c].gamPointer = GetFunctionPointer(c % (numberOfCopies));
                        copyTable[c].type = signalType;
                        uint32 dataSourceOffset = GetCopyOffset(c % (numberOfCopies));

                        /*lint -e{613} null pointer checked before.*/
                        signalIdxArr[c % (numberOfCopies)] = signalIdx;
                        samples[c % (numberOfCopies)] = nSamples;
                        maxOffset[c % (numberOfCopies)] = maxSignalOffset;
                        void *dataSourceSignalAddress;
                        ret = dataSource->GetSignalMemoryBuffer(signalIdx, c0, dataSourceSignalAddress);
                        char8 *dataSourceSignalAddressChar = reinterpret_cast<char8 *>(dataSourceSignalAddress);
                        if (ret) {
                            dataSourceSignalAddressChar = &dataSourceSignalAddressChar[dataSourceOffset];
                            copyTable[c].dataSourcePointer = reinterpret_cast<void *>(dataSourceSignalAddressChar);
                        }

                        c++;
                    }
                }
            }
        }
    }

    return ret;
}

}

