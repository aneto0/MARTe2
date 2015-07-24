/**
 * @file FastMath.h
 * @brief Header file for class FastMath
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

 * @details This header file contains the declaration of the class FastMath
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
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,FastMathA.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Performs mathematical operations using fast processor instructions.
 */
class FastMath {

public:

    /**
     * The constant for e
     */
    static const float64 E;

    /**
     * The constant for log2(e)
     */
    static const float64 LOG2E;

    /**
     * The constant for log10(e)
     */
    static const float64 LOG10E;

    /**
     * The constant for log(2)
     */
    static const float64 LN2;

    /**
     * The constant for log(10)
     */
    static const float64 LN10;

    /**
     * The constant for pi
     */
    static const float64 PI;

    /**
     * The constant for pi/2
     */
    static const float64 PI_2;

    /**
     * The constant for pi/4
     */
    static const float64 PI_4;

    /**
     * The constant for 1/pi
     */
    static const float64 ONE_PI;

    /**
     * The constant for 2/pi
     */
    static const float64 TWO_PI;

    /**
     * The constant for 2/sqrt(pi)
     */
    static const float64 TWO_SQRTPI;

    /**
     * The constant for sqrt(2)
     */
    static const float64 SQRT2;

    /**
     * The constant for sqrt(1/2)
     */
    static const float64 SQRT1_2;

    /**
     * @brief Converts a float to an integer using a processor instruction.
     * @param[in] input the value to convert
     * @return the input as an integer
     */
    static inline int32 FloatToInt(const float32 input);

    /**
     * @brief Computes the cosine of an angle using a processor instruction.
     * @param[in] angle the angle to compute
     * @return the cosine of angle
     */
    static inline float32 Cos(const float32 angle);

    /**
     * @brief Computes the sine of an angle using a processor instruction.
     * @param[in] angle the angle to compute
     * @return the sine of angle
     */
    static inline float32 Sin(const float32 angle);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

int32 FastMath::FloatToInt(const float32 input) {
    return FastMathA::FloatToInt(input);
}

float32 FastMath::Cos(const float32 angle) {
    return FastMathA::Cos(angle);
}

float32 FastMath::Sin(const float32 angle) {
    return FastMathA::Sin(angle);
}

#endif /* FASTMATH_H_ */

