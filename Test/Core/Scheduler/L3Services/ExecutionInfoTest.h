/**
 * @file ExecutionInfoTest.h
 * @brief Header file for class ExecutionInfoTest
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

 * @details This header file contains the declaration of the class ExecutionInfoTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXECUTIONINFOTEST_H_
#define EXECUTIONINFOTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ExecutionInfo.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the ExecutionInfo public methods.
 */
class ExecutionInfoTest {
public:

    /**
     * @brief Default constructor
     */
    ExecutionInfoTest();

    /**
     * @brief Destructor
     */
    virtual ~ExecutionInfoTest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the SetThreadNumber method.
     */
    bool TestSetThreadNumber();

    /**
     * @brief Tests the SetThreadNumber method and verifies that the thread number is not changed if the stage is not in Startup.
     */
    bool TestSetThreadNumber_StageNotStartup();

    /**
     * @brief Tests the SetStage method.
     */
    bool TestSetStage();

    /**
     * @brief Tests the SetStageSpecific method.
     */
    bool TestSetStageSpecific();

    /**
     * @brief Tests the GetThreadNumber method.
     */
    bool TestGetThreadNumber();

    /**
     * @brief Tests the GetStage method.
     */
    bool TestGetStage();

    /**
     * @brief Tests the GetStageSpecific method.
     */
    bool TestGetStageSpecific();

    /**
     * @brief Tests the Reset method.
     */
    bool TestReset();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EMBEDDEDTHREADTEST_H_ */

