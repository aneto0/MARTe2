/**
 * @file MultiClientEmbeddedThread.cpp
 * @brief Source file for class MultiClientEmbeddedThread
 * @date 02/09/2016
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
 * the class MultiThreadService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MultiClientEmbeddedThread.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MultiClientEmbeddedThread::MultiClientEmbeddedThread(EmbeddedServiceMethodBinderI &binder,
                                                     MultiClientService &managerIn) :
        EmbeddedThreadI(binder),
        manager(managerIn) {
}

MultiClientEmbeddedThread::~MultiClientEmbeddedThread() {
}

void MultiClientEmbeddedThread::ThreadLoop() {
    SetCommands(KeepRunningCommand);
    ExecutionInfo information;
    information.Reset();
    information.SetThreadNumber(threadNumber);
    // any error in execution will only abort the sequence - but not the thread
    // thread is killed at this stage if commands != KeepRunningCommand or if there more service threads that the minimum needed
    //Allow new threads to enter....
    bool moreThanEnoughtThreads = false;
    ErrorManagement::ErrorType err;
    while ((GetCommands() == KeepRunningCommand) && (!moreThanEnoughtThreads)) {
        information.SetStage(ExecutionInfo::StartupStage);
        information.SetStageSpecific(ExecutionInfo::NullStageSpecific);
        if (GetCommands() == KeepRunningCommand) {
            err = Execute(information);
        } // start

        // main loop - wait for service request - service the request
        bool errorsCleared = err.ErrorsCleared();
        while ((GetCommands() == KeepRunningCommand) && (errorsCleared)) {
            information.SetStage(ExecutionInfo::MainStage);
            information.SetStageSpecific(ExecutionInfo::WaitRequestStageSpecific);

            // simulate timeout to allow entering next loop
            err.timeout = true;
            // wait for service request loop
            // keep at it only if answer is timeout
            // any other answer - including no errors - continue
            bool hasToContinue = (err == ErrorManagement::Timeout);
            while ((GetCommands() == KeepRunningCommand) && (hasToContinue)) {
                err = Execute(information);
                hasToContinue = (err == ErrorManagement::Timeout);
            } // wait service

            errorsCleared = err.ErrorsCleared();
            if ((GetCommands() == KeepRunningCommand) && (errorsCleared)) {
                // Try start new service thread
                bool threadAddedOk = manager.AddThread();
                if (!threadAddedOk) {
                    REPORT_ERROR(ErrorManagement::RecoverableError, "Failed to AddThread... Increase the maximum number of threads allowed in the MultiClientService...");
                }

                information.SetStageSpecific(ExecutionInfo::ServiceRequestStageSpecific);
                // exit on error including ErrorManagement::completed
                errorsCleared = err.ErrorsCleared();
                while ((GetCommands() == KeepRunningCommand) && (errorsCleared)) {
                    err = Execute(information);
                    errorsCleared = err.ErrorsCleared();
                }
            } // loop service
            errorsCleared = err.ErrorsCleared();
        } // loop (wait service - loop (service) ) -

        // assuming one reason for exiting (not multiple errors together with a command change)
        information.SetStageSpecific(ExecutionInfo::NullStageSpecific);
        if (err.completed.operator bool()) {
            information.SetStage(ExecutionInfo::TerminationStage);
        }
        else {
            information.SetStage(ExecutionInfo::BadTerminationStage);
        }
        err = Execute(information);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::RecoverableError, "Callback returned error. Restarting MultiClientEmbeddedThread loop.");
        }

        moreThanEnoughtThreads = manager.MoreThanEnoughThreads();
    } // main loop (start - loop (wait service - loop (service) ) - end)
    err = manager.RemoveThread(GetThreadId());
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::RecoverableError, "Failed to remove thread from pool");
    }
}

}
