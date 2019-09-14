/**
 * @file AtomicA.h
 * @brief Header file for module AtomicA
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

 * @details This header file contains the declaration of the module AtomicA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMICA_H_
#define ATOMICA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <windows.h>
#include <intrin.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Atomic {

inline int64 Increment(volatile int64 *p) {
    volatile LONGLONG* pp = (volatile LONGLONG *) p;
    return InterlockedIncrement64(pp);
}

inline int32 Increment(volatile int32 *p) {
    volatile long* pp = (volatile long *) p;
    return _InterlockedIncrement(pp);
}

inline int16 Increment(volatile int16 *p) {
    volatile short* pp = (volatile short*) p;
    return _InterlockedIncrement16(pp);
}

inline void Increment(volatile int8 *p) {
    //There is not _InterlockedIncrement8 function.
    volatile char8 *pp = (volatile char8*) p;
    _InterlockedExchangeAdd8(pp, 1);
}

inline int64 Decrement(volatile int64 *p) {
    volatile LONGLONG* pp = (volatile LONGLONG *) p;
    return InterlockedDecrement64(pp);
}

inline int32 Decrement(volatile int32 *p) {
    volatile long* pp = (volatile long *) p;
    return _InterlockedDecrement(pp);
}

inline int16 Decrement(volatile int16 *p) {
    volatile short* pp = (volatile short *) p;
    return _InterlockedDecrement16(pp);
}

inline void Decrement(volatile int8 *p) {
    volatile char8 *pp = (volatile char8 *) p;
    _InterlockedExchangeAdd8(pp, -1);
}

inline int32 Exchange(volatile int32 *p,int32 v) {
    volatile long* pp = (volatile long *) p;
    return _InterlockedExchange(pp, v);
}

inline bool TestAndSet(volatile int32 *p) {
    volatile long* pp = (volatile long *) p;
    //returns the initial value of *pp
    return _InterlockedCompareExchange(pp, 1, 0) == 0;
}

inline bool TestAndSet(volatile int16 *p) {
    volatile short* pp = (volatile short *) p;
    //returns the initial value of *pp
    return _InterlockedCompareExchange16(pp, 1, 0) == 0;
}

inline bool TestAndSet(volatile int8 *p) {
    volatile char8 *pp = (volatile char8*) p;

    //returns the initial value of *pp
    return _InterlockedCompareExchange8(pp, 1, 0) == 0;
}

inline int32 Add(volatile int32 *p,int32 value) {
    volatile long* pp = (volatile long *) p;
    return InterlockedAdd(pp, value);
}

inline int64 Add(volatile int64 *p,int64 value) {
    volatile LONG64* pp = (volatile LONG64 *) p;
    return InterlockedAdd64(pp, value);
}

inline int32 Sub(volatile int32 *p,int32 value) {
    volatile long* pp = (volatile long *) p;
    return InterlockedAdd(pp, -value);
}

inline int64 Sub(volatile int64 *p,int64 value) {
    volatile LONG64* pp = (volatile LONG64 *) p;
    return InterlockedAdd64(pp, -value);
}

}
}
#endif /* ATOMICA_H_ */
