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
#include "RealTimeApplicationConfigurationBuilder.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StatefulI.h"
#include "GAMSchedulerI.h"
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
 *
 * RealTimeApplication_name = {\n
 *     Class = RealTimeApplication\n
 *     +Functions = {\n
 *         Class = ReferenceContainer\n
 *         GAM_name = {\n
 *             Class = GAM\n
 *             ...\n
 *         }\n
 *         GAM_Group_name = {\n
 *             Class = GAMGroup\n
 *             ...\n
 *         }\n
 *         ...\n
 *     }\n
 *     +States = {\n
 *         Class = ReferenceContainer\n
 *         State_name = {\n
 *             Class = RealTimeState\n
 *             +Threads = {\n
 *                 Class = RealTimeThread\n
 *                 ...\n
 *             }\n
 *             ...\n
 *         }\n
 *         ...\n
 *     }\n
 *     +Data = {\n
 *         Class = RealTimeDataSourceContainer
 *         IsFinal = true v false\n
 *         DataSource_name = {\n
 *             ...\n
 *         }\n
 *         ...
 *     }\n
 *     +Scheduler = {\n
 *         Class = Scheduler_class_name (inherited from GAMSchedulerI)\n
 *         ...\n
 *     }\n
 * }\n
 */
/*lint -e{9109} RealTimeApplication is forward declared in RealTimeApplicationConfigurationBuilder.*/
class DLL_API RealTimeApplication: public ReferenceContainer {
public:
	CLASS_REGISTER_DECLARATION()

	/**
	 * @brief Constructor
	 * @post
	 *    GetActiveBuffer() = 1
	 */
RealTimeApplication	();

	/**
	 * @brief Destructor. NOOP.
	 */
	virtual ~RealTimeApplication();

	/**
	 * @brief Initialises the application from a StructuredDataI in input.
	 * @details The following fields must be specified:
	 *
	 *   FirstState = (the name of the first state to be executed) //TODO (State Machine ?)
	 *     Class = RealTimeApplication\n
	 *     +Functions = {\n
	 *         Class = ReferenceContainer\n
	 *         GAM_name = {\n
	 *             Class = GAM\n
	 *             ...\n
	 *         }\n
	 *         GAM_Group_name = {\n
	 *             Class = GAMGroup\n
	 *             ...\n
	 *         }\n
	 *         ...\n
	 *     }\n
	 *     +States = {\n
	 *         Class = ReferenceContainer\n
	 *         State_name = {\n
	 *             Class = RealTimeState\n
	 *             +Threads = {\n
	 *                 Class = RealTimeThread\n
	 *                 ...\n
	 *             }\n
	 *             ...\n
	 *         }\n
	 *         ...\n
	 *     }\n
	 *     +Data = {\n
	 *         Class = RealTimeDataSourceContainer
	 *         IsFinal = true v false\n
	 *         DataSource_name = {\n
	 *             ...\n
	 *         }\n
	 *         ...
	 *     }\n
	 *     +Scheduler = {\n
	 *         Class = Scheduler_class_name (inherited from GAMSchedulerI)\n
	 *         ...\n
	 *     }\n
	 *
	 * @param[in] data contains the initialisation data.
	 * @return true if the FirstState field is specified and if the parameters +Functions, +States, +Data and +Scheduler all
	 * exist and each inherit from ReferenceContainer.
	 */
	virtual bool Initialise(StructuredDataI & data);

	bool ConfigureApplication();

	bool ConfigureApplication(ConfigurationDatabase &functionsDatabaseIn,
			ConfigurationDatabase &dataDatabaseIn);

	/**
	 * @brief Prepares the environment for the next state and starts the new execution.
	 * @details This function has to be executed in a low-priority thread in order to prepare the context for the contextful GAMs
	 * and resets the variables in the RealTimeDataSource to the default values if they will be used in the next state but are not
	 * used in the current (the value is supposed to be consistent if it is used in both two consecutive states). When all is ready,
	 * this function calls the scheduler to stop the current state execution and starts the next state execution.
	 * @param[in] status contains informations about the current and the next state.
	 * @return false in case of errors, true otherwise.
	 */
	virtual bool PrepareNextState(const char8 * const nextStateName);

	void StartExecution();

	/**
	 * @brief Stops the application execution.
	 * @details Calls the Scheduler::StopExecution() to terminate the threads running in the current active state.
	 * @return true if the scheduler container is valid, false otherwise.
	 */
	void StopExecution();

	/**
	 * @brief Configuration of the main application environment.
	 * @details Propagates the configuration setup request to the States (RealTimeState.ConfigureArchitecture) and Scheduler entries (@see Initialise).
	 * @return true if all the declared States and Scheduler elements are valid, false otherwise.
	 */
	bool ConfigureArchitecture();


	static uint32 GetIndex();

private:

	bool AllocateGAMMemory();

	bool AllocateDataSourceMemory();

	bool AddBrokersToFunctions();

	/**
	 * The current state name.
	 */
	StreamString stateNameHolder[2];

	static uint32 index;

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

	ReferenceContainer statefulsInData;

	ConfigurationDatabase functionsDatabase;
	ConfigurationDatabase dataSourcesDatabase;

	StreamString defaultDataSourceName;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATION_H_ */

