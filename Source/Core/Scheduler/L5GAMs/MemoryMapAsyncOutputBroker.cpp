/**
 * @file MemoryMapAsyncOutputBroker.cpp
 * @brief Source file for class MemoryMapAsyncOutputBroker
 * @date 13/02/2017
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
 * the class MemoryMapAsyncOutputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapAsyncOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapAsyncOutputBroker::MemoryMapAsyncOutputBroker() :
        MemoryMapBroker(),
        service(binder),
        binder(*this, &MemoryMapAsyncOutputBroker::BufferLoop) {
    bufferMemoryMap = NULL_PTR(MemoryMapAsyncOutputBrokerBufferEntry*);
    numberOfBuffers = 0u;
    writeIdx = 0u;
    readSynchIdx = 0u;
    cpuMask = ProcessorType(0xffu); // WARNING USING UINT32 TO INITIALIZE `ProcessorType`
    stackSize = THREADS_DATABASE_GRANULARITY;
    if (!sem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Create the EventSem.");
    }
    if (!sem.Reset()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Reset the EventSem.");
    }
    fastSem.Create();
    posted = false;
    flushed = true;
    destroying = false;
    ignoreBufferOverrun = false;
}

/*lint -e{1551} the destructor must guarantee that the SingleThreadService is stopped and that buffer memory is freed.*/
MemoryMapAsyncOutputBroker::~MemoryMapAsyncOutputBroker() {
    UnlinkDataSource();
    if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncOutputBrokerBufferEntry*)) {
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
        bufferMemoryMap = NULL_PTR(MemoryMapAsyncOutputBrokerBufferEntry*);
    }
}

void MemoryMapAsyncOutputBroker::UnlinkDataSource() {
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
    if (service.GetStatus() != EmbeddedThreadI::OffState) {
        service.SetTimeout(1000);
        if (service.Stop() != ErrorManagement::NoError) {
            REPORT_ERROR(ErrorManagement::Warning, "Going to kill the EmbbeddedService");
            if (service.Stop() != ErrorManagement::NoError) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the EmbeddedService");
            }
        }
    }
    dataSourceRef = Reference();
}

/*lint -e{715} This function is implemented just to avoid using this Broker as MemoryMapBroker.*/
bool MemoryMapAsyncOutputBroker::Init(const SignalDirection direction,
                                      DataSourceI &dataSourceIn,
                                      const char8 *const functionName,
                                      void *const gamMemoryAddress) {
    return false;
}

bool MemoryMapAsyncOutputBroker::InitWithBufferParameters(const SignalDirection direction,
                                                          DataSourceI &dataSourceIn,
                                                          const char8 *const functionName,
                                                          void *const gamMemoryAddress,
                                                          const uint32 numberOfBuffersIn,
                                                          const ProcessorType &cpuMaskIn,
                                                          const uint32 stackSizeIn) {
    bool ok = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    numberOfBuffers = numberOfBuffersIn;
    cpuMask = cpuMaskIn;
    stackSize = stackSizeIn;

    if (ok) {
        ok = (numberOfBuffers > 0u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfMemoryBuffers() shall be > 0");
    }
    if (ok) {
        ok = (dataSourceIn.GetNumberOfFunctions() == 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfFunctions() shall be == 1");
    }
    if (ok) {
        ok = (dataSourceIn.GetNumberOfMemoryBuffers() == 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfMemoryBuffers() shall be == 1");
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
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples on each signal shall be positivedd.");
            }
        }
    }
    if (ok) {
        dataSourceRef = Reference(&dataSourceIn);
    }
    if (ok) {
        bufferMemoryMap = new MemoryMapAsyncOutputBrokerBufferEntry[numberOfBuffers];
        uint32 i;
        for (i = 0u; i < numberOfBuffers; i++) {
            bufferMemoryMap[i].index = i;
            bufferMemoryMap[i].toConsume = false;
            uint32 c;
            bufferMemoryMap[i].mem = new void*[numberOfCopies];
            for (c = 0u; (c < numberOfCopies) && (ok); c++) {
                bufferMemoryMap[i].mem[c] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[c].copySize);
                ok = MemoryOperationsHelper::Set(bufferMemoryMap[i].mem[c], '\0', copyTable[c].copySize);
            }
        }
    }
    if (ok) {
        service.SetStackSize(stackSize);
        service.SetCPUMask(cpuMask);
    }
    if (ok) {
        readSynchIdx = numberOfBuffers - 1u;
        StreamString serviceName;
        if (serviceName.Printf("%s:MemoryMapAsyncOutputBroker", dataSourceIn.GetName())) {
            service.SetName(serviceName.Buffer());
        }
        ok = (service.Start() == ErrorManagement::NoError);
    }
    return ok;
}

