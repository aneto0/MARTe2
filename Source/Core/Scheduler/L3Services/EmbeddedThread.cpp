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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EmbeddedThread::EmbeddedThread(EmbeddedServiceMethodBinderI &binder) :
        EmbeddedThreadI(binder) {

}

EmbeddedThread::~EmbeddedThread() {

}

void EmbeddedThread::ThreadLoop() {
    commands = KeepRunningCommand;

    ErrorManagement::ErrorType err;

    while (commands == KeepRunningCommand) {
        //Reset sets stage = StartupStage;
        information.Reset();
        information.SetThreadNumber(GetThreadNumber());

        // startup
        err = Execute(information);

        // main stage
        if (err.ErrorsCleared() && (commands == KeepRunningCommand)) {

            information.SetStage(ExecutionInfo::MainStage);
            while (err.ErrorsCleared() && (commands == KeepRunningCommand)) {
                err = Execute(information);
            }
        }

        // assuming one reason for exiting (not multiple errors together with a command change)
        if (err.completed) {
            information.SetStage(ExecutionInfo::TerminationStage);
        }
        else {
            information.SetStage(ExecutionInfo::BadTerminationStage);
        }

        //Return value is ignored as thread cycle will start afresh whatever the return value.
        Execute(information);
    }
}

}
