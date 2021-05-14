/**
 * @file FastScheduler.cpp
 * @brief Source file for class FastScheduler
 * @date 21/05/2020
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
 * the class FastScheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ExecutionInfo.h"
#include "FastScheduler.h"
#include "MultiThreadService.h"
#include "RealTimeApplication.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

const uint64 ALL_CPUS = 0xFFFFFFFFFFFFFFFFull;

FastScheduler::FastScheduler() :
        GAMSchedulerI(),
        binder(*this, &FastScheduler::Execute) {
    initialised = false;
    cpuMap = NULL_PTR(uint64 **);
    cpuThreadMap = NULL_PTR(uint32 **);
    multiThreadService = NULL_PTR(MultiThreadService *);
    rtThreadInfo[0] = NULL_PTR(RTThreadParam *);
    rtThreadInfo[1] = NULL_PTR(RTThreadParam *);
    if (!eventSem.Create()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Create(*) of the event semaphore");
    }
    if (!unusedThreadsSem.Create()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Create(*) of the event semaphore");
    }
    maxNThreads = 0u;
    superFast = 0u;
}

/*lint -e{1551} the destructor must guarantee that the resources are stopped and freed. No exception should be thrown given that
 * the memory addresses are checked before being used.*/
FastScheduler::~FastScheduler() {
    if (!eventSem.Post()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
    }
    if (!countingSem.ForcePass()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
    }
    if (!unusedThreadsSem.Post()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
    }
    if (multiThreadService != NULL_PTR(MultiThreadService *)) {
        ErrorManagement::ErrorType err;
        err = multiThreadService->Stop();
        if (!err.ErrorsCleared()) {
            //REPORT_ERROR(ErrorManagement::FatalError, "Could not StopCurrentStateExecution multiThreadService[1]");
        }
        delete multiThreadService;
    }
    if (rtThreadInfo[0] != NULL) {
        delete rtThreadInfo[0];
    }
    if (rtThreadInfo[1] != NULL) {
        delete rtThreadInfo[1];
    }
    if (cpuMap != NULL) {
        for (uint32 i = 0u; i < numberOfStates; i++) {
            if (cpuMap[i] != NULL) {
                delete cpuMap[i];
            }
        }
        delete cpuMap;
    }
    if (cpuThreadMap != NULL) {
        for (uint32 i = 0u; i < numberOfStates; i++) {
            if (cpuThreadMap[i] != NULL) {
                delete cpuThreadMap[i];
            }
        }
        delete cpuThreadMap;
    }
}

bool FastScheduler::Initialise(StructuredDataI & data) {
    bool ret = GAMSchedulerI::Initialise(data);
    if (ret) {
        if (!data.Read("NoWait", superFast)) {
            superFast = 0u;
        }

        if (Size() > 0u) {
            ret = (Size() == 1u);
            if (ret) {
                errorMessage = Get(0u);
                ret = errorMessage.IsValid();
                if (!ret) {
                    //REPORT_ERROR(ErrorManagement::ParametersError, "The ErrorMessage is not valid");
                }
            }
            else {
                //REPORT_ERROR(ErrorManagement::ParametersError, "Only one ErrorMessage shall be defined");
            }
        }
    }

    return ret;
}

bool FastScheduler::ConfigureScheduler(Reference realTimeAppIn) {
    bool ret = GAMSchedulerI::ConfigureScheduler(realTimeAppIn);
    if (ret) {
        realTimeApplicationT = realTimeApp;
        ret = realTimeApplicationT.IsValid();
    }

    return ret;
}

void FastScheduler::Purge(ReferenceContainer &purgeList) {
    //Post the semaphore to make sure that no Threads are awaiting to start!
    if (!eventSem.Post()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
    }
    if (!unusedThreadsSem.Post()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
    }
    if (!countingSem.ForcePass()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
    }
    if (multiThreadService != NULL) {
        ErrorManagement::ErrorType err;
        err = multiThreadService->Stop();
        if (!err.ErrorsCleared()) {
            //REPORT_ERROR(ErrorManagement::FatalError, "Could not StopCurrentStateExecution multiThreadService[1]");
        }
    }
    ReferenceContainer::Purge(purgeList);
}

