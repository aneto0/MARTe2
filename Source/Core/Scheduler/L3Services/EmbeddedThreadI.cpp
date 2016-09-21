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
}

EmbeddedThreadI::~EmbeddedThreadI() {

}

void EmbeddedThreadI::LaunchThread() {
    if (!Threads::IsAlive(threadId)) {
        const void * const parameters = static_cast<void *>(this);
        threadId = Threads::BeginThread(ServiceThreadLauncher, parameters);
    }
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
        threadId = 0;
    }
}

}

