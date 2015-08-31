/**
 * @file FastMathA.h
 * @brief Header file for module FastMathA
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

 * @details This header file contains the declaration of the module FastMathA
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
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace FastMath {

float Sin(float angle) {
    volatile float output;
    __asm__ __volatile__(
            "fsin"
            : "=t" (output) : "0" (angle)
    );
    return output;
}

float Cos(float angle) {
    volatile float output;
    __asm__ __volatile__(
            "fcos;"
            : "=t" (output) : "0" (angle)
    );
    return output;
}

int32 FloatToInt(float input) {
    volatile int32 output;
    __asm__ __volatile__(
            "fld   %1;\n"
            "fistpl %0;"
            : "=m" (output) : "m" (input)
    );
    return output;
}

}

#endif /* FASTMATHA_H_ */
