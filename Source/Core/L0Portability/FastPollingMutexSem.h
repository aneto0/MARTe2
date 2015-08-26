/**
 * @file FastPollingMutexSem.h
 * @brief Header file for class FastPollingMutexSem
 * @date 18/06/2015
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

 * @details This header file contains the declaration of the class FastPollingMutexSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTPOLLINGMUTEXSEM_H_
#define FASTPOLLINGMUTEXSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "Atomic.h"
#include "ErrorType.h"
#include "HighResolutionTimer.h"
#include "TimeoutType.h"
#include "Sleep.h"
#include "Logger.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Implementation of semaphore based on spin locks.
 *
 * @details This semaphore is not recursive i.e is the same thread locks two times sequentially causes a deadlock.
 * Moreover a thread can unlock the semaphore locked by another thread.
 */
class FastPollingMutexSem {

public:

    /**
     * @brief Initializes the semaphore as unlocked.
     * @details The atomic variable is set to zero.
     */
    inline FastPollingMutexSem();

    /**
     * @brief Initializes the semaphore as locked or unlocked.
     * @param[in] locked defines if the semaphore must be initialized locked or unlocked (default locked=false)
     */
    inline void Create(const bool locked = false);

    /**
     * @brief Returns the status of the semaphore.
     * @return true if the semaphore is locked, false if it is unlocked.
     */
    inline bool Locked() const;

    /**
     * @brief Locks the semaphore.
     * @details If the semaphore is locked tries to lock until the timeout expire. A double consecutive lock
     * by the same thread causes a deadlock.
     * @param[in] msecTimeout is the desired timeout.
     * @return Timeout if the semaphore is locked for a period which is greater than the
     * specified timeout. Otherwise NoError is returned.
     */
    inline ErrorType FastLock(const TimeoutType &msecTimeout = TTInfiniteWait);

    /**
     * @brief Tries to lock and in case of failure returns immediately.
     * @return true if the semaphore was unlocked and the thread locks it, false if it was
     * already locked.
     */
    inline bool FastTryLock();

    /**
     * @brief Unlocks the semaphore.
     * @details A thread could unlock the semaphore locked by another thread.
     * @details If a thread locks this type of semaphore, another threads can unlock it.
     */
    inline void FastUnLock();

private:

    /**
     * Atomic variable
     */
    volatile int32 flag;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

FastPollingMutexSem::FastPollingMutexSem() {
    flag = 0;
}

void FastPollingMutexSem::Create(const bool locked) {
    if (locked) {
        flag = 1;
    }
    else {
        flag = 0;
    }
}

bool FastPollingMutexSem::Locked() const {
    return flag == 1;
}

ErrorType FastPollingMutexSem::FastLock(const TimeoutType &msecTimeout) {
    int64 ticksStop = msecTimeout.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();
    ErrorType err = NoError;
    while (!Atomic::TestAndSet(&flag)) {
        if (msecTimeout != TTInfiniteWait) {
            int64 ticks = HighResolutionTimer::Counter();
            if (ticks > ticksStop) {
                err = Timeout;
                REPORT_LOG_MESSAGE(Timeout,"Information: timeout occurred")
                break;
            }
        }
        // yield CPU
        Sleep::MSec(1);
    }
    return err;
}

bool FastPollingMutexSem::FastTryLock() {
    return (Atomic::TestAndSet(&flag));
}

void FastPollingMutexSem::FastUnLock() {
    flag = 0;
}

#endif /* FASTPOLLINGMUTEXSEM_H_ */
