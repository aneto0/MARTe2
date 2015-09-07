/**
 * @file FastMath.h
 * @brief Header file for module FastMath
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

 * @details This header file contains the declaration of the module FastMath
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATH_H_
#define FASTMATH_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Performs mathematical operations using fast processor instructions.
 */
namespace FastMath {

/**
 * The constant for e
 */
const float64 E = 2.7182818284590452354;

/**
 * The constant for log2(e)
 */
const float64 LOG2E = 1.4426950408889634074;

/**
 * The constant for log10(e)
 */
const float64 LOG10E = 0.43429448190325182765;

/**
 * The constant for log(2)
 */
const float64 LN2 = 0.69314718055994530942;

/**
 * The constant for log(10)
 */
const float64 LN10 = 2.30258509299404568402;

/**
 * The constant for pi
 */
const float64 PI = 3.14159265358979323846;

/**
 * The constant for pi/2
 */
const float64 PI_2 = 1.57079632679489661923;

/**
 * The constant for pi/4
 */
const float64 PI_4 = 0.78539816339744830962;

/**
 * The constant for 1/pi
 */
const float64 ONE_PI = 0.31830988618379067154;

/**
 * The constant for 2/pi
 */
const float64 TWO_PI = 0.63661977236758134308;

/**
 * The constant for 2/sqrt(pi)
 */
const float64 TWO_SQRTPI = 1.12837916709551257390;

/**
 * The constant for sqrt(2)
 */
const float64 SQRT2 = 1.41421356237309504880;

/**
 * The constant for sqrt(1/2)
 */
const float64 SQRT1_2 = 0.70710678118654752440;

/**
 * @brief Converts a float to an integer using a processor instruction.
 * @param[in] input the value to convert
 * @return the input as an integer
 */
inline int32 FloatToInt(const float32 input);

/**
 * @brief Computes the cosine of an angle using a processor instruction.
 * @param[in] angle the angle to compute
 * @return the cosine of angle
 */
inline float32 Cos(const float32 angle);

/**
 * @brief Computes the sine of an angle using a processor instruction.
 * @param[in] angle the angle to compute
 * @return the sine of angle
 */
inline float32 Sin(const float32 angle);
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,FastMathA.h)

#endif /* FASTMATH_H_ */
