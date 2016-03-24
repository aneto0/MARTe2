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
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RealTimeStateInfo.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A container of Functions (GAMGroup or GAM), and States (RealTimeState) references.
 * @details The syntax in the configuration stream should be:
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
 *             ...\n
 *         }\n
 *         ...\n
 *     }\n
 *     +Data = {\n
 *         Class = RealTimeDataSource
 *         IsFinal = true v false\n
 *         DataSource_name = {\n
 *             ...\n
 *         }\n
 *         ...
 *     }\n
 * }\n
 */
class RealTimeApplication: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    RealTimeApplication();

    /**
     * @brief Destructor
     */
    virtual ~RealTimeApplication();

    /**
     * @brief Configuration of the application environment.
     * @details Checks if the functions (GAM or GAMGroup) declared in the RealTimeThread configuration are really defined
     * and supports the state where they are declared into. Moreover creates accelerators to the specific GAM References
     * for each RealTimeState and adds the GAM References in each RealTimeThread.
     * @return true if all the GAMs declared in the RealTimeThread configuration are really defined
     * and supports the state where they are declared into, false otherwise.
     */
    bool ConfigureArchitecture();

    /**
     * @brief Configuration of the data sources.
     * @details Generates the data source definitions from the RealTimeDataDef definitions in each GAM. If the
     * container "+Data" is declared as final (i.e IsFinal=true in the configuration), it is not possible add
     * other data sources over the ones already defined by configuration. Inside the "+Data" container will be added
     * the definitions of data source types as trees generated accordingly with the field "Path" in each RealTimeDataDef
     * in the GAMs. Each leaf of these trees is a RealTimeDataSourceDef containing, for each state, the GAMs which will produce
     * and consume that specific data.
     * @return false in case of errors, true otherwise.
     */
    bool ConfigureDataSource();

    /**
     * @brief Validates the data sources.
     * @details Checks if for each state there is only a single GAM producer for each data source definition. Moreover
     * if there are not GAM consumer, a warning will be generated (that source will be never read).
     * @return true if for each state there is an unique GAM producer, false otherwise.
     */
    bool ValidateDataSource();

    /**
     * @brief Allocates the data sources memory.
     * @details Browses the RealTimeDataSourceDef tree (+Data is the root) built using ConfigureDataSource() function and for each
     * definition (leaf of the tree) allocates the memory for the specified type and dimension (scalar, vector or matrix). For each variable
     * a double buffer of memory is provided, in order to reset the variable in safe mode in case of state switching. See PrepareNextState(*)
     * for more documentation.
     * @return false in case of errors, true otherwise.
     */
    bool AllocateDataSource();

    /**
     * @brief Creates the interfaces for read and write operations between GAMs and RealTimeDataSource.
     * @details Browses the +Functions tree and for each GAM (leaf of the tree) configures its internally RealTimeDataSourceInputReader
     * and RealTimeDataSourceOutputWriter. See GAM::ConfigureDataSourceLinks(*) for more documentation.
     * @return false in case of errors, true otherwise.
     */
    bool ConfigureDataSourceLinks();

    /**
     * @brief Validates the data source links.
     * @details Checks for each thread if there is no more than one synchronization point (i.e a GAM with a synchronized RealTimeDataSourceInputReader).
     * return true if for each RealTimeThread there is an unique synchronization point in the cycle.
     */
    bool ValidateDataSourceLinks();

    /**
     * @brief Prepares the environment for the next state.
     * @details This function has to be executed in a low-priority thread in order to prepare the context for the contextful GAMs
     * and resets the variables in the RealTimeDataSource to the default values if they will be used in the next state but are not
     * used in the current (the value is supposed to be consistent if it is used in both two consecutive states).
     * @param[in] status contains informations about the current and the next state.
     * @return false in case of errors, true otherwise.
     */
    bool PrepareNextState(const char8 * const nextStateName);

    /**
     * @brief Stops the application execution.
     * @details Calls the Scheduler::StopExecution() to terminate the threads running in the current active state.
     * @return true if the scheduler container is valid, false otherwise.
     */
    bool StopExecution();

    /**
     * @brief Retrieves the current active buffer index.
     * @return the value of the current buffer index (0 or 1).
     */
    uint8 GetActiveBuffer() const;

    /**
     * @brief Initialises the application from a StructuredDataI in input.
     * @details The following fields must be specified:
     *
     *   FirstState = (the name of the first state to be executed)
     *
     * @param[in] data contains the initialisation data.
     * @return false if FirstState field is not specified or if the initialisation of the sub-objects fails. True otherwise.
     */
    virtual bool Initialise(StructuredDataI & data);

private:

    /**
     * The active buffer.
     */
    uint8 activeBuffer;

    /**
     * The current state name.
     */
    StreamString currentStateName;

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
    ReferenceT<ReferenceContainer> schedulerContainer;

    /**
     * The +Data container
     */
    ReferenceT<ReferenceContainer> dataSourceContainer;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATION_H_ */

