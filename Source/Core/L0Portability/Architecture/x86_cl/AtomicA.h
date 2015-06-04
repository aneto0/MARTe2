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
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
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
inline void AtomicIncrement32(volatile int32 *p) {
__asm {
    mov ebx,p
    lock inc DWORD PTR[ebx]
}
}

/** @brief Atomically increment a 16 bit integer in memory.
  * @param p is the 16 bits variable which must be incremented.    */
inline void AtomicIncrement16(volatile int16 *p) {
__asm {
mov ebx,p
lock inc WORD PTR[ebx]
}
}

/** @brief Atomically increment a 8 bit integer in memory.
  * @param p is 8 bits variable which must be incremented.  */
inline void AtomicIncrement8(volatile int8 *p) {
__asm {
mov ebx,p
lock inc [ebx]
}
}

/** @brief Atomically decrement a 32 bit integer in memory.
  * @param p is the 32 bits variable to decrement. */
inline void AtomicDecrement32(volatile int32 *p) {
__asm {
mov ebx,p
lock dec DWORD PTR[ebx]
}
}

/** @brief Atomically decrement a 16 bit integer in memory.
  * @param p is the 16 bits variable to decrement. */
inline void AtomicDecrement16(volatile int16 *p) {
__asm {
mov ebx,p
lock dec WORD PTR[ebx]
}
}

/** @brief Atomically decrement a 8 bit integer in memory. 
  * @param p is the 8 bits variable to decrement. */
inline void AtomicDecrement8(volatile int8 *p) {
__asm {
mov ebx,p
lock dec [ebx]
}
}

/** @brief Atomically exchange the contents of a variable with the specified memory location.
  * @param p is the variable to change.
  * @param v is the variable to store. */
inline int32 AtomicExchange32(volatile int32 *p, int32 v) {
__asm {
mov ebx,p
mov eax,v
lock xchg DWORD PTR [ebx], eax
}
}

/** @brief Test and set a 32 bit memory location in a thread safe way.
  * @param p is the 32 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false. */
inline bool AtomicTestAndSet32(int32 volatile *p) {
int32 temp;
__asm {
mov ebx,p
mov eax,1
xchg DWORD PTR [ebx], eax
mov temp,eax
}
return (temp == 0);
}

/** @brief Test and set a 16 bit memory location in a thread safe way. 
  * @param p is the 16 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false.  */

inline bool AtomicTestAndSet16(int16 volatile *p) {
int16 temp;
__asm {
mov ebx,p
mov ax,1
xchg WORD PTR [ebx], ax
mov temp,ax
}
return (temp == 0);
}

/** @brief Test and set a 8 bit memory location in a thread safe way.
  * @param p is the 8 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false. */
inline bool AtomicTestAndSet8(int8 volatile *p) {
int8 temp;
__asm {
mov ebx,p
mov al,1
xchg [ebx], al
mov temp,al
}
return (temp == 0);
}

/**
 * @brief Atomic addition.
 * @param p is the variable.
 * @param value is the value to sum to p. */
inline void AtomicAdd32(volatile int32 *p, int32 value) {
__asm {
mov ebx,p
mov eax,value
lock add DWORD PTR[ebx], eax
}
}

/**
 * @brief Atomic subtraction.
 * @param p is the variable.
 * @param value is the value to subtract to p.
 */
inline void AtomicSub32(volatile int32 *p, int32 value) {
__asm {
mov ebx,p
mov eax,value
lock sub DWORD PTR[ebx], eax
}
}

#endif

