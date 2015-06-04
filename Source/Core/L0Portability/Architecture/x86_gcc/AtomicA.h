/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they 
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
 * See the Licence for the specific language governing 
 permissions and limitations under the Licence. 
 *
 * $Id: Atomic.h 75 2012-11-07 09:42:41Z aneto $
 *
 **/

/**
 * @file
 * @brief Routines that are performed by the CPU without interruption.
 *
 * These ones are slower than those in SPAtomic.h, but are granted
 * to work on multiprocessor machines.
 *
 * It's a collector of functions that are executed atomically even on multiprocessor machines. */

#ifndef ATOMIC_P_H
#define ATOMIC_P_H

 
  
/** @brief Atomically increment a 32 bit integer in memory.
  * @param p is the 32 bit variable which must be incremented. */
static inline void AtomicIncrement32(volatile int32 *p) {
    asm volatile(
            "lock incl (%0)\n"
            : : "r" (p)
    );
}

/** @brief Atomically increment a 16 bit integer in memory.
  * @param p is the 16 bits variable which must be incremented.    */
static inline void AtomicIncrement16(volatile int16 *p) {
    asm volatile(
            "lock incw (%0)\n"
            : : "r" (p)
    );
}

/** @brief Atomically increment a 8 bit integer in memory.
  * @param p is 8 bits variable which must be incremented.  */
static inline void AtomicIncrement8(volatile int8 *p) {
    asm volatile(
            "lock incb (%0)\n"
            : : "r" (p)
    );
}

/** @brief Atomically decrement a 32 bit integer in memory.
  * @param p is the 32 bits variable to decrement. */
static inline void AtomicDecrement32(volatile int32 *p) {
    asm volatile(
            "lock decl (%0)\n"
            : : "r" (p)
    );
}

/** @brief Atomically decrement a 16 bit integer in memory.
  * @param p is the 16 bits variable to decrement. */
static inline void AtomicDecrement16(volatile int16 *p) {
    asm volatile(
            "lock decw (%0)\n"
            : : "r" (p)
    );
}

/** @brief Atomically decrement a 8 bit integer in memory. 
  * @param p is the 8 bits variable to decrement. */
static inline void AtomicDecrement8(volatile int8 *p) {
    asm volatile(
            "lock decb (%0)\n"
            : : "r" (p)
    );
}

/** @brief Atomically exchange the contents of a variable with the specified memory location.
  * @param p is the variable to change.
  * @param v is the variable to store. */
static inline int32 AtomicExchange32(volatile int32 *p, int32 v) {
    asm volatile(
            "lock xchg (%1), %0"
            :"=r" (v) : "r" (p), "0" (v)
    );
    return v;
}

/** @brief Test and set a 32 bit memory location in a thread safe way.
  * @param p is the 32 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false. */
static inline bool AtomicTestAndSet32(int32 volatile *p) {
    register int32 out = 1;
    asm volatile (
            "lock xchg (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
}

/** @brief Test and set a 16 bit memory location in a thread safe way. 
  * @param p is the 16 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false.  */
static inline bool AtomicTestAndSet16(int16 volatile *p) {
    register int16 out = 1;
    asm volatile (
            "lock xchgw (%2),%1"
            : "=r" (out) : "0" (out), "r" (p)
    );
    return (out == 0);
}

/** @brief Test and set a 8 bit memory location in a thread safe way.
  * @param p is the 8 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false. */
static inline bool AtomicTestAndSet8(int8 volatile *p) {
    register int8 out = 1;
    asm volatile (
            "lock xchgb (%2),%1"
            : "=q" (out) : "0" (out), "q" (p)
    );
    return (out == 0);
}

/**
 * @brief Atomic addition.
 * @param p is the variable.
 * @param value is the value to sum to p. */
static inline void AtomicAdd32(volatile int32 *p, int32 value) {
    asm volatile (
            "lock addl %1, (%0)"
            : /* output */
            :"r" (p), "ir" (value) /* input */
    );
}

/**
 * @brief Atomic subtraction.
 * @param p is the variable.
 * @param value is the value to subtract to p.
 */
static inline void AtomicSub32(volatile int32 *p, int32 value) {
    asm volatile (
            "lock subl %1, (%0)"
            : /* output */
            :"r" (p), "ir" (value) /* input */
    );
}

#endif

