/**
 * @file GAMSchedulerI.h
 * @brief Header file for class GAMSchedulerI
 * @date 22/02/2016
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

 * @details This header file contains the declaration of the class GAMSchedulerI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSCHEDULERI_H_
#define GAMSCHEDULERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ExecutableI.h"
#include "GAM.h"
#include "ProcessorType.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StatefulI.h"
#include "TimingDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief POD to store information about a thread that is schedulable by a GAMSchedulerI.
 */
struct ScheduledThread {
    /**
     * List of pointers to the ExecutableI components (typically GAM and BrokerI) that belong to this thread.
     */
    ExecutableI ** executables;

    /**
     * Number of ExecutableI components that belong to this thread.
     */
    uint32 numberOfExecutables;

    /**
     * Memory address where the total cycle time signal is stored.
     */
    uint32 *cycleTime;

    /**
     * The cpus where is possible to run the thread
     */
    ProcessorType cpu;

    /**
     * The thread stack size
     */
    uint32 stackSize;

    /**
     * This thread name.
     */
    const char8 * name;
};

/**
 * @brief POD to store information about a state that is schedulable by a GAMSchedulerI.
 */
struct ScheduledState {
    /**
     * List of threads that are executed during this state.
     */
    ScheduledThread * threads;

    /**
     * Number of threads that are executed during this state.
     */
    uint32 numberOfThreads;

    /**
     * The name of this state.
     */
    const char8 * name;
};

/**
 * @brief The GAM scheduler interface.
 *
 * @details Provides methods to link the scheduler to the threads to be executed and
 * the mechanisms to allow a swiftly change of state.
 *
 * @details The syntax in the configuration stream has to be:
 *
 * +Scheduler = {\n
 *    Class = Scheduler_name
 *     ...\n
 *    TimingDataSource = "Name of the TimingDataSource"
 * }\n
 *
 * and it has to be contained in the [RealTimeApplication] declaration.
 */
class DLL_API GAMSchedulerI: public ReferenceContainer, public StatefulI {

public:

    /**
     * @brief Constructor.
     */
    GAMSchedulerI();

    /**
     * @brief Destructor. Frees the SchedulableTable.
     */
    virtual ~GAMSchedulerI();

    /**
     * @brief Reads the TimingDataSource name.
     * @param[in] data the StructuredDataI with the TimingDataSource.
     * @return false if the TimingDataSource is not defined.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Generates a SchedulableTable containing the information the states which can be executed by this GAMSchedulerI.
     * @param[in] realTimeAppIn the RealTimeApplication using this scheduler.
     * @details The SchedulableTable is a dual-buffer array of ScheduledState PODs.
     * @return true if the SchedulableTable can be successfully constructed.
     */
    virtual bool ConfigureScheduler(Reference realTimeAppIn);

    /**
     * @brief Stores the GAMSchedulerRecord for the new state in the next buffer.
     * @param[in] currentStateName is the name of the current state
     * @param[in] nextStateName is the name of the next state
     * @return true if the next state name is found, false otherwise.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName);

    /**
     * @brief Executes a list of ExecutableIs storing their execution times with respect the start time instant.
     * @details Note that if one of the executables returns ErrorManagement::Completed, the current cycle will be 
     * deemed to be completed and thus no other executables will run.
     * @param[in] executables the list of ExecutablesIs to be executed
     * @param[in] numberOfExecutables how many ExecutableIs have to be executed.
     */
    bool ExecuteSingleCycle(ExecutableI * const * const executables, const uint32 numberOfExecutables) const;

    /**
     * @brief Gets the number of ExecutableI components for this \a threadName in this \a stateName.
     * @param[in] stateName the name of the state.
     * @param[in] threadName the name of the thread.
     * @return the number of ExecutableI components for this \a threadName in this \a stateName.
     */
    uint32 GetNumberOfExecutables(const char8 * const stateName, const char8 * const threadName) const;

    /**
     * @brief Starts the execution of the next state threads.
     * @pre
     *    PrepareNextState();
     */
    virtual ErrorManagement::ErrorType StartNextStateExecution()=0;

    /**
     * @brief Stops the execution of the current state threads.
     */
    virtual ErrorManagement::ErrorType StopCurrentStateExecution()=0;

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

protected:
    /**
     * @brief Gets a pointer to the address of the two possible ScheduledStates (the current and the next).
     * @details Upon a PrepareNextState this table is automatically updated in order to
     * have ScheduledState[RealTimeApplication->GetIndex()] pointing at the correct ScheduledState to be executed.
     * @return a pointer to address of the two possible ScheduledStates (the current and the next).
     */
    ScheduledState * const * GetSchedulableStates();

    /**
     * @brief Custom routine to prepare the specific scheduler for the next state execution.
     */
    virtual void CustomPrepareNextState()=0;

    /**
     * Clock period
     */
    const float64 clockPeriod;

    /**
     * The real-time application linked to this scheduler
     */
    Reference realTimeApp;

    /**
    * @brief Pointer to the memory area where a numeric identifier for the current state is stored 
    */
    uint32 *currentStateIdentifier;

    /**
    * @brief Numeric identifier for the next state
    */
    uint32 nextStateIdentifier;


    /**
     * Double buffer accelerator to the threads to be executed for the current
     * and next state.
     */
    ScheduledState *scheduledStates[2];

    /**
     * The name of the TimingDataSource where the execution times are to be stored.
     */
    StreamString timingDataSourceAddress;

    /**
     * The TimingDataSource where the execution times are to be stored.
     */
    ReferenceT<TimingDataSource> timingDataSource;

    /**
     * List of all the possible application states.
     */
    ScheduledState *states;

    /**
     * Number of possible application states.
     */
    uint32 numberOfStates;

    /**
     * @brief Helper function to add the input brokers of the \a gam to the table of states to be executed.
     * @param[in] gamFullName the GAM fully qualified name.
     * @param[in] stateIdx the index of the state to which this GAM input brokers is being added to.
     * @param[in] threadIdx the index of the thread to which this GAM input brokers is being added to.
     * @param[out] executableIdx the index number of the executable (it will be incremented by the number of input brokers that were added).
     * @return true if the GAM input brokers could be successfully added.
     */
    bool InsertInputBrokers(ReferenceT<GAM> gam, const char8 * const gamFullName, const uint32 stateIdx, const uint32 threadIdx, uint32 &executableIdx) const;

    /**
     * @brief Helper function to add the output brokers of the \a gam to the table of states to be executed.
     * @param[in] gamFullName the GAM fully qualified name.
     * @param[in] stateIdx the index of the state to which this GAM output brokers is being added to.
     * @param[in] threadIdx the index of the thread to which this GAM output brokers is being added to.
     * @param[out] executableIdx the index number of the executable (it will be incremented by the number of output brokers that were added).
     * @return true if the GAM output brokers could be successfully added.
     */
    bool InsertOutputBrokers(ReferenceT<GAM> gam, const char8 * const gamFullName, const uint32 stateIdx, const uint32 threadIdx, uint32 &executableIdx) const;

    /**
     * @brief Helper function to add this \a gam to the table of states to be executed.
     * @param[in] gamFullName the GAM fully qualified name.
     * @param[in] stateIdx the index of the state to which this GAM is being added to.
     * @param[in] threadIdx the index of the thread to which this GAM is being added to.
     * @param[in] executableIdx the index number of the executable.
     * @return true if the GAM could be successfully added.
     */
    bool InsertGAM(ReferenceT<GAM> gam, const char8 * const gamFullName, const uint32 stateIdx, const uint32 threadIdx, const uint32 executableIdx) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERI_H_ */

