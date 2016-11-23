/**
 * @file GAMScheduler.cpp
 * @brief Source file for class GAMScheduler
 * @date 20/11/2016
 * @author Giuseppe Ferro
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
 * the class GAMScheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMScheduler.h"
#include "ExecutionInfo.h"
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

GAMScheduler::GAMScheduler() :
        GAMSchedulerI(),
        binder(*this, &GAMScheduler::Execute) {
    cycleTimeStamp = 0u;
    multiThreadService[0] = NULL_PTR(MultiThreadService *);
    multiThreadService[1] = NULL_PTR(MultiThreadService *);
    rtThreadInfo[0] = NULL_PTR(RTThreadParam *);
    rtThreadInfo[1] = NULL_PTR(RTThreadParam *);
    if (!eventSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed Create(*) of the event semaphore");
    }
}

/*lint -e{1551} the destructor must guarantee that the resources are stopped and freed. No exception should be thrown given that
 * the memory addresses are checked before being used.*/
GAMScheduler::~GAMScheduler() {
    if (multiThreadService[0] != NULL) {
        ErrorManagement::ErrorType err;
        err = multiThreadService[0]->Stop();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not StopCurrentStateExecution multiThreadService[0]");
        }
        delete multiThreadService[0];
    }
    if (multiThreadService[1] != NULL) {
        ErrorManagement::ErrorType err;
        err = multiThreadService[1]->Stop();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not StopCurrentStateExecution multiThreadService[1]");
        }
        delete multiThreadService[1];
    }
    if (rtThreadInfo[0] != NULL) {
        delete rtThreadInfo[0];
    }
    if (rtThreadInfo[1] != NULL) {
        delete rtThreadInfo[1];
    }
}

ErrorManagement::ErrorType GAMScheduler::StartNextStateExecution() {
    ErrorManagement::ErrorType err;
    if (GetSchedulableStates() != NULL_PTR(ScheduledState **)) {
        uint32 newBuffer = RealTimeApplication::GetIndex();
        ScheduledState *newState = GetSchedulableStates()[newBuffer];
        if (newState != NULL_PTR(ScheduledState *)) {
            if (!eventSem.Post()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed Post(*) of the event semaphore");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "newState is NULL. Did you call PrepareNextState?");
            err.fatalError = true;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "No states defined.");
        err.fatalError = true;
    }
    return err;
}

ErrorManagement::ErrorType GAMScheduler::StopCurrentStateExecution() {
    uint32 currentIndex = RealTimeApplication::GetIndex();
    ErrorManagement::ErrorType err;
    if (multiThreadService[currentIndex] != NULL) {
        err = multiThreadService[currentIndex]->Stop();
    }
    else {
        err.fatalError = true;
    }
    return err;
}

void GAMScheduler::CustomPrepareNextState() {
    ErrorManagement::ErrorType err;
    if (eventSem.Reset()) {
        //Launches the threads for the next state
        uint32 nextBuffer = (RealTimeApplication::GetIndex() + 1u) % 2u;
        ScheduledState *nextState = GetSchedulableStates()[nextBuffer];
        uint32 numberOfThreads = nextState->numberOfThreads;
        if (multiThreadService[nextBuffer] != NULL) {
            err = multiThreadService[nextBuffer]->Stop();
            delete multiThreadService[nextBuffer];
        }
        if (err.ErrorsCleared()) {
            multiThreadService[nextBuffer] = new (NULL) MultiThreadService(binder);
            multiThreadService[nextBuffer]->SetNumberOfPoolThreads(numberOfThreads);
            if (rtThreadInfo[nextBuffer] != NULL) {
                delete rtThreadInfo[nextBuffer];
            }
            err = multiThreadService[nextBuffer]->CreateThreads();
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to Stop() MultiThreadService.");
        }
        if (err.ErrorsCleared()) {
            rtThreadInfo[nextBuffer] = new RTThreadParam[numberOfThreads];
            for (uint32 i = 0u; i < numberOfThreads; i++) {
                rtThreadInfo[nextBuffer][i].executables = nextState->threads[i].executables;
                rtThreadInfo[nextBuffer][i].numberOfExecutables = nextState->threads[i].numberOfExecutables;
                rtThreadInfo[nextBuffer][i].cycleTime = nextState->threads[i].cycleTime;
                multiThreadService[nextBuffer]->SetPriorityClassThreadPool(Threads::RealTimePriorityClass, i);
                multiThreadService[nextBuffer]->SetCPUMaskThreadPool(nextState->threads[i].cpu, i);
            }
            err = multiThreadService[nextBuffer]->Start();
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to CreateThreads().");
        }
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to Start() MultiThreadService.");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed Reset(*) of the event semaphore");
    }

}

ErrorManagement::ErrorType GAMScheduler::Execute(const ExecutionInfo &information) {
    ErrorManagement::ErrorType ret;
    if (information.GetStage() == MARTe::ExecutionInfo::StartupStage) {
        ret = eventSem.Wait(TTInfiniteWait);
        cycleTimeStamp = HighResolutionTimer::Counter();
    }
    else if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
        uint32 threadNumber = information.GetThreadNumber();
        uint32 idx = RealTimeApplication::GetIndex();
        if (rtThreadInfo[idx] != NULL_PTR(RTThreadParam *)) {
            bool ok = ExecuteSingleCycle(rtThreadInfo[idx][threadNumber].executables, rtThreadInfo[idx][threadNumber].numberOfExecutables);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed to ExecuteSingleCycle().");
                //Do not set ret.fatalError = true because when ExecuteSingleCycle returns false it will trigger the MultiThreadService to restart the execution of ThreadLoop.
                //If this was not handled then it would wait on eventSem.Wait(TTInfiniteWait) every time ExecuteSingleCycle returns false.
                //ret.fatalError = true;
            }
            uint64 tmp = (HighResolutionTimer::Counter() - cycleTimeStamp);
            float64 ticksToTime = (static_cast<float64>(tmp) * clockPeriod) * 1e6;
            uint32 absTime = static_cast<uint32>(ticksToTime);  //us
            uint32 sizeToCopy = static_cast<uint32>(sizeof(uint32));
            if(!MemoryOperationsHelper::Copy(rtThreadInfo[idx][threadNumber].cycleTime, &absTime, sizeToCopy)) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not copy cycle time information.");
            }
            cycleTimeStamp = HighResolutionTimer::Counter();
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "RTThreadParam is NULL.");
        }
    }
    else {
        //Other states not used.
    }
    return ret;
}

CLASS_REGISTER(GAMScheduler, "1.0")

}
