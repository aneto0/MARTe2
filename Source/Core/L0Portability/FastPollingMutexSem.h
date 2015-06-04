/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence for the specific language governing 
 permissions and limitations under the Licence. 
 *
 * $Id:$
 *
 **/

/**
 * @file
 * @brief A semaphore based on spin locks.
 */
#ifndef FAST_POLLING_MUTEX_SEM
#define FAST_POLLING_MUTEX_SEM

#include "GeneralDefinitions.h"
#include "Atomic.h"
#include "HighResolutionTimer.h"
#include "TimeoutType.h"
#include "Sleep.h"

/** @brief A semaphore based on spin locks.
  *
  * This type of semaphore uses just a single spinlock instruction: Atomic::TestAndSet. The Timeout is calculated using the HighResolutionTimer class. 
  *
  * With this type of semaphores we have a different implementation of the lock with timeout, where a thread tries to lock until the timeout is expired without 
  * the failure of the mutex (thing that happens using pthread_mutex functions).
  *
  * Furthermore a thread can unlocks a semaphore locked by another thread. */
class FastPollingMutexSem {
protected:
    
    /** Atomic variable */
    volatile int32 flag;
public:
    /** @brief Constructor. */
    FastPollingMutexSem() {
        flag = 0;
    }

    /** @brief Initializes the semaphore and readies it. 
      * @param locked define if the semaphore must be initialized locked or unlocked.
      * @return true. */
    bool Create(bool locked = False) {
        if (locked == True)
            flag = 1;
        else
            flag = 0;
        return True;
    }

    /** @brief Undo semphore initialization. 
      * @return true. */
    bool Close() {
        return True;
    }

    /** @brief Returns the status of the semaphore.
      * @return true if the semaphore is locked. */
    inline bool Locked() {
        return flag == 1;
    }

    /** @brief If the semaphore is locked tries to lock until the timeout is expired.
      * @param msecTimeout is the timeout.
      * @return true if it be able to lock the semaphore before the expired of the timeout,
      * false otherwise. */
    inline bool FastLock(TimeoutType msecTimeout = TTInfiniteWait) {
        int64 ticksStop = msecTimeout.HighResolutionTimerTicks();
        ticksStop += HighResolutionTimer::Counter();
        while (!Atomic::TestAndSet((int32 *) &flag)) {
            if (msecTimeout != TTInfiniteWait) {
                int64 ticks = HighResolutionTimer::Counter();
                if (ticks > ticksStop)
                    return False;
            }
            // yield CPU
            SleepMSec(1);
        }
        return True;
    }

    /** @brief Tries to lock without waiting. 
      * @return true if the semaphore was unlocked and locks it, false if it was 
      * already locked. */ 
    inline bool FastTryLock() {
        return (Atomic::TestAndSet((int32 *) &flag));
    }

    /** @brief Unlocks mux.
      * If a thread locks this type of semaphore, another threads can unlock it. 
      * @return true. */
    inline bool FastUnLock(void) {
        flag = 0;
        return True;
    }
};

#endif

