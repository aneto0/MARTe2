/**
 * @file Atomic.h
 * @brief Header file for module Atomic
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

 * @details This header file contains the declaration of the module Atomic
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMIC_H_
#define ATOMIC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(BareMetal,L1Portability,ARCHITECTURE,AtomicA.h)

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /*lint -e(766) the header file AtomicA.h is not used here. This file is just for declaration and documentation. */
    /*lint -e(762) these declarations are redundant. */
    namespace Atomic {

        /**
         * @brief Atomically increments a 32 bit integer in memory.
         * @param[in,out] p the pointer to the 32 bit variable to increment.
         * @pre p != NULL.
         */
        inline void Increment (volatile int32 *p );

        /**
         * @brief Atomically increments a 16 bit integer in memory.
         * @param[in,out] p the pointer to the 16 bit variable to increment.
         * @pre p != NULL.
         */
        inline void Increment (volatile int16 *p);

        /**
         * @brief Atomically increments a 8 bit integer in memory.
         * @param[in,out] p the pointer to the 8 bit variable to increment.
         * @pre p != NULL.
         */
        inline void Increment (volatile int8 *p);

        /**
         * @brief Atomically decrements a 32 bit integer in memory.
         * @param[in,out] p the pointer to the 32 bit variable to decrement.
         * @pre p != NULL.
         */
        inline void Decrement (volatile int32 *p);

        /**
         * @brief Atomically decrements a 16 bit integer in memory.
         * @param[in,out] p the pointer to the 16 bit variable to decrement.
         * @pre p != NULL.
         */
        inline void Decrement (volatile int16 *p);

        /**
         * @brief Atomically decrements a 8 bit integer in memory.
         * @param[in,out] p the pointer to the 8 bit variable to decrement.
         * @pre p != NULL.
         */
        inline void Decrement (volatile int8 *p);

        /**
         * @brief Atomically exchanges the contents of a variable with the specified memory location.
         * @param[out] p the pointer to the 32 bit variable where will be stored the value of v atomically.
         * @param[in] v the value to store in the variable pointed by p.
         * @return the exchanged value.
         */
        inline int32 Exchange (volatile int32 *p, int32 v);

        /**
         * @brief Tests and sets a 32 bit memory location in a thread safe way.
         * @param[in,out] p the pointer to the 32 bit variable which must be tested.
         * @return true if *p = 0 and set *p = 1. If *p != 0 return false.
         * @pre p != NULL.
         */
        inline bool TestAndSet(volatile int32 *p);

        /**
         * @brief Tests and sets a 16 bit memory location in a thread safe way.
         * @param[in,out] p the pointer to the 16 bit variable which must be tested.
         * @return true if *p = 0 and set *p = 1. If *p != 0 return false.
         * @pre p != NULL.
         */
        inline bool TestAndSet(volatile int16 *p);

        /**
         * @brief Tests and sets a 8 bit memory location in a thread safe way.
         * @param[in,out] p the pointer to the 8 bit variable which must be tested.
         * @return true if *p = 0 and set *p = 1. If *p != 0 return false.
         * @pre p != NULL.
         */
        inline bool TestAndSet(volatile int8 *p);

        /**
         * @brief Atomically adds two 32 bit integers.
         * @param[in,out] p the pointer to the variable which must be incremented.
         * @param[in] value the value to add to *p.
         * @pre p != NULL.
         */
        inline void Add (volatile int32 *p, int32 value);

        /**
         * @brief Atomically subtracts two 32 bit integers.
         * @param[in,out] p the pointer to the variable which must be decremented.
         * @param[in] value the value to subtract to *p.
         * @pre p != NULL.
         */
        inline void Sub (volatile int32 *p, int32 value);

    }

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATOMIC_H_ */
