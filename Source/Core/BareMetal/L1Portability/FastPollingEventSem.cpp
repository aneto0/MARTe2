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

#include "ErrorManagement.h"
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
                                                         const uint32 sleepTimeUsec) const {
    // default initialisation is without error
	ErrorManagement::ErrorType err;

    uint64 ticksSleep = timeout.HighResolutionTimerTicks();
    uint64 ticksStop = HighResolutionTimer::Counter() + ticksSleep;


    while (*flag == 0) {
        if (timeout != TTInfiniteWait) {

        	/*
        	 * to proper handle saturation the best approach is to first calculate missing time as unsigned
        	 * This calculation works even if tickStop is result of an overflow.
        	 * When the time is passed the result would be negative and thus overflows as a very large int
        	 */
        	uint64 toWait = HighResolutionTimer::Counter() - ticksStop;
            if (toWait > ticksSleep) {
                err = ErrorManagement::Timeout;
                break;
            }
        }
        if (sleepTimeUsec > 0) {
            Sleep::PreciseUsec(sleepTimeUsec,0);
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
                                                              const uint32 sleepTimeUsec) {
    Reset();
    return FastWait(timeout, sleepTimeUsec);
}

}
