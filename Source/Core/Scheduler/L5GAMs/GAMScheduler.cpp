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
#include "MemoryMapOutputWriter.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static void RTThreadRoutine(RTThreadParam &par) {

    while ((*par.spinLock) == 0) {
        par.scheduler->ExecuteSingleCycle(par.threadId, par.activeBuffer);
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
    param = NULL_PTR(RTThreadParam *);
}

GAMScheduler::~GAMScheduler() {
    if (tid != NULL) {
        delete[] tid;
        tid = NULL_PTR(ThreadIdentifier *);
    }
    if (param != NULL) {
        delete[] param;
        param = NULL_PTR(RTThreadParam *);
    }
}

void GAMScheduler::StartExecution(const uint32 activeBuffer) {

    numberOfThreads = statesInExecution[activeBuffer]->GetNumberOfThreads();

    // do always a stop execution before a start execution
    tid = new ThreadIdentifier[numberOfThreads];
    param = new RTThreadParam[numberOfThreads];

    printf("\n#threads = %d\n", numberOfThreads);
    for (uint32 i = 0u; i < numberOfThreads; i++) {
        ReferenceT<RealTimeThread> thread = statesInExecution[activeBuffer]->Peek(i);
        if (thread.IsValid()) {
            param[i].activeBuffer = activeBuffer;
            param[i].scheduler = this;
            param[i].spinLock = &spinLock;
            param[i].threadId = i;
            uint32 stackSize = thread->GetStackSize();
            tid[i] = Threads::BeginThread(reinterpret_cast<ThreadFunctionType>(RTThreadRoutine), &param[i], stackSize, thread->GetName(),
                                          ExceptionHandler::NotHandled, thread->GetCPU());
        }
        else {
            printf("\nInvalid thread\n");
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
                //  printf("\nWaiting here!\n");
                Sleep::MSec(1);
            }
        }
        delete[] tid;
        tid = NULL_PTR(ThreadIdentifier *);
        if (param != NULL) {
            delete[] param;
            param = NULL_PTR(RTThreadParam *);
        }
        Atomic::Decrement(&spinLock);
    }

}

CLASS_REGISTER(GAMScheduler, "1.0")

}
