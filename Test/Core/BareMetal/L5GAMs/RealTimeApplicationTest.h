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
     * @brief Initialises the ObjectRegistryDatabase.
     */
    RealTimeApplicationTest();

    /**
     * @brief Destroys the ObjectRegistryDatabase.
     */
    ~RealTimeApplicationTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function configures correctly the desired environment calling the ConfigureArchitecture(*) for
     * each internally declared thread.
     */
    bool TestConfigureArchitecture();

    /**
     * @brief Tests if the function fails if the application does not contain the "+States" container.
     */
    bool TestConfigureArchitectureFalse_NoStates();

    /**
     * @brief Tests if the function creates correctly all the RealTimeDataSourceDefs calling ConfigureDataSource(*)
     * for each internally declared GAM.
     */
    bool TestConfigureDataSource();

    /**
     * @brief Tests if the function fails if the application does not contain the "+Functions" container.
     */
    bool TestConfigureDataSourceFalse_NoFunctions();

    /**
     * @brief Tests if the function fails if "+Functions" is not a ReferenceContainer.
     */
    bool TestConfigureDataSourceFalse_InvalidFunction();

    /**
     * @brief Tests if the function returns true if there is no more than one producer GAM per state.
     */
    bool TestValidateDataSource();

    /**
     * @brief Tests if the function returns false if there is more than one producer GAM in a state.
     */
    bool TestValidateDataSourceFalse_MoreThanOneProducer();

    /**
     * @brief Tests if the function returns false if the application does not contain the "+Data" container.
     */
    bool TestValidateDataSourceFalse_NoData();

    /**
     * @brief Tests if the function allocates correctly the RealTimeDataSource memory.
     */
    bool TestAllocateDataSource();

    /**
     * @brief Tests if the function fails if the application does not contain the "+Data" container.
     */
    bool TestAllocateDataSourceFalse_NoData();

    /**
     * @brief Tests if the function creates for each gam its RealTimeInputReader and RealTimeOutputWriter.
     */
    bool TestConfigureDataSourceLinks();

    /**
     * @brief Tests if the function fails if the application does not contain the "+Functions" container.
     */
    bool TestConfigureDataSourceLinksFalse_NoFunctions();

    /**
     * @brief Tests if the function fails if the "+Functions" is not a container.
     */
    bool TestConfigureDataSourceLinksFalse_InvalidFunction();

    /**
     * @brief Tests if the function can set the default values of the RealTimeDataSource variables
     * if it is requested by the change of state.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests if the function fails if the application does not contain the "+Data" container.
     */
    bool TestPrepareNextStateFalse_NoData();

    /**
     * @brief Tests if the function flips the active buffer flag.
     */
    bool TestChangeState();

    /**
     * @brief Tests if the function returns correctly the current active buffer number.
     */
    bool TestGetActiveBuffer();



};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONTEST_H_ */

