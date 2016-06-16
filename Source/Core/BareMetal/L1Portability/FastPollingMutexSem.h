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
#define FASTPOLLINGMUTEXSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Atomic.h"
#include "ErrorManagement.h"
#include "ErrorType.h"
#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"
#include "TimeoutType.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class is a mutex semaphore based on spin locks which can be used
 * without depending on the operating system scheduler.
 *
 * @details This semaphore is not recursive i.e is the same thread locks two times sequentially causes a deadlock.
 * Moreover a thread can unlock the semaphore locked by another thread.
 */
class DLL_API FastPollingMutexSem {

public:

    /**
     * @brief Initializes the semaphore as unlocked.
     * @details The atomic variable is set to zero.
     */
    FastPollingMutexSem();


    /**
     * @brief Constructor by external spin-lock.
     * @param[in] externalFlag is the spin-lock which will drive the semaphore.
     */
    FastPollingMutexSem(volatile int32 &externalFlag);

    /**
     * @brief Initializes the semaphore as locked or unlocked.
     * @param[in] locked defines if the semaphore is to be initialized in a locked in an unlocked state (default locked=false)
     */
    void Create(const bool locked = false);

    /**
     * @brief Returns the status of the semaphore.
     * @return true if the semaphore is locked, false if it is unlocked.
     */
    bool Locked() const;

    /**
     * @brief Locks the semaphore.
     * @details If the semaphore is locked tries to lock until the timeout expire. A double consecutive lock
     * by the same thread causes a deadlock.
     * @param[in] timeout is the desired timeout.
     * @param[in] sleepTime is the amount of time the CPU is to be released in-between each polling loop cycle.
     * If sleepTime = 0 the CPU is never released and the spin-lock is continuously polled.
     * @return ErrorManagement::Timeout if the semaphore is locked for a period which is greater than the
     * specified timeout. Otherwise ErrorManagement::NoError is returned.
     */
    ErrorManagement::ErrorType FastLock(const TimeoutType &timeout = TTInfiniteWait,
                                        float64 sleepTime = 1e-3);

    /**
     * @brief Tries to lock and in case of failure returns immediately.
     * @return true if the semaphore was unlocked and the thread locks it, false if it was
     * already locked.
     */
    bool FastTryLock();

    /**
     * @brief Unlocks the semaphore.
     * @details A thread could unlock the semaphore locked by another thread.
     * @details If a thread locks this type of semaphore, another threads can unlock it.
     */
    void FastUnLock();

private:

    /**
     * The internal spin-lock
     */
    volatile int32 internalFlag;

    /**
     * Pointer to the used spin-lock
     */
    volatile int32 *flag;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* FASTPOLLINGMUTEXSEM_H_ */
