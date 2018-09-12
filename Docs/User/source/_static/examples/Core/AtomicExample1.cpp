/**
 * @file AtomicExample1.cpp
 * @brief Source file for class AtomicExample1
 * @date 23/04/2018
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
 * the class AtomicExample1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Atomic.h"
#include "ErrorLoggerExample.h"
#include "Sleep.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//The lock variable
static MARTe::int32 locked = 0;

//Unlock after sleeping
static void UnlockWithTestAndSet (const void * const args) {
    MARTe::Sleep::Sec(2.0);
    locked = 0;
}

int main(int argc, char *argv[]) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    int32 a = 3;
    int32 b = 4;

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Starting with a=%d b=%d", a, b);
    //Exchange
    b = Atomic::Exchange(&a, b);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "After exchanging a=%d b=%d", a, b);
    //Atomic increment
    Atomic::Increment(&a);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "After incrementing a=%d", a);

    locked = 1;
    //Try to enter a locked region
    if (!Atomic::TestAndSet(&locked)) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "As expected TestAndSet failed");
    }
    locked = 0;
    if (!Atomic::TestAndSet(&locked)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "TestAndSet and failed");
    }

    //Lock again
    REPORT_ERROR_STATIC(ErrorManagement::Information, "locked should now be 1 locked = %d", locked);
    //Create thread to perform the unlock
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to wait for thread to unlock");
    Threads::BeginThread(UnlockWithTestAndSet);
    //Spin lock
    while (!Atomic::TestAndSet(&locked))
        ;

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Unlocked by thread");

    return 0;
}

