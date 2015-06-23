/**
 * @file EventSem.cpp
 * @brief Source file for class EventSem
 * @date 17/06/2015
 * @author Giuseppe Ferrò
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
 * the class EventSem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSem::EventSem() {
}

EventSem::EventSem(HANDLE h) {
    Init(h);
}

EventSem::~EventSem() {
    Close();
}

bool EventSem::Create() {
    return EventSemOS::Create(semH);
}

bool EventSem::Close(void) {
    return EventSemOS::Close(semH);
}

bool EventSem::Wait(TimeoutType msecTimeout, Error &error) {
    return EventSemOS::Wait(semH, msecTimeout, error);
}

bool EventSem::ResetWait(TimeoutType msecTimeout, Error &error) {
    return EventSemOS::ResetWait(semH, msecTimeout, error);
}

bool EventSem::Post() {
    return EventSemOS::Post(semH);
}

bool EventSem::Reset() {
    return EventSemOS::Reset(semH);
}

