/**
 * @file Endianity.h
 * @brief Header file for class Endianity
 * @date 05/giu/2015
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
 *
 * @details This header file contains the declaration of the class Endianity
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

#ifndef ENDIANITY_H_
#define 		ENDIANITY_H_

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

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
/**
 * @brief EndianitySwaps the 4 bytes in a 32 bit number.
 * @param[in][out] x the number to be swapped
 */
static inline void EndianitySwap32(volatile void * x) {

    unsigned long *xx = (unsigned long *) x;
    *xx = _byteswap_ulong(*xx);
    /*__asm {
     mov ebx,x
     mov eax,DWORD PTR [ebx]
     bswap eax
     mov DWORD PTR [ebx],eax
     }*/
}

/**
 * @brief EndianitySwaps the 4 bytes in a 32 bit number for all the elements
 * of a vector
 * @param[in][out] x the number to be swapped
 * @param[in] sizer the number of elements in the vector
 */
static inline void EndianitySwap32(volatile void *x,
                                   uint32 sizer) {

    unsigned long *xx = (unsigned long *) x;
    for (int i = 0; i < sizer; i++) {
        xx[i] = _byteswap_ulong(xx[i]);
    }

    /*

     __asm {
     mov ecx,sizer
     test ecx,ecx
     je SHORT $swap_32_mul_2
     mov ebx,x
     $swap_32_mul_1:
     mov eax,DWORD PTR [ebx]
     bswap eax
     mov DWORD PTR [ebx],eax
     add ebx,4
     dec ecx
     jne SHORT $swap_32_mul_1
     $swap_32_mul_2:
     }*/
}

/**
 * @brief EndianitySwaps the 4 bytes while copying a vector of 32 bit numbers
 * @param[out] dest the destination vector (must be allocated in memory)
 * @param[in] src the source vector
 * @param[in] sizer the number of elements in the vector
 */
static inline void EndianityMemCopySwap32(volatile void *dest,
                                          volatile const void *src,
                                          uint32 sizer) {

    unsigned long *srcCopy = (unsigned long *) src;
    unsigned long *destCopy = (unsigned long *) dest;

    for (int i = 0; i < sizer; i++) {
        destCopy[i] = _byteswap_ulong(srcCopy[i]);
    }
    /*__asm {
     mov ecx,sizer
     test ecx,ecx
     je SHORT $swap_32_copy_mul_2
     mov ebx,src
     mov edx,dest
     $swap_32_copy_mul_1:
     mov eax,DWORD PTR [ebx]
     add ebx,4
     bswap eax
     mov DWORD PTR [edx],eax
     add edx,4
     dec ecx
     jne SHORT $swap_32_copy_mul_1
     $swap_32_copy_mul_2:
     }*/
}

/**
 * @brief EndianitySwaps the 2 bytes in a 16 bit number.
 * @param[in][out] x the number to be swapped
 */
static inline void EndianitySwap16(volatile void *x) {
    unsigned short *xx = (unsigned short *) x;
    *xx = _byteswap_ushort(*xx);
    /*__asm {
     mov ebx,x
     mov cx,WORD PTR [ebx]
     xchg cl, ch
     mov WORD PTR [ebx], cx
     }*/
}

/**
 * @brief EndianitySwaps the 2 bytes in a 16 bit number for all the elements
 * of a vector
 * @param[in][out] x the number to be swapped
 * @param[in] sizer the number of elements in the vector
 */
static inline void EndianitySwap16(volatile void *x,
                                   uint32 sizer) {

    unsigned short *xx = (unsigned short *) x;
    for (int i = 0; i < sizer; i++) {
        xx[i] = _byteswap_ushort(xx[i]);
    }
    /*__asm {
     mov ecx,sizer
     test ecx,ecx
     je SHORT $swap_16_mul_2
     mov ebx,x
     $swap_16_mul_1:
     mov ax,WORD PTR [ebx]
     xchg al, ah
     mov WORD PTR [ebx],ax
     add ebx,2
     dec ecx
     jne SHORT $swap_16_mul_1
     $swap_16_mul_2:
     }*/
}

/**
 * @brief EndianitySwaps the 2 bytes while copying a vector of 16 bit numbers
 * @param[out] dest the destination vector (must be allocated in memory)
 * @param[in] src the source vector
 * @param[in] sizer the number of elements in the vector
 */
static inline void EndianityMemCopySwap16(volatile void *dest,
                                          volatile const void *src,
                                          uint32 sizer) {

    unsigned short *srcCopy = (unsigned short *) src;
    unsigned short *destCopy = (unsigned short *) dest;

    for (int i = 0; i < sizer; i++) {
        destCopy[i] = _byteswap_ushort(srcCopy[i]);
    }

    /*__asm {
     mov ecx,sizer
     test ecx,ecx
     je SHORT $swap_16_copy_mul_2
     mov ebx,src
     mov edx,dest
     $swap_16_copy_mul_1:
     mov ax,WORD PTR [ebx]
     add ebx,2
     xchg al, ah
     mov WORD PTR [edx],ax
     add edx,2
     dec ecx
     jne SHORT $swap_16_copy_mul_1
     $swap_16_copy_mul_2:
     }*/
}

