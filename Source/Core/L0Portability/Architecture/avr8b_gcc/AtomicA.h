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
#include <util/atomic.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class AtomicA {
public:

    static inline void Increment32(volatile int32 *p) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            (*p)++;
        }
    }

    static inline void Increment16(volatile int16 *p) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            (*p)++;
        }
    }

    static inline void Increment8(volatile int8 *p) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            (*p)++;
        }
    }

    static inline void Decrement32(volatile int32 *p) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            (*p)--;
        }
    }

    static inline void Decrement16(volatile int16 *p) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            (*p)--;
        }
    }

    static inline void Decrement8(volatile int8 *p) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            (*p)--;
        }
    }

    static inline int32 Exchange32(volatile int32 *p, int32 v) {
        int32 old = 0;
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            old = *p;
            *p = v;
        }
        return old;
    }

    /**
     * @brief Test and set a 32 bit memory location in a thread safe way.
     * @param[in,out] p is the 32 bit variable to test and set.
     * @return return true if p=0 and it sets p to one, else return false.
     */
    static inline bool TestAndSet32(volatile int32 *p) {
        bool ok = false;
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            if (*p == 0) {
                ok = true;
                *p = 1;
            }
        }
        return ok;
    }

    static inline bool TestAndSet16(volatile int16 *p) {
        volatile int32 *pp = reinterpret_cast<volatile int32 *>(p);
        return TestAndSet32(pp);
    }

    static inline bool TestAndSet8(volatile int8 *p) {
        volatile int32 *pp = reinterpret_cast<volatile int32 *>(p);
        return TestAndSet32(pp);
    }

    static inline void Add32(volatile int32 *p, int32 value) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            *p += value;
        }
    }

    static inline void Sub32(volatile int32 *p, int32 value) {
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            *p -= value;
        }

    }

};

#endif /* ATOMICA_H_ */

