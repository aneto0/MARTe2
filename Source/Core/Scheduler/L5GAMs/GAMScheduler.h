/**
 * @file GAMScheduler.h
 * @brief Header file for class GAMScheduler
 * @date 09/ago/2016
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

 * @details This header file contains the declaration of the class GAMScheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSCHEDULER_H_
#define GAMSCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSchedulerI.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Thread parameter structure
 */
struct RTThreadParam {
    /**
     * The scheduler
     */
    GAMSchedulerI *scheduler;
    /**
     * The list of executables
     */
    ExecutableI **executables;
    /**
     * The number of executables
     */
    uint32 numberOfExecutables;
    /**
     * The cycle time
     */
    uint32* cycleTime;
    /**
     * A spinlock allowing to stop the thread execution
     */
    volatile int32 *spinLock;

    /**
     * The event semaphore where threads wait before start their execution
     */
    EventSem *eventSem;
};

/**
 * @brief The GAM scheduler
 */
class DLL_API GAMScheduler: public GAMSchedulerI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    GAMScheduler();

    /**
     * @brief Destructor
     */
    virtual ~GAMScheduler();

    /**
     * @brief Starts the multi-thread execution for the current state.
     */
    virtual void StartExecution();

    /**
     * @brief Stops the execution application
     */
    virtual void StopExecution();

protected:

    /**
     * @brief Starts the threads for the next state
     */
    virtual void CustomPrepareNextState();


private:

    /**
     * The array of identifiers of the thread in execution.
     */
    ThreadIdentifier *tid[2];

    /**
     * Synchronization spin-lock
     */
    volatile int32 spinLock[2];

    /**
     * The array of the thread parameters
     */
    RTThreadParam *param[2];

    /**
     * The eventSemaphore
     */
    EventSem eventSem;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULER_H_ */


