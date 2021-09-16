/**
 * @file GAMBareScheduler.h
 * @brief Header file for class GAMBareScheduler
 * @date 01/06/2021
 * @author Andre Neto
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class GAMBareScheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMBARESCHEDULER_H_
#define GAMBARESCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "GAMSchedulerI.h"
#include "RealTimeApplication.h"
#include "Reference.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief A single-threaded GAM scheduler that can be used without an operating system support.
 * @details The GAMBareScheduler will execute in an infinite loop all the GAM declared for a given RealTimeThread.
 * +BareScheduler = {
 *    Class = GAMBareScheduler
 *    MaxCycles = 0 //Optional, if not 0 the scheduler will stop executing after MaxCycles have been executed.
 * }
 */
class GAMBareScheduler: public GAMSchedulerI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Construct a new GAMBareScheduler object
     */
    GAMBareScheduler();

    /**
     * @brief Destroy the GAMBareScheduler object
     */
    virtual ~GAMBareScheduler();

    /**
     * @brief Starts the single thread execution for the current state, which will execute
     *  on a single-threaded infinite loop all the GAMs until the StopCurrentStateExecution is called.
     * @return ErrorManagement::NoError if the base Scheduler succeeds
     */
    virtual ErrorManagement::ErrorType StartNextStateExecution();

    /**
     * @brief Changes the flag which keeps the cycle execution running
     * @return ErrorManagement::NoError always 
     */
    virtual ErrorManagement::ErrorType StopCurrentStateExecution();

    /**
     * @brief Calls the parent class to generate the schedulable table and assigns
     *          the Realtime Application reference
     * @param realTimeAppIn The RealTime Application using the scheduler
     * @return true If the table can be successfully constructed and the RealTimeApplication reference is valid
     */
    virtual bool ConfigureScheduler(Reference realTimeAppIn);

    /**
     * @brief Initialises the GAMBareScheduler (see GAMSchedulerI::Initialise).
     * @param[in] data The configuration information which may include the MaxCycles parameter.
     * @return true if the Scheduler is successfully initialised.
     */
    virtual bool Initialise(StructuredDataI & data);

protected: 
    /**
     * @brief Resets the current cycle counter to zero (only meaningful if MaxCycles != 0) 
     */
    virtual void CustomPrepareNextState();


private:

    /**
     * @brief Flag used internally to control the continuation of the execution
     */
    bool isAlive;

    /**
     * @brief Reference to the RealTimeApplication
     */
    ReferenceT<RealTimeApplication> realTimeApplication;

   /**
     * @brief Executes a single cycle of the RealTimeApplication executables for the specified thread identifier
     * @param threadId Identifier of the thread 
     */
    void Cycle(uint32 threadId);

    /**
     * Maximum number of cycles to execute. 0=>Forever
     */
    uint32 maxCycles;

    /**
     * Current cycle number, only meaningful if maxCycles > 0
     */
    uint32 nCycle;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
}

#endif /* GAMBARESCHEDULER_H_ */