ErrorManagement::ErrorType FastScheduler::StartNextStateExecution() {

    ErrorManagement::ErrorType err;
    err = !realTimeApplicationT.IsValid();

    if (err.ErrorsCleared()) {
        err = (multiThreadService == NULL);
    }
    if (err.ErrorsCleared()) {
        *currentStateIdentifier=nextStateIdentifier;
        if (!initialised) {
            /*lint -e{613} multiThreadService cannot be NULL as otherwise err != ErrorsCleared*/
            err = multiThreadService->Start();
            if (!err.ErrorsCleared()) {
                //REPORT_ERROR(ErrorManagement::FatalError, "Failed to Start() MultiThreadService.");
            }
            initialised = true;
        }
        //stop all and release the useless ones
        if (superFast == 0u) {
            if (!countingSem.Reset()) {
                //REPORT_ERROR(ErrorManagement::FatalError, "Failed Reset(*) of the event semaphore");
            }
        }

        if (!unusedThreadsSem.Post()) {
            //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
        }
        (void) unusedThreadsSem.Reset();
        if (!eventSem.Post()) {
            //REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
        }

    }
    return err;
}

ErrorManagement::ErrorType FastScheduler::StopCurrentStateExecution() {
    ErrorManagement::ErrorType err(realTimeApplicationT.IsValid());

    if (!eventSem.Reset()) {
        //REPORT_ERROR(ErrorManagement::FatalError, "Failed Reset(*) of the event semaphore");
    }

    return err;
}

void FastScheduler::ComputeMaxNThreads() {

    for (uint32 i = 0u; i < numberOfStates; i++) {
        uint32 nThreads = states[i].numberOfThreads;
        for (uint32 j = 0u; j < nThreads; j++) {
            uint64 cpu = static_cast<uint64>(states[i].threads[j].cpu.GetProcessorMask());
            //different from the previous
            bool found = false;
            //loop on the previous states and previous states to find the same cpu
            for (uint32 h = 0u; (h <= i) && (!found); h++) {
                uint32 nOtherThreads = states[h].numberOfThreads;
                if (h == i) {
                    nOtherThreads = j;
                }
                for (uint32 k = 0u; (k < nOtherThreads) && (!found); k++) {
                    uint64 otherCpu = static_cast<uint64>(states[h].threads[k].cpu.GetProcessorMask());
                    found = (otherCpu == cpu);
                }
            }

            if (!found) {
                uint32 maxNcpus = 0u;
                //different from all the others
                for (uint32 h = i; h < numberOfStates; h++) {
                    uint32 nEqualCpus = 0u;
                    for (uint32 k = 0u; k < states[h].numberOfThreads; k++) {
                        uint64 otherCpu = static_cast<uint64>(states[h].threads[k].cpu.GetProcessorMask());
                        if (otherCpu == cpu) {
                            nEqualCpus++;
                        }
                    }
                    if (nEqualCpus > maxNcpus) {
                        maxNcpus = nEqualCpus;
                    }
                }
                maxNThreads += maxNcpus;
            }
        }
    }
}

