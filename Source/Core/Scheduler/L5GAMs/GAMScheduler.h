/**
 * @file GAMScheduler.h
 * @brief Header file for class GAMScheduler
 * @date 09/08/2016
 * @author Giuseppe Ferro
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
#include "EventSem.h"
#include "GAMSchedulerI.h"
#include "Message.h"
#include "MultiThreadService.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Thread parameter structure
 */
struct RTThreadParam {

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
     * HRT value last cycle time
     */
    uint64 lastCycleTimeStamp;
};

/**
 * @brief The GAM scheduler.
 * @details The syntax in the configuration stream has to be:
 *
 * +Scheduler = {\n
 *    Class = Scheduler_name
 *     ...\n
 *    TimingDataSource = "Name of the TimingDataSource"
 *    +ErrorMessage = { //Optional. Fired every time there is an execution error. Name is only an example.
 *        Class = Message
 *        ...
 *    }
 * }\n
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
     * @brief Verifies if there is an ErrorMessage defined.
     * @param[in] data the StructuredDataI with the TimingDataSource name and with an optional ErrorMessage defined.
     * @return At most one message shall be defined and this will be considered as the ErrorMessage.
     * @see GAMSchedulerI::Initialise.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Starts the multi-thread execution for the current state.
     * @return ErrorManagement::NoError if the next state was configured (see PrepareNextState) and the MultiThreadService could be successfully started.
     * @pre
     *   PrepareNextState()
     */
    virtual ErrorManagement::ErrorType StartNextStateExecution();

    /**
     * @brief Stops the execution application
     * @return ErrorManagement::NoError if the current state was configured (see PrepareNextState) and the MultiThreadService could be successfully stopped.
     * @pre
     *   PrepareNextState()
     */
    virtual ErrorManagement::ErrorType StopCurrentStateExecution();

    /**
     * @brief Callback function for the MultiThreadService.
     * @details Loops on all the real-time threads and executes its ExecutableI
     * @param[in] information (see EmbeddedThread)
     * @return ErrorManagement::NoError if every ExecutableI did not return any error.
     */
    ErrorManagement::ErrorType Execute(ExecutionInfo &information);

    /**
     * @brief Stops the active MultiThreadService running services and calls ReferenceContainer::Purge
     * @see ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);

protected:

    /**
     * @brief Starts the threads for the next state
     */
    virtual void CustomPrepareNextState();

    /**
     * The array of identifiers of the thread in execution.
     */
    MultiThreadService *multiThreadService[2];

    /**
     * The array of the thread parameters
     */
    RTThreadParam *rtThreadInfo[2];

    /**
     * The eventSemaphore
     */
    EventSem eventSem;

    /**
     * Registers the callback function to be called by the MultiThreadService
     */
    EmbeddedServiceMethodBinderT<GAMScheduler> binder;

    /**
     * Message to be fired in case of execution error
     */
    ReferenceT<Message> errorMessage;

    /**
     * Specialised real-time application reference.
     */
    ReferenceT<RealTimeApplication> realTimeApplicationT;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULER_H_ */


