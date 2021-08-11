/**
 * @file MemoryMapAsyncTriggerOutputBroker.cpp
 * @brief Source file for class MemoryMapAsyncTriggerOutputBroker
 * @date 24/01/2017
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
 * the class MemoryMapAsyncTriggerOutputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapAsyncTriggerOutputBroker.h"

#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapAsyncTriggerOutputBroker::MemoryMapAsyncTriggerOutputBroker() :
        MemoryMapBroker(),
        service(binder),
        binder(*this, &MemoryMapAsyncTriggerOutputBroker::BufferLoop) {
    bufferMemoryMap = NULL_PTR(MemoryMapAsyncTriggerOutputBrokerBufferEntry*);
    numberOfBuffers = 0u;
    writeIdx = 0u;
    readSynchIdx = 0u;
    preTriggerBuffers = 0u;
    cpuMask = ProcessorType(0xffu); // WARNING USING `uint32` TO INIT A `ProcessorType`
    postTriggerBuffersCounter = 0u;
    postTriggerBuffers = 0u;
    numberOfPreBuffersWritten = 0;
    wasTriggered = false;
    bufferLoopExecuting = false;
    stackSize = THREADS_DATABASE_GRANULARITY;
    if (!sem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Create the EventSem.");
    }
    if (!sem.Reset()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Reset the EventSem.");
    }
    fastSem.Create();
    posted = false;
    destroying = false;
    triggerIndexInGAMMemory = 0u;
}

/*lint -e{1551} the destructor must guarantee that the SingleThreadService is stopped and that buffer memory is freed.*/
MemoryMapAsyncTriggerOutputBroker::~MemoryMapAsyncTriggerOutputBroker() {
    UnlinkDataSource();
    if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncTriggerOutputBrokerBufferEntry*)) {
        uint32 i;
        for (i = 0u; i < numberOfBuffers; i++) {
            uint32 c;
            for (c = 0u; c < numberOfCopies; c++) {
                GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(bufferMemoryMap[i].mem[c]);
            }
            delete[] bufferMemoryMap[i].mem;
            bufferMemoryMap[i].mem = NULL_PTR(void**);
        }

        delete[] bufferMemoryMap;
        bufferMemoryMap = NULL_PTR(MemoryMapAsyncTriggerOutputBrokerBufferEntry*);
    }
}

void MemoryMapAsyncTriggerOutputBroker::UnlinkDataSource() {
    if (!sem.IsClosed()) {
        if (fastSem.FastLock() == ErrorManagement::NoError) {
            destroying = true;
            fastSem.FastUnLock();
            if (!sem.Post()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not Post the EventSem.");
            }
            posted = true;
        }
        if (!sem.Close()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not Close the EventSem.");
        }
    }
    service.SetTimeout(1000);
    if (service.Stop() != ErrorManagement::NoError) {
        REPORT_ERROR(ErrorManagement::Warning, "Going to kill the EmbbeddedService");
        if (service.Stop() != ErrorManagement::NoError) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the EmbeddedService");
        }
    }

    dataSourceRef = Reference();
}

/*lint -e{715} This function is implemented just to avoid using this Broker as MemoryMapBroker.*/
bool MemoryMapAsyncTriggerOutputBroker::Init(const SignalDirection direction,
                                             DataSourceI &dataSourceIn,
                                             const char8 *const functionName,
                                             void *const gamMemoryAddress) {
    return false;
}

