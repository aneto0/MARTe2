/**
 * @file GAMBareSchedulerTest.h
 * @brief Header file for class GAMBareSchedulerTest
 * @date 18/06/2021
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

 * @details This header file contains the declaration of the class GAMBareSchedulerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMBARESCHEDULERTEST_H_
#define GAMBARESCHEDULERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMBareScheduler.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the GAMBareScheduler class.
 */
class GAMBareSchedulerTest {

public:

	/**
	 * @brief Initialise GAMBareScheduler
	 */
    GAMBareSchedulerTest();

	/**
	 * @brief Destroys GAMBareScheduler
	 */
    ~GAMBareSchedulerTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method 
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function configures correctly the scheduler
     */
    bool TestConfigureScheduler();

    /**
     * @brief Tests if the scheduler can be started and executes
     */
    bool TestStartCurrentStateExecution();

    /**
     * @brief Tests the correct stop of cycle execution
     */
    bool TestStopCurrentStateExecution();

private:
    /**
     * @brief Reference to the real time application used internally in tests
     */
    ReferenceT<RealTimeApplication> realTimeApplication;

    /**
     * @brief Reference to the gam bare scheduler used internally in tests
     */
    ReferenceT<GAMBareScheduler> scheduler;

    /**
     * @brief Configures the RealTimeApplication ready for the tests
     * @return The configured RTApp based on the configuration file
     */
    bool PrepareApplication();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERITEST_H_ */

