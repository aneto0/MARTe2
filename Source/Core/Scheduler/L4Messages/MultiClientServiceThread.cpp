/**
 * @file MultiThreadService.cpp
 * @brief Source file for class MultiThreadService
 * @date Sep 2, 2016
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
 * the class MultiThreadService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MultiClientServiceThread.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

MultiClientServiceThread::~MultiClientServiceThread(){ }

MultiClientServiceThread::MultiClientServiceThread(MethodBinderI &binder,MultiThreadService &managerIn): EmbeddedThread(binder),manager(managerIn){ }


void MultiClientServiceThread::ThreadLoop(){
    commands = KeepRunningCommand;
    information.Reset();

    // thread is decontextualised.
    // any error in execution will only abort the sequence - no the thread
    // thread is killed at this stage if commands != KeepRunningCommand or if there more service threads that the minimum needed
    while((commands == KeepRunningCommand) && (!manager.MoreThanEnoughThreads())){
        ErrorManagement::ErrorType err;

        information.SetStage(startupStage);
        information.SetStage2(nullStage2);
        if (commands == KeepRunningCommand){
            err = Execute(information);
        } // start

        // main loop - wait for service request - service the request
        while (err.ErrorsCleared() && (commands == KeepRunningCommand)){
            information.SetStage(mainStage);
            information.SetStage2(waitRequestStage2);

            // simulate timeout to allow entering next loop
            err.timeout = true;
            // wait for service request loop
            // keep at it only if answer is timeout
            // any other answer - including no errors - continue
            while ((err == ErrorManagement::Timeout) && (commands == KeepRunningCommand) ){
                err = Execute(information);
            } // wait service

            if (err.ErrorsCleared() && (commands == KeepRunningCommand)){
                // Try start new service thread
                manager.AddThread();

                information.SetStage2(serviceRequestStage2);
                // exit on error including ErrorManagement::completed
                while (err.ErrorsCleared() && (commands == KeepRunningCommand)){
                    err = Execute(information);
                }
            } // loop service
        } // loop (wait service - loop (service) ) -

        // assuming one reason for exiting (not multiple errors together with a command change)
        information.SetStage2(nullStage2);
        if (err.completed) {
            information.SetStage(terminationStage);
        } else {
            information.SetStage(badTerminationStage);
        }
        Execute(information);

    }// main loop (start - loop (wait service - loop (service) ) - end)

}



}
