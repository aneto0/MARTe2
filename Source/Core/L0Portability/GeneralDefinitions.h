/**
 * @file GeneralDefinitions.h
 * @brief Header file for class GeneralDefinitions
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

 * @details This header file contains the declaration of the class GeneralDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONS_H_
#define GENERALDEFINITIONS_H_

/*lint -save -e9026, function-like macro defined */
#define QUOTE(x) QUOTE_1(x)
/*lint -restore */
/*lint -save -e9026 -e9024, function-like macro defined, '#/##' operators used in macro */
#define QUOTE_1(x) #x
/*lint -restore */
/*lint -save -e9026 -estring(1960, *16-0-6*) , function-like macro defined, unparenthesized macro parameter*/
#define INCLUDE_FILE_ARCHITECTURE(x,y) QUOTE(Architecture/x/y)
#define INCLUDE_FILE_OPERATING_SYSTEM(x,y) QUOTE(OperatingSystem/x/y)
/*lint -restore */

#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,GeneralDefinitionsA.h)
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,GeneralDefinitionsOS.h)

/**
 * Uncomment this section to use memory statistics functions
 #ifndef MEMORY_STATISTICS
 #define MEMORY_STATISTICS
 #endif
 */

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

/** Large enough to store a pointer*/
#ifdef __LP64__
typedef unsigned long intptr;
#elif defined __ILP64__
typedef unsigned long intptr;
#elif defined __LLP64__
typedef unsigned long long intptr;
#else
typedef unsigned long intptr;
#endif

#endif /* GENERALDEFINITIONS_H_ */

