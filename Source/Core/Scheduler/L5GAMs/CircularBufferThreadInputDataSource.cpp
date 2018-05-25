/**
 * @file CircularBufferThreadInputDataSource.cpp
 * @brief Source file for class CircularBufferThreadInputDataSource
 * @date 13/04/2018
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
 * the class CircularBufferThreadInputDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CircularBufferThreadInputDataSource.h"
#include "AdvancedErrorManagement.h"
#include "Threads.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

CircularBufferThreadInputDataSource::CircularBufferThreadInputDataSource() :
        MemoryDataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    mutex.Create();
    currentBuffer = NULL_PTR(uint32 *);

    isRefreshed = NULL_PTR(uint8 *);
    lastReadBuffer = NULL_PTR(uint32 *);
    lastReadBuffer_1 = NULL_PTR(uint32 *);

    triggerAfterNPackets = 1u;
    nBrokerOpPerSignal = NULL_PTR(uint32 *);
    nBrokerOpPerSignalCounter = NULL_PTR(uint32 *);
    numberOfChannels = 0u;
    syncSignal = 0u;

    timeStampSignalIndex = 0xFFFFFFFFu;
    errorCheckSignalIndex = 0xFFFFFFFFu;

}

CircularBufferThreadInputDataSource::~CircularBufferThreadInputDataSource() {
    if (isRefreshed != NULL_PTR(uint8 *)) {
        delete[] isRefreshed;
        isRefreshed = NULL_PTR(uint8 *);
    }

    if (currentBuffer != NULL_PTR(uint32 *)) {
        delete[] currentBuffer;
        currentBuffer = NULL_PTR(uint32 *);
    }
    if (lastReadBuffer != NULL_PTR(uint32 *)) {
        delete[] lastReadBuffer;
        lastReadBuffer = NULL_PTR(uint32 *);
    }
    if (lastReadBuffer_1 != NULL_PTR(uint32 *)) {
        delete[] lastReadBuffer_1;
        lastReadBuffer_1 = NULL_PTR(uint32 *);
    }
    if (nBrokerOpPerSignal != NULL_PTR(uint32 *)) {
        delete[] nBrokerOpPerSignal;
        nBrokerOpPerSignal = NULL_PTR(uint32 *);
    }
    if (nBrokerOpPerSignalCounter != NULL_PTR(uint32 *)) {
        delete[] nBrokerOpPerSignalCounter;
        nBrokerOpPerSignalCounter = NULL_PTR(uint32 *);
    }

}

void CircularBufferThreadInputDataSource::Purge(ReferenceContainer &purgeList) {

    if (executor.GetStatus() != EmbeddedThreadI::OffState) {
        if (!executor.Stop()) {
            if (!executor.Stop()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
            }
        }
    }
    ReferenceContainer::Purge(purgeList);
}

bool CircularBufferThreadInputDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        // Read cpu mask
        uint32 cpuMask = 0u;
        if (!data.Read("CpuMask", cpuMask)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "CpuMask was not specified. Using default: %d", cpuMask);
            cpuMask = 0xFFFFu;
        }
        uint32 receiverThreadPriority;
        if (!data.Read("ReceiverThreadPriority", receiverThreadPriority)) {
            receiverThreadPriority = 31u;
        }
        receiverThreadPriority %= 32u;
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(THREADS_DEFAULT_STACKSIZE);
        executor.SetPriorityClass(Threads::RealTimePriorityClass);
        executor.SetPriorityLevel(receiverThreadPriority);

    }

    if (ret) {
        if (!data.Read("SleepInMutexSec", sleepInMutexSec)) {
            sleepInMutexSec = 1e-6;
        }
    }

    return ret;
}
/*lint -e{613} null pointer checked before.*/
void CircularBufferThreadInputDataSource::PrepareInputOffsets() {

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        if (i != syncSignal) {
            lastReadBuffer_1[i] = lastReadBuffer[i];

            uint32 nStepsForward = 0u;
            //always go to the end

            //roll on consuming the circular buffer until the last written
            while (nStepsForward < numberOfBuffers) {
                uint32 lastReadBufTemp = (lastReadBuffer[i] + 1u);
                if (lastReadBufTemp >= numberOfBuffers) {
                    lastReadBufTemp = 0u;
                }
                (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
                uint32 index = (lastReadBufTemp * numberOfSignals) + i;
                if (isRefreshed[index] == 0u) {
                    mutex.FastUnLock();
                    break;
                }
                mutex.FastUnLock();
                lastReadBuffer[i] = lastReadBufTemp;
                nStepsForward++;
            }
        }
    }
}

