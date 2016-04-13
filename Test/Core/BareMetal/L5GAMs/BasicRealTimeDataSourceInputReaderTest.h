/**
 * @file BasicRealTimeDataSourceInputReaderTest.h
 * @brief Header file for class BasicRealTimeDataSourceInputReaderTest
 * @date 22/mar/2016
 * @author pc
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

 * @details This header file contains the declaration of the class BasicRealTimeDataSourceInputReaderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICREALTIMEDATASOURCEINPUTREADERTEST_H_
#define BASICREALTIMEDATASOURCEINPUTREADERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicRealTimeDataSourceInputReader.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeDataSourceInputReader functions.
 */
class BasicRealTimeDataSourceInputReaderTest {

public:

    /**
     * @brief Creates a configuration database used in tests.
     */
    BasicRealTimeDataSourceInputReaderTest();

    /**
     * @brief tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the read of default value on preallocated memory.
     */
    bool TestRead_DefaultValues_Static();

    /**
     * @brief Tests the read of default value on internal allocated memory.
     */
    bool TestRead_DefaultValues_Allocation();

    /**
     * @brief Tests the read on a preallocated memory.
     */
    bool TestRead_Static();

    /**
     * @brief Tests the read on the internal alloctaed memory.
     */
    bool TestRead_Allocation();

    /**
     * @brief Tests the read of more variables.
     */
    bool TestRead_MoreThanOneVariable();

    /**
     * @brief Tests the read of a vector variable.
     */
    bool TestRead_MultiDim_Vector();

    /**
     * @brief Tests the read of a matrix variable.
     */
    bool TestRead_MultiDim_Matrix();

    /**
     * @brief Tests if the function behaves as a normal read if the spin-lock
     * semaphore is not set.
     */
    bool TestSynchroniseOnSpinLockSemNotSync();

    /**
     * @brief Tests if the function performs a synchronised read.
     */
    bool TestSynchroniseOnSpinLockSem();

    /**
     * @brief Tests if the function does not exit before the sample time set.
     */
    bool TestSynchroniseOnSpinLockSemWaitTime();

    /**
     * @brief Tests if the function performs the specified number of synchronised read before exiting.
     */
    bool TestSynchroniseOnSpinLockSemMoreReads();

    /**
     * @brief Tests if the function exits after the timeout expire waiting for data on the spin-lock semaphore.
     */
    bool TestSynchroniseOnSpinLockSemTimeout();

    /**
     * @brief Tests if the function does not exit before the sample time set waiting without releasing the cpu.
     */
    bool TestSynchroniseOnSpinLockNoSleep();

private:

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase pidCDB;

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase plantCDB;

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase appCDB;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICREALTIMEDATASOURCEINPUTREADERTEST_H_ */

