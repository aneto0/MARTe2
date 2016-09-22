/**
 * @file EmbeddedThreadI.cpp
 * @brief Source file for class EmbeddedThreadI
 * @date 21/09/2016
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
 * the class EmbeddedThreadI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EmbeddedThreadI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
static void ServiceThreadLauncher(const void * const parameters) {
    EmbeddedThreadI * thread = dynamic_cast<EmbeddedThreadI *>(reinterpret_cast<EmbeddedThreadI *>(const_cast<void *>(parameters)));

    // call
    thread->ThreadLoop();

}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EmbeddedThreadI::EmbeddedThreadI(EmbeddedServiceMethodBinderI &binder) :
        method(binder) {
    threadId = InvalidThreadIdentifier;
    threadNumber = 0u;
    commands = StopCommand;
    maxCommandCompletionHRT = 0u;
    timeoutHRT = -1;
}

EmbeddedThreadI::~EmbeddedThreadI() {

}

uint16 EmbeddedThreadI::GetThreadNumber() const {
    return threadNumber;
}

void EmbeddedThreadI::SetThreadNumber(const uint16 threadNumberIn) {
    threadNumber = threadNumberIn;
}

EmbeddedThreadI::Commands EmbeddedThreadI::GetCommands() const {
    return commands;
}

void EmbeddedThreadI::SetCommands(Commands commandsIn) {
    commands = commandsIn;
}

ThreadIdentifier EmbeddedThreadI::GetThreadId() const {
    return threadId;
}

void EmbeddedThreadI::ResetThreadId() {
    if (!Threads::IsAlive(threadId)) {
        threadId = InvalidThreadIdentifier;
    }
}

void EmbeddedThreadI::SetTimeout(TimeoutType msecTimeoutIn) {
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

TimeoutType EmbeddedThreadI::GetTimeout() const {
    return msecTimeout;
}

EmbeddedThreadI::States EmbeddedThreadI::GetStatus() {
    States status = NoneState;
    EmbeddedThreadI::Commands commands = GetCommands();
    bool isAlive = false;

    if (GetThreadId() == InvalidThreadIdentifier) {
        status = OffState;
    }
    else {
        isAlive = Threads::IsAlive(GetThreadId());

        if (!isAlive) {
            status = OffState;
            ResetThreadId();
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

ErrorManagement::ErrorType EmbeddedThreadI::Start() {
    ErrorManagement::ErrorType err;

    //check if thread already running
    if (GetStatus() != OffState) {
        err.illegalOperation = true;
    }
    if (err.ErrorsCleared()) {
        err.illegalOperation = Threads::IsAlive(threadId);
    }
    if (err.ErrorsCleared()) {
        SetCommands(EmbeddedThreadI::StartCommand);
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        const void * const parameters = static_cast<void *>(this);
        threadId = Threads::BeginThread(ServiceThreadLauncher, parameters);

        err.fatalError = (GetThreadId() == 0);
    }

    return err;
}

ErrorManagement::ErrorType EmbeddedThreadI::Stop() {
    ErrorManagement::ErrorType err;
    States status = GetStatus();
    //check if thread already stopped
    if (status == OffState) {

    }
    else if (status == RunningState) {
        SetCommands(EmbeddedThreadI::StopCommand);
        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;

        while (GetStatus() == StoppingState) {
            Sleep::MSec(1);
        }

        err.timeout = (GetStatus() != OffState);
        if (err.ErrorsCleared()) {
            ResetThreadId();
        }

    }
    else if ((status == TimeoutStoppingState) || (status == StoppingState)) {
        SetCommands(EmbeddedThreadI::KillCommand);

        maxCommandCompletionHRT = HighResolutionTimer::Counter32() + timeoutHRT;
        err.fatalError = !Threads::Kill(GetThreadId());

        if (err.ErrorsCleared()) {

            while (GetStatus() == KillingState) {
                Sleep::MSec(1);
            }

        }

        err.timeout = (GetStatus() != OffState);

        // in any case notify the main object of the fact that the thread has been killed
        ExecutionInfo information;
        information.SetThreadNumber(GetThreadNumber());
        information.SetStage(ExecutionInfo::AsyncTerminationStage);
        Execute(information);
        ResetThreadId();
    }
    else {
        err.illegalOperation = true;
    }

    return err;
}

}

