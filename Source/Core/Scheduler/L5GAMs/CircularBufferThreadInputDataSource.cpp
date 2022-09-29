/**
 * @file CircularBufferThreadInputDataSource.cpp
 * @brief Source file for class CircularBufferThreadInputDataSource.
 * @date 12/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class CircularBufferThreadInputDataSource (public, protected, and
 * private). Be aware that some methods, such as those inline could be defined
 * on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CircularBufferThreadInputDataSource.h"
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
    currentBuffer = NULL_PTR(uint32*);
    sleepInMutexSec = 1e-6F;

    isRefreshed = NULL_PTR(uint8*);
    lastReadBuffer = NULL_PTR(uint32*);
    lastReadBuffer_1 = NULL_PTR(uint32*);

    triggerAfterNSamples = 1u;
    nBrokerOpPerSignal = NULL_PTR(uint32*);
    nBrokerOpPerSignalCounter = NULL_PTR(uint32*);
    numberOfChannels = 0u;
    syncSignal = 0u;

    timeStampSignalIndex = 0xFFFFFFFFu;
    errorCheckSignalIndex = 0xFFFFFFFFu;

    numberOfInterleavedSamples = NULL_PTR(uint32*);
    interleavedSignalByteSize = NULL_PTR(uint32*);

    numberOfInterleavedSignalMembers = NULL_PTR(uint32*);
    interleavedPacketMemberByteSize = NULL_PTR(uint32*);

    headerSize = NULL_PTR(uint32*);

    signalDefinitionInterleaved = false;
    getFirst = false;
    stop = 0;
    sleepTime = 0.F;

}

CircularBufferThreadInputDataSource::~CircularBufferThreadInputDataSource() {

    if (isRefreshed != NULL_PTR(uint8*)) {
        delete[] isRefreshed;
        isRefreshed = NULL_PTR(uint8*);
    }
    if (currentBuffer != NULL_PTR(uint32*)) {
        delete[] currentBuffer;
        currentBuffer = NULL_PTR(uint32*);
    }
    if (lastReadBuffer != NULL_PTR(uint32*)) {
        delete[] lastReadBuffer;
        lastReadBuffer = NULL_PTR(uint32*);
    }
    if (lastReadBuffer_1 != NULL_PTR(uint32*)) {
        delete[] lastReadBuffer_1;
        lastReadBuffer_1 = NULL_PTR(uint32*);
    }
    if (nBrokerOpPerSignal != NULL_PTR(uint32*)) {
        delete[] nBrokerOpPerSignal;
        nBrokerOpPerSignal = NULL_PTR(uint32*);
    }
    if (nBrokerOpPerSignalCounter != NULL_PTR(uint32*)) {
        delete[] nBrokerOpPerSignalCounter;
        nBrokerOpPerSignalCounter = NULL_PTR(uint32*);
    }
    if (numberOfInterleavedSamples != NULL_PTR(uint32*)) {
        delete[] numberOfInterleavedSamples;
    }
    if (interleavedSignalByteSize != NULL_PTR(uint32*)) {
        delete[] interleavedSignalByteSize;
    }
    if (numberOfInterleavedSignalMembers != NULL_PTR(uint32*)) {
        delete[] numberOfInterleavedSignalMembers;
    }
    if (interleavedPacketMemberByteSize != NULL_PTR(uint32*)) {
        delete[] interleavedPacketMemberByteSize;
    }
    if (headerSize != NULL_PTR(uint32*)) {
        delete[] headerSize;
    }
}

void CircularBufferThreadInputDataSource::Purge(ReferenceContainer &purgeList) {

    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Purge - Start.");

    uint8 numberOfAttempts = 2u;
    ErrorManagement::ErrorType err;
    if (executor.GetStatus() != EmbeddedThreadI::OffState) {
        // DEBUG
        REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Purge - Going to stop.");
        for (uint32 i = 0u; i < numberOfAttempts; i++) {
            err = (!executor.Stop());
            bool ret = err.ErrorsCleared();
            REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError,
                         "CircularBufferThreadInputDataSource::Purge SingleThreadService stoping attempt %u of %u returned %s", i + 1u, numberOfAttempts,
                         ret ? "true" : "false");
            if (ret) {
                break;
            }
        }
    }

    // DEBUG
    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Purge - Purging list.");
    ReferenceContainer::Purge(purgeList);
    
    // DEBUG
    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Purge - Done.");
}

bool CircularBufferThreadInputDataSource::Initialise(StructuredDataI &data) {

    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Initialise");

    ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        // Read cpu mask
        uint64 cpuMask = 0u;
        if (!data.Read("CpuMask", cpuMask)) {
            cpuMask = 0xFFFFu;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "CircularBufferThreadInputDataSource::Initialise CpuMask was not specified. Using default: %d",
                                    cpuMask);
        }
        uint8 receiverThreadPriority;
        if (!data.Read("ReceiverThreadPriority", receiverThreadPriority)) {
            receiverThreadPriority = 99u;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
                                    "CircularBufferThreadInputDataSource::Initialise ReceiverThreadPriority was not specified. Using default: %d",
                                    receiverThreadPriority);
        }
        uint32 receiverThreadStackSize;
        if (!data.Read("ReceiverThreadStackSize", receiverThreadStackSize)) {
            receiverThreadStackSize = THREADS_DEFAULT_STACKSIZE;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
                                    "CircularBufferThreadInputDataSource::Initialise ReceiverThreadStackSize was not specified. Using default: %d",
                                    receiverThreadStackSize);
        }
        if (!data.Read("SleepTime", sleepTime)) {
            sleepTime = 1e-6F;
        }
        receiverThreadPriority %= 100u;
        executor.SetCPUMask(static_cast<uint32>(cpuMask));
        executor.SetStackSize(receiverThreadStackSize);

        uint8 prioClass = (receiverThreadPriority / 25u);
        //Transform to the MARTe internal thread range.
        uint32 receiverThreadPriorityT = receiverThreadPriority;
        receiverThreadPriorityT = receiverThreadPriorityT % 25u;
        float32 prioLevelT = static_cast<float32>(receiverThreadPriorityT);
        prioLevelT *= 15.F; 
        prioLevelT /= 24.F;
        prioLevelT += 0.5F;
        uint8 prioLevel = static_cast<uint8>(prioLevelT);

        Threads::PriorityClassType prioClassT = Threads::NormalPriorityClass;
        if (prioClass == 0u) {
            prioClassT = Threads::UnknownPriorityClass;
        }
        else if (prioClass == 1u) {
            prioClassT = Threads::IdlePriorityClass;
        }
        else if (prioClass == 2u) {
            prioClassT = Threads::NormalPriorityClass;
        }
        else if (prioClass == 3u) {
            prioClassT = Threads::RealTimePriorityClass;
        }
        else {//NOOP
        }
        executor.SetPriorityClass(prioClassT);
        executor.SetPriorityLevel(prioLevel);

        uint32 signalDefinitionInterleavedUInt32 = 0u;
        if (!data.Read("SignalDefinitionInterleaved", signalDefinitionInterleavedUInt32)) {
            signalDefinitionInterleavedUInt32 = 0u;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
                                    "CircularBufferThreadInputDataSource::Initialise SignalDefinitionInterleaved was not specified. Using default: %d",
                                    signalDefinitionInterleavedUInt32);
        }
        signalDefinitionInterleaved = (signalDefinitionInterleavedUInt32 == 1u);
    }
    if (ret) {
        if (!data.Read("SleepInMutexSec", sleepInMutexSec)) {
            sleepInMutexSec = 1e-6F;
        }
    }
    if (ret) {
        uint8 getFirstTemp = 0u;
        if (!data.Read("GetFirst", getFirstTemp)) {
            getFirstTemp = 0u;
        }
        getFirst = (getFirstTemp > 0u);
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "CircularBufferThreadInputDataSource::Initialise returning %s",
                 ret ? "true" : "false");
    return ret;
}
/*lint -e{613} null pointer checked before.*/
void CircularBufferThreadInputDataSource::PrepareInputOffsets() {

    // REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::PrepareInputOffsets");

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

    uint32 nStepsForward = 0u;
    // REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Synchronise");

    lastReadBuffer_1[syncSignal] = lastReadBuffer[syncSignal];
    if (!getFirst) {
        //always go to the end
        //roll on consuming the circular buffer until the last written
        /*lint -e{9113} -e{9131} -e{9007} known dependences and side effects.*/
        while (nStepsForward < numberOfBuffers && (stop == 0)) {
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
    }
    bool ret = (nStepsForward < numberOfBuffers);
    /*lint -e{9113} -e{9131} -e{9007} known side effects.*/
    if (ret && (stop == 0)) {
        uint32 stepsBack = nStepsForward % triggerAfterNSamples;
        //if asynchronous return back to the last arrived samples, otherwise it will wait for new ones
        //at least double buffer (stepsBack+triggerAfterNSamples)<numberOfBuffers
        lastReadBuffer[syncSignal] -= stepsBack;
        if (lastReadBuffer[syncSignal] >= numberOfBuffers) {
            lastReadBuffer[syncSignal] += numberOfBuffers;
        }
        //return to the last sub-block
        //triggerAfterNSamples not arrived yet
        uint32 numberOfSamplesSinceLastTrigger = triggerAfterNSamples;
        /*lint -e{9113} -e{9131} -e{9007} known dependences and side effects.*/
        while (numberOfSamplesSinceLastTrigger > 0u && (stop == 0)) {
            lastReadBuffer[syncSignal]++;
            if (lastReadBuffer[syncSignal] >= numberOfBuffers) {
                lastReadBuffer[syncSignal] = 0u;
            }
            uint32 index = (lastReadBuffer[syncSignal] * numberOfSignals) + syncSignal;
            (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
            bool isArrived = (isRefreshed[index] == 1u);
            mutex.FastUnLock();
            /*lint -e{9131} -e{9007} known dependences and side effects.*/
            while ((!isArrived) && (stop == 0)) {
                (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
                isArrived = (isRefreshed[index] == 1u);
                mutex.FastUnLock();
                if (!isArrived) {
                    Sleep::Sec(sleepTime);
                }
            }
            numberOfSamplesSinceLastTrigger--;
        }
    }

    // REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "CircularBufferThreadInputDataSource::Initialise returning %s with buffers %u", ret ? "true" : "false", lastReadBuffer[syncSignal]);
    return ret;
}

const char8* CircularBufferThreadInputDataSource::GetBrokerName(StructuredDataI &data,
                                                                const SignalDirection direction) {

    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::GetBrokerName");

    const char8 *brokerName = NULL_PTR(const char8*);
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

/*lint -e{715} the default implementation returns false (only input supported).*/
bool CircularBufferThreadInputDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                           const char8 *const functionName,
                                                           void *const gamMemPtr) {
    // REPORT_ERROR(ErrorManagement::Warning, "CircularBufferThreadInputDataSource::GetOutputBrokers returning false (no OutputBrokers).");
    return false;
}

/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::SetConfiguredDatabase(StructuredDataI &data) {

    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::SetConfiguredDatabase");

    ret = MemoryDataSourceI::SetConfiguredDatabase(data);
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
                        REPORT_ERROR(ErrorManagement::InitialisationError,
                                     "CircularBufferThreadInputDataSource::SetConfiguredDatabase The InternalTimeStamp signal type must be uint64");
                    }
                }
                if (ret) {
                    if (signalName == "ErrorCheck") {
                        errorCheckSignalIndex = i;
                        //check the type of the time stamp signal
                        TypeDescriptor td = GetSignalType(errorCheckSignalIndex);
                        ret = (td == UnsignedInteger32Bit);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::InitialisationError,
                                         "CircularBufferThreadInputDataSource::SetConfiguredDatabase The ErrorCheck signal type must be uint32");
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
                        ret = (numberOfProducers == 0u);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(
                                    ErrorManagement::FatalError,
                                    "CircularBufferThreadInputDataSource::SetConfiguredDatabase The signal with id=%u is written... all the signals must be only read",
                                    i);
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
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint64));
                    if (!signalDefinitionInterleaved) {
                        sizeCheck *= (numberOfChannels);
                    }
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(
                                ErrorManagement::FatalError,
                                "CircularBufferThreadInputDataSource::SetConfiguredDatabase The size of the InternalTimeStamp signal must be %u != %u",
                                sizeCheck, signalByteSize);
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
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint32));
                    if (!signalDefinitionInterleaved) {
                        sizeCheck *= (numberOfChannels);
                    }
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                "CircularBufferThreadInputDataSource::SetConfiguredDatabase The size of the ErrorCheck signal must be %u != %u",
                                                sizeCheck, signalByteSize);
                    }
                }
            }
        }
    }
    if (ret) {
        if (signalDefinitionInterleaved) {
            ret = GenererateInterleavedAcceleratorsSignalDefinitionInterleaved();
        }
        else {
            ret = GenererateInterleavedAccelerators();
        }
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "CircularBufferThreadInputDataSource::SetConfiguredDatabase returning %s",
                 ret ? "true" : "false");
    return ret;
}

