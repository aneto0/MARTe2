/**
 * @file EndianityA.h
 * @brief Header file for module EndianityA
 * @date 17/06/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the module EndianityA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ENDIANITYA_H_
#define ENDIANITYA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <circle/util.h>


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace Endianity {

inline void Swap32(volatile void *x) {
    uint32* xu32 = (uint32*)x;
    *x = bswap32(*xu32);
}

inline void Swap32(volatile void *x,
                   uint32 sizer) {
    uint32* xu32 = (uint32*)x;
    while(sizer-- != 0) {
        xu32[sizer] = bswap32(xu32[sizer]);
    }
}

inline void MemCopySwap32(volatile void *dest,
                          volatile const void *src,
                          uint32 sizer) {
    uint32* destu32 = (uint32*)dest;
    uint32* srcu32 = (uint32*)src;
    while(sizer-- != 0) {
        destu32[sizer] = bswap32(srcu32[sizer]);
    }
}

inline void Swap16(volatile void *x) {
    uint16* xu16 = (uint16*)x;
    *xu16 = bswap16(*xu16);
}

inline void Swap16(volatile void *x,
                   uint32 sizer) {
    uint16* xu16 = (uint16*)x;
    while(sizer-- != 0) {
        xu16[sizer] = bswap16(xu16[sizer]);
    }
}

inline void MemCopySwap16(volatile void *dest,
                          volatile const void *src,
                          uint32 sizer) {
    uint16* destu16 = (uint16*)dest;
    uint16* srcu16 = (uint16*)src;

    while(sizer-- != 0) {
        destu16[sizer] = bswap16(srcu16[sizer]);
    }
}

inline void Swap64(volatile void *x) {
    uint32 *xPtr = (uint32 *)x;
    Swap32(&xPtr[0]);
    Swap32(&xPtr[1]);
    uint32 tempXchg = xPtr[0];
    xPtr[0] = xPtr[1];
    xPtr[1] = tempXchg;
}

inline void MemCopySwap64(volatile void *dest,
                          volatile const void *src,
                          uint32 sizer) {
    //We are going to treat everything as uint32 to avoid the temp exchange variable
    //which is in Swap64 shorthand and directly do the swap in the loop

    uint32* srcPtr = (uint32*)src;
    uint32* destPtr = (uint32*)dest;
    uint32 currentElement = 0u;
    sizer += sizer;

    while((sizer -= 2) != 0) {
        destPtr[currentElement] = bswap32(&srcPtr[currentElement + 1]);
        destPtr[currentElement + 1] = bswap32(&srcPtr[currentElement]);
        currentElement += 2;
    }
}

inline void FromBigEndian(volatile float64 &x) {
    Swap64(&x);
}

inline void FromBigEndian(volatile float32 &x) {
    Swap32(&x);
}

inline void FromBigEndian(volatile uint64 &x) {
    Swap64(&x);
}

inline void FromBigEndian(volatile uint32 &x) {
    Swap32(&x);
}

inline void FromBigEndian(volatile uint16 &x) {
    Swap16(&x);
}

inline void FromBigEndian(volatile int64 &x) {
    Swap64(&x);
}

inline void FromBigEndian(volatile int32 &x) {
    Swap32(&x);
}

inline void FromBigEndian(volatile int16 &x) {
    Swap16(&x);
}

inline void FromLittleEndian(volatile float64 &x) {
}

inline void FromLittleEndian(volatile float32 &x) {
}

inline void FromLittleEndian(volatile uint64 &x) {
}

inline void FromLittleEndian(volatile uint32 &x) {
}

inline void FromLittleEndian(volatile uint16 &x) {
}

inline void FromLittleEndian(volatile int64 &x) {
}

inline void FromLittleEndian(volatile int32 &x) {
}

inline void FromLittleEndian(volatile int16 &x) {
}

inline void ToBigEndian(volatile float64 &x) {
    Swap64(&x);
}

inline void ToBigEndian(volatile float32 &x) {
    Swap32(&x);
}

inline void ToBigEndian(volatile uint64 &x) {
    Swap64(&x);
}

inline void ToBigEndian(volatile uint32 &x) {
    Swap32(&x);
}

inline void ToBigEndian(volatile uint16 &x) {
    Swap16(&x);
}

inline void ToBigEndian(volatile int64 &x) {
    Swap64(&x);
}

inline void ToBigEndian(volatile int32 &x) {
    Swap32(&x);
}

inline void ToBigEndian(volatile int16 &x) {
    Swap16(&x);
}

inline void ToLittleEndian(volatile float64 &x) {
}

inline void ToLittleEndian(volatile float32 &x) {
}

inline void ToLittleEndian(volatile uint64 &x) {
}

inline void ToLittleEndian(volatile uint32 &x) {
}

inline void ToLittleEndian(volatile uint16 &x) {
}

inline void ToLittleEndian(volatile int64 &x) {
}

inline void ToLittleEndian(volatile int32 &x) {
}

inline void ToLittleEndian(volatile int16 &x) {
}

inline void MemCopyFromBigEndian(float64 *dest,
                                 const float64 *src,
                                 uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void MemCopyFromBigEndian(uint64 *dest,
                                 const uint64 *src,
                                 uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void MemCopyFromBigEndian(int64 *dest,
                                 const int64 *src,
                                 uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void MemCopyFromBigEndian(float32 *dest,
                                 const float32 *src,
                                 uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void MemCopyFromBigEndian(uint32 *dest,
                                 const uint32 *src,
                                 uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void MemCopyFromBigEndian(uint16 *dest,
                                 const uint16 *src,
                                 uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void MemCopyFromBigEndian(int32 *dest,
                                 const int32 *src,
                                 uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void MemCopyFromBigEndian(int16 *dest,
                                 const int16 *src,
                                 uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void MemCopyFromLittleEndian(float64 *dest,
                                    const float64 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(float64) * size);
}

inline void MemCopyFromLittleEndian(uint64 *dest,
                                    const uint64 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(uint64) * size);
}

inline void MemCopyFromLittleEndian(int64 *dest,
                                    const int64 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(int64) * size);
}

inline void MemCopyFromLittleEndian(float32 *dest,
                                    const float32 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(float32) * size);
}

inline void MemCopyFromLittleEndian(uint32 *dest,
                                    const uint32 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(uint32) * size);
}

inline void MemCopyFromLittleEndian(uint16 *dest,
                                    const uint16 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(uint16) * size);
}

inline void MemCopyFromLittleEndian(int32 *dest,
                                    const int32 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(int32) * size);
}

inline void MemCopyFromLittleEndian(int16 *dest,
                                    const int16 *src,
                                    uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(int16) * size);
}

inline void MemCopyToBigEndian(float64 *dest,
                               const float64 *src,
                               uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void MemCopyToBigEndian(uint64 *dest,
                               const uint64 *src,
                               uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void MemCopyToBigEndian(int64 *dest,
                               const int64 *src,
                               uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void MemCopyToBigEndian(float32 *dest,
                               const float32 *src,
                               uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void MemCopyToBigEndian(uint32 *dest,
                               const uint32 *src,
                               uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void MemCopyToBigEndian(uint16 *dest,
                               const uint16 *src,
                               uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void MemCopyToBigEndian(int32 *dest,
                               const int32 *src,
                               uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void MemCopyToBigEndian(int16 *dest,
                               const int16 *src,
                               uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void MemCopyToLittleEndian(float64 *dest,
                                  const float64 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(float64) * size);
}

inline void MemCopyToLittleEndian(uint64 *dest,
                                  const uint64 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(uint64) * size);
}

inline void MemCopyToLittleEndian(int64 *dest,
                                  const int64 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(int64) * size);
}

inline void MemCopyToLittleEndian(float32 *dest,
                                  const float32 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(float32) * size);
}

inline void MemCopyToLittleEndian(uint32 *dest,
                                  const uint32 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(uint32) * size);
}

inline void MemCopyToLittleEndian(uint16 *dest,
                                  const uint16 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(uint16) * size);
}

inline void MemCopyToLittleEndian(int32 *dest,
                                  const int32 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(int32) * size);
}

inline void MemCopyToLittleEndian(int16 *dest,
                                  const int16 *src,
                                  uint32 size) {
    memcpy((void*)dest, (void*)src, sizeof(int16) * size);
}

}
}
#endif /* ENDIANITYA_H_ */

