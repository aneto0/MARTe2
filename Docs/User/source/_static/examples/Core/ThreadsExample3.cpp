/**
 * @file ThreadsExample2.cpp
 * @brief Source file for class ThreadsExample2
 * @date 23/04/2018
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
 * the class ThreadsExample2 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorLoggerExample.h"
#include "FastPollingEventSem.h"
#include "FastPollingMutexSem.h"
#include "Sleep.h"
#include "StreamString.h"
#include "Threads.h"
#include "ThreadInformation.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//An exit condition for all the threads
static MARTe::int32 exitAfterCalls = 50;

//Shared variable to be incremented by the threads
static MARTe::int32 sharedVariable = 0;

//Event semaphore so that all the threads start at the same time
static MARTe::FastPollingEventSem eventSem;

//Mutex semaphore protecting the sharedVariable
static MARTe::FastPollingMutexSem mutexSem;

//The number of threads
static MARTe::int32 numberOfThreads = 10;

//The number of threads to be started
static MARTe::int32 numberOfThreadsLeftToStart = numberOfThreads;

//The number of threads terminated
static MARTe::int32 numberOfThreadsTerminated = 0;

//Simulate complicated analysis
static void ComplexAnalysis(float sec){
    MARTe::Sleep::Sec(sec);
}

//Thread function call back
static void IncrementDecrementFunction(const void * const params){
    using namespace MARTe;
    float32 *sleepSec = (float32 *) params;
    ThreadInformation info;
    Threads::GetThreadInfoCopy(info, Threads::Id());
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Thread %s waiting for event sem", info.ThreadName());
    numberOfThreadsLeftToStart--;
    if(!eventSem.FastWait()){
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Thread %s failed to wait in event sem (timeout?)", info.ThreadName());
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Thread %s started", info.ThreadName());
    while(exitAfterCalls > 0){
        //The mutex protects this region of code
        if(!mutexSem.FastLock()){
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Thread %s failed to wait in mutex sem (timeout?)", info.ThreadName());
        }
        sharedVariable++;
        ComplexAnalysis(*sleepSec);
        sharedVariable--;
        exitAfterCalls--;
        mutexSem.FastUnLock();
    }
    numberOfThreadsTerminated++;
}

int main(int argc, char *argv[]){
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    int32 i = 0;

    float32 *sleepTimes = new float32[numberOfThreads];
    //Configure the semaphores
    eventSem.Create();
    eventSem.Reset();
    mutexSem.Create(false);

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Number of threads = %d", numberOfThreads);
    for(i=0; i<numberOfThreads; i++){
        StreamString threadName;
        threadName.Printf("Thread-%d", i);
        sleepTimes[i] = ((i + 1) * 1e-3);
        Threads::BeginThread(&IncrementDecrementFunction, &sleepTimes[i], THREADS_DEFAULT_STACKSIZE, threadName.Buffer(), ExceptionHandler::NotHandled, 0x1);
    }
    //FastWait for all threads to be ready and waiting on the event semaphore (this isn't actually needed, but guarantees that the
    //REPORT_ERROR_STATIC come in the right order)
    while(!Atomic::TestAndSet(&numberOfThreadsLeftToStart));

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Starting all threads!");
    //Allow all threads to start
    eventSem.FastPost();
    //FastWait for the thread to run for exitAfterCalls times
    while(exitAfterCalls > 0){
        MARTe::Sleep::Sec(1e-3);
    }
    //Wait to check if some calculation are still to be terminated
    while (numberOfThreadsTerminated < numberOfThreads) {
        MARTe::Sleep::Sec(1e-3);
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Value of sharedVariable = %d (should be zero)", sharedVariable);
    delete sleepTimes;
    return 0;
}
