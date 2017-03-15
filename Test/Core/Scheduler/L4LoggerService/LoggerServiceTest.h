/**
 * @file LoggerServiceTest.h
 * @brief Header file for class LoggerServiceTest
 * @date 10/03/2017
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

 * @details This header file contains the declaration of the class LoggerServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERSERVICETEST_H_
#define LOGGERSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LoggerService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the LoggerService public methods.
 */
class LoggerServiceTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the initialise method with the default values.
     */
    bool TestInitialise_Defaults();

    /**
     * @brief Tests the initialise method without specifying the CPUs.
     */
    bool TestInitialise_False_NoCPUs();

    /**
     * @brief Tests the initialise method with a zero StackSize.
     */
    bool TestInitialise_False_StackSize_Zero();

    /**
     * @brief Tests the initialise method with a zero number of logger pages.
     */
    bool TestInitialise_False_NumberOfLoggerPages_Zero();

    /**
     * @brief Tests the initialise method without adding any consumer.
     */
    bool TestInitialise_False_NoConsumers();

    /**
     * @brief Tests the initialise method adding a consumer which is not of type LoggerConsumerI.
     */
    bool TestInitialise_False_NotLoggerConsumerI();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the GetNumberOfLogPages method.
     */
    bool TestGetNumberOfLogPages();

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERSERVICETEST_H_ */

