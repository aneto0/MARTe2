/**
 * @file MemoryMapTriggerOutputBroker.cpp
 * @brief Source file for class MemoryMapTriggerOutputBroker
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
 * the class MemoryMapTriggerOutputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapTriggerOutputBroker.h"

#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapTriggerOutputBroker::MemoryMapTriggerOutputBroker() {
    buffer = NULL_PTR(MemoryMapTriggerOutputBrokerBufferEntry *);
    numberOfBuffers = 0u;
    writeIdx = 0u;
    readSynchIdx = 0u;
    preTriggerBuffers = 0u;
    cpuMask = 0xff;
    postTriggerBuffersCounter = 0u;
    postTriggerBuffers = 0u;
    wasTriggered = false;
    binder = new EmbeddedServiceMethodBinderT<MemoryMapTriggerOutputBroker>(*this,
                                                                            &MemoryMapTriggerOutputBroker::BufferLoop);
    service = new (NULL) SingleThreadService(*binder);
    dataSource = NULL_PTR(DataSourceI *);
    sem.Create();
}

MemoryMapTriggerOutputBroker::~MemoryMapTriggerOutputBroker() {
    if (!sem.Post()) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not Post the EventSem.");
    }
    if (!sem.Close()) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not Close the EventSem.");
    }
    if (service != NULL_PTR(SingleThreadService *)) {
        service->SetTimeout(10);
        if (service->Stop() != ErrorManagement::NoError) {
            if (service->Stop() != ErrorManagement::NoError) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not stop the EmbeddedService");
            }
        }
        delete service;
    }
    if (binder != NULL_PTR(EmbeddedServiceMethodBinderI *)) {
        delete binder;
    }
    if (buffer != NULL_PTR(MemoryMapTriggerOutputBrokerBufferEntry *)) {
        uint32 numberOfCopies = GetNumberOfCopies();
        uint32 i;
        for (i = 0u; i < numberOfBuffers; i++) {
            uint32 c;
            for (c = 0u; c < numberOfCopies; c++) {
                GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(buffer[i].mem[c]);
            }
            delete[] buffer[i].mem;
        }

        delete[] buffer;
    }
}

bool MemoryMapTriggerOutputBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName, void * const gamMemoryAddress) {
    bool ok = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    if (ok) {
        numberOfBuffers = dataSourceIn.GetNumberOfMemoryBuffers();
        ok = (numberOfBuffers > 0);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfMemoryBuffers() shall be > 0");
    }
    if (ok) {
        ok = (preTriggerBuffers < numberOfBuffers);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "preTriggerBuffers shall be < numberOfBuffers");
    }
    if (ok) {
        ok = (dataSourceIn.GetNumberOfFunctions() == 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfFunctions() shall be == 1");
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
                ok = (samples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples on each signal shall be == 1.");
            }
        }
    }
    //The trigger signal shall be the first signal in the DataSource and shall have type uint8
    if (ok) {
        ok = (dataSourceIn.GetSignalType(0u) == UnsignedInteger8Bit);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (trigger) shall be of type uint8");
    }
    if (ok) {
        dataSource = &dataSourceIn;
    }
    if (ok) {
        uint32 numberOfCopies = GetNumberOfCopies();
        uint32 numberOfBuffers = dataSourceIn.GetNumberOfMemoryBuffers();
        buffer = new MemoryMapTriggerOutputBrokerBufferEntry[numberOfBuffers];
        uint32 i;
        for (i = 0u; i < numberOfBuffers; i++) {
            buffer[i].index = i;
            buffer[i].triggered = false;
            uint32 c;
            buffer[i].mem = new void*[numberOfCopies];
            for (c = 0u; c < numberOfCopies; c++) {
                buffer[i].mem[c] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[c].copySize);
            }
        }
    }
    if (ok) {
        service->SetCPUMask(cpuMask);
    }
    if (ok) {
        readSynchIdx = numberOfBuffers - preTriggerBuffers - 1u;
        ok = (service->Start() == ErrorManagement::NoError);
    }
    return ok;
}

void MemoryMapTriggerOutputBroker::SetCPUMask(const ProcessorType& cpuMaskIn) {
    cpuMask = cpuMaskIn;
}

void MemoryMapTriggerOutputBroker::SetPreTriggerBuffers(const uint32 preTriggerBuffersIn) {
    preTriggerBuffers = preTriggerBuffersIn;
}

void MemoryMapTriggerOutputBroker::SetPostTriggerBuffers(const uint32 postTriggerBuffersIn) {
    postTriggerBuffers = postTriggerBuffersIn;
}

bool MemoryMapTriggerOutputBroker::Execute() {
    bool ret = true;

    if (buffer[writeIdx].triggered) {
        //Buffer overrun...
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Buffer overrun for index %d ", writeIdx)
        ret = false;
    }
    uint32 n;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            ret = MemoryOperationsHelper::Copy(buffer[writeIdx].mem[n], copyTable[n].gamPointer, copyTable[n].copySize);
        }
    }
    //Check if the trigger is set to 1
    buffer[writeIdx].triggered = (*static_cast<uint8*>(buffer[writeIdx].mem[0u]) > 0u);
    if (buffer[writeIdx].triggered) {
        //If it wasn't already triggered mark the pre-trigger samples
        if (!wasTriggered) {
            wasTriggered = true;
            int32 j;
            for (j = 0; j < static_cast<int32>(preTriggerBuffers); j++) {
                int32 pre = static_cast<int32>(writeIdx) - j - 1;
                if (pre < 0) {
                    //pre < 0 so +...
                    pre = numberOfBuffers + pre;
                }
                buffer[pre].triggered = true;
            }
        }
        //Reset the post trigger buffers
        postTriggerBuffersCounter = postTriggerBuffers;
    }
    else {
        //Mark all the post trigger buffers
        if (postTriggerBuffersCounter > 0) {
            buffer[writeIdx].triggered = true;
            postTriggerBuffersCounter--;
        }
        wasTriggered = false;
    }
    writeIdx++;
    if (writeIdx == numberOfBuffers) {
        writeIdx = 0u;
    }
    if (ret) {
        ret = sem.Post();
    }
    return ret;
}

ErrorManagement::ErrorType MemoryMapTriggerOutputBroker::BufferLoop(const ExecutionInfo & info) {
    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::MainStage) {
        //Always stay preTriggerBuffers behind from the writeIdx
        int32 synchStopIdx = writeIdx - preTriggerBuffers;
        if (synchStopIdx < 0) {
            //Notice that synchStopIdx is < 0
            synchStopIdx = numberOfBuffers + synchStopIdx;
        }
        synchStopIdx++;
        if (synchStopIdx == static_cast<int32>(numberOfBuffers)) {
            synchStopIdx = 0;
        }
        do {
            if (buffer[readSynchIdx].triggered) {
                bool ret = true;
                uint32 c;
                for (c = 0u; (c < numberOfCopies) && (ret); c++) {
                    //Copy from the buffer to the DataSource memory
                    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                        ret = MemoryOperationsHelper::Copy(copyTable[c].dataSourcePointer, buffer[readSynchIdx].mem[c],
                                                           copyTable[c].copySize);
                    }
                }
                if (ret) {
                    if (dataSource != NULL_PTR(DataSourceI *)) {
                        ret = dataSource->Synchronise();
                    }
                }
                buffer[readSynchIdx].triggered = false;
            }
            readSynchIdx++;
            if (readSynchIdx == numberOfBuffers) {
                readSynchIdx = 0u;
            }
            //do-while to ensure that the readSynchIdx == synchStopIdx is also included.
        }
        while (readSynchIdx != static_cast<uint32>(synchStopIdx));

        //Wait for new data to be available from the real-time thread.
        err = sem.ResetWait(TTInfiniteWait);
    }
    return err;
}

CLASS_REGISTER(MemoryMapTriggerOutputBroker, "1.0")
}
