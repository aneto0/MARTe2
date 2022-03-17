/**
 * @file AtomicA.h
 * @brief Header file for module AtomicA
 * @date 17/06/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the module AtomicA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMICA_H_
#define ATOMICA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#define MEMMODEL        __ATOMIC_SEQ_CST
#define TSL_MEMMODEL    __ATOMIC_SEQ_CST

namespace MARTe {

namespace Atomic {

inline void Increment(volatile int32 *p) {
    __atomic_add_fetch(p, 1, MEMMODEL);
}

inline void Increment(volatile int16 *p) {
    __atomic_add_fetch(p, 1, MEMMODEL);
}

inline void Increment(volatile int8 *p) {
    __atomic_add_fetch(p, 1, MEMMODEL);
}

inline void Decrement(volatile int32 *p) {
    __atomic_sub_fetch(p, 1, MEMMODEL);
}

inline void Decrement(volatile int16 *p) {
    __atomic_sub_fetch(p, 1, MEMMODEL);
}

inline void Decrement(volatile int8 *p) {
    __atomic_sub_fetch(p, 1, MEMMODEL);
}

inline int32 Exchange(volatile int32 *p,
                      int32 v) {
    int32 retVal;
    __atomic_exchange(p, &v, &retVal, MEMMODEL);
    return retVal;
}

inline bool TestAndSet(volatile int32 *p) {
    return (!__atomic_test_and_set(p, TSL_MEMMODEL));
}

inline bool TestAndSet(volatile int16 *p) {
    return (!__atomic_test_and_set(p, TSL_MEMMODEL));
}

inline bool TestAndSet(volatile int8 *p) {
    return (!__atomic_test_and_set(p, TSL_MEMMODEL));
}

inline void Add(volatile int32 *p, int32 value) {
    __atomic_add_fetch(p, value, MEMMODEL);
}

inline void Sub(volatile int32 *p, int32 value) {
    __atomic_sub_fetch(p, value, MEMMODEL);
}

}

}
#endif /* ATOMICA_H_ */
