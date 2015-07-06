/**
 * @file HighResolutionTimerTest.h
 * @brief Header file for class HighResolutionTimerTest
 * @date 26/06/2015
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

 * @details This header file contains the declaration of the class HighResolutionTimerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERTEST_H_
#define 		HIGHRESOLUTIONTIMERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/**
 * @brief A Class to test the HighResolutionTimer functions.
 */
class HighResolutionTimerTest {

private:

public:

    /**
     * @brief Tests The HighResolutionTimer::Period function.
     * @return true if the returned value is greater than zero.
     */
    bool TestPeriod();

    /**
     * @brief Tests The HighResolutionTimer::Frequency function.
     * @return true if the returned value is greater than zero.
     */
    bool TestFrequency();


    /**
     * @brief Checks if period and frequency are mutual.
     * @return true if frequency=1/period.
     */
    bool TestPeriodFrequency();

    /**
     * @brief Sleep for sleepTime time and check if the time measured with HighResolutionTimer::Counter is more or less the same.
     * @param[in] sleepTime is the time to sleep.
     * @return true if the measured time is the same of the sleep time less than a tolerance, false otherwise.
     */
    bool TestCounter(float64 sleepTime);


    /**
     * @brief Sleep for sleepTime time and check if the time measured with HighResolutionTimer::Counter32 is more or less the same.
     * @param[in] sleepTime is the time to sleep.
     * @return true if the measured time is the same of the sleep time less than a tolerance, false otherwise.
     */
    bool TestCounter32(float64 sleepTime);

    /**
     * @brief Tests the HighResolutionTimer::TicksToTime function.
     * @details converts the time passed by argument in ticks and checks if HighResolutionTimer::TicksToTime returns the same time.
     * @param[in] sleepTime is the time argument.
     * @return true if the conversion from ticks to time is correct.
     */
    bool TestTicksToTime(float64 sleepTime);

    /**
     * @brief Test the time stamp returned by HighResolutionTimer::GetTimeStamp.
     * @details Gets first a time stamp, then sleeps for the specified milliseconds and gets again another timestamp. Checks if the difference in time between the time stamps is
     * consistent with the time interval elapsed. At last checks the boundaries of the time stamp structure fields.
     * @param[in] millisecs is the interval between two timestamp get functions.
     * @return true if successful, false otherwise.
     */
    bool TestTimeStamp(uint32 millisecs);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HIGHRESOLUTIONTIMERTEST_H_ */

