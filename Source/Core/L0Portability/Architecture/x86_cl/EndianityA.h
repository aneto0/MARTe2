/**
 * @file EndianityA.h
 * @brief Header file for class EndianityA
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

 * @details This header file contains the declaration of the class EndianityA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ENDIANITYA_H_
#define ENDIANITYA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <intrin.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

inline void Endianity::Swap32(volatile void * x) {

    unsigned long *xx = (unsigned long *) x;
    *xx = _byteswap_ulong(*xx);
}

inline void Endianity::Swap32(volatile void *x,
                              uint32 sizer) {

    unsigned long *xx = (unsigned long *) x;
    for (int i = 0; i < sizer; i++) {
        xx[i] = _byteswap_ulong(xx[i]);
    }
}

inline void Endianity::MemCopySwap32(volatile void *dest,
                                     volatile const void *src,
                                     uint32 sizer) {

    unsigned long *srcCopy = (unsigned long *) src;
    unsigned long *destCopy = (unsigned long *) dest;

    for (int i = 0; i < sizer; i++) {
        destCopy[i] = _byteswap_ulong(srcCopy[i]);
    }
}

inline void EndianitySwap16(volatile void *x) {
    unsigned short *xx = (unsigned short *) x;
    *xx = _byteswap_ushort(*xx);
}

inline void Endianity::Swap16(volatile void *x,
                              uint32 sizer) {

    unsigned short *xx = (unsigned short *) x;
    for (int i = 0; i < sizer; i++) {
        xx[i] = _byteswap_ushort(xx[i]);
    }
}

inline void Endianity::MemCopySwap16(volatile void *dest,
                                     volatile const void *src,
                                     uint32 sizer) {

    unsigned short *srcCopy = (unsigned short *) src;
    unsigned short *destCopy = (unsigned short *) dest;

    for (int i = 0; i < sizer; i++) {
        destCopy[i] = _byteswap_ushort(srcCopy[i]);
    }
}

inline void Endianity::Swap64(volatile void *x) {

    // unsigned __int64 _byteswap_uint64(unsigned __int64 value);
    uint32 *p = (uint32 *) x;
    Swap32(&p[0]);
    uint32 temp = p[0];
    Swap32(&p[1]);
    p[0] = p[1];
    p[1] = temp;
}

inline void Endianity::MemCopySwap64(volatile void *dest,
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

inline void Endianity::FromBigEndian(volatile float64 &x) {
    Swap64(&x);
}

inline void Endianity::FromBigEndian(volatile float32 &x) {
    Swap32(&x);
}

inline void Endianity::FromBigEndian(volatile uint64 &x) {
    Swap64(&x);
}

inline void Endianity::FromBigEndian(volatile uint32 &x) {
    Swap32(&x);
}

inline void Endianity::FromBigEndian(volatile uint16 &x) {
    Swap16(&x);
}

inline void Endianity::FromBigEndian(volatile int64 &x) {
    Swap64(&x);
}

inline void Endianity::FromBigEndian(volatile int32 &x) {
    Swap32(&x);
}

inline void Endianity::FromBigEndian(volatile int16 &x) {
    Swap16(&x);
}

inline void Endianity::FromLittleEndian(volatile float64 &x) {
}

inline void Endianity::FromLittleEndian(volatile float32 &x) {
}

inline void Endianity::FromLittleEndian(volatile uint64 &x) {
}

inline void Endianity::FromLittleEndian(volatile uint32 &x) {
}

inline void Endianity::FromLittleEndian(volatile uint16 &x) {
}

inline void Endianity::FromLittleEndian(volatile int64 &x) {
}

inline void Endianity::FromLittleEndian(volatile int32 &x) {
}

inline void Endianity::FromLittleEndian(volatile int16 &x) {
}

inline void Endianity::ToBigEndian(volatile float64 &x) {
    Swap64(&x);
}

inline void Endianity::ToBigEndian(volatile float32 &x) {
    Swap32(&x);
}

inline void Endianity::ToBigEndian(volatile uint64 &x) {
    Swap64(&x);
}

inline void Endianity::ToBigEndian(volatile uint32 &x) {
    Swap32(&x);
}

inline void Endianity::ToBigEndian(volatile uint16 &x) {
    Swap16(&x);
}

inline void Endianity::ToBigEndian(volatile int64 &x) {
    Swap64(&x);
}

inline void Endianity::ToBigEndian(volatile int32 &x) {
    Swap32(&x);
}

inline void Endianity::ToBigEndian(volatile int16 &x) {
    Swap16(&x);
}

inline void Endianity::ToLittleEndian(volatile float64 &x) {
}

inline void Endianity::ToLittleEndian(volatile float32 &x) {
}

inline void Endianity::ToLittleEndian(volatile uint64 &x) {
}

inline void Endianity::ToLittleEndian(volatile uint32 &x) {
}

inline void Endianity::ToLittleEndian(volatile uint16 &x) {
}

inline void Endianity::ToLittleEndian(volatile int64 &x) {
}

inline void Endianity::ToLittleEndian(volatile int32 &x) {
}

inline void Endianity::ToLittleEndian(volatile int16 &x) {
}

inline void Endianity::MemCopyFromBigEndian(float64 *dest,
                                            const float64 *src,
                                            uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(uint64 *dest,
                                            const uint64 *src,
                                            uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(int64 *dest,
                                            const int64 *src,
                                            uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(float32 *dest,
                                            const float32 *src,
                                            uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(uint32 *dest,
                                            const uint32 *src,
                                            uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(uint16 *dest,
                                            const uint16 *src,
                                            uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(int32 *dest,
                                            const int32 *src,
                                            uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void Endianity::MemCopyFromBigEndian(int16 *dest,
                                            const int16 *src,
                                            uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void Endianity::MemCopyFromLittleEndian(float64 *dest,
                                               const float64 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(uint64 *dest,
                                               const uint64 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(int64 *dest,
                                               const int64 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(float32 *dest,
                                               const float32 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(uint32 *dest,
                                               const uint32 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(uint16 *dest,
                                               const uint16 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(int32 *dest,
                                               const int32 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyFromLittleEndian(int16 *dest,
                                               const int16 *src,
                                               uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToBigEndian(float64 *dest,
                                          const float64 *src,
                                          uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(uint64 *dest,
                                          const uint64 *src,
                                          uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(int64 *dest,
                                          const int64 *src,
                                          uint32 size) {
    MemCopySwap64(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(float32 *dest,
                                          const float32 *src,
                                          uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(uint32 *dest,
                                          const uint32 *src,
                                          uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(uint16 *dest,
                                          const uint16 *src,
                                          uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(int32 *dest,
                                          const int32 *src,
                                          uint32 size) {
    MemCopySwap32(dest, src, size);
}

inline void Endianity::MemCopyToBigEndian(int16 *dest,
                                          const int16 *src,
                                          uint32 size) {
    MemCopySwap16(dest, src, size);
}

inline void Endianity::MemCopyToLittleEndian(float64 *dest,
                                             const float64 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(uint64 *dest,
                                             const uint64 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(int64 *dest,
                                             const int64 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(float32 *dest,
                                             const float32 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(uint32 *dest,
                                             const uint32 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(uint16 *dest,
                                             const uint16 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(int32 *dest,
                                             const int32 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

inline void Endianity::MemCopyToLittleEndian(int16 *dest,
                                             const int16 *src,
                                             uint32 size) {
    for (uint32 i = 0; i < size; i++) {
        *dest = *src;
        src++;
        dest++;
    }
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ENDIANITYA_H_ */

