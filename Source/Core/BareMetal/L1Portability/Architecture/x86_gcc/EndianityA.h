/**
 * @file EndianityA.h
 * @brief Header file for module EndianityA
 * @date 17/06/2015
 * @author Giuseppe Ferro
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
    int32 *xx = (int32 *) x;
    register int32 temp = *xx;
    asm(
            "bswap %1"
            : "=r" (temp) : "0" (temp)
    );
    *xx = temp;
}

inline void Swap32(volatile void *x,
                   uint32 sizer) {
    register int32 *xx = (int32 *) x;
    for (uint32 i = 0; i < sizer; i++) {
        register int32 temp = *xx;
        asm(
                "bswap %1"
                : "=r" (temp) : "0" (temp)
        );
        *xx = temp;
        xx++;
    }
}

inline void MemCopySwap32(volatile void *dest,
                          volatile const void *src,
                          uint32 sizer) {
    register int32 *s = (int32 *) src;
    register int32 *d = (int32 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        register int32 temp = *s;
        asm(
                "bswap %1"
                : "=r" (temp) : "0" (temp)
        );
        *d = temp;
        d++;
        s++;
    }
}

inline void Swap16(volatile void *x) {
    asm(
            "movw (%0), %%dx\n"
            "xchgb %%dl, %%dh\n"
            "movw %%dx, (%0)"
            : : "r" (x) :"dx"
    );
}

inline void Swap16(volatile void *x,
                   uint32 sizer) {
    register int16 *xx = (int16 *) x;
    for (uint32 i = 0; i < sizer; i++) {
        asm(
                "movw (%0), %%dx\n"
                "xchgb %%dl, %%dh\n"
                "movw %%dx, (%0)"
                : : "r" (xx) :"dx"
        );
        xx++;
    }
}

inline void MemCopySwap16(volatile void *dest,
                          volatile const void *src,
                          uint32 sizer) {
    int16 *s = (int16 *) src;
    int16 *d = (int16 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        asm(
                "movw (%0), %%dx\n"
                "xchgb %%dl, %%dh\n"
                "movw %%dx, (%1)"
                : : "r" (s), "r" (d) :"dx"
        );
        s++;
        d++;
    }
}

inline void Swap64(volatile void *x) {
    uint32 *p = (uint32 *) x;
    Swap32(&p[0]);
    uint32 temp = p[0];
    Swap32(&p[1]);
    p[0] = p[1];
    p[1] = temp;
}

inline void MemCopySwap64(volatile void *dest,
                          volatile const void *src,
                          uint32 sizer) {
    int64 *s = (int64 *) src;
    int64 *d = (int64 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        *d = *s;
        Swap64(d);
        d++;
        s++;
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
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(uint64 *dest,
                                    const uint64 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(int64 *dest,
                                    const int64 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(float32 *dest,
                                    const float32 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(uint32 *dest,
                                    const uint32 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(uint16 *dest,
                                    const uint16 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(int32 *dest,
                                    const int32 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyFromLittleEndian(int16 *dest,
                                    const int16 *src,
                                    uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
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
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(uint64 *dest,
                                  const uint64 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(int64 *dest,
                                  const int64 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(float32 *dest,
                                  const float32 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(uint32 *dest,
                                  const uint32 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(uint16 *dest,
                                  const uint16 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(int32 *dest,
                                  const int32 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

inline void MemCopyToLittleEndian(int16 *dest,
                                  const int16 *src,
                                  uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        dest++;
        src++;
    }
}

}
}
#endif /* ENDIANITYA_H_ */

