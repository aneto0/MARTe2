/**
 * @file IEventSem.h
 * @brief Header file for class EventSem
 * @date 06/07/2015
 * @author André Neto
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

 * @details This header file contains the declaration of the class IEventSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef IEVENTSEM_H_
#define IEVENTSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeoutType.h"
#include "FlagsType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This semaphore is used mostly for thread synchronization.
 *
 * @details After being reset the semaphore is ready to Wait.
 * Once waiting, until a Post arrives all the threads will wait on
 * the semaphore. After the post all tasks are allowed to proceed.
 * A Reset is then required to use the semaphore again.
 *
 * The Lock functions uses an ErrorType object defined in GeneralDefinition.h that could be used by the user to
 * know if an eventual lock error happened because of the timeout or for other reasons.
 *
 * Most of the implementation is delegated to the EventSemOS.h file
 * which is different for each operating system and provides non-portable
 * functions to implement this kind of semaphore.
 */

class IEventSem {
public:
    /**
     * @brief Constructor.
     */
    IEventSem() {
    }

    /**
     * @brief Two semaphores that share the same low-level semaphore
     * (i.e. that share the same handle).
     * @param source the semaphore with the original and valid handle.
     */
    IEventSem(const IEventSem &source) {
    }

    /**
     * @brief Destructor.
     */
    virtual ~IEventSem(){
    }

    /**
     *  @brief Creates the semaphore.
     *  @return true if the semaphore was successfully created and is ready to be used.
     */
    virtual bool Create() = 0;

    /**
     * @brief Closes the semaphore.
     * @return true if the system level function returns without errors.
     */
    virtual bool Close() = 0;

    /**
     * @brief Wait for an event until the timeout expire or a post condition.
     * @param[out] error is the error type in return.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if the system level function returns without errors.
     */
    virtual bool Wait(FlagsType &error, const TimeoutType &msecTimeout) = 0;

    /**
     * @brief Resets the semaphore and then waits.
     * @param[out] error is the error type in return.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if both system level Reset and Wait functions return true.
     */
    virtual bool ResetWait(FlagsType &error, const TimeoutType &msecTimeout) = 0;

    /**
     * @brief Unlocks the semaphore.
     * @return true if the system level function returns without errors.
     */
    virtual bool Post() = 0;

    /**
     * @brief Reset the semaphore to its unposted state.
     * @return true if the semaphore state is resetted correctly.
     */
    virtual bool Reset() = 0;
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEM_H_ */

