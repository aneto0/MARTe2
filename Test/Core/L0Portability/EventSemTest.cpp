/**
 * @file EventSemTest.cpp
 * @brief Source file for class EventSemTest
 * @date 26/giu/2015
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
 * the class EventSemTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EventSemTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSemTest::EventSemTest() {
    eventSem.Create();
}

EventSemTest::~EventSemTest() {
    eventSem.Close();
}

bool EventSemTest::TestWait(TimeoutType timeout) {

    Error myErrorType = Debug;

    if (eventSem.Wait(timeout, myErrorType)) {
        return False;
    }

    return myErrorType == Timeout;

}

bool EventSemTest::TestPost() {
    if (!eventSem.Post()) {
        return False;
    }

    if (eventSem.Post()) {
        return False;
    }

    if (!eventSem.Reset()) {
        return False;
    }

    if (!eventSem.Post()) {
        return False;
    }

    return True;
}
