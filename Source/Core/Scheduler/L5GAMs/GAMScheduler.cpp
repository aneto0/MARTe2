/**
 * @file GAMScheduler.cpp
 * @brief Source file for class GAMScheduler
 * @date 09/ago/2016
 * @author pc
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
#define DLL_API

#include "GAMScheduler.h"
#include "Threads.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static void RTThreadRoutine(const RTThreadParam &par) {
    (void) par.eventSem->Wait(TTInfiniteWait);

    float64 clockPeriod = HighResolutionTimer::Period();
    uint64 cycleTimeStamp = HighResolutionTimer::Counter();
    while ((*par.spinLock) == 1) {
        //TODO
        (void) par.scheduler->ExecuteSingleCycle(par.executables, par.numberOfExecutables);
        uint64 tmp=(HighResolutionTimer::Counter() - cycleTimeStamp);
        float64 ticksToTime = (static_cast<float64>(tmp) * clockPeriod) * 1e6;
        uint32 absTime = static_cast<uint32>(ticksToTime);  //us
        uint32 sizeToCopy = static_cast<uint32>(sizeof(uint32));
        (void) MemoryOperationsHelper::Copy(par.cycleTime, &absTime, sizeToCopy);
        cycleTimeStamp = HighResolutionTimer::Counter();
    }

    Threads::EndThread();
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMScheduler::GAMScheduler() :
        GAMSchedulerI() {
    spinLock[0] = 0;
    spinLock[1] = 0;

    tid[0] = NULL_PTR(ThreadIdentifier *);
    tid[1] = NULL_PTR(ThreadIdentifier *);
    param[0] = NULL_PTR(RTThreadParam *);
    param[1] = NULL_PTR(RTThreadParam *);
    if (!eventSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failer Create(*) of the event semaphore");
    }
}

GAMScheduler::~GAMScheduler() {
    if (tid[0] != NULL) {
        delete[] tid[0];
    }
    if (tid[1] != NULL) {
        delete[] tid[1];
    }
    if (param[0] != NULL) {
        delete[] param[0];
    }
    if (param[1] != NULL) {
        delete[] param[1];
    }
}

void GAMScheduler::StartExecution() {
    Atomic::Increment(&spinLock[RealTimeApplication::GetIndex()]);
    if(!eventSem.Post()){
        REPORT_ERROR(ErrorManagement::FatalError, "Failer Post(*) of the event semaphore");
    }
}

void GAMScheduler::StopExecution() {

    uint32 currentIndex = RealTimeApplication::GetIndex();

    if (tid[currentIndex] != NULL) {
        Atomic::Decrement(&spinLock[RealTimeApplication::GetIndex()]);
        uint32 numberOfThreads=GetSchedulableStates()[currentIndex]->numberOfThreads;
        for (uint32 i = 0u; i < numberOfThreads; i++) {
            // safe exit
            while (Threads::IsAlive(tid[currentIndex][i])) {
                Sleep::MSec(1);
            }
        }
    }

}

void GAMScheduler::CustomPrepareNextState() {

    if (eventSem.Reset()) {

        //Launches the threads for the next state
        uint32 nextBuffer = (RealTimeApplication::GetIndex() + 1u) % 2u;
        ScheduledState *nextState = GetSchedulableStates()[nextBuffer];
        uint32 numberOfThreads = nextState->numberOfThreads;
        if (tid[nextBuffer] != NULL) {
            delete[] tid[nextBuffer];
        }
        tid[nextBuffer] = new ThreadIdentifier[numberOfThreads];
        if (param[nextBuffer] != NULL) {
            delete [] param[nextBuffer];
        }
        param[nextBuffer] = new RTThreadParam[numberOfThreads];

        for (uint32 i = 0u; i < numberOfThreads; i++) {
            param[nextBuffer][i].scheduler = this;
            param[nextBuffer][i].spinLock = &spinLock[nextBuffer];
            param[nextBuffer][i].executables = nextState->threads[i].executables;
            param[nextBuffer][i].numberOfExecutables = nextState->threads[i].numberOfExecutables;
            param[nextBuffer][i].cycleTime = nextState->threads[i].cycleTime;
            param[nextBuffer][i].eventSem = &eventSem;
            tid[nextBuffer][i] = Threads::BeginThread(reinterpret_cast<ThreadFunctionType>(&RTThreadRoutine), &param[nextBuffer][i],
                                                      nextState->threads[i].stackSize, nextState->threads[i].name, ExceptionHandler::NotHandled,
                                                      nextState->threads[i].cpu);
        }
    }
    else{
        REPORT_ERROR(ErrorManagement::FatalError, "Failer Reset(*) of the event semaphore");
    }

}

CLASS_REGISTER(GAMScheduler, "1.0")

}
