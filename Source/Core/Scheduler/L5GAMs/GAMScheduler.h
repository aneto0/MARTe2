/**
 * @file GAMScheduler.h
 * @brief Header file for class GAMScheduler
 * @date 23/03/2016
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
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

struct RTThreadParam {
    GAMSchedulerI *scheduler;
    uint32 activeBuffer;
    uint32 threadId;
    volatile int32 *spinLock;
};

/**
 * @brief The GAM scheduler
 */
class GAMScheduler: public GAMSchedulerI {

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
    virtual void StartExecution(const uint32 activeBuffer);

    /**
     * @brief Stops the execution.
     */
    virtual void StopExecution();

private:

    /**
     * The array of identifiers of the thread in execution.
     */
    ThreadIdentifier *tid;

    /**
     * The number of thread in execution.
     */
    uint32 numberOfThreads;

    /**
     * Synchronization spin-lock
     */
    volatile int32 spinLock;

    RTThreadParam *param;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_SCHEDULER_L5GAMS_GAMSCHEDULER_H_ */

