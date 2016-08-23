/**
 * @file EmbeddedThread.cpp
 * @brief Source file for class EmbeddedThread
 * @date Aug 23, 2016
 * @author fsartori
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
 * the class EmbeddedThread (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EmbeddedThread.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void EmbeddedThread::SetTimeout(TimeoutType msecTimeout){
    if (msecTimeout == TTInfiniteWait) {
        timeoutHRT = -1;
    } else {
        uint64 tt64 = msecTimeout.HighResolutionTimerTicks();
        if (tt64 < 0x7FFFFFFF){
            timeoutHRT = tt64;
        } else {
            timeoutHRT = 0x7FFFFFFF;
        }
    }
}




/**
 * TODO
 */
EmbeddedThread::EmbeddedThread(){
    threadId = InvalidThreadIdentifier;
    commands = Stop;
    maxCommandCompletionHRT = 0;
    timeoutHRT = -1;
}

/**
 * TODO
 */
virtual EmbeddedThread::~EmbeddedThread(){
    Stop();

}

virtual bool EmbeddedThread::Initialise(StructuredDataI &data){
    uint32  msecTimeout;
    bool ret = data.Read("Timeout",msecTimeout);
    if (ret){
        SetTimeout(msecTimeout);
    }
    ret = data.Read("Timeout",msecTimeout);
    if (ret){
        SetTimeout(msecTimeout);
    }


    return false;
}



EmbeddedThreadStates EmbeddedThread::GetStatus(){
    EmbeddedThreadStates  etos = None;

    if (threadId == InvalidThreadIdentifier) {
                etos = Off;
    }

    if (etos == None){
        if (commands == KeepRunning)  {
                etos = Running;
        } else if (commands == Start) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                etos = TimeoutStarting;
            }
            else            {
                etos = Starting;
            }
        } else if (commands == Stop) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                etos = TimeoutStopping;
            }
            else            {
                etos = Stopping;
            }
        } else if (commands == Kill){
        if (Threads::IsAlive(threadId)){
                int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
                if ((deltaT > 0) && (timeoutHRT != -1)) {
                    etos = TimeoutKilling;
                }
                else {
                    etos = Killing;
                }

        } else {
        etos = Off;
        threadId = InvalidThreadIdentifier;
        }
        }
    }
    return etos;
}

static void  EmbeddedThreadThreadLauncher(const void * const parameters){
    EmbeddedThread *thread;
    // get object
    thread = reinterpret_cast<EmbeddedThread *>(const_cast<void *>(parameters));

    // call
    thread->ThreadStartUp();
}

void EmbeddedThread::ThreadStartUp(){
    commands = KeepRunning;

    ErrorManagement::ErrorType err;

    while (err.ErrorsCleared() && (commands == KeepRunning)){
    err = Loop();
    }
}


ErrorManagement::ErrorType EmbeddedThread::Start(){
    ErrorManagement::ErrorType err;

    //check if thread already running
    if (GetStatus() != Off ){
        err.illegalOperation = true;
    }

    if (err.ErrorsCleared()){
        commands = Start;
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        const void * const parameters = static_cast<void *> (this);

        threadId = Threads::BeginThread(EmbeddedThreadThreadLauncher,parameters);

        err.fatalError =  (threadId == 0);
    }

    return err;
}

/**
 * TODO
 * First time attempt gentle Stop
 * Second time called Kills thread
 */
ErrorManagement::ErrorType EmbeddedThread::Stop(){
    ErrorManagement::ErrorType err;
    EmbeddedThreadStates status = GetStatus();

    //check if thread already running
    if (status == Off ){

    } else if (status == Running ) {
        commands = Stop;
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;

        while(GetStatus() == Stopping){
        Sleep::MSec(1);
        }

        err.timeout = (GetStatus() != Off);

    } else if ((status == TimeoutStopping) || (status == Stopping)){
        commands = Kill;

        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        err.fatalError = Threads::Kill(threadId);

        if (err.ErrorsCleared()){

            while(GetStatus() == Killing){
            Sleep::MSec(1);
            }

        }

        err.timeout = (GetStatus() != Off);

    } else  {
    err.illegalOperation = true;
    }

    return err;
}



}

	