/**
 * @brief EndianitySwaps the 8 bytes in a 64 bit number.
 * Not optimised!
 * @param[in][out] x the number to be swapped
 */
static inline void EndianitySwap64(volatile void *x) {

    // unsigned __int64 _byteswap_uint64(unsigned __int64 value);
    uint32 *p = (uint32 *) x;
    EndianitySwap32(&p[0]);
    uint32 temp = p[0];
    EndianitySwap32(&p[1]);
    p[0] = p[1];
    p[1] = temp;
}

/**
 * @brief EndianitySwaps the 8 bytes while copying a vector of 64 bit numbers
 * Not optimised!
 * @param[out] dest the destination vector (must be allocated in memory)
 * @param[in] src the source vector
 * @param[in] sizer the number of elements in the vector
 */
static inline void EndianityMemCopySwap64(volatile void *dest,
                                          volatile const void *src,
                                          uint32 sizer) {
    int64 *s = (int64 *) src;
    int64 *d = (int64 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
        *d = *s;
        EndianitySwap64(d);
        d++;
        s++;
    }
}

/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianDouble(volatile double &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianFloat(volatile float &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianUInt64(volatile uint64 &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianUInt32(volatile uint32 &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianUInt16(volatile uint16 &x) {
    EndianitySwap16(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianInt64(volatile int64 &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianInt32(volatile int32 &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromBigEndianInt16(volatile int16 &x) {
    EndianitySwap16(&x);
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianDouble(volatile double &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianFloat(volatile float &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianUInt64(volatile uint64 &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianUInt32(volatile uint32 &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianUInt16(volatile uint16 &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianInt64(volatile int64 &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianInt32(volatile int32 &x) {
    x = x;
}
/**
 * @brief NOOP since the system is already little endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityFromLittleEndianInt16(volatile int16 &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianDouble(volatile double &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianFloat(volatile float &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianUInt64(volatile uint64 &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianUInt32(volatile uint32 &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianUInt16(volatile uint16 &x) {
    EndianitySwap16(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianInt64(volatile int64 &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianInt32(volatile int32 &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToBigEndianInt16(volatile int16 &x) {
    EndianitySwap16(&x);
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianDouble(volatile double &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianFloat(volatile float &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianUInt64(volatile uint64 &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianUInt32(volatile uint32 &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianUInt16(volatile uint16 &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianInt64(volatile int64 &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianInt32(volatile int32 &x) {
    x = x;
}
/**
 * @brief Converts a number from little endian to big endian
 * @param[in][out] x the number to convert
 */
static inline void EndianityToLittleEndianInt16(volatile int16 &x) {
    x = x;
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianDouble(double *dest,
                                                       const double *src,
                                                       uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianUInt64(uint64 *dest,
                                                       const uint64 *src,
                                                       uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianInt64(int64 *dest,
                                                      const int64 *src,
                                                      uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianFloat(float *dest,
                                                      const float *src,
                                                      uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianUInt32(uint32 *dest,
                                                       const uint32 *src,
                                                       uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianUInt16(uint16 *dest,
                                                       const uint16 *src,
                                                       uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianInt32(int32 *dest,
                                                      const int32 *src,
                                                      uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianInt16(int16 *dest,
                                                      const int16 *src,
                                                      uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianDouble(double *dest,
                                                          const double *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianUInt64(uint64 *dest,
                                                          const uint64 *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianInt64(int64 *dest,
                                                         const int64 *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianFloat(float *dest,
                                                         const float *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianUInt32(uint32 *dest,
                                                          const uint32 *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianUInt16(uint16 *dest,
                                                          const uint16 *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianInt32(int32 *dest,
                                                         const int32 *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianInt16(int16 *dest,
                                                         const int16 *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianDouble(double *dest,
                                                     const double *src,
                                                     uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianUInt64(uint64 *dest,
                                                     const uint64 *src,
                                                     uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianInt64(int64 *dest,
                                                    const int64 *src,
                                                    uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianFloat(float *dest,
                                                    const float *src,
                                                    uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianUInt32(uint32 *dest,
                                                     const uint32 *src,
                                                     uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianUInt16(uint16 *dest,
                                                     const uint16 *src,
                                                     uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianInt32(int32 *dest,
                                                    const int32 *src,
                                                    uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/**
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToBigEndianInt16(int16 *dest,
                                                    const int16 *src,
                                                    uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianDouble(double *dest,
                                                        const double *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianUInt64(uint64 *dest,
                                                        const uint64 *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianInt64(int64 *dest,
                                                       const int64 *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianFloat(float *dest,
                                                       const float *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianUInt32(uint32 *dest,
                                                        const uint32 *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianUInt16(uint16 *dest,
                                                        const uint16 *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianInt32(int32 *dest,
                                                       const int32 *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/**
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param[out] dest the destination
 * @param[in] src the source
 * @param[in] size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianInt16(int16 *dest,
                                                       const int16 *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ENDIANITY_H_ */

