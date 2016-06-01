/**
 * @file FastPollingMutexSem.cpp
 * @brief Source file for class FastPollingMutexSem
 * @date 22/07/2015
 * @author Giuseppe Ferr�
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
 * the class FastPollingMutexSem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FastPollingMutexSem.h"
#include "ErrorType.h"
#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

FastPollingMutexSem::FastPollingMutexSem() {
    internalFlag = 0;
    flag = &internalFlag;
}

FastPollingMutexSem::FastPollingMutexSem(volatile int32 &externalFlag) {
    flag = &externalFlag;
}

void FastPollingMutexSem::Create(const bool locked) {
    if (locked) {
        *flag = 1;
    }
    else {
        *flag = 0;
    }
}

bool FastPollingMutexSem::Locked() const {
    return *flag == 1;
}

ErrorManagement::ErrorType FastPollingMutexSem::FastLock(const TimeoutType &msecTimeout,
                                                         float64 sleepTime ) {
    uint64 ticksStop = msecTimeout.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();
    ErrorManagement::ErrorType err = ErrorManagement::NoError;

    // sets the default if it is negative
    if (sleepTime < 0.0) {
        sleepTime = 1e-3;
    }
    bool noSleep = IsEqual(sleepTime, 0.0);

    while (!Atomic::TestAndSet(flag)) {
        if (msecTimeout != TTInfiniteWait) {
            uint64 ticks = HighResolutionTimer::Counter();
            if (ticks > ticksStop) {
                err = ErrorManagement::Timeout;
                REPORT_ERROR(ErrorManagement::Timeout, "FastPollingMutexSem: Timeout expired");
                break;
            }
        }

        if (!noSleep) {
           // Sleep::Sec(sleepTime);
        }
    }
    return err;
}

bool FastPollingMutexSem::FastTryLock() {
    return (Atomic::TestAndSet(flag));
}

void FastPollingMutexSem::FastUnLock() {
    *flag = 0;
}

}
