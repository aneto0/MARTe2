/**
 * @file RealTimeApplication.h
 * @brief Header file for class RealTimeApplication
 * @date 19/02/2016
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

 * @details This header file contains the declaration of the class RealTimeApplication
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEAPPLICATION_H_
#define REALTIMEAPPLICATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "GAMSchedulerI.h"
#include "MessageI.h"
#include "RealTimeApplicationConfigurationBuilder.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StatefulI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class responsible for the orchestration of a MARTe real-time application.
 * @details This is the main class of any MARTe real-time application. It is a container of
 * Functions (GAMGroup or GAM), States (RealTimeState), Data (RealTimeDataSourceContainer) and
 * Scheduler references. This class is responsible for validating the global configuration and
 * for verifying that the application is complete (i.e. that not only each reference is valid
 * but that also cross-dependencies between references are also valid, e.g. there is at least
 * one producer for every data signal data is produced).
 *
 * The syntax in the configuration stream should be:
 * <pre>
 * RealTimeApplication_name = {
 *     Class = RealTimeApplication
 *     +Functions = {
 *         Class = ReferenceContainer
 *         GAM_name = {
 *             Class = GAM
 *             ...
 *         }
 *         GAM_Group_name = {
 *             Class = GAMGroup
 *             ...
 *         }
 *         ...
 *     }
 *     +States = {
 *         Class = ReferenceContainer
 *         State_name = {
 *             Class = RealTimeState
 *             +Threads = {
 *                 Class = RealTimeThread
 *                 ...
 *             }
 *             ...
 *         }
 *         ...
 *     }
 *     +Data = {
 *         Class = ReferenceContainer
 *         DataSource_name = {
 *             Class = (inherited from DataSourceI)
 *             ...
 *         }
 *         Timings_name = {
 *             Class = TimingDataSource
 *         }
 *         ...
 *     }
 *     +Scheduler = {
 *         Class = Scheduler_class_name (inherited from GAMSchedulerI)
 *         ...
 *     }
 * }
 * </pre>
 */
/*lint -e{9109} RealTimeApplication is forward declared in RealTimeApplicationConfigurationBuilder.*/
class DLL_API RealTimeApplication: public ReferenceContainer, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP
     */
    RealTimeApplication();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~RealTimeApplication();

    /**
     * @brief Initialises the application from a StructuredDataI in input.
     * @details The following fields must be specified:
     *
     *<pre>
     *     Class = RealTimeApplication
     *     +Functions = {
     *         Class = ReferenceContainer
     *         GAM_name = {
     *             Class = GAM
     *             ...
     *         }
     *         GAM_Group_name = {
     *             Class = GAMGroup
     *             ...
     *         }
     *         ...
     *     }
     *     +States = {
     *         Class = ReferenceContainer
     *         State_name = {
     *             Class = RealTimeState
     *             +Threads = {
     *                 Class = RealTimeThread
     *                 ...
     *             }
     *             ...
     *         }
     *         ...
     *     }
     *     +Data = {
     *         Class = ReferenceContainer
     *         IsFinal = true v false
     *         DataSource_name = {
     *             ...
     *         }
     *         Timings_name = {
     *             Class = TimingDataSource
     *         }
     *         ...
     *     }
     *     +Scheduler = {
     *         Class = Scheduler_class_name (inherited from GAMSchedulerI)
     *         ...
     *     }
     *</pre>
     * @details The user can specify the following parameters
     *   CheckMultipleProducersWrites = 1 //enable-disable check multiple producers to write on the same data source signal. Default = 1
     *   CheckSameGamInMoreThreads = 1 //allow a GAM to be declared in more than one RTT per state. Default = 1
     * @param[in] data contains the initialisation data.
     * @return true if the parameters +Functions, +States, +Data and +Scheduler
     * exist and each inherit from ReferenceContainer.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Configures a RealTimeApplication (most of the work delegated to a RealTimeApplicationConfigurationBuilder).
     * @return true if all the RealTimeApplication could be successfully configured. When this function returns true it
     *  guarantees that all the MARTe components are ready to be used (e.g. the Scheduler may start to work and execute the GAMs).
     */
    bool ConfigureApplication();

    /**
     * @brief Configures a RealTimeApplication starting from an existing database.
     * @details This function expects that \a functionsDatabaseIn and the \a dataDatabaseIn were
     * already processed (possibly in another application) by a RealTimeApplicationConfigurationBuilder.
     * @param[in] functionsDatabaseIn the Functions database (see RealTimeApplicationConfigurationBuilder::Set).
     * @param[in] dataDatabaseIn the Data database (see RealTimeApplicationConfigurationBuilder::Set).
     * @return true if all the RealTimeApplication could be successfully configured. When this function returns true it
     *  guarantees that all the MARTe components are ready to be used (e.g. the Scheduler may start to work and execute the GAMs).
     */
    bool ConfigureApplication(ConfigurationDatabase &functionsDatabaseIn,
            ConfigurationDatabase &dataDatabaseIn);

    /**
     * @brief Calls PrepareNextState on all the StatefulI components that belong to this application.
     * @details Typically the StatefulI components are the GAMGroup, the GAMSchedulerI, DataSourceI and the RealTimeState components.
     * @param[in] nextStateName the name of the next state to be executed.
     * @return true iff PrepareNextState is successful on all the StatefulI components.
     */
    ErrorManagement::ErrorType PrepareNextState(StreamString nextStateName);

    /**
     * @brief Swaps the current execution index (RealTimeApplication::GetIndex) and calls GAMSchedulerI::StartExecution on the defined application Scheduler.
     * @return GAMSchedulerI::StartNextStateExecution
     */
    ErrorManagement::ErrorType StartNextStateExecution();

    /**
     * @brief Calls GAMSchedulerI::StopCurrentExecution on the defined application Scheduler.
     * @return GAMSchedulerI::StopCurrentExecution
     */
    ErrorManagement::ErrorType StopCurrentStateExecution();

    /**
     * @brief Gets the declared RealTimeState components.
     * @param[out] states container to add all the RealTimeApplication States.
     * @return true if the RealTimeState components exist and can be successfully copied into the \a states container.
     */
    bool GetStates(ReferenceContainer &states) const;

    /**
     * @brief Gets the current execution index. This number swaps between 0 and 1 each time StartExecution is called.
     * @return the current execution index.
     */
    uint32 GetIndex() const;

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);


    /**
     * @brief Returns true if the RealTimeApplicationConfigurationBuilder must check if a GAM is declared in more than one
     * RealTimeThread for each RealTimeState
     */
    bool CheckSameGamInMoreThreads() const;

    /**
     * @brief Returns true if the RealTimeApplicationConfigurationBuilder must check if a signal in the DataSource is produced
     * by more than one GAM in the RealTimeState
     */
    bool CheckMultipleProducersWrites() const;
