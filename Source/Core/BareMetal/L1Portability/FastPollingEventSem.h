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

/**
 * @brief This class is an event semaphore based on spin locks.
 */
class DLL_API FastPollingEventSem {
public:

    /**
     * @brief Constructor.
     * @details This constructor initialized the internal spin-lock such that the FastWait()
     * operation will be blocking waiting for the FastPost().
     */
    FastPollingEventSem();

    /**
     * @brief Constructor by external spin-lock.
     * @param[in] the external spin-lock which will drive the semaphore.
     */
    FastPollingEventSem(volatile int32 &externalFlag);

    /**
     * @brief Sets the spin-lock value.
     * @param[in] wait specifies if the next FastWait() operation will blocking (true), or if
     * the semaphore is supposed to be posted and it is necessary a Reset() before the next reuse (false).
     */
    void Create(const bool wait = true);

    /**
     * @brief Waits until the spin-lock will be set by a FastPost (or the timeout expire).
     * @param[in] msecTimeout is the maximum amount of time to wait for the FastPost() before exiting.
     * @param[in] sleepTime is the amount of time to release the CPU in each waiting loop cycle.
     * @return ErrorManagement::NoError if the spin-lock is set by a FastPost(), ErrorManagement::Timeout if
     * the timeout expires before the FastPost().
     */
    ErrorManagement::ErrorType FastWait(const TimeoutType &msecTimeout = TTInfiniteWait,
                                        float64 sleepTime = 1e-3);

    /**
     * @brief Posts the semaphore.
     */
    void FastPost();

    /**
     * @brief Resets the semaphore in oreder to be reused again.
     */
    void Reset();

    /**
     * @brief Resets the semaphore and waits on the post.
     * @param[in] msecTimeout is the maximum amount of time to wait for the FastPost() before exiting.
     * @param[in] sleepTime is the amount of time to release the CPU in each waiting loop cycle.
     * @return ErrorManagement::NoError if the spin-lock is set by a FastPost(), ErrorManagement::Timeout if
     * the timeout expires before the FastPost().
     */
    ErrorManagement::ErrorType FastResetWait(const TimeoutType &msecTimeout = TTInfiniteWait,
                                             float64 sleepTime = 1e-3);

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

