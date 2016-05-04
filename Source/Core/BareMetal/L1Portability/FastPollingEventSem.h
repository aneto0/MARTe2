/**
 * @file FastPollingEventSem.h
 * @brief Header file for class FastPollingEventSem
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

/**
 * @brief This class is an event semaphore based on spin locks which can be used
 * without depending on the operating system scheduler.
 * @details The FastPollingEventSem offers the possibility to block any number of threads in
 * a spin-lock based barrier (i.e  without depending to the operating system scheduler).
 */
class DLL_API FastPollingEventSem {
public:

    /**
     * @brief Initialises the semaphore as unlocked.
     */
    FastPollingEventSem();

    /**
     * @brief Initialises the semaphore against an external spin-lock variable.
     * @param[in] externalFlag the external spin-lock which will drive the semaphore.
     */
    FastPollingEventSem(volatile int32 &externalFlag);

    /**
     * @brief Sets the spin-lock barrier value.
     * @param[in] wait if true the semaphore starts in a blocking state, i.e. with a lowered barrier.
     * If the semaphore is not started in a blocking state then Reset must be called in order to lower the barrier.
     */
    void Create(const bool wait = true);

    /**
     * @brief Waits until the spin-lock barrier is raised by a FastPost (or if the timeout expire).
     * @param[in] timeout is the maximum amount of time to wait for the FastPost() before exiting.
     * @param[in] sleepTime is the amount of time the CPU is to be released in-between each polling loop cycle.
     * If sleepTime = 0 the CPU is never released and the spin-lock is continuously polled.
     * @return ErrorManagement::NoError if the spin-lock barrier is raised by a FastPost(), ErrorManagement::Timeout if
     * the timeout expires before the FastPost().
     * @pre Reset is called to lower the barrier.
     */
    ErrorManagement::ErrorType FastWait(const TimeoutType &timeout = TTInfiniteWait,
                                        float64 sleepTime = 1e-3) const;

    /**
     * @brief Posts the semaphore raising the spin-lock barrier.
     */
    void FastPost();

    /**
     * @brief Resets the semaphore and lowers the spin-lock barrier.
     */
    void Reset();

    /**
     * @brief Resets the semaphore and waits.
     * @param[in] timeout is the maximum amount of time to wait for the FastPost() before exiting.
     * @param[in] sleepTime is the amount of time the CPU is to be released in-between each polling loop cycle.
     * If sleepTime = 0 the CPU is never released and the spin-lock is continuously polled.
     * @return ErrorManagement::NoError if the spin-lock is set by a FastPost(), ErrorManagement::Timeout if
     * the timeout expires before the FastPost().
     */
    ErrorManagement::ErrorType FastResetWait(const TimeoutType &timeout = TTInfiniteWait,
                                             const float64 &sleepTime = 1e-3);

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

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTPOLLINGEVENTSEM_H_ */

