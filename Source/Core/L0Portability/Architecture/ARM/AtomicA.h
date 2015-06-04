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

 /* 
  static inline int
  __attribute__ ((__unused__))
  __exchange_and_add (volatile int* __mem, int __val)
  {
   int __tmp, __tmp2, __result;
 #ifdef __thumb__
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0 \n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, %4 \n\t"
 	"swp     %2, %1, [%3] \n\t"
         "cmp     %0, %2 \n\t"
         "swpne   %1, %2, [%3] \n\t"
         "bne     0b \n\t"
 	"ldr     %1, 1f \n\t"
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "=&l"(__result), "+r"(__tmp), "+r"(__tmp2) 
 	: "r" (__mem), "r"(__val) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, %4 \n\t"
 	"swp     %2, %1, [%3] \n\t"
 	"cmp     %0, %2 \n\t"
 	"swpne   %1, %2, [%3] \n\t"
 	"bne     0b \n\t"
 	""
 	: "+r"(__result), "+r"(__tmp), "+r"(__tmp2) 
 	: "r" (__mem), "r"(__val) 
 	: "cc", "memory");
 #endif
    return __result;
  }
  
  
  typedef int int;
  

  static inline int
  __attribute__ ((__unused__))
  __exchange_and_add (volatile int* __mem, int __val)
  {
   int __result = *__mem;
 
   *__mem = __result + __val;
    return __result;
  }
  
static inline void
  __attribute__ ((__unused__))
  __atomic_add (volatile int *__mem, int __val)
  {
   int __tmp, __tmp2, __tmp3;
 #ifdef __thumb__
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0\n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, %4 \n\t"
         "swp     %2, %1, [%3] \n\t"
         "cmp     %0, %2 \n\t"
         "swpne   %1, %2,[%3] \n\t"
         "bne     0b \n\t"
 	"ldr     %1, 1f \n\t"
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "=&l"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (__mem), "r"(__val) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, %4 \n\t"
 	"swp     %2, %1, [%3] \n\t"
 	"cmp     %0, %2 \n\t"
 	"swpne   %1, %2, [%3] \n\t"
 	"bne     0b \n\t"
 	""
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (__mem), "r"(__val) 
 	: "cc", "memory");
 #endif
 }
 
 static inline long
 __attribute__ ((__unused__))
 __always_swap (volatile long *__p, long __newval)
 {
   long __result;
 #ifdef __thumb__
   long __tmp;
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0 \n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32\n"
 	"0:\t"
 	"swp     %0, %3, [%2] \n\t"
 	"ldr     %1, 1f \n\t"
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "=&l"(__result), "+r"(__tmp)
 	: "r"(__p), "r"(__newval)
 	: "memory");
 #else
   __asm__ __volatile__ (
 	"\n\t"
 	"swp     %0, %2, [%1] \n\t"
 	""
 	: "+r"(__result)
 	: "r"(__p), "r"(__newval)
 	: "memory");
 #endif
   return __result;
 }
 
 static inline int
 __attribute__ ((__unused__))
 __test_and_set (volatile long *__p, long __newval)
 {
   int __result;
   long __tmp;
 #ifdef __thumb__
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0 \n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"
 	"ldr     %0, [%2] \n\t"
         "cmp     %0, #0 \n\t"
         "bne     1f \n\t"
         "swp     %1, %3, [%2] \n\t"
         "cmp     %0, %1 \n\t"
         "swpne   %0, %1, [%2]\n\t"
         "bne     0b \n"
 	"1:\t"
 	"ldr     %1, 2f \n\t"
 	"bx      %1 \n"
 	"2:\t"
 	".word   3f \n\t"
 	".code 16 \n"
 	"3:"
 	: "=&l"(__result), "+r" (__tmp) 
 	: "r"(__p), "r"(__newval) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%2] \n\t"
 	"cmp     %0, #0 \n\t"
 	"bne     1f \n\t"
 	"swp     %1, %3, [%2] \n\t"
 	"cmp     %0, %1 \n\t"
 	"swpne   %0, %1, [%2] \n\t"
 	"bne     0b \n"
 	"1:\n\t"
 	""
 	: "+r"(__result), "+r" (__tmp) 
 	: "r"(__p), "r"(__newval) 
 	: "cc", "memory");
 #endif
   return __result;
  }
*/


 
  
/** @brief Atomically increment a 32 bit integer in memory.
  * @param p is the 32 bit variable which must be incremented. */
