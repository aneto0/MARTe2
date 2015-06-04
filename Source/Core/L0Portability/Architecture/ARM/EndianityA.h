/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
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
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/

/** 
 * @file
 * @brief A NameSpace holder of all the Endianity conversion routines. 
 */
#ifndef ENDIANITY_A_H
#define ENDIANITY_A_H

#include "../../GeneralDefinitions.h"
/** 
 * @brief EndianitySwaps the 4 bytes in a 32 bit number.  
 * @param x the number to be swapped
 */
static inline void EndianitySwap32(volatile void *x) {
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

/** 
 * @brief EndianitySwaps the 4 bytes in a 32 bit number for all the elements
 * of a vector
 * @param x the number to be swapped
 * @param sizer the number of elements in the vector
 */
static inline void EndianitySwap32(volatile void *x, uint32 sizer) {
    int32 *xx = (int32 *) x;
    for (uint32 i = 0; i < sizer; i++) {

	void* xptr=(void *)xx;
	EndianitySwap32(xptr);

        xx++;
    }
}

/** 
 * @brief EndianitySwaps the 4 bytes while copying a vector of 32 bit numbers
 * @param dest the destination vector (must be allocated in memory)
 * @param src the source vector 
 * @param sizer the number of elements in the vector
 */
static inline void EndianityMemCopySwap32(volatile void *dest,
                                          volatile const void *src,
                                          uint32 sizer) {
    int32 *s = (int32 *) src;
    int32 *d = (int32 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
	void* sPtr=(void *)s;
	EndianitySwap32(sPtr);
	*d++=*s++;
    }
}

/** 
 * @brief EndianitySwaps the 2 bytes in a 16 bit number.  
 * @param x the number to be swapped
 */
static inline void EndianitySwap16(volatile void *x) {

	int16 *xx=(int16 *)x;
	int __reg1;

    asm(
            "ldr    %0, [%1] \n\t "
            "rev16  %0, %0   \n\t "
            "str    %0, [%1] \n"
            : "=&r" (__reg1) : "r" (xx)
	    : "memory"
    );
}

/** 
 * @brief EndianitySwaps the 2 bytes in a 16 bit number for all the elements
 * of a vector
 * @param x the number to be swapped
 * @param sizer the number of elements in the vector
 */
static inline void EndianitySwap16(volatile void *x, uint32 sizer) {
    int16 *xx = (int16 *) x;

    for (uint32 i = 0; i < sizer; i++) {
	void* xptr=(void *)xx;
        EndianitySwap16(xptr);
	xx++; 
    }
}

/** 
 * @brief EndianitySwaps the 2 bytes while copying a vector of 16 bit numbers
 * @param dest the destination vector (must be allocated in memory)
 * @param src the source vector 
 * @param sizer the number of elements in the vector
 */
static inline void EndianityMemCopySwap16(volatile void *dest,
                                          volatile const void *src,
                                          uint32 sizer) {
    int16 *s = (int16 *) src;
    int16 *d = (int16 *) dest;
    for (uint32 i = 0; i < sizer; i++) {
	void* sPtr=(void *)s;
	EndianitySwap16(sPtr);
	*d++=*s++;   
    }
}

/** 
 * @brief EndianitySwaps the 8 bytes in a 64 bit number.  
 * Not optimised!
 * @param x the number to be swapped
 */
static inline void EndianitySwap64(volatile void *x) {
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
 * @param dest the destination vector (must be allocated in memory)
 * @param src the source vector 
 * @param sizer the number of elements in the vector
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
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianDouble(volatile double &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianFloat(volatile float &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianUInt64(volatile uint64 &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianUInt32(volatile uint32 &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianUInt16(volatile uint16 &x) {
    EndianitySwap16(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianInt64(volatile int64 &x) {
    EndianitySwap64(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianInt32(volatile int32 &x) {
    EndianitySwap32(&x);
}
/**
 * @brief Converts a number from big endian to little endian
 * @param x the number to convert
 */
static inline void EndianityFromBigEndianInt16(volatile int16 &x) {
    EndianitySwap16(&x);
}
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianDouble(volatile double &x) {
}
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianFloat(volatile float &x) {
}
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianUInt64(volatile uint64 &x) {
}
;
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianUInt32(volatile uint32 &x) {
}
;
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianUInt16(volatile uint16 &x) {
}
;
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianInt64(volatile int64 &x) {
}
;
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianInt32(volatile int32 &x) {
}
;
/**
 * @brief NOOP since the system is already little endian
 * @param x the number to convert
 */
static inline void EndianityFromLittleEndianInt16(volatile int16 &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianDouble(volatile double &x) {
    EndianitySwap64(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianFloat(volatile float &x) {
    EndianitySwap32(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianUInt64(volatile uint64 &x) {
    EndianitySwap64(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianUInt32(volatile uint32 &x) {
    EndianitySwap32(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianUInt16(volatile uint16 &x) {
    EndianitySwap16(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianInt64(volatile int64 &x) {
    EndianitySwap64(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianInt32(volatile int32 &x) {
    EndianitySwap32(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToBigEndianInt16(volatile int16 &x) {
    EndianitySwap16(&x);
}
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianDouble(volatile double &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianFloat(volatile float &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianUInt64(volatile uint64 &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianUInt32(volatile uint32 &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianUInt16(volatile uint16 &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianInt64(volatile int64 &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianInt32(volatile int32 &x) {
}
;
/** 
 * @brief Converts a number from little endian to big endian
 * @param x the number to convert
 */
static inline void EndianityToLittleEndianInt16(volatile int16 &x) {
}
;
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianDouble(double *dest,
                                                       const double *src,
                                                       uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianUInt64(uint64 *dest,
                                                       const uint64 *src,
                                                       uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianInt64(int64 *dest,
                                                      const int64 *src,
                                                      uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianFloat(float *dest,
                                                      const float *src,
                                                      uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianUInt32(uint32 *dest,
                                                       const uint32 *src,
                                                       uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianUInt16(uint16 *dest,
                                                       const uint16 *src,
                                                       uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianInt32(int32 *dest,
                                                      const int32 *src,
                                                      uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from big endian to little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromBigEndianInt16(int16 *dest,
                                                      const int16 *src,
                                                      uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianDouble(double *dest,
                                                          const double *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianUInt64(uint64 *dest,
                                                          const uint64 *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianInt64(int64 *dest,
                                                         const int64 *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianFloat(float *dest,
                                                         const float *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianUInt32(uint32 *dest,
                                                          const uint32 *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianUInt16(uint16 *dest,
                                                          const uint16 *src,
                                                          uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianInt32(int32 *dest,
                                                         const int32 *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyFromLittleEndianInt16(int16 *dest,
                                                         const int16 *src,
                                                         uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianDouble(double *dest,
                                                     const double *src,
                                                     uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianUInt64(uint64 *dest,
                                                     const uint64 *src,
                                                     uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianInt64(int64 *dest,
                                                    const int64 *src,
                                                    uint32 size) {
    EndianityMemCopySwap64(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianFloat(float *dest,
                                                    const float *src,
                                                    uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianUInt32(uint32 *dest,
                                                     const uint32 *src,
                                                     uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianUInt16(uint16 *dest,
                                                     const uint16 *src,
                                                     uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianInt32(int32 *dest,
                                                    const int32 *src,
                                                    uint32 size) {
    EndianityMemCopySwap32(dest, src, size);
}
/** 
 * @brief Copies a block of memory and converts from little endian to big endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToBigEndianInt16(int16 *dest,
                                                    const int16 *src,
                                                    uint32 size) {
    EndianityMemCopySwap16(dest, src, size);
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianDouble(double *dest,
                                                        const double *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianUInt64(uint64 *dest,
                                                        const uint64 *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianInt64(int64 *dest,
                                                       const int64 *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianFloat(float *dest,
                                                       const float *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianUInt32(uint32 *dest,
                                                        const uint32 *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianUInt16(uint16 *dest,
                                                        const uint16 *src,
                                                        uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianInt32(int32 *dest,
                                                       const int32 *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
/** 
 * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
 * @param dest the destination
 * @param src the source
 * @param size the number of elements
 */
static inline void EndianityMemCopyToLittleEndianInt16(int16 *dest,
                                                       const int16 *src,
                                                       uint32 size) {
    for (uint32 i = 0; i < size; i++)
        *dest++ = *src++;
}
#endif

