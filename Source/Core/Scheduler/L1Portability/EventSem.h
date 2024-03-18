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
#define EVENTSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "ErrorType.h"
#include "FastPollingMutexSem.h"
#include "GeneralDefinitions.h"
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * Forward declaration of the operating system specific properties (defined in the operating system
 * specific unit file).
 */
//struct EventSemProperties;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Event semaphore.
 *
 * @details The EventSem offer the possibility to block any number of threads in
 * a barrier. This barrier is lowered by calling the Reset method and raised
 * by the Post method. Threads are blocked in the barrier by calling one of the Wait methods.
 * Once the barrier is raised all the threads are allowed to concurrently proceed.
 */
class DLL_API EventSem {

public:

    /**
     * @brief Initializes the semaphore operating system specific properties.
     */
    EventSem();

    /**
     * @brief This semaphore instance will share the same low-level semaphore
     * (i.e. that share the same handle) with the source semaphore.
     * @param[in] source the semaphore providing the access to the low-level semaphore.
     */
    /*lint -e{1724} source cannot be made a const reference*/
    EventSem(EventSem &source);

    /**
     * @brief If it was not already closed, the destructor closes the semaphore.
     */
    virtual ~EventSem();

    /**
     * @brief Creates the semaphore.
     * @return true if the operating system call returns with no errors.
     */
    bool Create();

    /**
     * @brief Closes the semaphore.
     * @return true if the operating system call returns with no errors.
     */
    bool Close();

    /**
     * @brief Waits for a post event without timeout (i.e. possibly forever).
     * @details Calling this function on a semaphore that was not reset will
     * not block the calling thread.
     * @return ErrorManagement::NoError if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Wait();

    /**
     * @brief Waits for a post event, limited by the timeout time.
     * @details Calling this function on a semaphore that was not reset will
     * not block the calling thread.
     * @param[in] timeout the maximum time that the barrier will be set.
     * @return ErrorManagement::NoError if the operating system call returns with no errors or
     * Timeout if the time waiting in the barrier (from when the function was called)
     * was greater than the specified timeout.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Wait(const TimeoutType &timeout);

    /**
     * @brief Resets the semaphore (raises the barrier).
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    virtual bool Reset();

    /**
     * @brief Posts the semaphore (lowers the barrier).
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    bool Post();

    /**
     * @brief Resets the semaphore (raises the barrier) and waits.
     * @param[in] timeout is the desired timeout.
     * @return true if both system level Reset returns true and
     * the Wait function returns ErrorManagement::NoError.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType ResetWait(const TimeoutType &timeout);

    /**
     * @brief Return the operating system low level properties.
     * @return the operating system low level semaphore properties structure.
     */
    EventSemProperties *GetProperties();

    /**
     * @brief Checks if the semaphore is closed.
     * @return true if the semaphore is closed.
     */
    bool IsClosed() const;

private:

    /**
     * Operating system specific properties to be used by the operating system specific implementation
     */
    EventSemHandle handle;

    /**
     * Global semaphore to protect access to the handle
     */
    FastPollingMutexSem mux;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEM_H_ */

