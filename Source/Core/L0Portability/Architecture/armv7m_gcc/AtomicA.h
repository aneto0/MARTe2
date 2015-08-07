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

inline void EXCHANGE(volatile int32 *oldValue,
                     int32 newValue) {

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

    register int32 readValue;
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

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class AtomicA {

public:
    /**
     * @brief Atomically increment a 32 bit integer in memory.
     * @param p is the 32 bit variable which must be incremented.
     */
    static inline void Increment32(volatile int32 *p) {

        ATOMIC_INCREMENT(p);
    }

    /**
     * @brief Atomically increment a 16 bit integer in memory.
     * @param p is the 16 bits variable which must be incremented.
     */
    static inline void Increment16(volatile int16 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Increment32(pp);
    }

    /** @brief Atomically increment a 8 bit integer in memory.
     * @param p is 8 bits variable which must be incremented.  */
    static inline void Increment8(volatile int8 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Increment32(pp);
    }

    /** @brief Atomically decrement a 32 bit integer in memory.
     * @param p is the 32 bits variable to decrement. */
    static inline void Decrement32(volatile int32 *p) {

        ATOMIC_DECREMENT(p);
    }

    /** @brief Atomically decrement a 16 bit integer in memory.
     * @param p is the 16 bits variable to decrement. */
    static inline void Decrement16(volatile int16 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Decrement32(pp);
    }

    /** @brief Atomically decrement a 8 bit integer in memory.
     * @param p is the 8 bits variable to decrement. */
    static inline void Decrement8(volatile int8 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Decrement32(pp);
    }

    /** @brief Atomically exchange the contents of a variable with the specified memory location.
     * @param p is the variable to change.
     * @param v is the variable to store. */
    static inline int32 Exchange32(volatile int32 *p,
                                   int32 v) {
        EXCHANGE(p, v);
        return v;

    }

    /** @brief Test and set a 32 bit memory location in a thread safe way.
     * @param p is the 32 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false. */
    static inline bool TestAndSet32(int32 volatile *p) {

        return TEST_AND_SET(p);
    }

    /** @brief Test and set a 16 bit memory location in a thread safe way.
     * @param p is the 16 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false.  */
    static inline bool TestAndSet16(int16 volatile *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        return TestAndSet32(pp);
    }

    /** @brief Test and set a 8 bit memory location in a thread safe way.
     * @param p is the 8 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false. */
    static inline bool TestAndSet8(int8 volatile *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        return TestAndSet32(pp);
    }

    /**
     * @brief Atomic addition.
     * @param p is the variable.
     * @param value is the value to sum to p. */
    static inline void Add32(volatile int32 *p,
                             int32 value) {

    }

    /**
     * @brief Atomic subtraction.
     * @param p is the variable.
     * @param value is the value to subtract to p.
     */
    static inline void Sub32(volatile int32 *p,
                             int32 value) {
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATOMICA_H_ */

