/**
 * @file GeneralDefinitions.h
 * @brief Header file for GeneralDefinitions
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the GeneralDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONS_H_
#define GENERALDEFINITIONS_H_

#include "CompilerTypes.h"

#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,GeneralDefinitions.h)

namespace MARTe{

#ifndef __ERROR_FUNCTION_NAME__
#define __ERROR_FUNCTION_NAME__ __func__
#endif


/** List of colors */
typedef enum {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Purple = 5,
    Cyan = 6,
    White = 7,
    Grey = 8,
    DarkRed = 9,
    DarkGreen = 10,
    DarkYellow = 11,
    DarkBlue = 12,
    DarkPurple = 13,
    DarkCyan = 14,
    DarkGrey = 15

}Colours;

/**
 * @brief A structure containing the time stamp informations.
 */
struct TimeValues {

    /** nanoseconds 0-999999 */
    uint32 microseconds;

    /** seconds 0-59 */
    uint32 seconds;

    /** minutes 0-59 */
    uint32 minutes;

    /** hours 0-23 */
    uint32 hours;

    /** days 1-31 */
    uint32 days;

    /** month 0-11 */
    uint32 month;

    /** year since 1900 */
    uint32 year;
};


/**
 * @brief Checks if the given float is a not-a-number value.
 */
static inline bool isNaN(const float32 x) {
    /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: It is a trick to detect nan floats in standard IEEE.*/
    return (x != x);
}

/**
 * @brief Checks if the given float is a not-a-number value.
 */
static inline bool isNaN(const float64 x) {
    /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: It is a trick to detect nan floats in standard IEEE.*/
    return (x != x);
}

/**
 * @brief Checks if the given float is a positive or negative infinity.
 */
static inline bool isInf(const float32 x) {
    return (!isNaN(x)) && (isNaN(x - x));
}

/**
 * @brief Checks if the given float is a positive or negative infinity
 */
static inline bool isInf(const float64 x) {
    return (!isNaN(x)) && (isNaN(x - x));
}

}

#endif /* GENERALDEFINITIONS_H_ */

