/**
 * @file RealTimeLoaderTest.h
 * @brief Header file for class RealTimeLoaderTest
 * @date 05/04/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class RealTimeLoaderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_REALTIME_LOADERTEST_H_
#define L6APP_REALTIME_LOADERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the Loader public methods.
 */
class RealTimeLoaderTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Configure method
     */
    bool TestConfigure();

    /**
     * @brief Tests the Configure method with a bad configuration (parsed ok but not initialised).
     */
    bool TestConfigure_False_FailedConfiguration();

    /**
     * @brief Tests the Configure method with a bad RealTimeApplication configuration
     */
    bool TestConfigure_False_FailedRealTimeApplicationConfiguration();

    /**
     * @brief Tests the Configure method more than RT one application.
     */
    bool TestConfigure_MoreApplications();

    /**
     * @brief Tests the Start method
     */
    bool TestStart_FirstState();

    /**
     * @brief Tests that the Start method also work with the message
     */
    bool TestStart_Message();

    /**
     * @brief Tests the Configure method with a bad RealTimeApplication that returns False on StartNextStateExecution
     */
    bool TestStart_False_FailedRealTimeApplicationStartNextStateExecutionConfiguration();

    /**
     * @brief Tests the Configure method with a bad RealTimeApplication that returns False on PrepareNextState
     */
    bool TestStart_False_FailedRealTimeApplicationPrepareNextStateConfiguration();

    /**
     * @brief Tests the Stop method.
     */
    bool TestStop();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_REALTIME_LOADERTEST_H_ */
