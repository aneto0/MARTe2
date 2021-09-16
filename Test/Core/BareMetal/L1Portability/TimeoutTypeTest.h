/**
 * @file TimeoutTypeTest.h
 * @brief Header file for class TimeoutTypeTest
 * @date 23/07/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class TimeoutTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMEOUTTYPETEST_H_
#define TIMEOUTTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test all the TimeoutType functions.
 */
class TimeoutTypeTest {

public:

    /**
     * @brief Default constructor
     */
    TimeoutTypeTest();

    /**
     * @brief Destructor
     */
    virtual ~TimeoutTypeTest();

    /**
     * @brief Test the constructor of the TimeoutType class
     * @detail declare a TimeoutType variable using the default time.
     * @return true if the TimeoutType::msecTimeout = 0xFFFFFFFFu
     */
    bool TestConstructorDefault();

    /**
     * @brief Test the constructor of the TimeoutType class
     * @detail declare a TimeoutType variable specifying the time in uint32.
     * @param[in] msec is the time in milliseconds.
     * @return true if the TimeoutType::msecTimeout = msec
     */
    bool TestConstructorNotDefault(uint32 msec);

    /**
     * @brief Test the constructor of the TimeoutType class
     * @detail declare a TimeoutType variable specifying the time in int32.
     * @param[in] msec is the time in milliseconds.
     * @return true if the TimeoutType::msecTimeout = msec
     */
    bool TestConstructorNotDefault_2(int32 msec);

    /**
     * @brief Test the constructor of the TimeoutType class
     * @detail declare a TimeoutType variable specifying the time in uint64.
     * @param[in] msec is the time in milliseconds.
     * @return true if the TimeoutType::msecTimeout = usec
     */
    bool TestConstructorNotDefault_3(uint64 usec);

    /**
     * @brief Test TimeoutType::SetTimeoutSec() function.
     * @detail creat a timeoutType variable by default and then set the msecTimeout using
     * TimeoutType::SetTimeoutSec.
     * @param[in] sec is the time in seconds.
     * @return true if theTimeoutType::msecTimeout is the  expected value.
     */
    bool TestSetTimeoutSec(double sec);

    /**
     * @brief Test TimeoutType::SetTimeoutHighResolutionTimerTicks() function.
     * @detail Set the TimeoutType::msecTimeout using ticks (both ticks > 0 and ticks < 0) and then
     * compare TimeoutType::msecTimeout with an expected value.
     * @return true if the TimeoutType::msecTimeout is the expected value when ticks > 0 and 0 when
     * ticks < 0.
     */
    bool TestSetTimeoutHighResolutionTimerTicks();

    /**
     * @brief Test TimeoutType::TestHighResolutionTimerTicks() function.
     * @detail Set the TimeoutType::msecTimeout and then get it using TestHighResolutionTimerTicks
     * @return true if the TimeoutType::TestHighResolutionTimerTicks() returns the expected ticks.
     */
    bool TestHighResolutionTimerTicks();

    /**
     * @brief Test -= operator.
     * @detail Use -= operator and compare the result with the expected value. The two possibilities are
     * tested: the result is positive (normal case) and the result is negative (in this case
     * msecTimeout = 0)
     * @return true if the result of the -= operation is as expected.
     */
    bool TestSubstractAssignOperator();

    /**
     * @brief Test == operator.
     * @detail compare 3 TimeoutType two of them are equals and the other are different.
     * @return true if the comparison is true when the equal TimeoutType variables are compared and
     * false when they are different.
     */
    bool TestEqualComparison();

    /**
     * @brief Test != operator.
     * @detail compare 3 TimeoutType two of them are equals and the other are different.
     * @return true if the comparison is false when the equal TimeoutType variables are compared and
     * true when they are different.
     */
    bool TestDiffComparison();

    /**
     * @brief Test = operator.
     * @detail copy timeout1 to timeout2 using = operator.
     * @return true if timeout1 = timeout2.
     */
    bool TestAssignOperator();

    /**
     * @brief Test TimeoutType::TestIsFinite() function.
     * @detail create two TimeoutType variables; one with a finite msecTimeout and another with
     * infinite msceTimeout
     * @return true if TimeoutType::TestIsFinite() returns true when msecTimeout is finite and returns
     * false when the msecTimeout is infinite.
     */
    bool TestIsFinite();

    /**
     * @brief Test TimeoutType::GetTimeoutMSec() function.
     * @detail create TimeoutType using default constructor and then check the msecTimeout
     * @return true if TimeoutType::TestGetTimeoutMSec() returns the expected msecTimeout.
     */
    bool TestGetTimeoutMSec();

private:

    /**
     * Value used for performing checks
     */
    bool retValue;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMEOUTTYPETEST_H_ */

