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

#ifndef Semaphore_H_
#define Semaphore_H_

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
 * @brief Flexible Semaphore able to perform different synchronisation functions
 * @details the handling of the semaphore is mainly in user space.
 * the cpu is yielded and kernel calls are performed only when the semaphore is red
 * Performs the following functions:
 * 1) simple event semaphore: (Latching): Take() waits if red(0) exits immediately if green (1). Set -> Green Reset ->Red
 * 2) serializer (AutoResetting): Take() waits if red(0) and if green(1) exits after turning the semaphore red
 * 3) counting (Counting): Take() waits if semaphore is <=0. if >0 decrements and exits.
 * 4) recursive mutex (Mutex): as AutoResetting, but allows multiple Take() by the owner thread. Reset is disabled and Set is accessible only by the owner.
 * 5) multiLocking (MultiLock): Reset decreases the counter below 0, Set Increases up to 0; Take simply waits
 */
class DLL_API Semaphore: public Synchronizer {

public:
	/**
	 *
	 */
	enum SemaphoreMode{
		Latching,
		AutoResetting,
		Counting,
		Mutex,
		MultiLock,
		Invalid,
		Exit,
		Closed
	};


    /**
     * @brief Initializes the semaphore operating system specific properties.
     */
    Semaphore();

    /**
     * @brief If it was not already closed, the destructor closes the semaphore.
     */
    virtual ~Semaphore();

    /**
     * @brief Creates the semaphore.
     * @return true if the operating system call returns with no errors.
     */
    ErrorManagement::ErrorType Open(SemaphoreMode mode);

    /**
     * @brief Closes the semaphore.
     * @return true if the operating system call returns with no errors.
     */
    ErrorManagement::ErrorType Close();

    /**
     * @brief Waits for semaphore status to be green and then affects the status depending on the mode.
     * @details Behaviour depends on the mode.
     * @param[in] timeout the maximum time that the barrier will be set.
     * @return ErrorManagement::NoError if the operating system call returns with no errors or
     * Timeout if the time waiting in the barrier (from when the function was called)
     * was greater than the specified timeout.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Take(const MilliSeconds &timeout= MilliSeconds::Infinite);

    /**
     * @brief Decreases the Status towards locked/reset/red.Behaviour depends on mode.
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Reset(uint32 count=1);

    /**
     * @brief Increases/Sets the Status. Behaviour depends on mode.
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Set(uint32 count=1);

    /**
     * @brief Checks if the semaphore is closed. >1 is free/posted/green <=0 is locked/reset/red
     * @return true if the semaphore is closed.
     */
    int32 Status() const;

    /**
     * The chosen semaphore mode
     */
    SemaphoreMode Mode() const;

    /**
     * The number of threads waiting
     */
    int32 Waiters() const;


private:
    /**
     */
    ErrorManagement::ErrorType UpdateLockHev(bool wasLocked);

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
    SemaphoreMode mode;

    /**
     * used for recursive mutexes
     */
    ThreadIdentifier owner;

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

    /**
     * disallow copying
     */
    void operator=(const Semaphore &s);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/




}
#endif /* EVENTSEM_H_ */

