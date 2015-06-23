/**
 * @file FastPollingMutexSem.h
 * @brief Header file for class FastPollingMutexSem
 * @date 18/06/2015
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

 * @details This header file contains the declaration of the class FastPollingMutexSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTPOLLINGMUTEXSEM_H_
#define 		FASTPOLLINGMUTEXSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "Atomic.h"
#include "HighResolutionTimer.h"
#include "TimeoutType.h"
#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Implementation of semaphore based on spin locks.
 *
 * @details This type of semaphore uses the spinlock instruction: Atomic::TestAndSet. The Timeout is calculated using the HighResolutionTimer class.\n
 * Using this implementation a thread can unlocks a semaphore locked by another thread.
 */
class FastPollingMutexSem {

public:
    /**
     * @brief Constructor.
     */
    inline FastPollingMutexSem();

    /**
     * @brief Initializes the semaphore and reads it.
     * @param[in] locked defines if the semaphore must be initialized locked or unlocked.
     */
    inline void Create(bool locked = False);


    /**
     * @brief Returns the status of the semaphore.
     * @return true if the semaphore is locked.
     */
    inline bool Locked() const;

    /**
     * @brief If the semaphore is locked tries to lock until the timeout is expired.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error is the error type in return.
     * @return true if the thread locks the semaphore before the timeout expired,
     * false otherwise.
     */
    inline bool FastLock(TimeoutType msecTimeout = TTInfiniteWait,
                         Error &error = Global::errorType);

    /**
     * @brief Tries to lock and in case of failure returns immediately.
     * @return true if the semaphore was unlocked and the thread locks it, false if it was
     * already locked.
     */
    inline bool FastTryLock();

    /**
     * @brief Unlocks the semaphore.
     * @details If a thread locks this type of semaphore, another threads can unlock it.
     */
    inline void FastUnLock();

protected:

    /** Atomic variable */
    volatile int32 flag;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
inline FastPollingMutexSem::FastPollingMutexSem() {
    flag = 0;
}

void FastPollingMutexSem::Create(bool locked) {
    if (locked == True) {
        flag = 1;
    }
    else {
        flag = 0;
    }
}


bool FastPollingMutexSem::Locked() const{
    return flag == 1;
}

bool FastPollingMutexSem::FastLock(TimeoutType msecTimeout,
                                   Error &error) {
    int64 ticksStop = msecTimeout.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();
    while (!Atomic::TestAndSet((int32 *) &flag)) {
        if (msecTimeout != TTInfiniteWait) {
            int64 ticks = HighResolutionTimer::Counter();
            if (ticks > ticksStop) {
                error = Timeout;
                return False;
            }
        }
        // yield CPU
        SleepMSec(1);
    }
    return True;
}

bool FastPollingMutexSem::FastTryLock() {
    return (Atomic::TestAndSet((int32 *) &flag));
}

void FastPollingMutexSem::FastUnLock() {
    flag = 0;
}

#endif /* FASTPOLLINGMUTEXSEM_H_ */