private:

    /**
     * @brief Calls GAM::AllocateInputSignalsMemory and GAM::AllocateOutputSignalsMemory on all the GAM components.
     * @return true if all the GAM::AllocateInputSignalsMemory and GAM::AllocateOutputSignalsMemory calls return true.
     */
    bool AllocateGAMMemory();

    /**
     * @brief Calls DataSourceI::AllocateMemory on all the DataSourceI components.
     * @return true if all the DataSourceI::AllocateMemory calls return true.
     */
    bool AllocateDataSourceMemory();

    /**
     * @brief Calls DataSourceI::AddBrokers(Input/OutputSignals) on all the DataSourceI components.
     * @return true if all the DataSourceI::AddBrokers calls return true.
     */
    bool AddBrokersToFunctions();

    /**
     * @brief Collects all the StatefulI components that were declared in the Data node.
     * @return true if a valid dataSourceContainer exists.
     */
    bool FindStatefulDataSources();

    /**
     * The current state name.
     */
    StreamString stateNameHolder[2];

    /**
     * The unique RealTimeApplication index which swaps between 0/1 at every state change.
     */
    uint32 index;

    /**
     * The +States container.
     */
    ReferenceT<ReferenceContainer> statesContainer;

    /**
     * The +Functions container.
     */
    ReferenceT<ReferenceContainer> functionsContainer;

    /**
     * The +Scheduler container.
     */
    ReferenceT<GAMSchedulerI> scheduler;

    /**
     * The +Data container
     */
    ReferenceT<ReferenceContainer> dataSourceContainer;

    /**
     * All the StatefulI DataSourceI components.
     */
    ReferenceContainer statefulsInData;

    /**
     * Configured Functions database.
     */
    ConfigurationDatabase functionsDatabase;

    /**
     * Configured Data database.
     */
    ConfigurationDatabase dataSourcesDatabase;

    /**
     * The default data source name to be used when this is not specified in the Signals.
     */
    StreamString defaultDataSourceName;

    /**
     * Filter to receive the RPC
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;

    /**
     * Check if the GAM is defined in more than one RTT
     */
    bool checkSameGamInMoreThreads;

    /**
     * Check if each signal has only one producer in each state
     */
    bool checkMultipleProducersWrites;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATION_H_ */

