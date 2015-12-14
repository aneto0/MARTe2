/**
 * @file EndianityA.h
 * @brief Header file for class EndianityA
 * @date 05/ago/2015
 * @author pc
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

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Endianity {

inline void EndianitySwap32(volatile void *x) {
    int32 *xx = (int32 *) x;
    int __reg1;

    asm(
            "ldr %0, [%1] \n\t "
            "rev %0, %0   \n\t "
            "str %0, [%1] \n"
            : "=&r" (__reg1) : "r" (xx)
            : "memory"
    );
}

inline void EndianitySwap32(volatile void *x, uint32 sizer) {
    int32 *xx = (int32 *) x;
    for (uint32 i = 0; i < sizer; i++) {

        void* xptr = (void *) xx;
        EndianitySwap32(xptr);

        xx++;
    }
}

inline void EndianityMemCopySwap32(volatile void *dest, volatile const void *src, uint32 sizer) {
    int32 *s = (int32 *) src;
    int32 *d = (int32 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        void* sPtr = (void *) s;
        EndianitySwap32(sPtr);
        *d++ = *s++;
    }
}

inline void EndianitySwap16(volatile void *x) {

    int16 *xx = (int16 *) x;
    int __reg1;

    asm(
            "ldr    %0, [%1] \n\t "
            "rev16  %0, %0   \n\t "
            "str    %0, [%1] \n"
            : "=&r" (__reg1) : "r" (xx)
            : "memory"
    );
}

inline void EndianitySwap16(volatile void *x, uint32 sizer) {
    int16 *xx = (int16 *) x;

    for (uint32 i = 0; i < sizer; i++) {
        void* xptr = (void *) xx;
        EndianitySwap16(xptr);
        xx++;
    }
}

inline void EndianityMemCopySwap16(volatile void *dest, volatile const void *src, uint32 sizer) {
    int16 *s = (int16 *) src;
    int16 *d = (int16 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        void* sPtr = (void *) s;
        EndianitySwap16(sPtr);
        *d++ = *s++;
    }
}

inline void EndianitySwap64(volatile void *x) {
    uint32 *p = (uint32 *) x;
    EndianitySwap32(&p[0]);
    uint32 temp = p[0];
    EndianitySwap32(&p[1]);
    p[0] = p[1];
    p[1] = temp;
}

inline void EndianityMemCopySwap64(volatile void *dest, volatile const void *src, uint32 sizer) {
    int64 *s = (int64 *) src;
    int64 *d = (int64 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        *d = *s;
        EndianitySwap64(d);
        d++;
        s++;
    }
}

inline void FromBigEndianDouble(volatile double &x) {
    EndianitySwap64(&x);
}

inline void FromBigEndianFloat(volatile float &x) {
    EndianitySwap32(&x);
}

inline void FromBigEndianUInt64(volatile uint64 &x) {
    EndianitySwap64(&x);
}

inline void FromBigEndianUInt32(volatile uint32 &x) {
    EndianitySwap32(&x);
}

inline void FromBigEndianUInt16(volatile uint16 &x) {
    EndianitySwap16(&x);
}

inline void FromBigEndianInt64(volatile int64 &x) {
    EndianitySwap64(&x);
}

inline void FromBigEndianInt32(volatile int32 &x) {
    EndianitySwap32(&x);
}

inline void FromBigEndianInt16(volatile int16 &x) {
    EndianitySwap16(&x);
}

inline void FromLittleEndianDouble(volatile double &x) {
}

inline void FromLittleEndianFloat(volatile float &x) {
}

inline void FromLittleEndianUInt64(volatile uint64 &x) {
}

inline void FromLittleEndianUInt32(volatile uint32 &x) {
}


inline void FromLittleEndianUInt16(volatile uint16 &x) {
}

inline void FromLittleEndianInt64(volatile int64 &x) {
}

inline void FromLittleEndianInt32(volatile int32 &x) {
}

inline void FromLittleEndianInt16(volatile int16 &x) {
}

inline void ToBigEndianDouble(volatile double &x) {
    EndianitySwap64(&x);
}

inline void ToBigEndianFloat(volatile float &x) {
    EndianitySwap32(&x);
}

inline void ToBigEndianUInt64(volatile uint64 &x) {
    EndianitySwap64(&x);
}

inline void ToBigEndianUInt32(volatile uint32 &x) {
    EndianitySwap32(&x);
}

inline void ToBigEndianUInt16(volatile uint16 &x) {
    EndianitySwap16(&x);
}

inline void ToBigEndianInt64(volatile int64 &x) {
    EndianitySwap64(&x);
}

inline void ToBigEndianInt32(volatile int32 &x) {
    EndianitySwap32(&x);
}

inline void ToBigEndianInt16(volatile int16 &x) {
    EndianitySwap16(&x);
}

inline void ToLittleEndianDouble(volatile double &x) {
}

inline void ToLittleEndianFloat(volatile float &x) {
}

inline void ToLittleEndianUInt64(volatile uint64 &x) {
}

inline void ToLittleEndianUInt32(volatile uint32 &x) {
}

inline void ToLittleEndianUInt16(volatile uint16 &x) {
}

inline void ToLittleEndianInt64(volatile int64 &x) {
}

inline void ToLittleEndianInt32(volatile int32 &x) {
}

inline void ToLittleEndianInt16(volatile int16 &x) {
}

inline void MemCopyFromBigEndianDouble(double *dest, const double *src, uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}

inline void MemCopyFromBigEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}

inline void MemCopyFromBigEndianInt64(int64 *dest, const int64 *src, uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}

inline void MemCopyFromBigEndianFloat(float *dest, const float *src, uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}

inline void MemCopyFromBigEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}

inline void MemCopyFromBigEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}

inline void MemCopyFromBigEndianInt32(int32 *dest, const int32 *src, uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}

inline void MemCopyFromBigEndianInt16(int16 *dest, const int16 *src, uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}

inline void MemCopyFromLittleEndianDouble(double *dest, const double *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianInt64(int64 *dest, const int64 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianFloat(float *dest, const float *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianInt32(int32 *dest, const int32 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyFromLittleEndianInt16(int16 *dest, const int16 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToBigEndianDouble(double *dest, const double *src, uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}

inline void MemCopyToBigEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}

inline void MemCopyToBigEndianInt64(int64 *dest, const int64 *src, uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}

inline void MemCopyToBigEndianFloat(float *dest, const float *src, uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}

inline void MemCopyToBigEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}

inline void MemCopyToBigEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}

inline void MemCopyToBigEndianInt32(int32 *dest, const int32 *src, uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}

inline void MemCopyToBigEndianInt16(int16 *dest, const int16 *src, uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}

inline void MemCopyToLittleEndianDouble(double *dest, const double *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianInt64(int64 *dest, const int64 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianFloat(float *dest, const float *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianInt32(int32 *dest, const int32 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

inline void MemCopyToLittleEndianInt16(int16 *dest, const int16 *src, uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ENDIANITYA_H_ */

