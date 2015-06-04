/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
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
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id:  $
 *
 **/

/**
 * @file 
 * @brief A semaphore used to synchronise several tasks.
 */

#ifndef EVENT_SEM_H
#define EVENT_SEM_H

#include "GeneralDefinitions.h"
#include "SemCore.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,EventSemOS.h)
/** 
 * @brief This semaphore is used mostly for thread syncronization.
 *
 * After being Reset the semaphore is ready to Wait.
 * Once waiting, until a Post arrives all the threads will wait on 
 * the semaphore. After the post all tasks are allowed to proceed.
 * A Reset is then required to use the semaphore again.
 *
 * Most of the implementation is delegated to the EventSemOS.h file
 * which is different for each operating system and provides non-portable
 * functions to implement this kind of semaphore.  
 */


class EventSem: public SemCore {
public:
    /** @brief Constructor. */
    EventSem() {
    }

    /** @brief Constructor by semaphore pointer. 
      * @param h is a pointer to the sempahore structure. */
    EventSem(HANDLE h) {
        Init(h);
    }

    /** @brief Copies semaphore and special infos as well. */
    void operator=(EventSem &s) {
        *this = s;
    }

    /** @brief Destructor */
    ~EventSem() {
        Close();
    }

    /** @brief Creates the semafore.
        @return true if the initialization of the semH handle goes fine (pthread initialization). */
    bool Create() {
        return EventSemCreate(semH);
    }

    /** @brief Closes the semafore.
        @return true if the system level function returns without errors. */
    bool Close(void) {
        return EventSemClose(semH);
    }

    /** @brief Wait for an event.
      * @return true if the system level function returns without errors. */
    bool Wait(TimeoutType msecTimeout = TTInfiniteWait) {
        return EventSemWait(semH, msecTimeout);
    }

    /** @brief Resets the semafore and then waits.
      * @return true if both system level Reset and Wait functions return true.*/
    bool ResetWait(TimeoutType msecTimeout = TTInfiniteWait) {
        return EventSemResetWait(semH, msecTimeout);
    }

    /** @brief Send an event to semafore, unlocking it.
        @return true if the system level function returns without errors. */
    bool Post() {
        return EventSemPost(semH);
    }

    /** @brief Reset the semafore to its unposted state.
      * @return true if the semaphore state is resetted correctly. */
    bool Reset() {
        return EventSemReset(semH);
    }
};

#endif

