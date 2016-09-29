/**
 * @file MultiClientServiceTest.h
 * @brief Header file for class MultiClientServiceTest
 * @date 23/09/2016
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

 * @details This header file contains the declaration of the class MultiClientServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTICLIENTSERVICETEST_H_
#define MULTICLIENTSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MultiClientService.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the MultiClientService public methods.
 */
class MultiClientServiceTest {
public:

    /**
     * @brief Default constructor
     */
    MultiClientServiceTest();

    /**
     * @brief Destructor
     */
    virtual ~MultiClientServiceTest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor_Template();

    /**
     * @brief Tests the Initialise function.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise function without specifying the Timeout parameter.
     */
    bool TestInitialise_False_Timeout();

    /**
     * @brief Tests the Initialise function without specifying the MinNumberOfThreads parameter.
     */
    bool TestInitialise_False_MinNumberOfThreads();

    /**
     * @brief Tests the Initialise function without specifying the MaxNumberOfThreads parameter.
     */
    bool TestInitialise_False_MaxNumberOfThreads();

    /**
     * @brief Tests the Initialise function with MaxNumberOfThreads<MinNumberOfThreads parameter.
     */
    bool TestInitialise_False_MinGreaterThanMax();

    /**
     * @brief Tests the Initialise function with MinNumberOfThreads = 0.
     */
    bool TestInitialise_False_MinNumberOfThreads_0();

    /**
     * @brief Tests the Start function.
     */
    bool TestStart();

    /**
     * @brief Tests the Start function and force not sufficient threads to be available.
     */
    bool TestStart_NotEnoughThreads();

    /**
     * @brief Tests the Start function calling it twice, which should trigger an illegal operation.
     */
    bool TestStart_False();

    /**
     * @brief Tests the Start function after a stop has been called.
     */
    bool TestStart_Restart();

    /**
     * @brief Tests the Stop function.
     */
    bool TestStop();

    /**
     * @brief Tests the Stop function forcing a kill.
     */
    bool TestStop_Kill();

    /**
     * @brief Tests the GetMaximumNumberOfPoolThreads function
     */
    bool TestGetMaximumNumberOfPoolThreads();

    /**
     * @brief Tests the GetMinimumNumberOfPoolThreads function
     */
    bool TestGetMinimumNumberOfPoolThreads();

    /**
     * @brief Tests the SetMaximumNumberOfPoolThreads function
     */
    bool TestSetMaximumNumberOfPoolThreads();

    /**
     * @brief Tests that the SetMaximumNumberOfPoolThreads function does allow to change the maximum if the value is less than the minimum.
     */
    bool TestSetMaximumNumberOfPoolThreads_MinGreaterThanMax();

    /**
     * @brief Tests that the SetMaximumNumberOfPoolThreads function does allow to change the maximum if the service has started. But allows again once the service is stopped.
     */
    bool TestSetMaximumNumberOfPoolThreads_AfterStart();

    /**
     * @brief Tests that the SetMinimumNumberOfPoolThreads function does allow to change the minimum if the value is more than the maximum.
     */
    bool TestSetMinimumNumberOfPoolThreads_MinGreaterThanMax();

    /**
     * @brief Tests that the SetMaximumNumberOfPoolThreads function does allow to change the minimum if the service has started. But allows again once the service is stopped.
     */
    bool TestSetMinimumNumberOfPoolThreads_AfterStart();

    /**
     * @brief Tests the GetNumberOfActiveThreads() function.
     */
    bool TestGetNumberOfActiveThreads();

    /**
     * @brief Tests that the SetMinimumNumberOfPoolThreads function does allow to change the minimum if the value is zero.
     */
    bool TestSetMinimumNumberOfPoolThreads_0();

    /**
     * @brief Tests the SetMinimumNumberOfPoolThreads function
     */
    bool TestSetMinimumNumberOfPoolThreads();

    /**
     * @brief Tests the AddThread method.
     */
    bool TestAddThread();

    /**
     * @brief Tests the RemoveThread method.
     */
    bool TestRemoveThread();

    /**
     * @brief Tests the MoreThanEnoughThreads method.
     */
    bool TestMoreThanEnoughThreads();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MULTICLIENTSERVICETEST_H_ */

