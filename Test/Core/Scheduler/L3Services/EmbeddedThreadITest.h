/**
 * @file EmbeddedThreadITest.h
 * @brief Header file for class EmbeddedThreadITest
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

 * @details This header file contains the declaration of the class EmbeddedThreadITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDTHREADSITEST_H_
#define EMBEDDEDTHREADSITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedThreadI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the EmbeddedThreadI public methods.
 */
class EmbeddedThreadITest {
public:

    /**
     * @brief Default constructor
     */
    EmbeddedThreadITest();

    /**
     * @brief Destructor
     */
    virtual ~EmbeddedThreadITest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the GetThreadNumber method.
     */
    bool TestGetThreadNumber();

    /**
     * @brief Tests the SetThreadNumber method.
     */
    bool TestSetThreadNumber();

    /**
     * @brief Tests the GetThreadId method.
     */
    bool TestGetThreadId();

    /**
     * @brief Tests the LaunchThread method.
     */
    bool TestLaunchThread();

    /**
     * @brief Tests the GetCommands method.
     */
    bool TestGetCommands();

    /**
     * @brief Tests the SetCommands method.
     */
    bool TestSetCommands();

    /**
     * @brief Tests the ResetThreadId method.
     */
    bool TestResetThreadId();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Callback function of the thread.
     */
    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information);

    /**
     * True when the Execute method is called.
     */
    bool executeCalled;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EMBEDDEDTHREADSITEST_H_ */

