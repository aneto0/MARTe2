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

#ifndef EVENTSEM2_H_
#define EVENTSEM2_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ErrorType.h"
#include "ErrorManagement.h"
#include "Synchronizer.h"
#include "FastPollingMutexSem.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Event semaphore.
 *
 * @details An optimised and resource-full Synchronizer
 * the handling of the semaphore is mainly in user space.
 * the cpu is yielded and kernel calls are performed only when the semaphore is red
 */
class DLL_API EventSem2: public Synchronizer {

public:
	/**
	 *
	 */
	enum EventMode{
		Latching = 0,
		AutoResetting = 1,
		Counting = 2,
		Invalid = 15,
		Exit = 31
	};


    /**
     * @brief Initializes the semaphore operating system specific properties.
     */
    EventSem2();

    /**
     * @brief If it was not already closed, the destructor closes the semaphore.
     */
    virtual ~EventSem2();

    /**
     * @brief Creates the semaphore.
     * @return true if the operating system call returns with no errors.
     */
    ErrorManagement::ErrorType Open(EventMode mode);

    /**
     * @brief Closes the semaphore.
     * @return true if the operating system call returns with no errors.
     */
    ErrorManagement::ErrorType Close();

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
    ErrorManagement::ErrorType Wait(const MilliSeconds &timeout,bool resetBeforeWait);

    /**
     * @brief Resets the semaphore (raises the barrier).
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Reset();

    /**
     * @brief Posts the semaphore (lowers the barrier).
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Post(uint32 count);

    /**
     * @brief Checks if the semaphore is closed.
     * @return true if the semaphore is closed.
     */
    uint32 Status() const;

private:
    /**
     * Number of threads waiting
     */
    volatile int32  waiters;

    /**
     * positive is a green status, 0 and below zero is negative.
     * negative values are used to store temporary over-locked states
     */
    volatile int32  status;

    /**
     * mode = 0 --> status is 0 or 1. 1 means green. Post makes it green. Reset makes it red.
     * mode = 1 --> status is 0 or 1. 1 means green. AutoResets on each thread exit.
     * mode = 2 --> status is -max to max. <=0 means red. Upon exit status--; Post => status++; reset => status = 0;
     */
    EventMode mode;

    /**
     *	protects the critical variables
     */
    FastPollingMutexSem lock;

    /**
     * wait here if access denied by lock
     */
    Synchronizer lockHev;

    /**
     *
     */
    ErrorManagement::ErrorType Lock(MilliSeconds &timeout);

    /**
     *
     */
    ErrorManagement::ErrorType UnLock();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/




}
#endif /* EVENTSEM_H_ */

