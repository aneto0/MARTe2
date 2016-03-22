/**
 * @file FastPollingEventSem.h
 * @brief Header file for class FastPollingEventSem
 * @date 08/ago/2015
 * @author pc
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

 * @details This header file contains the declaration of the class FastPollingEventSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTPOLLINGEVENTSEM_H_
#define FASTPOLLINGEVENTSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Atomic.h"
#include "ErrorType.h"
#include "HighResolutionTimer.h"
#include "Sleep.h"
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class FastPollingEventSem {
public:

    inline FastPollingEventSem();

    inline void Create();

    inline ErrorManagement::ErrorType FastWait(const TimeoutType &msecTimeout = TTInfiniteWait);

    inline void FastPost();

    inline void Reset();

    inline ErrorManagement::ErrorType FastResetWait(const TimeoutType &msecTimeout = TTInfiniteWait);

private:
    /**
     * Atomic variable
     */
    volatile int32 flag;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
FastPollingEventSem::FastPollingEventSem() {
    flag = 0;
}

void FastPollingEventSem::Create() {
    flag = 0;
}

ErrorManagement::ErrorType FastPollingEventSem::FastWait(const TimeoutType &msecTimeout) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    uint64 ticksStop = msecTimeout.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();

    while (flag == 0u) {
        if (msecTimeout != TTInfiniteWait) {
            if (HighResolutionTimer::Counter() > ticksStop) {
                err = ErrorManagement::Timeout;
                break;
            }
        }
        Sleep::MSec(1);
    }

    return err;
}

void FastPollingEventSem::FastPost() {
    (void) (Atomic::TestAndSet(&flag));
}

void FastPollingEventSem::Reset() {
    flag = 0;
}

ErrorManagement::ErrorType FastPollingEventSem::FastResetWait(const TimeoutType &msecTimeout) {
    Reset();
    return FastWait(msecTimeout);
}

}
#endif /* FASTPOLLINGEVENTSEM_H_ */

