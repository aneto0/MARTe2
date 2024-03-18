/**
 * @file EmbeddedThread.cpp
 * @brief Source file for class EmbeddedThread
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
 * the class EmbeddedThread (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EmbeddedThread.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EmbeddedThread::EmbeddedThread(EmbeddedServiceMethodBinderI &binder) :
        EmbeddedThreadI(binder) {
}

EmbeddedThread::EmbeddedThread(EmbeddedServiceMethodBinderI &binder,
                               const uint16 threadNumberIn) :
        EmbeddedThreadI(binder, threadNumberIn) {
}

EmbeddedThread::~EmbeddedThread() {

}

void EmbeddedThread::ThreadLoop() {
    if (mux.FastLock() == ErrorManagement::NoError) {
        //Needed to avoid that the command is reset to KeepRunningCommand after a StopCommand was set (see EmbeddedThreadI::Stop) (i.e. a racing condition)
        bool setKeepRunning = (GetCommands() == EmbeddedThreadI::StartCommand);
        mux.FastUnLock();
        if (setKeepRunning) {
            SetCommands(KeepRunningCommand);
        }
    }

    ErrorManagement::ErrorType err;

    while (GetCommands() == KeepRunningCommand) {
        //Reset sets stage = StartupStage;
        threadId = Threads::Id();
        information.Reset();
        information.SetThreadNumber(threadNumber);

        // startup
        err = Execute(information);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::RecoverableError, "Callback returned error. Entering EmbeddedThread loop nevertheless.");
        }

        // main stage
        if (GetCommands() == KeepRunningCommand) {

            information.SetStage(ExecutionInfo::MainStage);
            bool ok = true;
            while ((GetCommands() == KeepRunningCommand) && (ok)) {
                err = Execute(information);
                ok = err.ErrorsCleared();
            }
        }

        // assuming one reason for exiting (not multiple errors together with a command change)
        if (err.completed.operator bool()) {
            information.SetStage(ExecutionInfo::TerminationStage);
        }
        else {
            information.SetStage(ExecutionInfo::BadTerminationStage);
        }

        err = Execute(information);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::RecoverableError, "Callback returned error. Restarting EmbeddedThread loop.");
        }
    }
}

}
