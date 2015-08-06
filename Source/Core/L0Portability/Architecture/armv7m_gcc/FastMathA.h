/**
 * @file FastMathA.h
 * @brief Header file for class FastMathA
 * @date 05/ago/2015
 * @author pc
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

 * @details This header file contains the declaration of the class FastMathA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATHA_H_
#define FASTMATHA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class FastMathA {
public:

    /**
     * @see FastMath::Sin
     * @brief Calculate the sine of an angle.
     * @param angle is the angle parameter in radiants.
     * @return the value of the sine of angle. */
    static inline float Sin(float angle) {
        return 0.0;
    }

    /**
     * @see FastMath::Cos
     * @brief Calculate the cosine of an angle.
     * @param angle is the angle parameter in radiants.
     * @return the value of the cosine of angle.
     */
    static inline float Cos(float angle) {
        return 0.0;
    }

    /**
     * @see FastMath::FloatToInt
     * @brief Fast conversion to int by float.
     * @param input is the float number to convert.
     * @return the floor of input (32 bits integer). */
    static inline int32 FloatToInt(float input) {
        return 0;
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTMATHA_H_ */

