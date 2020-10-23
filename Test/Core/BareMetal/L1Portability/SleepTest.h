/**
 * @file SleepTest.h
 * @brief Header file for class SleepTest
 * @date 25/06/2015
 * @author Giuseppe Ferr�
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
#define SLEEPTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Sleep.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test all the Sleep functions.
 */
class SleepTest {

public:

    /**
     * @brief Tests the Sleep::GetDateSeconds function.
     * @return true if the returned value is equal to the seconds elapsed since epoch.
     */
    bool TestGetDateSeconds();

    /**
     * @brief Tests the Sleep::NoMore function
     * @param[in] sec is the maximum time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestNoMore(float64 sec);

    /**
     * @brief Tests the Sleep::Sec function
     * @param[in] sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSec(float64 sec);

    /**
     * @brief Tests the Sleep::MSec function
     * @param[in] msec is the time to sleep in milliseconds.
     * @return true if it sleeps less than the double of msec.
     */
    bool TestMSec(int32 msec);

    /**
     * @brief Tests the Sleep::Busy function.
     * @param[in] sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestBusy(float64 sec);

    /**
     * @brief Tests the Sleep::SemiBusy function.
     * @param[in] totalSleepSec is the time to sleep using cpu.
     * @param[in] nonBusySleepSec is the time to sleep without using cpu.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSemiBusy(float64 totalSleepSec,
                      float64 nonBusySleepSec);

    /**
     * @brief Tests the GetSchedulerGranularity function.
     */
    bool TestGetSchedulerGranularity();
 
    /**
     * @brief Tests the SetSchedulerGranularity function.
     */
    bool TestSetSchedulerGranularity();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SLEEPTEST_H_ */