static inline void AtomicIncrement32(volatile int32 *p) {

   int __tmp, __tmp2, __tmp3;//registers

 #ifdef __thumb__
   /* Since this function is inlined, we can't be sure of the alignment.  */
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0\n"         //word alignment
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"              //loop label
 	"ldr     %0, [%3] \n\t"          //load the variable in tmp
 	"add     %1, %0, #1 \n\t"        //add 1 to tmp and put it in tmp2
         "swp     %2, %1, [%3] \n\t"     //put temp3=variable and then variable=tmp2
         "cmp     %0, %2 \n\t"           //if tmp and tmp3 are not equal this is not atomic
         "swpne   %1, %2,[%3] \n\t"      //reput variable=tmp3 and tmp2=variable in this case
         "bne     0b \n\t"               //re try to increment in this case.
 	"ldr     %1, 1f \n\t"            //finish.
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, #1 \n\t"
 	"swp     %2, %1, [%3] \n\t"
 	"cmp     %0, %2 \n\t"
 	"swpne   %1, %2, [%3] \n\t"
 	"bne     0b \n\t"
 	""
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p) 
 	: "cc", "memory");
 #endif
   
}

/** @brief Atomically increment a 16 bit integer in memory.
  * @param p is the 16 bits variable which must be incremented.    */
static inline void AtomicIncrement16(volatile int16 *p) {
 	void* ptr=(void*)p;
	volatile int32* pp=(int32*)ptr;
	AtomicIncrement32(pp);
}

/** @brief Atomically increment a 8 bit integer in memory.
  * @param p is 8 bits variable which must be incremented.  */
static inline void AtomicIncrement8(volatile int8 *p) {
 	void* ptr=(void*)p;
	volatile int32* pp=(int32*)ptr;
	AtomicIncrement32(pp); 
}

/** @brief Atomically decrement a 32 bit integer in memory.
  * @param p is the 32 bits variable to decrement. */
static inline void AtomicDecrement32(volatile int32 *p) {
 
 int __tmp, __tmp2, __tmp3;//registers

 #ifdef __thumb__
   /* Since this function is inlined, we can't be sure of the alignment.  */
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0\n"         //word alignment
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"              //loop label
 	"ldrl    %0, [%3] \n\t"          //load the variable in tmp
 	"sub     %1, %0, #1 \n\t"        //sub 1 to tmp and put it in tmp2
         "swp     %2, %1, [%3] \n\t"     //put temp3=variable and then variable=tmp2
         "cmp     %0, %2 \n\t"           //if tmp and tmp3 are not equal this is not atomic
         "swpne   %1, %2,[%3] \n\t"      //reput variable=tmp3 and tmp2=variable in this case
         "bne     0b \n\t"               //re try to increment in this case.
 	"ldr     %1, 1f \n\t"            //finish.
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"sub     %1, %0, #1 \n\t"
 	"swp     %2, %1, [%3] \n\t"
 	"cmp     %0, %2 \n\t"
 	"swpne   %1, %2, [%3] \n\t"
 	"bne     0b \n\t"
 	""
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p) 
 	: "cc", "memory");
 #endif
   
}

/** @brief Atomically decrement a 16 bit integer in memory.
  * @param p is the 16 bits variable to decrement. */
static inline void AtomicDecrement16(volatile int16 *p) {
 	void* ptr=(void*)p;
	volatile int32* pp=(int32*)ptr;
	AtomicDecrement32(pp);
}

/** @brief Atomically decrement a 8 bit integer in memory. 
  * @param p is the 8 bits variable to decrement. */
static inline void AtomicDecrement8(volatile int8 *p) {
	void* ptr=(void*)p;
	volatile int32* pp=(int32*)ptr;
   AtomicDecrement32(pp);
}

/** @brief Atomically exchange the contents of a variable with the specified memory location.
  * @param p is the variable to change.
  * @param v is the variable to store. */
static inline int32 AtomicExchange32(volatile int32 *p, int32 v) { 
   int32 __result;
 #ifdef __thumb__
   long __tmp;
   /* Since this function is inlined, we can't be sure of the alignment.  */
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0 \n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32\n"
 	"0:\t"
 	"swp     %0, %3, [%2] \n\t"  //put p in result and v in p 
 	"ldr     %1, 1f \n\t"        //finish
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "+r"(__result), "+r"(__tmp)
 	: "r"(p), "r"(v)
 	: "memory");
 #else
   __asm__ __volatile__ (
 	"\n\t"
 	"swp     %0, %2, [%1] \n\t"
 	""
 	: "+r"(__result)
 	: "r"(p), "r"(v)
 	: "memory");
 #endif
   return __result; 
}

/** @brief Test and set a 32 bit memory location in a thread safe way.
  * @param p is the 32 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false. */
