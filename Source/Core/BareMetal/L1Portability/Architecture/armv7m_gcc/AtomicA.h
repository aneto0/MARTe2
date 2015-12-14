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

inline void ATOMIC_INCREMENT(volatile int32 *value) {

    register int32 readValue;
    register int32 ret;
    do {
        asm volatile(
                "ldrex %0, [%1]\n"
                "add %0, %0, #1\n"
                "strex %2, %0, [%1]\n"
                "cmp %2, #0\n"
                : : "r" (readValue), "r" (value), "r" (ret)
        );
    }
    while (ret != 0);
}

inline void ATOMIC_DECREMENT(volatile int32 *value) {

    register int32 readValue;
    register int32 ret;
    do {
        asm volatile(
                "ldrex %0, [%1]\n"
                "sub %0, %0, #1\n"
                "strex %2, %0, [%1]\n"
                "cmp %2, #0\n"
                : : "r" (readValue), "r" (value), "r" (ret)
        );
    }
    while (ret != 0);
}

inline void EXCHANGE(volatile int32 *oldValue, int32 newValue) {

    register int32 readValue;
    register int32 ret;
    do {
        asm volatile(
                "ldrex %0, [%1]\n"
                "strex %2, %3, [%1]\n"
                : : "r" (readValue), "r" (oldValue), "r" (ret), "r" (newValue)
        );
    }
    while (ret != 0);
}

inline bool TEST_AND_SET(volatile int32 *value) {

    register int32 readValue = 0;
    register int32 ret;

    do {
        asm volatile(
                "ldrex %0, [%1]"
                : : "r" (readValue), "r" (value)
        );

        if (readValue != 0) {
            break;
        }
        asm volatile(
                "add %0, %0, #1\n"
                "strex %2, %0, [%1]\n"
                "mov %0, %2"
                : : "r" (readValue), "r" (value), "r" (ret)
        );

    }
    while (ret != 0);

    return readValue == 0;
}

namespace Atomic {

inline void Increment(volatile int32 *p) {
    ATOMIC_INCREMENT(p);
}

inline void Increment(volatile int16 *p) {
    void* ptr = (void*) p;
    volatile int32* pp = (int32*) ptr;
    Increment(pp);
}

inline void Increment(volatile int8 *p) {
    void* ptr = (void*) p;
    volatile int32* pp = (int32*) ptr;
    Increment(pp);
}

inline void Decrement(volatile int32 *p) {
    ATOMIC_DECREMENT(p);
}

inline void Decrement(volatile int16 *p) {
    void* ptr = (void*) p;
    volatile int32* pp = (int32*) ptr;
    Decrement(pp);
}

inline void Decrement(volatile int8 *p) {
    void* ptr = (void*) p;
    volatile int32* pp = (int32*) ptr;
    Decrement(pp);
}

inline int32 Exchange32(volatile int32 *p, int32 v) {
    EXCHANGE(p, v);
    return v;

}

inline bool TestAndSet(int32 volatile *p) {
    return TEST_AND_SET(p);
}

inline bool TestAndSet(int16 volatile *p) {
    void* ptr = (void*) p;
    volatile int32* pp = (int32*) ptr;
    return TestAndSet(pp);
}

inline bool TestAndSet(int8 volatile *p) {
    void* ptr = (void*) p;
    volatile int32* pp = (int32*) ptr;
    return TestAndSet(pp);
}

inline void Add(volatile int32 *p, int32 value) {

}

inline void Sub(volatile int32 *p, int32 value) {
}

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATOMICA_H_ */

