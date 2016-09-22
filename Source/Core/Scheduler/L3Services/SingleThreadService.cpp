/**
 * @file SingleThreadService.cpp
 * @brief Source file for class SingleThreadService
 * @date 23/08/2016
 * @author Filippo Sartori
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
 * the class SingleThreadService (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <SingleThreadService.h>
#include "ExecutionInfo.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SingleThreadService::SingleThreadService(EmbeddedServiceMethodBinderI &binder) :
        EmbeddedServiceI(),
        embeddedThread(binder) {
    maxCommandCompletionHRT = 0u;
    SetTimeout(TTInfiniteWait);
}

SingleThreadService::~SingleThreadService() {
    Stop();
    Stop();
}

bool SingleThreadService::Initialise(StructuredDataI &data) {
    uint32 msecTimeout;
    ErrorManagement::ErrorType err;
    err.parametersError = !data.Read("Timeout", msecTimeout);
    if (err.ErrorsCleared()) {
        if (msecTimeout == 0u) {
            msecTimeout = TTInfiniteWait.GetTimeoutMSec();
        }
        SetTimeout(msecTimeout);
    }

    return err;
}

void SingleThreadService::SetTimeout(TimeoutType msecTimeoutIn) {
    msecTimeout = msecTimeoutIn;
    if (msecTimeout == TTInfiniteWait) {
        timeoutHRT = -1;
    }
    else {
        uint64 tt64 = msecTimeout.HighResolutionTimerTicks();
        if (tt64 < 0x7FFFFFFF) {
            timeoutHRT = tt64;
        }
        else {
            timeoutHRT = 0x7FFFFFFF;
        }
    }
}

TimeoutType SingleThreadService::GetTimeout() const {
    return msecTimeout;
}

EmbeddedServiceI::States SingleThreadService::GetStatus() {
    States status = NoneState;
    EmbeddedThreadI::Commands commands = embeddedThread.GetCommands();
    bool isAlive = false;

    if (embeddedThread.GetThreadId() == InvalidThreadIdentifier) {
        status = OffState;
    }
    else {
        isAlive = Threads::IsAlive(embeddedThread.GetThreadId());

        if (!isAlive) {
            status = OffState;
            embeddedThread.ResetThreadId();
        }
    }

    if (status == NoneState) {
        if (commands == EmbeddedThreadI::KeepRunningCommand) {
            status = RunningState;
        }
        else if (commands == EmbeddedThreadI::StartCommand) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                status = TimeoutStartingState;
            }
            else {
                status = StartingState;
            }
        }
        else if (commands == EmbeddedThreadI::StopCommand) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                status = TimeoutStoppingState;
            }
            else {
                status = StoppingState;
            }
        }
        else if (commands == EmbeddedThreadI::KillCommand) {
            int32 deltaT = HighResolutionTimer::Counter32() - maxCommandCompletionHRT;
            if ((deltaT > 0) && (timeoutHRT != -1)) {
                status = TimeoutKillingState;
            }
            else {
                status = KillingState;
            }
        }

    }
    return status;
}

ErrorManagement::ErrorType SingleThreadService::Start() {
    ErrorManagement::ErrorType err;

    //check if thread already running
    if (GetStatus() != OffState) {
        err.illegalOperation = true;
    }

    if (err.ErrorsCleared()) {
        embeddedThread.SetCommands(EmbeddedThreadI::StartCommand);
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        embeddedThread.LaunchThread();

        err.fatalError = (embeddedThread.GetThreadId() == 0);
    }

    return err;
}

ErrorManagement::ErrorType SingleThreadService::Stop() {
    ErrorManagement::ErrorType err;
    States status = GetStatus();
    //check if thread already stopped
    if (status == OffState) {

    }
    else if (status == RunningState) {
        embeddedThread.SetCommands(EmbeddedThreadI::StopCommand);
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;

        while (GetStatus() == StoppingState) {
            Sleep::MSec(1);
        }

        err.timeout = (GetStatus() != OffState);
        if (err.ErrorsCleared()) {
            embeddedThread.ResetThreadId();
        }

    }
    else if ((status == TimeoutStoppingState) || (status == StoppingState)) {
        embeddedThread.SetCommands(EmbeddedThreadI::KillCommand);

        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        err.fatalError = !Threads::Kill(embeddedThread.GetThreadId());

        if (err.ErrorsCleared()) {

            while (GetStatus() == KillingState) {
                Sleep::MSec(1);
            }

        }

        err.timeout = (GetStatus() != OffState);

        // in any case notify the main object of the fact that the thread has been killed
        ExecutionInfo information;
        information.SetThreadNumber(embeddedThread.GetThreadNumber());
        information.SetStage(ExecutionInfo::AsyncTerminationStage);
        embeddedThread.Execute(information);
        embeddedThread.ResetThreadId();
    }
    else {
        err.illegalOperation = true;
    }

    return err;
}

const EmbeddedThread &SingleThreadService::GetThread() {
    return embeddedThread;
}

uint16 SingleThreadService::GetThreadNumber() const {
    return embeddedThread.GetThreadNumber();
}

void SingleThreadService::SetThreadNumber(uint16 threadNumberIn) {
    embeddedThread.SetThreadNumber(threadNumberIn);
}

}
