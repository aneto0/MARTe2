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
#ifndef LINT
#include <pthread.h>
#include <math.h>
#include <sys/timeb.h>
#else
#include "lint-linux.h"
#endif
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../../IEventSem.h"
#include "../../FastPollingMutexSem.h"
#include "PThreadSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Private event semaphore class used for Solaris and Linux when using pThread.
 *
 * @details The event semaphore is implemented using pthread_cond functions, but this class
 * use also a pthread_mutex to assure consistency of critical operations on the event semaphore
 * shared by threads.
 */
class EventSemOS: public IEventSem {

public:
    /**
     * @copydetails IEventSemOS::IEventSemOS
     */
    EventSemOS();

    /**
     * @copydetails IEventSemOS::IEventSemOS
     */
    EventSemOS(const EventSemOS &source);

    /**
     * @copydetails IEventSemOS::IEventSemOS
     */
    ~EventSemOS();

    /**
     * @copydetails IEventSemOS::Create
     */
    virtual bool Create();

    /**
     * @copydetails IEventSemOS::Close
     */
    virtual bool Close();

    /**
     * @copydetails IEventSemOS::Wait
     */
    virtual bool Wait(FlagsType &error, const TimeoutType &msecTimeout);

    /**
     * @copydetails IEventSemOS::Post
     */
    virtual bool Post();

    /**
     * @copydetails IEventSemOS::Reset
     */
    virtual bool Reset();

    /**
     * @copydetails IEventSemOS::ResetWait
     */
    inline bool ResetWait(FlagsType &error, const TimeoutType &msecTimeout) {
        Reset();
        return Wait(error, msecTimeout);
    }

    /**
     * @copydetails IEventSemOS::GetHandle
     */
    virtual PThreadSem *GetHandle() const {
        return pthreadSem;
    }

    /**
     * @brief Checks if the semaphore was already closed
     * @return true if the semaphore is closed
     */
    bool IsClosed() const {
        return closed;
    }

private:
    /**
     * Semaphore pthread information holder
     */
    PThreadSem *pthreadSem;

    /** This flag is set to true when the semaphore is closed.
     * We need this because when we use shared semaphores (created with
     * the copy constructor) once the semaphore has been closed by the
     * operating system, calling a second close (for the copy constructed
     * semaphore) has an undefined behaviour.
     */
    bool closed;
};

#endif /* EVENTSEMOS_H_ */

