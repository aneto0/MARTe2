/**
 * @file FastScheduler.h
 * @brief Header file for class FastScheduler
 * @date 21/05/2020
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

 * @details This header file contains the declaration of the class FastScheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTSCHEDULER_H_
#define FASTSCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CountingSem.h"
#include "GAMScheduler.h"
#include "GAMSchedulerI.h"
#include "Message.h"
#include "MultiThreadService.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The Fast scheduler.
 * @details The syntax in the configuration stream has to be:
 *
 * +Scheduler = {\n
 *    Class = FastScheduler
 *    NoWait = 0 //Wait for all the cycles to terminate before executing the executables of the next cycle. Default is 0
 *     ...\n
 *    TimingDataSource = "Name of the TimingDataSource"
 *    +ErrorMessage = { //Optional. Fired every time there is an execution error. Name is only an example.
 *        Class = Message
 *        ...
 *    }
 * }\n
 *
 * @details This scheduler executes from the beginning a number of threads and keeps them executing across the state transitions.
 * The number of spawned threads depends on the cpus defined. It is the summatory over the number of cpus of [max(Ti)] where max(Ti)
 * is the maximum number of RTTs on the cpu i over the states
 * To give an example, let´s assume the following scenario:
 *   -State1
 *     -RTT1, cpu=1
 *     -RTT2, cpu=1
 *     -RTT3, cpu=2
 *   -State2
 *     -RTT1, cpu=3
 *     -RTT2, cpu=1
 *     -RTT3, cpu=2
 *     -RTT4, cpu=2
 *
 * Then the the scheduler will execute 5 threads:
 *   - T1 executing RTT1 in state1 and RTT2 in state2 (cpu 1)
 *   - T2 executing RTT2 in state1 and idle in state2 (cpu 1)
 *   - T3 executing RTT3 in state1 and RTT3 in state2 (cpu 2)
 *   - T4 idle in state1 and executing RTT4 in state2 (cpu 2)
 *   - T5 idle in state1 and executing RTT1 in state3 (cpu 3)
 *
 * The mapping between Tx and RTTy is computed in the configuration stage.
 */
class FastScheduler: public GAMSchedulerI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    FastScheduler();

    /**
     * @brief Destructor
     */
    virtual ~FastScheduler();

    /**
     * @brief Verifies if there is an ErrorMessage defined.
     * @param[in] data the StructuredDataI with the TimingDataSource name and with an optional ErrorMessage defined.
     * @details User can configure the parameter:
     *   NoWait = 0|1
     * If 0, after a StartNextStateExecution call, the scheduler waits for all the RTTs (Real Time Threads) of the previous state to terminate
     * before executing the executables of the next state. If 1, the RTT of the next state are executed immediately after the termination
     * of the last RTT execution from the previous state. Since every RTT has a different synchronisation point, this might lead to the
     * execution of RTTs of next and previous state at the same time.
     *
     * @return At most one message shall be defined and this will be considered as the ErrorMessage.
     * @see FastSchedulerI::Initialise.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @see FastSchedulerI::ConfigureScheduler
     */
    virtual bool ConfigureScheduler(Reference realTimeAppIn);

    /**
     * @brief Starts the RTTs of the current state.
     * @return ErrorManagement::NoError if the next state was configured (see PrepareNextState)
     * @pre
     *   PrepareNextState()
     */
    virtual ErrorManagement::ErrorType StartNextStateExecution();

    /**
     * @brief Stops the execution application
     * @details This function is not really necessary if one wants to ensure fast transition from the current state to the next.
     * Call this only in case of need to stop the execution of the RTTs.
     * @return ErrorManagement::NoError
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
     * @brief Map the RTTs to the executed threads depending on the cpus
     */
    ErrorManagement::ErrorType SetupThreadMap();

    /**
     * @brief Map the RTTs to the executables threads depending on the cpus
     */
    void CreateThreadMap(uint64 cpu, uint32 state, uint32 thread);

    /**
    * @brief Compute the number of executables threads
    */
    void ComputeMaxNThreads();

    /**
     * The array of identifiers of the thread in execution.
     */
    MultiThreadService *multiThreadService;

    /**
     * The array of the thread parameters
     */
    RTThreadParam *rtThreadInfo[2];

    /**
     * The eventSemaphore
     */
    EventSem eventSem;

    /**
     * The counting semaphore
     */
    CountingSem countingSem;

    /**
     * Semaphore where idle threads wait for their state
     */
    EventSem unusedThreadsSem;

    /**
     * Registers the callback function to be called by the MultiThreadService
     */
    EmbeddedServiceMethodBinderT<FastScheduler> binder;

    /**
     * Message to be fired in case of execution error
     */
    ReferenceT<Message> errorMessage;

    /**
     * Specialised real-time application reference.
     */
    ReferenceT<RealTimeApplication> realTimeApplicationT;

    /**
     * Initialised flag
     */
    bool initialised;

    /**
     * cpu map
     */
    uint64 **cpuMap;

    /**
     * cpu thread map
     */
    uint32 **cpuThreadMap;

    /**
     * number of executable threads
     */
    uint32 maxNThreads;

    /**
     * Fast scheduler mode
     */
    uint8 superFast;
};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTSCHEDULER_H_ */