ProcessorType MemoryMapAsyncOutputBroker::GetCPUMask() const {
    return cpuMask;
}

uint32 MemoryMapAsyncOutputBroker::GetStackSize() const {
    return stackSize;
}

uint32 MemoryMapAsyncOutputBroker::GetNumberOfBuffers() const {
    return numberOfBuffers;
}

bool MemoryMapAsyncOutputBroker::Execute() {
    bool ret = true;

    if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncOutputBrokerBufferEntry*)) {
        if (!ignoreBufferOverrun) {
            if (bufferMemoryMap[writeIdx].toConsume) {
                //Buffer overrun...
                const uint32 idx = writeIdx;
                REPORT_ERROR(ErrorManagement::FatalError, "Buffer overrun for index %d ", idx);
                ret = false;
            }
        }
        uint32 n;
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*)) {
                //Copy into the buffered table from the GAM memory
                ret = MemoryOperationsHelper::Copy(bufferMemoryMap[writeIdx].mem[n], copyTable[n].gamPointer, copyTable[n].copySize);
            }
        }
        bufferMemoryMap[writeIdx].toConsume = true;
        if (ret) {
            //Fast semaphore to increment writeIdx and to atomically Post the semaphore which warns the BufferLoop that there might be new data to be consumed.
            ret = (fastSem.FastLock() == ErrorManagement::NoError);
        }
        writeIdx++;
        if (writeIdx == numberOfBuffers) {
            writeIdx = 0u;
        }
        if (ret) {
            ret = sem.Post();
            posted = true;
        }
        fastSem.FastUnLock();
    }
    return ret;
}

bool MemoryMapAsyncOutputBroker::Flush() {
    bool ret = true;
    if (service.GetStatus() != EmbeddedThreadI::OffState) {
        ret = (fastSem.FastLock() == ErrorManagement::NoError);
        if (ret) {
            flushed = false;
            ret = sem.Post();
            posted = true;
        }
        fastSem.FastUnLock();
        while (!flushed) {
            Sleep::Sec(0.1F);
        }
    }
    return ret;
}

/*lint -e{1764} EmbeddedServiceMethodBinderI callback method pointer prototype requires a non constant ExecutionInfo*/
ErrorManagement::ErrorType MemoryMapAsyncOutputBroker::BufferLoop(ExecutionInfo &info) {
    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::MainStage) {
        int32 synchStopIdx = 0;
        if (fastSem.FastLock() == ErrorManagement::NoError) {
            //Always stay preTriggerBuffers behind from the writeIdx so that we don't lose the pre-trigger buffers
            synchStopIdx = static_cast<int32>(writeIdx);
        }
        fastSem.FastUnLock();

        //Have to treat the special case when only one buffer is configured
        if (numberOfBuffers == 1u) {
            readSynchIdx = 0u;
            synchStopIdx = 1;
        }
        bool ret = true;
        //Check all the buffers until writeIdx - preTriggerBuffers (inclusive)
        while ((readSynchIdx != static_cast<uint32>(synchStopIdx)) && (ret)) {
            if (bufferMemoryMap != NULL_PTR(MemoryMapAsyncOutputBrokerBufferEntry*)) {
                if (bufferMemoryMap[readSynchIdx].toConsume) {
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
                    bufferMemoryMap[readSynchIdx].toConsume = false;
                }
                readSynchIdx++;
                if (numberOfBuffers > 1u) {
                    if (readSynchIdx == numberOfBuffers) {
                        readSynchIdx = 0u;
                    }
                }
            }
        }

        if (ret) {
            flushed = true;
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
    }
    return err;
}

void MemoryMapAsyncOutputBroker::SetIgnoreBufferOverrun(const bool ignoreBufferOverrunIn) {
    ignoreBufferOverrun = ignoreBufferOverrunIn;
}

bool MemoryMapAsyncOutputBroker::IsIgnoringBufferOverrun() const {
    return ignoreBufferOverrun;
}

CLASS_REGISTER(MemoryMapAsyncOutputBroker, "1.0")
}