/*lint -e{613} the caller of CreateThreadMap guarantees that the cpuMap memory is allocated*/
/*lint -e{850} variables are not modified by the REPORT_ERROR*/
void FastScheduler::CreateThreadMap(const uint64 cpu,
                                    const uint32 state,
                                    const uint32 thread) {
    bool found = false;
    uint32 UNDEFINED_THREAD_ID = 0xFFFFFFFFu;
    uint32 firstInvalid = UNDEFINED_THREAD_ID;

    //match the cpu with the threads on the same line
    for (uint32 h = 0u; (h < maxNThreads) && (!found); h++) {
        if (cpuMap[state][h] == ALL_CPUS) {
            bool allInvalids = false;
            for (uint32 k = 0u; (k < state) && (!found); k++) {
                found = (cpuMap[k][h] == cpu);
                if (found) {
                    //the cpu belongs to thread h
                    cpuMap[state][h] = cpu;
                    //map the thread i,j to the thread h
                    cpuThreadMap[state][thread] = h;
                    REPORT_ERROR(ErrorManagement::Information, "cpuThreadMap[%d][%d]=%!", state, thread, h);
                }
                allInvalids = (cpuMap[k][h] == ALL_CPUS);
            }
            if (allInvalids || (state == 0u)) {
                if (firstInvalid == UNDEFINED_THREAD_ID) {
                    firstInvalid = h;
                }
            }
        }
    }
    if (!found) {
        multiThreadService->SetCPUMaskThreadPool(BitSet(cpu), firstInvalid);
        multiThreadService->SetPriorityClassThreadPool(Threads::RealTimePriorityClass, firstInvalid);

        cpuMap[state][firstInvalid] = cpu;
        cpuThreadMap[state][thread] = firstInvalid;
        REPORT_ERROR(ErrorManagement::Information, "cpuThreadMap[%d][%d]=%!", state, thread, firstInvalid);
    }
}

ErrorManagement::ErrorType FastScheduler::SetupThreadMap() {
    ErrorManagement::ErrorType err;
    ComputeMaxNThreads();
    REPORT_ERROR(ErrorManagement::Information, "Max Threads=%!", maxNThreads);

    multiThreadService = new (NULL) MultiThreadService(binder);
    multiThreadService->SetNumberOfPoolThreads(maxNThreads);
    err = multiThreadService->CreateThreads();
    if (err.ErrorsCleared()) {
        rtThreadInfo[0] = new RTThreadParam[maxNThreads];
        rtThreadInfo[1] = new RTThreadParam[maxNThreads];

        //set all as invalid
        for (uint32 i = 0u; i < numberOfStates; i++) {
            cpuMap[i] = new uint64[maxNThreads];
            for (uint32 j = 0u; j < maxNThreads; j++) {
                cpuMap[i][j] = ALL_CPUS;
            }
        }

        if (countingSem.Create(maxNThreads)) {

            //group for each state the threads depending on the cpu
            //if a cpu is not used in that state it will be invalid
            for (uint32 i = 0u; i < numberOfStates; i++) {
                uint32 nThreads = states[i].numberOfThreads;
                cpuThreadMap[i] = new uint32[nThreads];
                for (uint32 j = 0u; j < nThreads; j++) {

                    uint64 cpu = static_cast<uint64>(states[i].threads[j].cpu.GetProcessorMask());
                    CreateThreadMap(cpu, i, j);
                }
            }

            //set all as invalid
            /*lint -e{850} variables are not modified by the REPORT_ERROR*/
            for (uint32 i = 0u; i < numberOfStates; i++) {
                for (uint32 j = 0u; j < maxNThreads; j++) {
                    REPORT_ERROR(ErrorManagement::Information, "cpuMap[%d][%d]=%!", i, j, cpuMap[i][j]);
                }
            }
        }
    }
    return err;
}