/*lint -e{613} ret prevents NULL pointers from being used*/
bool CircularBufferThreadInputDataSource::GenererateInterleavedAccelerators() {

    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::GenererateInterleavedAccelerators");

    if (numberOfInterleavedSamples != NULL_PTR(uint32*)) {
        delete[] numberOfInterleavedSamples;
    }
    if (interleavedSignalByteSize != NULL_PTR(uint32*)) {
        delete[] interleavedSignalByteSize;
    }
    if (numberOfInterleavedSignalMembers != NULL_PTR(uint32*)) {
        delete[] numberOfInterleavedSignalMembers;
    }
    if (interleavedPacketMemberByteSize != NULL_PTR(uint32*)) {
        delete[] interleavedPacketMemberByteSize;
    }
    if (headerSize != NULL_PTR(uint32*)) {
        delete[] headerSize;
    }
    numberOfInterleavedSamples = new uint32[numberOfSignals];
    interleavedSignalByteSize = new uint32[numberOfSignals];
    numberOfInterleavedSignalMembers = new uint32[numberOfSignals];
    headerSize = new uint32[numberOfSignals];
    uint32 totalNumberOfInputChunkSizes = 0u;
    bool ret = true;
    //Look for all the "PacketMemberSizes". Each signal is potentially a packet
    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        numberOfInterleavedSignalMembers[i] = 0u;
        headerSize[i] = 0u;
        StreamString signalName;
        ret = GetSignalName(i, signalName);
        bool found = false;
        if (ret) {
            found = signalsDatabase.MoveAbsolute("Signals");
            if (found) {
                found = signalsDatabase.MoveRelative(signalName.Buffer());
            }
        }
        if (ret && found) {
            //read the packet config
            AnyType at = signalsDatabase.GetType("PacketMemberSizes");
            if (!at.IsVoid()) {
                numberOfInterleavedSignalMembers[i] = at.GetNumberOfElements(0u);
                totalNumberOfInputChunkSizes += numberOfInterleavedSignalMembers[i];
            }
            //read the header size
            if (!signalsDatabase.Read("HeaderSize", headerSize[i])) {
                headerSize[i] = 0u;
            }
        }
    }
    if (ret) {
        if (totalNumberOfInputChunkSizes > 0u) {
            interleavedPacketMemberByteSize = new uint32[totalNumberOfInputChunkSizes];
        }
    }
    uint32 cnt = 0u;
    uint32 index = 0u;
    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        if (numberOfInterleavedSignalMembers[i] > 0u) {
            StreamString signalName;
            ret = GetSignalName(i, signalName);
            if (ret) {
                ret = signalsDatabase.MoveAbsolute("Signals");
                if (ret) {
                    ret = signalsDatabase.MoveRelative(signalName.Buffer());
                }
            }
            if (ret) {
                /*lint -e{449} will only get here if totalNumberOfInputChunkSizes > 0u => new interleavedPacketMemberByteSize*/
                Vector<uint32> packetChunkSizeVec(&interleavedPacketMemberByteSize[index], numberOfInterleavedSignalMembers[i]);
                ret = signalsDatabase.Read("PacketMemberSizes", packetChunkSizeVec);
                index += numberOfInterleavedSignalMembers[i];
            }
        }
        if (ret) {
            ret = GetSignalByteSize(i, interleavedSignalByteSize[i]);
            if (ret) {
                interleavedSignalByteSize[i] -= headerSize[i];
                uint32 packetSize = 0u;
                //check that the byte size is consistent with the packet configuration
                for (uint32 j = 0u; j < numberOfInterleavedSignalMembers[i]; j++) {
                    packetSize += interleavedPacketMemberByteSize[cnt];
                    cnt++;
                }
                if (packetSize == 0u) {
                    packetSize = interleavedSignalByteSize[i];
                }
                uint32 fakeSamples = (interleavedSignalByteSize[i] / packetSize);
                ret = (fakeSamples > 0u);
                if (ret) {
                    ret = ((interleavedSignalByteSize[i] % packetSize) == 0u);
                    if (ret) {
                        //How many times is the PacketMemberSizes repeated inside the memory
                        numberOfInterleavedSamples[i] = fakeSamples;
                        //What is the size of the PacketMemberSizes, i.e. the size of each packet?
                        interleavedSignalByteSize[i] /= fakeSamples;
                    }
                    else {
                        uint32 ii = i;
                        REPORT_ERROR(
                                ErrorManagement::InitialisationError,
                                "CircularBufferThreadInputDataSource::GenererateInterleavedAccelerators The signal %d byte size %d must be divided exactly by the sum of the packet member sizes %d",
                                ii, interleavedSignalByteSize[ii], packetSize);
                    }
                }
                else {
                    uint32 ii = i;
                    REPORT_ERROR(
                            ErrorManagement::InitialisationError,
                            "CircularBufferThreadInputDataSource::GenererateInterleavedAccelerators The sum of the packet member sizes %d cannot be greater than the signal %d byte size %d",
                            packetSize, ii, interleavedSignalByteSize[ii]);
                }
            }
        }
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError,
                 "CircularBufferThreadInputDataSource::GenererateInterleavedAccelerators returning %s", ret ? "true" : "false");
    return ret;
}