/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::Synchronise() {

    lastReadBuffer_1[syncSignal] = lastReadBuffer[syncSignal];

    uint32 nStepsForward = 0u;
    //always go to the end

    //roll on consuming the circular buffer until the last written
    while (nStepsForward < numberOfBuffers) {
        uint32 lastReadBufTemp = (lastReadBuffer[syncSignal] + 1u);
        if (lastReadBufTemp >= numberOfBuffers) {
            lastReadBufTemp = 0u;
        }
        (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
        uint32 index = (lastReadBufTemp * numberOfSignals) + syncSignal;
        if (isRefreshed[index] == 0u) {
            mutex.FastUnLock();
            break;
        }
        mutex.FastUnLock();
        lastReadBuffer[syncSignal] = lastReadBufTemp;
        nStepsForward++;
    }

    bool ret = (nStepsForward < numberOfBuffers);
    if (ret) {
        uint32 stepsBack = nStepsForward % triggerAfterNPackets;
        //if asynchronous return back to the last arrived samples, otherwise it will wait for new ones
        //at least double buffer (stepsBack+triggerAfterNPackets)<numberOfBuffers
        lastReadBuffer[syncSignal] -= stepsBack;
        if (lastReadBuffer[syncSignal] >= numberOfBuffers) {
            lastReadBuffer[syncSignal] += numberOfBuffers;
        }

        //return to the last sub-block
        //triggerAfterNPackets not arrived yet

        uint32 numberOfPacketsSinceLastTrigger = triggerAfterNPackets;

        while (numberOfPacketsSinceLastTrigger > 0u) {

            lastReadBuffer[syncSignal]++;
            if (lastReadBuffer[syncSignal] >= numberOfBuffers) {
                lastReadBuffer[syncSignal] = 0u;
            }

            bool isArrived = false;

            while (!(isArrived)) {
                (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
                uint32 index = (lastReadBuffer[syncSignal] * numberOfSignals) + syncSignal;
                isArrived = (isRefreshed[index] == 1u);
                mutex.FastUnLock();
            }

            numberOfPacketsSinceLastTrigger--;
        }
    }

    //REPORT_ERROR(ErrorManagement::Information, "buffers %d", lastReadBuffer[syncSignal]);

    return ret;
}

const char8 *CircularBufferThreadInputDataSource::GetBrokerName(StructuredDataI &data,
                                                                const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        float32 freq = -1.0F;
        if (!data.Read("Frequency", freq)) {
            freq = -1.0F;
        }

        if (freq >= 0.F) {
            brokerName = "MemoryMapSynchronisedMultiBufferInputBroker";
        }
        else {
            brokerName = "MemoryMapMultiBufferInputBroker";
        }
    }

    return brokerName;

}

/*lint -e{715} .*/
bool CircularBufferThreadInputDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                           const char8* const functionName,
                                                           void * const gamMemPtr) {
    return false;
}

