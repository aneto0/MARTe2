/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/

/**
 * @file
 * @brief Routines that are performed by the CPU without interruption.
 */
#ifndef ATOMIC_H
#define ATOMIC_H

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,AtomicA.h)

/** @brief A collector of functions that are executed atomically even on multiprocessor machines. 
 *
 * The target architecture is the one of the machine where the code is being executed. 
 * Most of the implementation is therefore delegated to the AtomicA.h implementation, 
 * which will be specific for each type of supported architecture. 
 *
 * These methods are particularly useful in the implementation of functionality that requires
 * protected basic operations between multiple asyncronus accesses like in a multithread system.
 * These methods are also used to implement thread's syncronization constructors like for example
 * fast polling mutex semaphores.
 */
class Atomic {

public:
    /** 
      * @brief Atomically increment a 32 bit integer in memory. 
      * @param p is the pointer to the 32 bit variable to increment. */
    static inline void Increment (volatile int32 *p ) {
        AtomicIncrement32(p);
    }

    /** @brief Atomically increment a 16 bit integer in memory. 
      * @param p is the pointer to the 16 bit variable to increment. */
    static inline void Increment (volatile int16 *p) {
        AtomicIncrement16(p);
    }

    /** @brief Atomically increment a 8 bit integer in memory.  
      * @param p is the pointer to the 8 bit variable to increment. */
    static inline void Increment (volatile int8 *p) {
        AtomicIncrement8(p);
    }

    /** @brief Atomically decrement a 32 bit integer in memory. 
      * @param p is the pointer to the 32 bit variable to decrement. */
    static inline void Decrement (volatile int32 *p) {
        AtomicDecrement32(p);
    }

    /** @brief Atomically decrement a 16 bit integer in memory. 
      * @param p is the pointer to the 16 bit variable to decrement. */
    static inline void Decrement (volatile int16 *p) {
        AtomicDecrement16(p);
    }

    /** @brief Atomically decrement a 8 bit integer in memory. 
      * @param p is the pointer to the 8 bit variable to decrement. */
    static inline void Decrement (volatile int8 *p) {
        AtomicDecrement8(p);
    }

    /** @brief Atomically exchange the contents of a variable with the specified memory location.
      * @param p is the pointer to the 32 bit variable where will be stored the value of v atomically. 
      * @param v is the value to store in the variable poited by p. */
    static inline int32 Exchange (volatile int32 *p, int32 v) {
        return AtomicExchange32(p, v);
    }

    /** @brief Test and set a 32 bit memory location in a thread safe way. 
      * @param p is the pointer to the 32 bit variable which must be tested.
      * @return true if *p = 0 and set *p = 1. If *p = 1 return false. */
    static inline bool TestAndSet(int32 volatile *p) {
        return AtomicTestAndSet32(p);
    }

    /** @brief Test and set a 16 bit memory location in a thread safe way. 
      * @param p is the pointer to the 16 bit variable which must be tested.
      * @return true if *p = 0 and set *p = 1. If *p = 1 return false. */
    static inline bool TestAndSet(int16 volatile *p) {
        return AtomicTestAndSet16(p);
    }

    /** @brief Test and set a 8 bit memory location in a thread safe way. 
      * @param p is the pointer to the 8 bit variable which must be tested.
      * @return true if *p = 0 and set *p = 1. If *p = 1 return false. */
    static inline bool TestAndSet(int8 volatile *p) {
        return AtomicTestAndSet8(p);
    }

    /**
     * @brief Atomic addition.
     * @param p is the pointer to the variable which must be incremented.
     * @param value is the value to add to *p. */
    static inline void Add (volatile int32 *p, int32 value) {
        AtomicAdd32(p, value);
    }

    /**
     * @brief Atomic subtraction.
     * @param p is the pointer to the variable which must be decremented.
     * @param value is the value to subtract to *p. */
    static inline void Sub (volatile int32 *p, int32 value) {
        AtomicSub32(p, value);
    }

};

#endif