bool MemoryMapAsyncTriggerOutputBroker::InitWithTriggerParameters(const SignalDirection direction,
                                                                  DataSourceI &dataSourceIn,
                                                                  const char8 *const functionName,
                                                                  void *const gamMemoryAddress,
                                                                  const uint32 numberOfBuffersIn,
                                                                  const uint32 preTriggerBuffersIn,
                                                                  const uint32 postTriggerBuffersIn,
                                                                  const ProcessorType &cpuMaskIn,
                                                                  const uint32 stackSizeIn) {
    bool ok = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    numberOfBuffers = numberOfBuffersIn;
    preTriggerBuffers = preTriggerBuffersIn;
    postTriggerBuffers = postTriggerBuffersIn;
    cpuMask = cpuMaskIn;
    stackSize = stackSizeIn;

    if (ok) {
        ok = (numberOfBuffers > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfMemoryBuffers() shall be > 0");
        }
    }
    if (ok) {
        ok = (preTriggerBuffers < numberOfBuffers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "preTriggerBuffers shall be < numberOfBuffers");
        }
    }
    if (ok) {
        ok = (postTriggerBuffers < numberOfBuffers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "postTriggerBuffers shall be < numberOfBuffers");
        }
    }
    if (ok) {
        ok = ((preTriggerBuffers + postTriggerBuffers) < numberOfBuffers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "preTriggerBuffers + postTriggerBuffers shall be < numberOfBuffers");
        }
    }
    if (ok) {
        ok = (dataSourceIn.GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfFunctions() shall be == 1");
        }
    }
    if (ok) {
        ok = (dataSourceIn.GetNumberOfMemoryBuffers() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfMemoryBuffers() shall be == 1");
        }
    }
    uint32 functionNumberOfSignals = 0u;
    if (ok) {
        ok = dataSourceIn.GetFunctionNumberOfSignals(direction, 0u, functionNumberOfSignals);
    }
    if (ok) {
        uint32 s;
        for (s = 0u; (s < functionNumberOfSignals) && (ok); s++) {
            uint32 samples;
            ok = dataSourceIn.GetFunctionSignalSamples(OutputSignals, 0u, s, samples);
            if (ok) {
                ok = (samples > 0u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples on each signal shall be positive.");
            }
        }
    }
    //The trigger signal shall be the first signal in the DataSource and shall have type uint8
    if (ok) {
        ok = (dataSourceIn.GetSignalType(0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (trigger) shall be of type uint8");
        }
    }
    if (ok) {
        dataSourceRef = Reference(&dataSourceIn);
    }
    if (ok) {
        bufferMemoryMap = new MemoryMapAsyncTriggerOutputBrokerBufferEntry[numberOfBuffers];
        uint32 i;
        for (i = 0u; i < numberOfBuffers; i++) {
            bufferMemoryMap[i].index = i;
            bufferMemoryMap[i].triggered = false;
            uint32 c;
            bufferMemoryMap[i].mem = new void*[numberOfCopies];
            for (c = 0u; (c < numberOfCopies) && (ok); c++) {
                bufferMemoryMap[i].mem[c] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[c].copySize);
                ok = MemoryOperationsHelper::Set(bufferMemoryMap[i].mem[c], '\0', copyTable[c].copySize);
            }
        }
    }
    //Have to discover where is the trigger signal in the GAM memory. I only know that it is for sure index zero in the DataSource signal list (i.e. signalIdx == 0u).
    if (ok) {
        uint32 n;
        uint32 c = 0u;
        bool done = false;
        for (n = 0u; (n < functionNumberOfSignals) && (ok) && (!done); n++) {
            if (dataSource->IsSupportedBroker(OutputSignals, 0u, n, "MemoryMapAsyncTriggerOutputBroker")) {
                uint32 numberOfByteOffsets = 0u;
                ok = dataSource->GetFunctionSignalNumberOfByteOffsets(OutputSignals, 0u, n, numberOfByteOffsets);

                StreamString functionSignalName;
                if (ok) {
                    ok = dataSource->GetFunctionSignalAlias(OutputSignals, 0u, n, functionSignalName);
                }
                uint32 signalIdx = 0u;
                if (ok) {
                    ok = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
                }
                if (ok) {
                    if (signalIdx == 0u) {
                        done = true;
                        triggerIndexInGAMMemory = c;
                        REPORT_ERROR(ErrorManagement::Information, "Trigger is at copy index %d", triggerIndexInGAMMemory);
                    }
                    else {
                        //Take into account different ranges for the same signal
                        c += numberOfByteOffsets;
                    }
                }
            }
        }
    }
    if (ok) {
        service.SetStackSize(stackSize);
        service.SetCPUMask(cpuMask);
    }
    if (ok) {
        if (preTriggerBuffers == 0u) {
            readSynchIdx = (numberOfBuffers - 1u);
        }
        else {
            readSynchIdx = (numberOfBuffers - preTriggerBuffers);
        }
        StreamString serviceName;
        if (serviceName.Printf("%s:MemoryMapAsyncOutputBroker", dataSourceIn.GetName())) {
            service.SetName(serviceName.Buffer());
        }

        ok = (service.Start() == ErrorManagement::NoError);
    }
    return ok;
}

