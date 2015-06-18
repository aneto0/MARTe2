/**
 * @file EventSem.h
 * @brief Header file for class EventSem
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class EventSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEM_H_
#define 		EVENTSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "SemCore.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,EventSemOS.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This semaphore is used mostly for thread syncronization.
 *
 * @details After being reset the semaphore is ready to Wait.
 * Once waiting, until a Post arrives all the threads will wait on
 * the semaphore. After the post all tasks are allowed to proceed.
 * A Reset is then required to use the semaphore again.
 *
 * @details Most of the implementation is delegated to the EventSemOS.h file
 * which is different for each operating system and provides non-portable
 * functions to implement this kind of semaphore.
 */

class EventSem: public SemCore {
public:
    /** @brief Constructor. */
    EventSem();

    /**
     * @brief Constructor by semaphore pointer.
     * @param[in] h is a pointer to the semaphore structure.
     */
    EventSem(HANDLE h);

    /** @brief Destructor */
    ~EventSem();


    /**
     *  @brief Creates the semaphore.
     *  @return true if the initialization of the semH handle has success.
     */
    bool Create();

    /**
     * @brief Closes the semaphore.
     * @return true if the system level function returns without errors.
     */
    bool Close();

    /**
     * @brief Wait for an event until the timeout expire or a post condition.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if the system level function returns without errors.
     */
    bool Wait(TimeoutType msecTimeout = TTInfiniteWait);

    /**
     * @brief Resets the semaphore and then waits.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if both system level Reset and Wait functions return true.
     */
    bool ResetWait(TimeoutType msecTimeout = TTInfiniteWait);

    /**
     * @brief Unlocks the semaphore.
     * @return true if the system level function returns without errors.
     */
    bool Post();

    /**
     * @brief Reset the semaphore to its unposted state.
     * @return true if the semaphore state is resetted correctly.
     */
    bool Reset();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEM_H_ */

