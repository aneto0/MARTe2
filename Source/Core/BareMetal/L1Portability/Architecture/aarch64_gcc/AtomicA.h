/**
 * @file AtomicA.h
 * @brief Header file for class AtomicA
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

 * @details This header file contains the declaration of the class AtomicA
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
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Atomic {

inline void Increment(volatile int32 *p) {
    __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
    /*register int32 readValue;
    register int32 ret;
    do {
        asm volatile(
                "ldrex %0, [%1]\n"
                "add %0, %0, #1\n"
                "strex %2, %0, [%1]\n"
                "cmp %2, #0\n"
                : : "r" (readValue), "r" (p), "r" (ret)
        );
    }
    while (ret != 0);*/
}

inline void Increment(volatile int16 *p) {
    __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
    /*asm volatile( "cpsid i");
    (*p)++;
    asm volatile( "cpsie i");*/
}

inline void Increment(volatile int8 *p) {
    __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
    /*asm volatile( "cpsid i");
    (*p)++;
    asm volatile( "cpsie i");*/
}

inline void Decrement(volatile int32 *p) {
    __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
    /*register int32 readValue;
    register int32 ret;
    do {
        asm volatile(
                "ldrex %0, [%1]\n"
                "sub %0, %0, #1\n"
                "strex %2, %0, [%1]\n"
                "cmp %2, #0\n"
                : : "r" (readValue), "r" (p), "r" (ret)
        );
    }
    while (ret != 0);*/
}

inline void Decrement(volatile int16 *p) {
    __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
    /*asm volatile( "cpsid i");
    (*p)--;
    asm volatile( "cpsie i");*/
}

inline void Decrement(volatile int8 *p) {
    __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
    /*asm volatile( "cpsid i");
    (*p)--;
    asm volatile( "cpsie i");*/
}

inline int32 Exchange(volatile int32 *p,
                      int32 v) {
    int32 ret;
    __atomic_exchange(p, &v, &ret, __ATOMIC_SEQ_CST);
    return ret;
    /*register int32 readValue;
    register int32 ret;
    do {
        asm volatile(
                "ldrex %0, [%1]\n"
                "strex %2, %3, [%1]\n"
                : : "r" (readValue), "r" (p), "r" (ret), "r" (v)
        );
    }
    while (ret != 0);
    return v;*/

}

inline bool TestAndSet(int32 volatile *p) {
    return __atomic_test_and_set(p, __ATOMIC_SEQ_CST);
    /*register int32 readValue;
    register int32 ret;

    bool ok = (*p == 0);

    if (ok) {
        do {
            asm volatile(
                    "ldrex %0, [%1]"
                    : : "r" (readValue), "r" (p)
            );

            if (readValue != 0) {
                break;
            }
            asm volatile(
                    "add %0, %0, #1\n"
                    "strex %2, %0, [%1]\n"
                    "mov %0, %2"
                    : : "r" (readValue), "r" (p), "r" (ret)
            );

        }
        while (ret != 0);
        ok = (readValue == 0);
    }
    return ok;*/
}

inline bool TestAndSet(int16 volatile *p) {
    return __atomic_test_and_set(p, __ATOMIC_ACQUIRE);
    /*bool ret = (*p == 0);
    if (ret) {
        asm volatile( "cpsid i");
        *p = 1;
        asm volatile( "cpsie i");
    }
    return ret;*/

}

inline bool TestAndSet(int8 volatile *p) {
    return __atomic_test_and_set(p, __ATOMIC_ACQUIRE);
    /*bool ret = (*p == 0);
     if (ret) {
         asm volatile( "cpsid i");
         *p = 1;
         asm volatile( "cpsie i");
     }
     return ret;*/
}

inline void Add(volatile int32 *p,
                int32 value) {
    __atomic_add_fetch(p, value, __ATOMIC_SEQ_CST);
/*
    asm volatile( "cpsid i");
    (*p) += value;
    asm volatile( "cpsie i");*/

}

inline void Sub(volatile int32 *p,
                int32 value) {
    __atomic_sub_fetch(p, value, __ATOMIC_SEQ_CST);
/*
    asm volatile( "cpsid i");
    (*p) -= value;
    asm volatile( "cpsie i");*/
}

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATOMICA_H_ */

