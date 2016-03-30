/**
 * @file SingleThreadGAMScheduler.cpp
 * @brief Source file for class SingleThreadGAMScheduler
 * @date 22/mar/2016
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
 * the class SingleThreadGAMScheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SingleThreadGAMScheduler.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

struct RTThreadParam {
    ReferenceT<RealTimeThread> thread;
    uint32 activeBuffer;
    volatile int32 *spinLock;
};

static void RTThreadRoutine(RTThreadParam &par) {

    ReferenceT<GAMI> *gamArray = par.thread->GetGAMs();
    uint32 numberOfGAMs = par.thread->GetNumberOfGAMs();
    while (*(par.spinLock) == 0) {
        for (uint32 i = 0u; i < numberOfGAMs; i++) {
            if (gamArray[i].IsValid()) {
                gamArray[i]->Execute(par.activeBuffer);
            }
            else {
                //TODO Invalid gam?
            }
        }
    }

    Threads::EndThread();
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SingleThreadGAMScheduler::SingleThreadGAMScheduler() :
        GAMSchedulerI() {
    currentActiveBuffer = 0u;
    spinLock = 0;
}

void SingleThreadGAMScheduler::StartExecution(const uint32 activeBuffer) {

    if (statesInExecution[activeBuffer]->GetNumberOfThreads() > 1u) {
        //TODO Warning
    }
    currentActiveBuffer = activeBuffer;

    ReferenceT<RealTimeThread> thread = statesInExecution[activeBuffer]->Peek(0);
    if (thread.IsValid()) {
        RTThreadParam param;
        param.activeBuffer = activeBuffer;
        param.thread = thread;
        param.spinLock = &spinLock;
        uint32 stackSize = thread->GetStackSize();
        tid = Threads::BeginThread(reinterpret_cast<ThreadFunctionType>(RTThreadRoutine), &param, stackSize, thread->GetName(), ExceptionHandler::NotHandled,
                                   thread->GetCPU());
    }
    else {
        //TODO Invalid thread?
    }
}

void SingleThreadGAMScheduler::StopExecution() {

    ReferenceT<RealTimeThread> thread = statesInExecution[currentActiveBuffer]->Peek(0);
    if (thread.IsValid()) {
        Atomic::Increment(&spinLock);
    }

    // safe exit
    while (Threads::IsAlive(tid)) {
        Sleep::MSec(1);
    }

    Atomic::Decrement(&spinLock);
}

}
