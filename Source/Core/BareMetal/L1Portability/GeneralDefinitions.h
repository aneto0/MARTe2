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

 * @details This header file contains the declaration of the GeneralDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONS_H_
#define GENERALDEFINITIONS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

#include INCLUDE_FILE_ENVIRONMENT(BareMetal,L1Portability,ENVIRONMENT,GeneralDefinitions.h)

namespace MARTe {

/**
 * @brief Macro for getting the name of the function on error reporting.
 */
#ifndef __ERROR_FUNCTION_NAME__
#define __ERROR_FUNCTION_NAME__ __func__
#endif

/**
 * @brief Type for a generic callback function.
 */
typedef void (GenericFn)(void*);



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
     * @brief Checks if the given float is a not-a-number value.
     */
    static inline bool IsNaN(const float32 x) {
        /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: It is a trick to detect nan floats in standard IEEE.*/
        return (x != x);
    }

    /**
     * @brief Checks if the given float is a not-a-number value.
     */
    static inline bool IsNaN(const float64 x) {
        /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: It is a trick to detect nan floats in standard IEEE.*/
        return (x != x);
    }

    /**
     * @brief Checks if the given float is a positive or negative infinity.
     */
    static inline bool IsInf(const float32 x) {
        return (!IsNaN(x)) && (IsNaN(x - x));
    }

    /**
     * @brief Checks if the given float is a positive or negative infinity
     */
    static inline bool IsInf(const float64 x) {
        return (!IsNaN(x)) && (IsNaN(x - x));
    }

    /**
     * @brief Checks if the given float numbers are equal
     */
    static inline bool IsEqual(const float32 f1, const float32 f2) {
        float32 *min=reinterpret_cast<float32*>(const_cast<uint32*>(&EPSILON_FLOAT32));
        return ((f1-f2)<(*min)) && ((f1-f2)>-(*min));
    }

    /**
     * @brief Checks if the given float numbers are equal
     */
    static inline bool IsEqual(const float64 f1, const float64 f2) {
        float64 *min=reinterpret_cast<float64*>(const_cast<uint64*>(&EPSILON_FLOAT64));
        return ((f1-f2)<(*min)) && ((f1-f2)>-(*min));
    }

    /**
     * @brief Checks if the given float numbers are equal
     */
    static inline bool IsEqual(const float64 f1, const float32 f2) {
        float32 *min=reinterpret_cast<float32*>(const_cast<uint32*>(&EPSILON_FLOAT32));
        return ((f1-f2)<(*min)) && ((f1-f2)>-(*min));
    }

    /**
     * @brief Checks if the given float numbers are equal
     */
    static inline bool IsEqual(const float32 f1, const float64 f2) {
        float32 *min=reinterpret_cast<float32*>(const_cast<uint32*>(&EPSILON_FLOAT32));
        return ((f1-f2)<(*min)) && ((f1-f2)>-(*min));
    }
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GENERALDEFINITIONS_H_ */

