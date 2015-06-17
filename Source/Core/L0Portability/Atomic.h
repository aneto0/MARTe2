/**
 * @file Atomic.h
 * @brief Header file for class Atomic
 * @date 17/06/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class Atomic
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMIC_H_
#define 		ATOMIC_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,AtomicA.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A collector of functions that are executed atomically even on multiprocessor machines.
 *
 * @details The target architecture is the one of the machine where the code is being executed.
 * Most of the implementation is therefore delegated to the AtomicA.h implementation,
 * which will be specific for each type of supported architecture.
 *
 * @details These methods are particularly useful in the implementation of functionality that requires
 * protected basic operations between multiple asynchronous accesses like in a multithread system.
 * These methods are also used to implement thread's synchronization constructors like for example
 * fast polling mutex semaphores.
 */
class Atomic {
public:
    /**
     * @brief Atomically increment a 32 bit integer in memory.
     * @param[in,out] p is the pointer to the 32 bit variable to increment.
     * @pre p != NULL. */
    static inline void Increment (volatile int32 *p );

    /**
     * @brief Atomically increment a 16 bit integer in memory.
     * @param[in,out] p is the pointer to the 16 bit variable to increment.
     * @pre p != NULL.
     */
    static inline void Increment (volatile int16 *p);

    /**
     * @brief Atomically increment a 8 bit integer in memory.
     * @param[in,out] p is the pointer to the 8 bit variable to increment.
     * @pre p != NULL.
     */
    static inline void Increment (volatile int8 *p);

    /**
     * @brief Atomically decrement a 32 bit integer in memory.
     * @param[in,out] p is the pointer to the 32 bit variable to decrement.
     * @pre p != NULL.
     */
    static inline void Decrement (volatile int32 *p);

    /**
     * @brief Atomically decrement a 16 bit integer in memory.
     * @param[in,out] p is the pointer to the 16 bit variable to decrement.
     * @pre p != NULL.
     */
    static inline void Decrement (volatile int16 *p);

    /**
     * @brief Atomically decrement a 8 bit integer in memory.
     * @param[in,out] p is the pointer to the 8 bit variable to decrement.
     * @pre p != NULL.
     */
    static inline void Decrement (volatile int8 *p);

    /**
     * @brief Atomically exchange the contents of a variable with the specified memory location.
     * @param[out] p is the pointer to the 32 bit variable where will be stored the value of v atomically.
     * @param[in] v is the value to store in the variable pointed by p.
     */
    static inline int32 Exchange (volatile int32 *p, int32 v);

    /**
     * @brief Test and set a 32 bit memory location in a thread safe way.
     * @param[in,out] p is the pointer to the 32 bit variable which must be tested.
     * @return true if *p = 0 and set *p = 1. If *p != 0 return false.
     * @pre p != NULL.
     */
    static inline bool TestAndSet(volatile int32 *p);

    /**
     * @brief Test and set a 16 bit memory location in a thread safe way.
     * @param[in,out] p is the pointer to the 16 bit variable which must be tested.
     * @return true if *p = 0 and set *p = 1. If *p != 0 return false.
     * @pre p != NULL.
     */
    static inline bool TestAndSet(volatile int16 *p);

    /**
     * @brief Test and set a 8 bit memory location in a thread safe way.
     * @param[in,out] p is the pointer to the 8 bit variable which must be tested.
     * @return true if *p = 0 and set *p = 1. If *p != 0 return false.
     * @pre p != NULL.
     */
    static inline bool TestAndSet(volatile int8 *p);

    /**
     * @brief Atomic addition.
     * @param[in,out] p is the pointer to the variable which must be incremented.
     * @param[in] value is the value to add to *p.
     * @pre p != NULL.
     */
    static inline void Add (volatile int32 *p, int32 value);

    /**
     * @brief Atomic subtraction.
     * @param[in,out] p is the pointer to the variable which must be decremented.
     * @param value is the value to subtract to *p.
     * @pre p != NULL.
     */
    static inline void Sub (volatile int32 *p, int32 value);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
void Atomic::Increment(volatile int32 *p) {
    AtomicA::Increment32(p);
}

void Atomic::Increment(volatile int16 *p) {
    AtomicA::Increment16(p);
}

void Atomic::Increment(volatile int8 *p) {
    AtomicA::Increment8(p);
}

void Atomic::Decrement(volatile int32 *p) {
    AtomicA::Decrement32(p);
}

void Atomic::Decrement(volatile int16 *p) {
    AtomicA::Decrement16(p);
}

void Atomic::Decrement(volatile int8 *p) {
    AtomicA::Decrement8(p);
}

int32 Atomic::Exchange(volatile int32 *p,
                       int32 v) {
    return AtomicA::Exchange32(p, v);
}

bool Atomic::TestAndSet(volatile int32 *p) {
    return AtomicA::TestAndSet32(p);
}

bool Atomic::TestAndSet(volatile int16 *p) {
    return AtomicA::TestAndSet16(p);
}

bool Atomic::TestAndSet(volatile int8 *p) {
    return AtomicA::TestAndSet8(p);
}

void Atomic::Add(volatile int32 *p,
                 int32 value) {
    AtomicA::Add32(p, value);
}

void Atomic::Sub(volatile int32 *p,
                 int32 value) {
    AtomicA::Sub32(p, value);
}

#endif /* ATOMIC_H_ */