/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        currentBuffer = new uint32[numberOfSignals];
        lastReadBuffer = new uint32[numberOfSignals];
        lastReadBuffer_1 = new uint32[numberOfSignals];
        nBrokerOpPerSignal = new uint32[numberOfSignals];
        nBrokerOpPerSignalCounter = new uint32[numberOfSignals];

        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            currentBuffer[i] = 0u;
            lastReadBuffer[i] = (numberOfBuffers - 1u);
            lastReadBuffer_1[i] = 0u;
            nBrokerOpPerSignal[i] = 0u;
            nBrokerOpPerSignalCounter[i] = 0u;

            StreamString signalName;
            ret = GetSignalName(i, signalName);
            if (ret) {
                if (signalName == "InternalTimeStamp") {
                    timeStampSignalIndex = i;
                    //check the type of the time stamp signal
                    TypeDescriptor td = GetSignalType(timeStampSignalIndex);
                    ret = (td == UnsignedInteger64Bit);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "The InternalTimeStamp signal type must be uint64");
                    }
                }
                if (ret) {
                    if (signalName == "ErrorCheck") {
                        errorCheckSignalIndex = i;
                        //check the type of the time stamp signal
                        TypeDescriptor td = GetSignalType(errorCheckSignalIndex);
                        ret = (td == UnsignedInteger32Bit);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::InitialisationError, "The ErrorCheck signal type must be uint32");
                        }
                    }
                }

            }

        }

        if (ret) {
            uint32 nTotalElements = numberOfBuffers * numberOfSignals;
            isRefreshed = new uint8[nTotalElements];
            (void) MemoryOperationsHelper::Set(&isRefreshed[0], '\0', numberOfBuffers * numberOfSignals);
            /*lint -e{850} the variable i is not really modified inside the loop.*/
            for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {

                uint32 numberOfStates = 0u;
                ret = GetSignalNumberOfStates(i, numberOfStates);
                //check that all the signals are consumed and not produced
                for (uint32 j = 0u; (j < numberOfStates) && (ret); j++) {
                    StreamString stateName;
                    ret = GetSignalStateName(i, j, stateName);
                    if (ret) {
                        uint32 numberOfProducers = 0u;
                        (void) GetSignalNumberOfProducers(i, stateName.Buffer(), numberOfProducers);
                        if (ret) {
                            ret = (numberOfProducers == 0u);
                            if (!ret) {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The signal with id=%d is written... all the signals must be only read",
                                                        i);
                            }
                        }
                    }
                }
            }
        }

        if (ret) {
            numberOfChannels = numberOfSignals;
            if (timeStampSignalIndex != 0xFFFFFFFFu) {
                numberOfChannels--;
            }
            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                numberOfChannels--;
            }
            //check the size of the time stamp signal...
            if (timeStampSignalIndex != 0xFFFFFFFFu) {
                uint32 signalByteSize;
                ret = GetSignalByteSize(timeStampSignalIndex, signalByteSize);
                if (ret) {
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint64)) * (numberOfChannels);
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The size of the InternalTimeStamp signal must be %d != %d %d", sizeCheck,
                                                signalByteSize, numberOfChannels);
                    }
                }

            }
        }
        if (ret) {
            //check the size of the time stamp signal...
            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                uint32 signalByteSize;
                ret = GetSignalByteSize(errorCheckSignalIndex, signalByteSize);
                if (ret) {
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint32)) * (numberOfChannels);
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The size of the ErrorCheck signal must be %d != %d", sizeCheck, signalByteSize);
                    }
                }
            }
        }
    }
    return ret;
}

