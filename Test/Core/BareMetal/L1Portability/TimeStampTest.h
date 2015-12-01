/**
 * @file TimeStampTest.h
 * @brief Header file for class TimeStampTest
 * @date 11/11/2015
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

 * @details This header file contains the declaration of the class TimeStampTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMESTAMPTEST_H_
#define TIMESTAMPTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeStamp.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests all the TimeStamp public functions.
 */
class TimeStampTest {
public:

    /**
     * @brief Tests if all members are initialized correcty.ù
     */
    bool TestConstructor();

    /**
     * @brief Tests if the set respect the microseconds range [0-999999].
     */
    bool TestSetMicroSeconds(uint32 input);

    /**
     * @brief Tests if the microseconds member is returned correctly.
     */
    bool TestGetMicroSeconds(uint32 input);

    /**
     * @brief Tests if the set respect the seconds range [0-59].
     */
    bool TestSetSeconds(uint32 input);

    /**
     * @brief Tests if the seconds member is returned correctly.
     */
    bool TestGetSeconds(uint32 input);

    /**
     * @brief Tests if the set respect the minutes range [0-59].
     */
    bool TestSetMinutes(uint32 input);

    /**
     * @brief Tests if the minutes member is returned correctly.
     */
    bool TestGetMinutes(uint32 input);

    /**
     * @brief Tests if the set respect the hour range [0-23].
     */
    bool TestSetHour(uint32 input);

    /**
     * @brief Tests if the hour member is returned correctly.
     */
    bool TestGetHour(uint32 input);

    /**
     * @brief Tests if the set respect the day range [0-30].
     */
    bool TestSetDay(uint32 input);

    /**
     * @brief Tests if the day member is returned correctly.
     */
    bool TestGetDay(uint32 input);

    /**
     * @brief Tests if the set respect the month range [0-11].
     */
    bool TestSetMonth(uint32 input);

    /**
     * @brief Tests if the month member is returned correctly.
     */
    bool TestGetMonth(uint32 input);

    /**
     * @brief Tests if the set respect the month range [>=1900].
     */
    bool TestSetYear(uint32 input);

    /**
     * @brief Tests if the year member is returned correctly.
     */
    bool TestGetYear(uint32 input);

    /**
     * @brief Tests if passing 0 in input it returns the epoch timestamp.
     */
    bool TestConvertFromEpoch();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMESTAMPTEST_H_ */

