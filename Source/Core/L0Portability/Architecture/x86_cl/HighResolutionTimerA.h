/**
 * @file HighResolutionTimerA.h
 * @brief Header file for class HighResolutionTimerA
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

 * @details This header file contains the declaration of the class HighResolutionTimerA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERA_H_
#define 		HIGHRESOLUTIONTIMERA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <intrin.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace HighResolutionTimer{

/**
 * @brief Reads the High Resolution Timer as 32 bit. Fast inline assembler.
 * @return number of cpu ticks in a 32 bit integer.
 */
inline uint32 Counter32() {

    return (uint32) __rdtsc();

}

/**
 * @brief Reads the High Resolution Timer as 64 bit int. Fast inline assembler.
 * @return number of cpu ticks in a 64 bit integer.
 */
inline int64 Counter() {

    return (int64) __rdtsc();

}

}

#endif /* HIGHRESOLUTIONTIMERA_H_ */
