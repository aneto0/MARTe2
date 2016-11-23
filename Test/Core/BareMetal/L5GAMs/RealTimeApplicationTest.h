/**
 * @file RealTimeApplicationTest.h
 * @brief Header file for class RealTimeApplicationTest
 * @date 26/02/2016
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

 * @details This header file contains the declaration of the class RealTimeApplicationTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEAPPLICATIONTEST_H_
#define REALTIMEAPPLICATIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeApplication.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeApplication functions.
 */
class RealTimeApplicationTest {
public:

    /**
     * @brief Initialises the configuration stream
     */
    RealTimeApplicationTest();

    /**
     * @brief Destroys the ObjectRegistryDatabase.
     */
    ~RealTimeApplicationTest();

    /**
     * @brief Initialises the ObhectRegistryDatabase
     */
    bool Init();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise function
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise failing if no States container declared
     */
    bool TestInitialiseFalse_NoStatesFound();

    /**
     * @brief Tests the Initialise failing if no Data container declared
     */
    bool TestInitialiseFalse_NoDataFound();

    /**
     * @brief Tests the Initialise failing if no Functions container declared
     */
    bool TestInitialiseFalse_NoFunctionsFound();

    /**
     * @brief Tests the Initialise failing if no Scheduler container declared
     */
    bool TestInitialiseFalse_NoSchedulerFound();

    /**
     * @brief Tests the ConfigureApplication compiling the ConfigurationDatabase after initialisation
     */
    bool TestConfigureApplication();

    /**
     * @brief Tests the ConfigureApplication compiling the ConfigurationDatabase before initialisation
     */
    bool TestConfigureApplicationNoInit();

    /**
     * @brief Tests if the function refreshes the correct GAMs, DataSources and Scheduler for a state switch
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests if the function launches the scheduler execution
     */
    bool TestStartNextStateExecution();

    /**
     * @brief Tests if the function stops the scheduler execution
     */
    bool TestStopCurrentStateExecution();

    /**
     * @brief Tests the GetStates method.
     */
    bool TestGetStates();

    /**
     * @brief Tests the GetIndex method.
     */
    bool TestGetIndex();

    StreamString config;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONTEST_H_ */