/*lint -e{715} .*/
/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::PrepareNextState(const char8 * const currentStateName,
                                                           const char8 * const nextStateName) {
    bool ret = true;
    syncSignal = numberOfSignals;

    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        nBrokerOpPerSignal[i] = 0u;
        nBrokerOpPerSignalCounter[i] = 0u;
        uint32 numberOfStates = 0u;
        ret = GetSignalNumberOfStates(i, numberOfStates);
        for (uint32 j = 0u; (j < numberOfStates) && (ret); j++) {
            StreamString stateName;
            ret = GetSignalStateName(i, j, stateName);
            if (ret) {
                if (stateName == nextStateName) {
                    uint32 numberOfFunctions = GetNumberOfFunctions();
                    for (uint32 h = 0u; (h < numberOfFunctions) && (ret); h++) {
                        uint32 numberOfFunctionSignals = 0u;
                        ret = GetFunctionNumberOfSignals(InputSignals, h, numberOfFunctionSignals);
                        for (uint32 k = 0u; (k < numberOfFunctionSignals) && (ret); k++) {
                            StreamString signalName;
                            ret = GetFunctionSignalAlias(InputSignals, h, k, signalName);
                            uint32 signalIdxTmp = 0u;
                            if (ret) {
                                ret = GetSignalIndex(signalIdxTmp, signalName.Buffer());
                            }
                            if (ret) {
                                if (signalIdxTmp == i) {
                                    uint32 nOffsets = 0u;
                                    ret = GetFunctionSignalNumberOfByteOffsets(InputSignals, h, k, nOffsets);
                                    uint32 rangeSize = 0u;
                                    if (ret) {
                                        uint32 offsetStart;
                                        ret = GetFunctionSignalByteOffsetInfo(InputSignals, h, k, 0u, offsetStart, rangeSize);
                                    }
                                    uint32 byteSize = 0u;
                                    if (ret) {
                                        ret = GetSignalByteSize(i, byteSize);
                                    }
                                    uint32 samples = 0u;
                                    if (ret) {
                                        ret = GetFunctionSignalSamples(InputSignals, h, k, samples);
                                    }

                                    float32 frequency = -1.F;
                                    if (ret) {
                                        ret = GetFunctionSignalReadFrequency(InputSignals, h, k, frequency);
                                    }
                                    if (ret) {
                                        if (frequency >= 0.F) {
                                            triggerAfterNPackets = samples;
                                            syncSignal = i;
                                        }
                                    }

                                    bool noRange = ((nOffsets == 1u) && (byteSize == rangeSize));
                                    if (noRange) {
                                        nBrokerOpPerSignal[i]++;
                                    }
                                    else {

                                        nBrokerOpPerSignal[i] += (nOffsets * samples);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        nBrokerOpPerSignalCounter[i] = nBrokerOpPerSignal[i];

    }

    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        ret = executor.Start();
    }
    return ret;
}

/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::GetInputOffset(const uint32 signalIdx,
                                                         const uint32 numberOfSamples,
                                                         uint32 &offset) {

    offset = ((lastReadBuffer[signalIdx] - numberOfSamples) + 1u);
    if (offset >= numberOfBuffers) {
        offset += (numberOfBuffers); //startFromIdx is negative hence the +
    }
    offset *= signalSize[signalIdx];

    return true;
}

/*lint -e{715} .*/
bool CircularBufferThreadInputDataSource::GetOutputOffset(const uint32 signalIdx,
                                                          const uint32 numberOfSamples,
                                                          uint32 &offset) {
    return false;
}

/*lint -e{613} null pointer checked before.*/
ErrorManagement::ErrorType CircularBufferThreadInputDataSource::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err;

    if (info.GetStage() == ExecutionInfo::MainStage) {
        //one read for each signal

        uint32 cnt = 0u;
        uint32 errorMemIndex = 0u;
        //reset the error check

        for (uint32 i = 0u; (i < numberOfSignals); i++) {

            if ((i != timeStampSignalIndex) && (i != errorCheckSignalIndex)) {
                uint32 readBytes = signalSize[i];
                uint32 memoryIndex = signalOffsets[i] + (currentBuffer[i] * signalSize[i]);
                if (DriverRead(reinterpret_cast<char8*>(&(memory[memoryIndex])), readBytes, i)) {
                    if (readBytes == signalSize[i]) {
                        //save the timestamp
                        if (timeStampSignalIndex != 0xFFFFFFFFu) {
                            uint32 index1 = (currentBuffer[timeStampSignalIndex] * (numberOfChannels));
                            uint32 timeMemIndex = (signalOffsets[timeStampSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint64))));
                            *reinterpret_cast<uint64*>(&(memory[timeMemIndex])) = HighResolutionTimer::Counter();
                        }
                        //the DriverRead returns the size read
                        uint32 index = ((currentBuffer[i] * (numberOfSignals)) + i);

                        if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                            uint32 index1 = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                            errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint32))));
                            //REPORT_ERROR(ErrorManagement::Information, "Here %d %d %d", isRefreshed[index + i], errorCheckSignalIndex, cnt);

                            //overlap error
                            (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
                            if (isRefreshed[index] == 1u) {
                                //REPORT_ERROR(ErrorManagement::Information, "Case 1");
                                *reinterpret_cast<uint32*>(&(memory[errorMemIndex])) |= 2u;
                            }
                            mutex.FastUnLock();

                        }

                        (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
                        isRefreshed[index] = 1u;
                        mutex.FastUnLock();

                        currentBuffer[i]++;
                        if (currentBuffer[i] >= numberOfBuffers) {
                            currentBuffer[i] = 0u;
                        }
                    }
                    else {
                        //copy the timestamp of the previous buffer
                        if (timeStampSignalIndex != 0xFFFFFFFFu) {
                            uint32 previousBuf = (currentBuffer[timeStampSignalIndex] - 1u);
                            if (previousBuf >= numberOfBuffers) {
                                previousBuf += numberOfBuffers;
                            }
                            uint32 index1 = (previousBuf * (numberOfChannels));
                            uint32 index2 = (currentBuffer[timeStampSignalIndex] * (numberOfChannels));
                            uint32 timeMemIndex1 = (signalOffsets[timeStampSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint64))));
                            uint32 timeMemIndex2 = (signalOffsets[timeStampSignalIndex] + ((index2 + cnt) * static_cast<uint32>(sizeof(uint64))));
                            *reinterpret_cast<uint64*>(&(memory[timeMemIndex2])) = *reinterpret_cast<uint64*>(&(memory[timeMemIndex1]));
                        }
                    }

                }
                else {
                    //driver read error
                    if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                        uint32 index = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                        errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index + cnt) * static_cast<uint32>(sizeof(uint32))));
                        *reinterpret_cast<uint32*>(&(memory[errorMemIndex])) |= 1u;
                    }
                    //copy the timestamp of the previous buffer
                    if (timeStampSignalIndex != 0xFFFFFFFFu) {
                        uint32 previousBuf = (currentBuffer[timeStampSignalIndex] - 1u);
                        if (previousBuf >= numberOfBuffers) {
                            previousBuf += numberOfBuffers;
                        }
                        uint32 index1 = (previousBuf * (numberOfChannels));
                        uint32 index2 = (currentBuffer[timeStampSignalIndex] * (numberOfChannels));
                        uint32 timeMemIndex1 = (signalOffsets[timeStampSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint64))));
                        uint32 timeMemIndex2 = (signalOffsets[timeStampSignalIndex] + ((index2 + cnt) * static_cast<uint32>(sizeof(uint64))));
                        *reinterpret_cast<uint64*>(&(memory[timeMemIndex2])) = *reinterpret_cast<uint64*>(&(memory[timeMemIndex1]));
                    }
                }
                cnt++;
            }
        }
        if (timeStampSignalIndex != 0xFFFFFFFFu) {
            uint32 index = (currentBuffer[timeStampSignalIndex] * (numberOfSignals)) + timeStampSignalIndex;
            (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
            isRefreshed[index] = 1u;
            mutex.FastUnLock();
            currentBuffer[timeStampSignalIndex]++;
            if (currentBuffer[timeStampSignalIndex] >= numberOfBuffers) {
                currentBuffer[timeStampSignalIndex] = 0u;
            }
        }
        if (errorCheckSignalIndex != 0xFFFFFFFFu) {
            uint32 index = (currentBuffer[errorCheckSignalIndex] * (numberOfSignals)) + errorCheckSignalIndex;
            (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
            isRefreshed[index] = 1u;
            mutex.FastUnLock();

            currentBuffer[errorCheckSignalIndex]++;
            if (currentBuffer[errorCheckSignalIndex] >= numberOfBuffers) {
                currentBuffer[errorCheckSignalIndex] = 0u;
            }
            index = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));

            for (uint32 i = 0u; i < numberOfChannels; i++) {
                errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index + i) * static_cast<uint32>(sizeof(uint32))));
                *reinterpret_cast<uint32*>(&(memory[errorMemIndex])) = 0u;
            }
        }

    }
    else if (info.GetStage() == ExecutionInfo::StartupStage) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            currentBuffer[i] = 0u;
        }
        (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
        (void) MemoryOperationsHelper::Set(&isRefreshed[0], '\0', numberOfBuffers * numberOfSignals);
        mutex.FastUnLock();

    }
    else {

    }
    return err;
}

/*lint -e{715} .*/
/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::TerminateInputCopy(const uint32 signalIdx,
                                                             const uint32 offset,
                                                             const uint32 numberOfSamples) {

    nBrokerOpPerSignalCounter[signalIdx]--;

    if ((nBrokerOpPerSignalCounter[signalIdx] == 0u) || (nBrokerOpPerSignalCounter[signalIdx] >= nBrokerOpPerSignal[signalIdx])) {
        //set as read
        nBrokerOpPerSignalCounter[signalIdx] = nBrokerOpPerSignal[signalIdx];
        uint32 index = (lastReadBuffer_1[signalIdx]);

        while (index != lastReadBuffer[signalIdx]) {
            index++;
            if (index >= numberOfBuffers) {
                index = 0u;
            }

            (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
            uint32 index1 = (index * numberOfSignals) + signalIdx;
            isRefreshed[index1] = 0u;
            mutex.FastUnLock();

        }

        nBrokerOpPerSignalCounter[signalIdx] = nBrokerOpPerSignal[signalIdx];
    }

    return true;
}

}
