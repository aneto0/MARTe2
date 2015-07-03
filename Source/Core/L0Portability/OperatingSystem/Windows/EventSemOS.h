/**
 * @file EventSemOS.h
 * @brief Header file for class EventSemOS
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class EventSemOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEMOS_H_
#define EVENTSEMOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../../TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief System dependent implementation of an event semaphore.
 *
 * @details These functions use a windows event with manual reset that can be triggered by SetEvent function in case of Post operation.
 */
class EventSemOS {

public:
    /**
     * @brief Create a new event semaphore handle.
     * @details Called by EventSem::Create
     * @param[in,out] semH is the semaphore handle in return.
     * @return true if successful, false otherwise.
     */
    static bool Create(HANDLE &semH) {
        //If the handle is valid close it
        CloseHandle(semH);

        semH = CreateEvent(NULL, TRUE, FALSE, NULL);

        return (semH != NULL);
    }

    /**
     * @brief Destroy the event semaphore.
     * @details Called by EventSem::Create
     * @param[in,out] semH is the semaphore handle.
     * @return true if successful, false otherwise.
     */
    static bool Close(HANDLE &semH) {
        if (semH == (HANDLE) NULL)
        return true;
        if (CloseHandle(semH) == FALSE) {
            return false;
        }
        semH = NULL;
        return true;
    }

    /**
     * @brief Wait condition.
     * @details Called by EventSem::Create
     * @param[in,out] semH is the semaphore handle.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error is the error type.
     * @return true if successful, false otherwise.
     */
    static inline bool Wait(HANDLE &semH,
    TimeoutType msecTimeout,
    Error &error) {
        int ret;

        ret = WaitForSingleObject((HEV) semH, msecTimeout.msecTimeout);
        if (ret == (int) WAIT_FAILED) {
            error = OSError;
            return false;
        }
        if (ret == (int) WAIT_TIMEOUT) {
            error = Timeout;
            return false;
        }
        return true;
    }

    /**
     * @brief Post condition.
     * @details Called by EventSem::Post
     * @param[in,out] semH is the semaphore handle.
     * @return true if successful, false otherwise.
     */
    static inline bool Post(HANDLE &semH) {

        if (SetEvent((HEV) semH) == FALSE) {
            return false;
        }
        return true;
    }

    /**
     * @brief Reset the semaphore for a new wait condition.
     * @details Called by EventSem::Reset
     * @param[in,out] semH is the semaphore handle.
     * @return true if successful, false otherwise.
     */
    static inline bool Reset(HANDLE &semH) {

        if (ResetEvent((HEV) semH) == FALSE) {
            return false;
        }
        return true;
    }

    /**
     * @brief Reset and then perform a wait condition of the event semaphore.
     * @details Called by EventSem::Reset
     * @param[in,out] semH is a pointer to the event semaphore.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error is the error type.
     * @return true if successful, false otherwise.
     */
    static inline bool ResetWait(HANDLE &semH,
    TimeoutType msecTimeout,
    Error &error) {
        Reset(semH);
        return Wait(semH, msecTimeout, error);
    }

    /**
     * @brief Do nothing
     */
    static inline void DuplicateHandle(HANDLE &semH) {

    }
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEMOS_H_ */

