/**
 * @file SleepTest.h
 * @brief Header file for class SleepTest
 * @date 25/06/2015
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

 * @details This header file contains the declaration of the class SleepTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLEEPTEST_H_
#define 		SLEEPTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


#include "Sleep.h"

/** @brief A class to test all the Sleep functions. */
class SleepTest {

public:
    SleepTest() {
    }

    /**
     * @brief Tests the SleepAtLeast function.
     * @param[in] sec is the minimum time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepAtleast(double sec);

    /**
     * @brief Tests the SleepNoMore function
     * @param[in] sec is the maximum time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepNoMore(double sec);

    /**
     * @brief Tests the SleepSec function
     * @param[in] sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepSec(double sec);

    /**
     * @brief Tests the SleepSec function
     * @param[in] sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepSec(float sec);

    /**
     * @brief Tests the SleepMSec function
     * @param[in] sec is the time to sleep in milliseconds.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepMSec(int32 msec);

    /**
     * @brief Tests the SleepBusy function.
     * @param[in] sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepBusy(double sec);

    /**
     * @brief Tests the SleepSemiBusy function.
     * @param[in] totalSleepSec is the time to sleep using cpu.
     * @param[in] nonBusySleepSec is the time to sleep without using cpu.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSemiBusy(double totalSleepSec, double nonBusySleepSec);

    /**
     * @brief Executes all the tests
     * @return true if all tests return true.
     */
    bool All();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SLEEPTEST_H_ */

