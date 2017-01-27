/**
 * @file SingleThreadServiceTest.h
 * @brief Header file for class SingleThreadServiceTest
 * @date 19/09/2016
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

 * @details This header file contains the declaration of the class SingleThreadServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SINGLETHREADSERVICETEST_H_
#define SINGLETHREADSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SingleThreadService.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the SingleThreadService public methods.
 */
class SingleThreadServiceTest {
public:

    /**
     * @brief Default constructor
     */
    SingleThreadServiceTest();

    /**
     * @brief Destructor
     */
    virtual ~SingleThreadServiceTest();

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
    bool TestInitialise_False();

    /**
     * @brief Tests the Start function.
     */
    bool TestStart();

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
     * @brief Tests the SetTimeout function
     */
    bool TestSetTimeout();

    /**
     * @brief Tests the GetTimeout function
     */
    bool TestGetTimeout();

    /**
     * @brief Tests the GetPriorityClass method.
     */
    bool TestGetPriorityClass();

    /**
     * @brief Tests the SetPriorityClass method.
     */
    bool TestSetPriorityClass();

    /**
     * Tests the GetPriorityLevel method.
     */
    bool TestGetPriorityLevel();

    /**
     * @brief Tests the SetPriorityLevel method.
     */
    bool TestSetPriorityLevel();

    /**
     * @brief Tests the TestGetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the TestSetCPUMask method.
     */
    bool TestSetCPUMask();

    /**
     * @brief Verifies that priority class is not changed after the service has started.
     */
    bool TestSetPriorityClass_Start();

    /**
     * @brief Verifies that priority level is not changed after the service has started.
     */
    bool TestSetPriorityLevel_Start();

    /**
     * @brief Verifies that priority class is not changed after the service has started.
     */
    bool TestSetCPUMask_Start();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SINGLETHREADSERVICETEST_H_ */

