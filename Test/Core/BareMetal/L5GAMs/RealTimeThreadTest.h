/**
 * @file RealTimeThreadTest.h
 * @brief Header file for class RealTimeThreadTest
 * @date 07/03/2016
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

 * @details This header file contains the declaration of the class RealTimeThreadTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADTEST_H_
#define REALTIMETHREADTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeThread.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeThreadTest functions.
 */
class RealTimeThreadTest {

public:

    /**
     * @brief Creates a configuration database used in tests.
     */
    RealTimeThreadTest();

    /**
     * @brief Destroys the ObjectRegistryDatabase.
     */
    ~RealTimeThreadTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function initialises correctly the object from StructuredDataI
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function fails if no functions are defined in the thread.
     */
    bool TestInitialiseFalse_NoFunctions();

    /**
     * @brief Tests if the function adds the references to the GAM to be executed (this is tested implicitly).
     */
    bool TestConfigureArchitecture();

    /**
     * @brief Tests if the function fails if the GAM is not declared inside a container.
     */
    bool TestConfigureArchitecture_OrphanGAM();

    /**
     * @brief Tests if the function fails if the type declared not inherits from GAMI
     */
    bool TestConfigureArchitecture_InvalidGAMType();

    /**
     * @brief Tests if the function fails if the thread declares a wrong path for the gam
     */
    bool TestConfigureArchitecture_InvalidGAMPath();

    /**
     * @brief Tests if the function retrieves correctly the number of thread functions.
     */
    bool TestGetNumberOfFunctions();

    /**
     * @brief Tests if the function retrieves correctly the array with the thread functions.
     */
    bool TestGetFunctions();

    /**
     * @brief Tests if the function returns correctly the thread number of GAMs.
     */
    bool TestGetNumberOfGAMs();

    /**
     * @brief Tests if the function returns the thread GAMs array.
     */
    bool TestGetGAMs();

    /**
     * @brief Tests if the function returns the thread stack size correctly.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests if the function returns correctly the thread cpu mask.
     */
    bool TestGetCPU();

    /**
     * @brief Tests if the function performs correctly the conversion to a StructuredDataI.
     */
    bool TestToStructuredData();

    /**
     * @brief Tests if the function returns true if there is only one synchronising
     * GAM in the thread.
     */
    bool TestValidateDataSourceLinks();

    /**
     * @brief Tests if the function returns false if there are more than one
     * synchronising GAM in the thread.
     */
    bool TestValidateDataSourceLinksFalse_MoreSync();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADTEST_H_ */

