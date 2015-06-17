/**
 * @file GeneralDefinitions.h
 * @brief Header file for class GeneralDefinitions
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class GeneralDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONS_H_
#define 		GENERALDEFINITIONS_H_

#define QUOTE(x) QUOTE_1(x)
#define QUOTE_1(x) #x
#define INCLUDE_FILE_ARCHITECTURE(x,y) QUOTE(Architecture/x/y)
#define INCLUDE_FILE_OPERATING_SYSTEM(x,y) QUOTE(OperatingSystem/x/y)

#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,GeneralDefinitionsA.h)
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,GeneralDefinitionsOS.h)


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

#ifndef True
/** Portable definition of true. */
#define True   (1==1)
/** Portable definition of false. */
#define False  (1==0)
#endif

/** Builds a 64 bit field with two 32 bit values. */
#define load64(x,a,b)  ((uint32 *)&x)[0] = b; ((uint32 *)&x)[1] = a;

#endif /* GENERALDEFINITIONS_H_ */

