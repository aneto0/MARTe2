/**
 * @file FastPollingEventSem.cpp
 * @brief Source file for class FastPollingEventSem
 * @date 20/04/2016
 * @author Giuseppe Ferro
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
 * the class FastPollingEventSem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FastPollingEventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
FastPollingEventSem::FastPollingEventSem() {
    internalFlag = 0;
    flag = &internalFlag;
}

FastPollingEventSem::FastPollingEventSem(volatile int32 &externalFlag) {
    internalFlag = 0;
    flag = &externalFlag;
}

void FastPollingEventSem::Create(const bool wait) {
    if (wait) {
        *flag = 0;
    }
    else {
        *flag = 1;
    }
}

ErrorManagement::ErrorType FastPollingEventSem::FastWait(const TimeoutType &timeout,
                                                         float64 sleepTime) const {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    int64 ticksStop = timeout.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();

    // sets the default if negative
    if (sleepTime < 0.0) {
        sleepTime = 1e-3;
    }

    bool noSleep = IsEqual(sleepTime, 0.0);

    while (*flag == 0) {
        if (timeout != TTInfiniteWait) {
            if (HighResolutionTimer::Counter() > ticksStop) {
                err = ErrorManagement::Timeout;
                break;
            }
        }
        if (!noSleep) {
            Sleep::Sec(sleepTime);
        }
    }

    return err;
}

void FastPollingEventSem::FastPost() {
    (void) (Atomic::TestAndSet(flag));
}

void FastPollingEventSem::Reset() {
    *flag = 0;
}

ErrorManagement::ErrorType FastPollingEventSem::FastResetWait(const TimeoutType &timeout,
                                                              const float64 &sleepTime) {
    Reset();
    return FastWait(timeout, sleepTime);
}

}
