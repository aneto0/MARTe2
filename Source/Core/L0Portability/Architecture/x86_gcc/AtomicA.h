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

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace Atomic {

inline void Increment(volatile int32 *p) {
    asm volatile(
            "lock incl (%0)\n"
            : : "r" (p)
    );
}

inline void Increment(volatile int16 *p) {
    asm volatile(
            "lock incw (%0)\n"
            : : "r" (p)
    );
}

inline void Increment(volatile int8 *p) {
    asm volatile(
            "lock incb (%0)\n"
            : : "r" (p)
    );
}

inline void Decrement(volatile int32 *p) {
    asm volatile(
            "lock decl (%0)\n"
            : : "r" (p)
    );
}

inline void Decrement(volatile int16 *p) {
    asm volatile(
            "lock decw (%0)\n"
            : : "r" (p)
    );
}

inline void Decrement(volatile int8 *p) {
    asm volatile(
            "lock decb (%0)\n"
            : : "r" (p)
    );
}

inline int32 Exchange(volatile int32 *p,
                      int32 v) {
    asm volatile(
            "lock xchg (%1), %0"
            :"=r" (v) : "r" (p), "0" (v)
    );
    return v;
}

inline bool TestAndSet(volatile int32 *p) {
    register int32 out = 1;
    asm volatile (
            "lock xchg (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
}

inline bool TestAndSet(volatile int16 *p) {
    register int16 out = 1;
    asm volatile (
            "lock xchgw (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
}

inline bool TestAndSet(volatile int8 *p) {
    register int8 out = 1;
    asm volatile (
            "lock xchgb (%2),%1"
            : "=q" (out) : "0" (out), "q" (p)
    );
    return (out == 0);
}

inline void Add(volatile int32 *p,
                int32 value) {
    asm volatile (
            "lock addl %1, (%0)"
            : /* output */
            :"r" (p), "ir" (value) /* input */
    );
}

inline void Sub(volatile int32 *p,
                int32 value) {
    asm volatile (
            "lock subl %1, (%0)"
            : /* output */
            :"r" (p), "ir" (value) /* input */
    );
}

}

#endif /* ATOMICA_H_ */
