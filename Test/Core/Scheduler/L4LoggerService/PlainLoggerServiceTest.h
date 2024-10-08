/**
 * @file PlainLoggerServiceTest.h
 * @brief Header file for class PlainLoggerServiceTest
 * @date 01/07/2024
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class PlainLoggerServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PLAINLOGGERSERVICETEST_H_
#define PLAINLOGGERSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "PlainLoggerService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the PlainLoggerService public methods.
 */
class PlainLoggerServiceTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise function. Does exactly the same as TestConstructor.
     */
    bool TestInitialise();

    /**
     * @brief Tests the initialise method without adding any consumer.
     */
    bool TestInitialise_False_NoConsumers();

    /**
     * @brief Tests the initialise method adding a consumer which is not of type LoggerConsumerI.
     */
    bool TestInitialise_False_NotLoggerConsumerI();

    /**
     * @brief Tests the initialise method assessing the failure if the more than the maximum number of Consumers is added.
    */
    bool TestInitialise_False_TooManyPlainLoggerServiceInstances();

    /**
     * @brief Tests the Log method indirectly, by building the whole chain around the Interface.
     */
    bool TestLog();

    /**
     * @brief Tests the Purge method indirectly, by building the whole chain around the Interface and calling explicitly the Purge on GlobalObjectDatabase.
     */
    bool TestPurge();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PLAINLOGGERSERVICETEST_H_ */

