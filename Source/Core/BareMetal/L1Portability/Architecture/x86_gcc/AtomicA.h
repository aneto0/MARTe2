/**
 * @file AtomicA.h
 * @brief Header file for module AtomicA
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

 * @details This header file contains the declaration of the module AtomicA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMICA_H_
#define ATOMICA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
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

namespace MARTe {

namespace Atomic {

inline void Increment(volatile int32 *p) {
#if GCC_VERSION > 40700
    __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
#else
    __sync_add_and_fetch(p, 1);
#endif
}

inline void Increment(volatile int16 *p) {
#if GCC_VERSION > 40700
    __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
#else
    __sync_add_and_fetch(p, 1);
#endif
}

inline void Increment(volatile int8 *p) {
#if GCC_VERSION > 40700
    __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
#else
    __sync_add_and_fetch(p, 1);
#endif
}

inline void Decrement(volatile int32 *p) {
#if GCC_VERSION > 40700
    __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
#else
    __sync_sub_and_fetch(p, 1);
#endif
}

inline void Decrement(volatile int16 *p) {
#if GCC_VERSION > 40700
    __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
#else
    __sync_sub_and_fetch(p, 1);
#endif
}

inline void Decrement(volatile int8 *p) {
#if GCC_VERSION > 40700
    __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
#else
    __sync_sub_and_fetch(p, 1);
#endif
}

inline int32 Exchange(volatile int32 *p,
                      int32 v) {
#if GCC_VERSION > 40700
    int32 ret;
    __atomic_exchange(p, &v, &ret, __ATOMIC_SEQ_CST);
    return ret;
#else
    asm volatile(
            "lock xchg (%1), %0"
            :"=r" (v) : "r" (p), "0" (v)
    );
    return v;
#endif
}

inline bool TestAndSet(volatile int32 *p) {
    /*
     * @retval 1 if the flag was already true - lock not taken
     * @retval 0 if the flag was false and is now set to true - lock taken
     */
#if GCC_VERSION > 40700
    return (__atomic_test_and_set(p, __ATOMIC_SEQ_CST) == 0u);
#else
    register int32 out = 1;
    asm volatile (
            "lock xchg (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
#endif    
}

inline bool TestAndSet(volatile int16 *p) {
#if GCC_VERSION > 40700
    return (__atomic_test_and_set(p, __ATOMIC_SEQ_CST) == 0u);
#else
    register int16 out = 1;
    asm volatile (
            "lock xchg (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
#endif    
}

inline bool TestAndSet(volatile int8 *p) {
#if GCC_VERSION > 40700
    return (__atomic_test_and_set(p, __ATOMIC_SEQ_CST) == 0u);
#else
    register int8 out = 1;
    asm volatile (
            "lock xchg (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
#endif
}

inline void Add(volatile int32 *p, int32 value) {
#if GCC_VERSION > 40700
    __atomic_add_fetch(p, value, __ATOMIC_SEQ_CST);
#else
    asm volatile (
            "lock addl %1, (%0)"
            : /* output */
            :"r" (p), "ir" (value) /* input */
    );

#endif
}

inline void Sub(volatile int32 *p, int32 value) {
#if GCC_VERSION > 40700
    __atomic_sub_fetch(p, value, __ATOMIC_SEQ_CST);
#else
    asm volatile (
            "lock subl %1, (%0)"
            : /* output */
            :"r" (p), "ir" (value) /* input */
    );
#endif
}

}

}
#endif /* ATOMICA_H_ */