ProcessorType MemoryMapAsyncTriggerOutputBroker::GetCPUMask() const {
    return cpuMask;
}

uint32 MemoryMapAsyncTriggerOutputBroker::GetStackSize() const {
    return stackSize;
}

uint32 MemoryMapAsyncTriggerOutputBroker::GetPreTriggerBuffers() const {
    return preTriggerBuffers;
}

uint32 MemoryMapAsyncTriggerOutputBroker::GetPostTriggerBuffers() const {
    return postTriggerBuffers;
}

uint32 MemoryMapAsyncTriggerOutputBroker::GetNumberOfBuffers() const {
    return numberOfBuffers;
}

bool MemoryMapAsyncTriggerOutputBroker::Execute() {
    bool ret = true;

    if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncTriggerOutputBrokerBufferEntry*)) {
        if (bufferMemoryMap[writeIdx].triggered) {
            //Buffer overrun...
            const uint32 idx = writeIdx;
            REPORT_ERROR(ErrorManagement::FatalError, "Buffer overrun for index %d ", idx);
            ret = false;
        }
        uint32 n;
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*)) {
                //Copy into the buffered table from the GAM memory
                ret = MemoryOperationsHelper::Copy(bufferMemoryMap[writeIdx].mem[n], copyTable[n].gamPointer, copyTable[n].copySize);
            }
        }
        //Check if the trigger is set to 1
        bufferMemoryMap[writeIdx].triggered = (*static_cast<uint8*>(bufferMemoryMap[writeIdx].mem[triggerIndexInGAMMemory]) > 0u);
        if (bufferMemoryMap[writeIdx].triggered) {
            //If it wasn't already triggered mark the pre-trigger samples to be stored by the BufferLoop
            if (!wasTriggered) {
                wasTriggered = true;
                int32 j;
                for (j = 0; j < static_cast<int32>(preTriggerBuffers); j++) {
                    int32 pre = (static_cast<int32>(writeIdx) - j) - 1;
                    if (pre < 0) {
                        //pre < 0 so +...
                        pre = static_cast<int32>(numberOfBuffers) + pre;
                    }
                    //Do not store pre-triggers of empty buffers (i.e. if there was a pre-trigger that leads to buffers @ t < 0).
                    if (j < numberOfPreBuffersWritten) {
                        bufferMemoryMap[pre].triggered = true;
                    }
                }
            }
            //Reset the post trigger buffers
            postTriggerBuffersCounter = postTriggerBuffers;
        }
        else {
            //Mark all the post trigger buffers
            if (postTriggerBuffersCounter > 0u) {
                bufferMemoryMap[writeIdx].triggered = true;
                postTriggerBuffersCounter--;
            }
            wasTriggered = false;
        }
        if (numberOfPreBuffersWritten < static_cast<int32>(preTriggerBuffers)) {
            numberOfPreBuffersWritten++;
        }
        if (ret) {
            //Fast semaphore to increment writeIdx and to atomically Post the semaphore which warns the BufferLoop that there might be new data to be consumed.
            ret = (fastSem.FastLock() == ErrorManagement::NoError);
        }
        writeIdx++;
        if (writeIdx == numberOfBuffers) {
            writeIdx = 0u;
        }
        if (ret) {
            posted = true;
            ret = sem.Post();
        }
        fastSem.FastUnLock();
    }
    return ret;
}

