/**
 * @file GAMScheduler.cpp
 * @brief Source file for class GAMScheduler
 * @date 23/mar/2016
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

#include "GAMScheduler.h"
#include "Threads.h"
#include "RealTimeDataSourceOutputWriter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

struct RTThreadParam {
    ReferenceT<RealTimeThread> thread;
    uint32 activeBuffer;
    ReferenceT<RealTimeDataSourceOutputWriter> writer;
    volatile int32 *spinLock;
};

static void RTThreadRoutine(RTThreadParam &par) {

    ReferenceT<GAMI> *gamArray = par.thread->GetGAMs();
    uint32 numberOfGAMs = par.thread->GetNumberOfGAMs();
    while (*(par.spinLock) == 0) {
        uint64 absTic = HighResolutionTimer::Counter();
        for (uint32 i = 0u; i < numberOfGAMs; i++) {
            if (gamArray[i].IsValid()) {
                gamArray[i]->Execute(par.activeBuffer);
                // save the time before
                uint64 relTic = HighResolutionTimer::Counter();
                // execute the gam
                gamArray[i]->Execute(par.activeBuffer);
                // writes the time stamps
                // 2*i because for each gam we have absolute and relative variable inserted
                uint64 * absTime = reinterpret_cast<uint64 *>((par.writer)->GetData(2 * i));
                *absTime = static_cast<uint64>(HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), absTic) * 1e6);
                uint64 * relTime = reinterpret_cast<uint64 *>((par.writer)->GetData((2 * i) + 1u));
                *relTime = static_cast<uint64>(HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), relTic) * 1e6);
                (par.writer)->Write(par.activeBuffer);
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

GAMScheduler::GAMScheduler() :
        GAMSchedulerI() {
    spinLock = 0;
    tid = NULL_PTR(ThreadIdentifier *);
    numberOfThreads = 0u;
}

GAMScheduler::~GAMScheduler() {
    if (tid != NULL) {
        delete [] tid;
        tid=NULL_PTR(ThreadIdentifier *);
    }
}

void GAMScheduler::StartExecution(const uint32 activeBuffer) {

    if (statesInExecution[activeBuffer]->GetNumberOfThreads() > 1u) {
        //TODO Warning
    }

    numberOfThreads = statesInExecution[activeBuffer]->GetNumberOfThreads();
    tid = new ThreadIdentifier[numberOfThreads];
    for (uint32 i = 0u; i < numberOfThreads; i++) {
        ReferenceT<RealTimeThread> thread = statesInExecution[activeBuffer]->Peek(i);
        if (thread.IsValid()) {
            RTThreadParam param;
            param.activeBuffer = activeBuffer;
            param.thread = thread;
            param.spinLock = &spinLock;
            param.writer = (writer[activeBuffer])[i];
            if (param.writer.IsValid()) {

                uint32 stackSize = thread->GetStackSize();
                tid[i] = Threads::BeginThread(reinterpret_cast<ThreadFunctionType>(RTThreadRoutine), &param, stackSize, thread->GetName(),
                                              ExceptionHandler::NotHandled, thread->GetCPU());
            }
            else {
                //TODO Invalid writer
            }
        }
        else {
            //TODO Invalid thread?
        }
    }
}

void GAMScheduler::StopExecution() {

    if (tid != NULL) {
        Atomic::Increment(&spinLock);

        for (uint32 i = 0u; i < numberOfThreads; i++) {
            // safe exit
            while (Threads::IsAlive(tid[i])) {
                Sleep::MSec(1);
            }
        }
        delete[] tid;
        tid=NULL_PTR(ThreadIdentifier *);
        Atomic::Decrement(&spinLock);
    }

}

}