/*lint -e{613} rtThreadInfo != NULL is guaranteed by the caller, i.e. the function will not be reached*/
void FastScheduler::CustomPrepareNextState() {
    ErrorManagement::ErrorType err;

    err = !realTimeApplicationT.IsValid();
    if (err.ErrorsCleared()) {
        uint8 nextBuffer = static_cast<uint8>(realTimeApplicationT->GetIndex());
        nextBuffer++;
        nextBuffer &= 0x1u;

        if (!initialised) {
            cpuMap = new uint64*[numberOfStates];
            cpuThreadMap = new uint32*[numberOfStates];
            err = SetupThreadMap();
        }
        if (err.ErrorsCleared()) {
            //reset
            for (uint32 j = 0u; j < maxNThreads; j++) {
                rtThreadInfo[nextBuffer][j].executables = NULL_PTR(ExecutableI **);
                rtThreadInfo[nextBuffer][j].numberOfExecutables = 0u;
                rtThreadInfo[nextBuffer][j].cycleTime = NULL_PTR(uint32 *);
                rtThreadInfo[nextBuffer][j].lastCycleTimeStamp = 0u;
            }

            //Launches the threads for the next state
            ScheduledState *nextState = GetSchedulableStates()[nextBuffer];
            uint32 numberOfThreads = nextState->numberOfThreads;
            for (uint32 i = 0u; i < numberOfThreads; i++) {
                rtThreadInfo[nextBuffer][cpuThreadMap[nextStateIdentifier][i]].executables = nextState->threads[i].executables;
                rtThreadInfo[nextBuffer][cpuThreadMap[nextStateIdentifier][i]].numberOfExecutables = nextState->threads[i].numberOfExecutables;
                rtThreadInfo[nextBuffer][cpuThreadMap[nextStateIdentifier][i]].cycleTime = nextState->threads[i].cycleTime;
                rtThreadInfo[nextBuffer][cpuThreadMap[nextStateIdentifier][i]].lastCycleTimeStamp = 0u;
                REPORT_ERROR(ErrorManagement::FatalError, "Configuring rtThreadInfo[%d][%d]=%!", nextBuffer, cpuThreadMap[nextStateIdentifier][i],
                        rtThreadInfo[nextBuffer][cpuThreadMap[nextStateIdentifier][i]].numberOfExecutables);
            }
        }
    }
}

/*lint -e{1764} EmbeddedServiceMethodBinderI callback method pointer prototype requires a non constant ExecutionInfo*/
ErrorManagement::ErrorType FastScheduler::Execute(ExecutionInfo & information) {
    ErrorManagement::ErrorType ret;

    if (information.GetStage() == MARTe::ExecutionInfo::StartupStage) {
    }
    else if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
        //everything ok at the post
        uint32 threadNumber = information.GetThreadNumber();
        //normal wait for an explicit post
        (void) eventSem.Wait(TTInfiniteWait);
        if (superFast == 0u) {
            (void) countingSem.WaitForAll(TTInfiniteWait);
        }

        uint32 idx = static_cast<uint32>(realTimeApplicationT->GetIndex());

        if (rtThreadInfo[idx] != NULL_PTR(RTThreadParam *)) {
            if (rtThreadInfo[idx][threadNumber].numberOfExecutables > 0u) {
                bool ok = ExecuteSingleCycle(rtThreadInfo[idx][threadNumber].executables, rtThreadInfo[idx][threadNumber].numberOfExecutables);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to ExecuteSingleCycle().");
                    //Do not set ret.fatalError = true because when ExecuteSingleCycle returns false it will trigger the MultiThreadService to restart the execution of ThreadLoop.
                    //If this was not handled then it would wait on eventSem.Wait(TTInfiniteWait) every time ExecuteSingleCycle returns false.
                    //ret.fatalError = true;
                    if (errorMessage.IsValid()) {
                        if (MessageI::SendMessage(errorMessage, this) != ErrorManagement::NoError) {
                            //REPORT_ERROR(ErrorManagement::FatalError, "Failed to SendMessage.");
                        }
                    }
                }
                uint32 absTime = 0u;
                if (rtThreadInfo[idx][threadNumber].lastCycleTimeStamp != 0u) {
                    uint64 tmp = (HighResolutionTimer::Counter() - rtThreadInfo[idx][threadNumber].lastCycleTimeStamp);
                    float64 ticksToTime = (static_cast<float64>(tmp) * clockPeriod) * 1e6;
                    absTime = static_cast<uint32>(ticksToTime); //us
                }
                uint32 sizeToCopy = static_cast<uint32>(sizeof(uint32));
                (void)MemoryOperationsHelper::Copy(rtThreadInfo[idx][threadNumber].cycleTime, &absTime, sizeToCopy);
                rtThreadInfo[idx][threadNumber].lastCycleTimeStamp = HighResolutionTimer::Counter();
            }
            else {
                (void) unusedThreadsSem.Wait(TTInfiniteWait);
            }
        }
        else {
            //REPORT_ERROR(ErrorManagement::FatalError, "RTThreadParam is NULL.");
        }
    }
    else {
        //Other states not used.
    }
    return ret;
}

CLASS_REGISTER(FastScheduler, "1.0")

}