/*lint -e{1764} EmbeddedServiceMethodBinderI callback method pointer prototype requires a non constant ExecutionInfo*/
ErrorManagement::ErrorType MemoryMapAsyncTriggerOutputBroker::BufferLoop(ExecutionInfo &info) {
    ErrorManagement::ErrorType err;
    bool ret = true;
    if (info.GetStage() == ExecutionInfo::MainStage) {
        int32 synchStopIdx = 0;
        if (fastSem.FastLock() == ErrorManagement::NoError) {
            bufferLoopExecuting = true;
            //Always stay preTriggerBuffers behind from the writeIdx so that we don't lose the pre-trigger buffers
            synchStopIdx = static_cast<int32>(writeIdx) - static_cast<int32>(preTriggerBuffers);
        }
        fastSem.FastUnLock();

        if (synchStopIdx < 0) {
            //Notice that synchStopIdx is < 0
            synchStopIdx = static_cast<int32>(numberOfBuffers) + synchStopIdx;
        }

        if (synchStopIdx == static_cast<int32>(numberOfBuffers)) {
            synchStopIdx = 0;
        }
        //Have to treat the special case when only one buffer is configured
        if (numberOfBuffers == 1u) {
            readSynchIdx = 0u;
            synchStopIdx = 1;
        }
        //Check all the buffers until writeIdx - preTriggerBuffers (inclusive)
        while ((readSynchIdx != static_cast<uint32>(synchStopIdx)) && (ret)) {
            if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncTriggerOutputBrokerBufferEntry*)) {
                if (bufferMemoryMap[readSynchIdx].triggered) {
                    uint32 c;
                    for (c = 0u; (c < numberOfCopies) && (ret); c++) {
                        //Copy from the buffer to the DataSource memory
                        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*)) {
                            ret = MemoryOperationsHelper::Copy(copyTable[c].dataSourcePointer, bufferMemoryMap[readSynchIdx].mem[c], copyTable[c].copySize);
                        }
                    }
                    if (ret) {
                        if (dataSourceRef.IsValid()) {
                            //Make sure that the dataSourceRef consumes this data.
                            ret = dataSourceRef->Synchronise();
                        }
                    }
                    bufferMemoryMap[readSynchIdx].triggered = false;
                }
                readSynchIdx++;
                if (numberOfBuffers > 1u) {
                    if (readSynchIdx == numberOfBuffers) {
                        readSynchIdx = 0u;
                    }
                }
            }
        }
    }
    bufferLoopExecuting = false;
    if (ret) {
        //Wait for new data to be available from the real-time thread.
        if (!destroying) {
            err = sem.Wait(TTInfiniteWait);
        }
        else {
            Sleep::Sec(0.1F);
        }
        if (err.ErrorsCleared()) {
            //Only reset the semaphore if it was not posted in-between the Wait exit and now...
            if (fastSem.FastLock() == ErrorManagement::NoError) {
                if (!posted) {
                    err.fatalError = !sem.Reset();
                }
                if (destroying) {
                    err = ErrorManagement::Completed;
                }
                posted = false;
            }
            fastSem.FastUnLock();
        }
    }

    return err;
}

bool MemoryMapAsyncTriggerOutputBroker::FlushAllTriggers() {
    uint32 i = 0u;
    uint32 idx = writeIdx;
    bool ret = true;
    if (service.GetStatus() != EmbeddedThreadI::OffState) {
        bool waitForBufferLoop = bufferLoopExecuting;
        //Wait for the BufferLoop to end.
        while ((waitForBufferLoop) && (ret)) {
            ret = (fastSem.FastLock() == ErrorManagement::NoError);
            waitForBufferLoop = bufferLoopExecuting;
            fastSem.FastUnLock();
            Sleep::Sec(1e-3F);
        }
        if (ret) {
            ret = (fastSem.FastLock() == ErrorManagement::NoError);
        }

        while ((i < numberOfBuffers) && (ret)) {
            if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncTriggerOutputBrokerBufferEntry*)) {
                if (bufferMemoryMap[idx].triggered) {
                    uint32 c;
                    for (c = 0u; (c < numberOfCopies) && (ret); c++) {
                        //Copy from the buffer to the DataSource memory
                        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*)) {
                            ret = MemoryOperationsHelper::Copy(copyTable[c].dataSourcePointer, bufferMemoryMap[idx].mem[c], copyTable[c].copySize);
                        }
                    }
                    if (ret) {
                        if (dataSourceRef.IsValid()) {
                            //Make sure that the dataSourceRef consumes this data.
                            ret = dataSourceRef->Synchronise();
                        }
                    }
                    bufferMemoryMap[idx].triggered = false;
                }
            }
            idx++;
            if (idx == numberOfBuffers) {
                idx = 0u;
            }
            i++;
        }
        fastSem.FastUnLock();
    }
    return ret;
}

void MemoryMapAsyncTriggerOutputBroker::ResetPreTriggerBuffers() {
    numberOfPreBuffersWritten = 0;
}

CLASS_REGISTER(MemoryMapAsyncTriggerOutputBroker, "1.0")
}
