/**
 * @file AtomicA.h
 * @brief Header file for class AtomicA
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

/**
 * @brief Platform dependent functions for atomic operations.
 * @details These functions define atomic operation optimizing as more as possible the performance.
 */
class AtomicA {
public:

    /**
     * @brief Atomic increment a 32 bit integer in memory.
     * @param[in,out] p is the 32 bit variable which must be incremented.
     */
    static inline void Increment32(volatile int32 *p) {

        int __tmp, __tmp2, __tmp3; //registers

#ifdef __thumb__
        /* Since this function is inlined, we can't be sure of the alignment.  */
        __asm__ __volatile__ (
                "ldr     %0, 4f \n\t"
                "bx      %0 \n\t"
                ".align 0\n"         //word alignment
                "4:\t"
                ".word   0f \n\t"
                ".code 16 \n"
                "0:\t"//loop label
                "ldr     %0, [%3] \n\t"//load the variable in tmp
                "add     %1, %0, #1 \n\t"//add 1 to tmp and put it in tmp2
                "ldr    %2, [%3] \n\t"
                "str    %1, [%3,#0]\n\t"
                //        "swp     %2, %1, [%3] \n\t"     //put temp3=variable and then variable=tmp2
                "cmp     %0, %2 \n\t"//if tmp and tmp3 are not equal this is not atomic
                "ldr    %1, [%3] \n\t"
                "str    %2, [%3,#0]\n\t"
                //       "swpne   %1, %2,[%3] \n\t"      //reput variable=tmp3 and tmp2=variable in this case
                "bne     0b \n\t"//re try to increment in this case.
                "ldr     %1, 1f \n\t"//finish.
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

    /**
     * @brief Atomic increment a 16 bit integer in memory.
     * @param[in,out] p is the 16 bits variable which must be incremented.
     */
    static inline void Increment16(volatile int16 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Increment32(pp);
    }

    /**
     * @brief Atomic increment a 8 bit integer in memory.
     * @param[in,out] p is 8 bits variable which must be incremented.
     */
    static inline void Increment8(volatile int8 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Increment32(pp);
    }

    /**
     * @brief Atomic decrement a 32 bit integer in memory.
     * @param[in,out] p is the 32 bits variable to decrement.
     */
    static inline void Decrement32(volatile int32 *p) {
        int __tmp, __tmp2, __tmp3;         //registers

#ifdef __thumb__
        /* Since this function is inlined, we can't be sure of the alignment.  */
        __asm__ __volatile__ (
                ".thumb    \n\t"
                "ldr     %0, 4f \n\t"
                "bx      %0 \n\t"
                ".align 0\n"         //word alignment
                "4:\t"
                ".word   0f \n\t"
                ".code 16 \n"
                "0:\t"//loop label
                "ldr    %0, [%3] \n\t"//load the variable in tmp
                "sub     %1, %0, #1 \n\t"//sub 1 to tmp and put it in tmp2
                "ldr    %2, [%3] \n\t"
                "str    %1, [%3,#0]\n\t"
                //    "swp     %2, %1, [%3] \n\t"     //put temp3=variable and then variable=tmp2
                "cmp     %0, %2 \n\t"//if tmp and tmp3 are not equal this is not atomic
                "ldr    %1, [%3] \n\t"
                "str    %2, [%3,#0]\n\t"
                //         "swpne   %1, %2,[%3] \n\t"      //reput variable=tmp3 and tmp2=variable in this case
                "bne     0b \n\t"//re try to increment in this case.
                "ldr     %1, 1f \n\t"//finish.
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

    /**
     * @brief Atomic decrement a 16 bit integer in memory.
     * @param[in,out] p is the 16 bits variable to decrement.
     */
    static inline void Decrement16(volatile int16 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Decrement32(pp);
    }

    /**
     * @brief Atomic decrement a 8 bit integer in memory.
     * @param[in,out] p is the 8 bits variable to decrement.
     */
    static inline void Decrement8(volatile int8 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        Decrement32(pp);
    }

    /**
     * @brief Atomic exchange the contents of a variable with the specified memory location.
     * @param[out] p is the variable to change.
     * @param[in] v is the variable to store.
     */
    static inline int32 Exchange32(volatile int32 *p, int32 v) {
        int32 __result;
#ifdef __thumb__
        long __tmp;
        /* Since this function is inlined, we can't be sure of the alignment.  */
        __asm__ __volatile__ (
                ".thumb    \n\t"
                "ldr     %0, 4f \n\t"
                "bx      %0 \n\t"
                ".align 0 \n"
                "4:\t"
                ".word   0f \n\t"
                ".code 16\n"
                "0:\t"
                "ldr    %0, [%0] \n\t"
                "str    %3, [%2,#0]\n\t"
                // "swp     %0, %3, [%2] \n\t"  //put p in result and v in p
                "ldr     %1, 1f \n\t"//finish
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
        return v;
    }

    /**
     * @brief Test and set a 32 bit memory location in a thread safe way.
     * @param[in,out] p is the 32 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false.
     */
    static inline bool TestAndSet32(volatile int32 *p) {
        int __result = 0;
        long __tmp = 0;
        volatile int __newval = 1;
#ifdef __thumb__
        /* Since this function is inlined, we can't be sure of the alignment.  */
        __asm__ __volatile__ (
                ".thumb   \n\t"
                "ldr     %0, 4f \n\t"
                "bx      %0 \n\t"
                ".align 0 \n"            //alignment
                "4:\t"
                ".word   0f \n\t"
                ".code 16 \n"
                "0:\t"
                "ldr     %0, [%2] \n\t"//load p in result
                "cmp     %0, #0 \n\t"//compare it with 0
                "bne     1f \n\t"//if it's different exit
                "ldr    %1, [%2] \n\t"
                "str    %3, [%2,#0]\n\t"
                //     "swp     %1, %3, [%2] \n\t"      //it it's 0 set it.
                "cmp     %0, %1 \n\t"
                "ldr    %0, [%2] \n\t"
                "str    %1, [%2,#0]\n\t"
                //        "swpne   %0, %1, [%2]\n\t"
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
        return true;
    }

    /**
     * @brief Test and set a 16 bit memory location in a thread safe way.
     * @param[in,out] p is the 16 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false.
     */
    static inline bool TestAndSet16(volatile int16 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        return TestAndSet32(pp);
    }

    /**
     * @brief Test and set a 8 bit memory location in a thread safe way.
     * @param[in,out] p is the 8 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false.
     */
    static inline bool TestAndSet8(volatile int8 *p) {
        void* ptr = (void*) p;
        volatile int32* pp = (int32*) ptr;
        return TestAndSet32(pp);
    }

    /**
     * @brief Atomic addition.
     * @param[in,out] p is the variable.
     * @param[in] value is the value to sum to p.
     */
    static inline void Add32(volatile int32 *p, int32 value) {
        int __tmp, __tmp2, __tmp3;
#ifdef __thumb__
        /* Since this function is inlined, we can't be sure of the alignment.  */
        __asm__ __volatile__ (
                ".thumb    \n\t"
                "ldr     %0, 4f \n\t"
                "bx      %0 \n\t"
                ".align 0\n"
                "4:\t"
                ".word   0f \n\t"
                ".code 16 \n"
                "0:\t"
                "ldr     %0, [%3] \n\t"
                "add     %1, %0, %4 \n\t"
                "ldr    %2, [%3] \n\t"
                "str    %1, [%3,#0]\n\t"
                //   "swp     %2, %1, [%3] \n\t"
                "cmp     %0, %2 \n\t"
                "ldr    %1, [%3] \n\t"
                "str    %2, [%3,#0]\n\t"
                //  "swpne   %1, %2,[%3] \n\t"
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
     * @param[in,out] p is the variable.
     * @param[in] value is the value to subtract to p.
     */
    static inline void Sub32(volatile int32 *p, int32 value) {
        int __tmp, __tmp2, __tmp3;
#ifdef __thumb__
        /* Since this function is inlined, we can't be sure of the alignment.  */
        __asm__ __volatile__ (
                ".thumb     \n\t"
                "ldr     %0, 4f \n\t"
                "bx      %0 \n\t"
                ".align 0\n"
                "4:\t"
                ".word   0f \n\t"
                ".code 16 \n"
                "0:\t"
                "ldr     %0, [%3] \n\t"
                "sub     %1, %0, %4 \n\t"
                "ldr    %2, [%3] \n\t"
                "str    %1, [%3,#0]\n\t"
                //         "swp     %2, %1, [%3] \n\t"
                "cmp     %0, %2 \n\t"
                "ldr    %1, [%3] \n\t"
                "str    %2, [%3,#0]\n\t"
                //"swpne   %1, %2,[%3] \n\t"
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

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATOMICA_H_ */