/*lint -e{613} ret prevents NULL pointers from being used*/
bool CircularBufferThreadInputDataSource::GenererateInterleavedAcceleratorsSignalDefinitionInterleaved() {

    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::GenererateInterleavedAcceleratorsSignalDefinitionInterleaved");

    if (numberOfInterleavedSamples != NULL_PTR(uint32*)) {
        delete[] numberOfInterleavedSamples;
    }
    if (interleavedSignalByteSize != NULL_PTR(uint32*)) {
        delete[] interleavedSignalByteSize;
    }
    if (numberOfInterleavedSignalMembers != NULL_PTR(uint32*)) {
        delete[] numberOfInterleavedSignalMembers;
    }
    if (interleavedPacketMemberByteSize != NULL_PTR(uint32*)) {
        delete[] interleavedPacketMemberByteSize;
    }
    if (headerSize != NULL_PTR(uint32*)) {
        delete[] headerSize;
    }
    numberOfInterleavedSamples = new uint32[1u];
    interleavedSignalByteSize = new uint32[1u];
    numberOfInterleavedSignalMembers = new uint32[1u];
    headerSize = new uint32[1u];
    //The packet is defined by the number of signals
    numberOfInterleavedSignalMembers[0u] = numberOfChannels;
    uint32 totalNumberOfInputChunkSizes = numberOfChannels;
    bool ret = (totalNumberOfInputChunkSizes > 0u);
    if (ret) {
        interleavedPacketMemberByteSize = new uint32[totalNumberOfInputChunkSizes];
    }
    headerSize[0] = 0u;
    //The number of samples must be the same for all signals
    uint32 nOfSamples = 0u;
    uint32 nOfFunctions = GetNumberOfFunctions();
    for (uint32 i = 0u; (i < nOfFunctions) && (ret); i++) {
        uint32 nOfFunctionSignals = 1u;
        ret = GetFunctionNumberOfSignals(InputSignals, i, nOfFunctionSignals);
        for (uint32 j = 0u; (j < nOfFunctionSignals) && (ret); j++) {
            StreamString alias;
            ret = GetFunctionSignalAlias(InputSignals, i, j, alias);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, alias.Buffer());
            }
            if (ret) {
                bool check = (signalIdx != timeStampSignalIndex);
                if (check) {
                    check = (signalIdx != errorCheckSignalIndex);
                }
                if (check) {
                    uint32 nOfSamplesRead;
                    ret = GetFunctionSignalSamples(InputSignals, i, j, nOfSamplesRead);
                    if (ret) {
                        if (nOfSamples == 0u) {
                            nOfSamples = nOfSamplesRead;
                        }
                        if (nOfSamples == 1u) {
                            if (headerSize[0] == 0u) {
                                ret = GetSignalByteSize(signalIdx, headerSize[0]);
                            }
                        }
                    }
                    if (ret) {
                        ret = (nOfSamples == nOfSamplesRead);
                        if (!ret) {
                            REPORT_ERROR(
                                    ErrorManagement::InitialisationError,
                                    "CircularBufferThreadInputDataSource::GenererateInterleavedAcceleratorsSignalDefinitionInterleaved With SignalDefinitionInterleaved the number of samples shall be the same for all signals (%d != %d)",
                                    nOfSamples, nOfSamplesRead);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        numberOfInterleavedSamples[0u] = nOfSamples;
        interleavedSignalByteSize[0u] = 0u;
    }
    uint32 k = 0u;
    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        if ((i != timeStampSignalIndex) && (i != errorCheckSignalIndex)) {
            uint32 byteSize;
            ret = GetSignalByteSize(i, byteSize);
            if (ret) {
                /*lint -e{449} ret = true => new of interleavedPacketMemberByteSize*/
                interleavedPacketMemberByteSize[k] = byteSize;
                interleavedSignalByteSize[0u] += byteSize;
            }
            k++;
        }
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError,
                 "CircularBufferThreadInputDataSource::GenererateInterleavedAcceleratorsSignalDefinitionInterleaved returning %s", ret ? "true" : "false");
    return ret;
}

/*lint -e{715} the behavior of this function is independent of the currentStateName and nextStateName.*/
/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::PrepareNextState(const char8 *const currentStateName,
                                                           const char8 *const nextStateName) {

    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::PrepareNextState");

    ret = true;
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
                                            triggerAfterNSamples = samples;
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
        executor.SetName(GetName());
        ret = executor.Start();
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "CircularBufferThreadInputDataSource::PrepareNextState returning %s",
                 ret ? "true" : "false");
    return ret;
}

/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::GetInputOffset(const uint32 signalIdx,
                                                         const uint32 numberOfSamples,
                                                         uint32 &offset) {

    // REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::GetInputOffset");

    offset = ((lastReadBuffer[signalIdx] - numberOfSamples) + 1u);
    if (offset >= numberOfBuffers) {
        offset += (numberOfBuffers); //startFromIdx is negative hence the +
    }
    offset *= signalSize[signalIdx];

    return true;
}

/*lint -e{715} Default implementation is not implemented.*/
bool CircularBufferThreadInputDataSource::GetOutputOffset(const uint32 signalIdx,
                                                          const uint32 numberOfSamples,
                                                          uint32 &offset) {
    // REPORT_ERROR(ErrorManagement::Warning, "CircularBufferThreadInputDataSource::GetOutputBrokers returning false (no OutputOffset).");
    return false;
}

/*lint -e{613} null pointer checked before.*/
ErrorManagement::ErrorType CircularBufferThreadInputDataSource::Execute(ExecutionInfo &info) {

    ErrorManagement::ErrorType err;
    // REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::Execute");

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
                            void *timerPtr = &memory[timeMemIndex];
                            *(reinterpret_cast<uint64*>(timerPtr)) = HighResolutionTimer::Counter();
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
                            void *timerPtr2 = &(memory[timeMemIndex2]);
                            void *timerPtr1 = &(memory[timeMemIndex1]);
                            *reinterpret_cast<uint64*>(timerPtr2) = *reinterpret_cast<uint64*>(timerPtr1);
                        }
                    }
                }
                else {
                    //driver read error
                    if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                        uint32 index = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                        errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index + cnt) * static_cast<uint32>(sizeof(uint32))));
                        void *errorPtr = &memory[errorMemIndex];
                        //if timeout give it a chance
                        *reinterpret_cast<uint32*>(errorPtr) |= 1u;
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
                        void *timePtr2 = &memory[timeMemIndex2];
                        void *timePtr1 = &memory[timeMemIndex1];
                        *reinterpret_cast<uint64*>(timePtr2) = *reinterpret_cast<uint64*>(timePtr1);
                    }
                }
                //refresh in any case... otherwise it will block the sync
                {
                    //the DriverRead returns the size read
                    uint32 index = ((currentBuffer[i] * (numberOfSignals)) + i);

                    if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                        uint32 index1 = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                        errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint32))));

                        //overlap error
                        (void) mutex.FastLock(TTInfiniteWait, sleepInMutexSec);
                        if (isRefreshed[index] == 1u) {
                            void *errorPtr = &memory[errorMemIndex];
                            *reinterpret_cast<uint32*>(errorPtr) |= 2u;
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
                void *errorPtr = &memory[errorMemIndex];
                *reinterpret_cast<uint32*>(errorPtr) = 0u;
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

    // bool ret = err.ErrorsCleared();
    // REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "CircularBufferThreadInputDataSource::Execute returning %s", ret ? "true" : "false");
    return err;
}

/*lint -e{715} the offset and the numberOfSamples are not required for the default implementation.*/
/*lint -e{613} null pointer checked before.*/
bool CircularBufferThreadInputDataSource::TerminateInputCopy(const uint32 signalIdx,
                                                             const uint32 offset,
                                                             const uint32 numberOfSamples) {

    // REPORT_ERROR(ErrorManagement::Information, "CircularBufferThreadInputDataSource::TerminateInputCopy");

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