static inline bool AtomicTestAndSet32(int32 volatile *p) {
    int __result;
   long __tmp;
	volatile int __newval=1;
 #ifdef __thumb__
   /* Since this function is inlined, we can't be sure of the alignment.  */
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0 \n"            //alignment
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"
 	"ldr     %0, [%2] \n\t"           //load p in result
         "cmp     %0, #0 \n\t"            //compare it with 0 
         "bne     1f \n\t"                //if it's different exit
         "swp     %1, %3, [%2] \n\t"      //it it's 0 set it.
         "cmp     %0, %1 \n\t"
         "swpne   %0, %1, [%2]\n\t"
         "bne     0b \n"
 	"1:\t"
 	"ldr     %1, 2f \n\t"
 	"bx      %1 \n"
 	"2:\t"
 	".word   3f \n\t"
 	".code 16 \n"
 	"3:"
 	: "+r"(__result), "+r" (__tmp) 
 	: "r"(p), "r"(__newval) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%2] \n\t"
 	"cmp     %0, #0 \n\t"
 	"bne     1f \n\t"
 	"swp     %1, %3, [%2] \n\t"
 	"cmp     %0, %1 \n\t"
 	"swpne   %0, %1, [%2] \n\t"
 	"bne     0b \n"
 	"1:\n\t"
 	""
 	: "+r"(__result), "+r" (__tmp) 
 	: "r"(p), "r"(__newval) 
 	: "cc", "memory");
 #endif
   return True;    
}

/** @brief Test and set a 16 bit memory location in a thread safe way. 
  * @param p is the 16 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false.  */
static inline bool AtomicTestAndSet16(int16 volatile *p) {
	void* ptr=(void*)p;
	volatile int32* pp=(int32*)ptr;
	return AtomicTestAndSet32(pp);
}

/** @brief Test and set a 8 bit memory location in a thread safe way.
  * @param p is the 8 bit variable to test and set.
  * @return return true if p=0 and it sets p to one, else return false. */
static inline bool AtomicTestAndSet8(int8 volatile *p) {
	void* ptr=(void*)p;
	volatile int32* pp=(int32*)ptr;
	return AtomicTestAndSet32(pp);
}

/**
 * @brief Atomic addition.
 * @param p is the variable.
 * @param value is the value to sum to p. */
static inline void AtomicAdd32(volatile int32 *p, int32 value) {
   int __tmp, __tmp2, __tmp3;
 #ifdef __thumb__
   /* Since this function is inlined, we can't be sure of the alignment.  */
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0\n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, %4 \n\t"
         "swp     %2, %1, [%3] \n\t"
         "cmp     %0, %2 \n\t"
         "swpne   %1, %2,[%3] \n\t"
         "bne     0b \n\t"
 	"ldr     %1, 1f \n\t"
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p), "r"(value) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"add     %1, %0, %4 \n\t"
 	"swp     %2, %1, [%3] \n\t"
 	"cmp     %0, %2 \n\t"
 	"swpne   %1, %2, [%3] \n\t"
 	"bne     0b \n\t"
 	""
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p), "r"(value) 
 	: "cc", "memory");
 #endif
}

/**
 * @brief Atomic subtraction.
 * @param p is the variable.
 * @param value is the value to subtract to p.
 */
static inline void AtomicSub32(volatile int32 *p, int32 value) {
   int __tmp, __tmp2, __tmp3;
 #ifdef __thumb__
   /* Since this function is inlined, we can't be sure of the alignment.  */
   __asm__ __volatile__ (
 	"ldr     %0, 4f \n\t"
 	"bx      %0 \n\t"
 	".align 0\n"
 	"4:\t"
 	".word   0f \n\t"
 	".code 32 \n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"sub     %1, %0, %4 \n\t"
         "swp     %2, %1, [%3] \n\t"
         "cmp     %0, %2 \n\t"
         "swpne   %1, %2,[%3] \n\t"
         "bne     0b \n\t"
 	"ldr     %1, 1f \n\t"
 	"bx      %1 \n"
 	"1:\t"
 	".word   2f \n\t"
 	".code 16 \n"
 	"2:\n"
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p), "r"(value) 
 	: "cc", "memory");
 #else
   __asm__ __volatile__ (
 	"\n"
 	"0:\t"
 	"ldr     %0, [%3] \n\t"
 	"sub     %1, %0, %4 \n\t"
 	"swp     %2, %1, [%3] \n\t"
 	"cmp     %0, %2 \n\t"
 	"swpne   %1, %2, [%3] \n\t"
 	"bne     0b \n\t"
 	""
 	: "+r"(__tmp), "+r"(__tmp2), "+r"(__tmp3) 
 	: "r" (p), "r"(value) 
 	: "cc", "memory");

#endif
}


#endif
