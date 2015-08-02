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

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Platform dependent endianity functions.
 * @details These functions implement conversion from little endian to big endian format and vice versa.
 */
class EndianityA {

public:

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianDouble(volatile float64 &x) {
        EndianitySwap64(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianFloat(volatile float32 &x) {
        EndianitySwap32(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianUInt64(volatile uint64 &x) {
        EndianitySwap64(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianUInt32(volatile uint32 &x) {
        EndianitySwap32(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianUInt16(volatile uint16 &x) {
        EndianitySwap16(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianInt64(volatile int64 &x) {
        EndianitySwap64(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianInt32(volatile int32 &x) {
        EndianitySwap32(&x);
    }

    /**
     * @brief Converts a number from big endian to little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndianInt16(volatile int16 &x) {
        EndianitySwap16(&x);
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianDouble(volatile float64 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianFloat(volatile float32 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianUInt64(volatile uint64 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianUInt32(volatile uint32 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianUInt16(volatile uint16 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianInt64(volatile int64 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianInt32(volatile int32 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndianInt16(volatile int16 &x) {
    }

    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianDouble(volatile float64 &x) {
        EndianitySwap64(&x);
    }
    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianFloat(volatile float32 &x) {
        EndianitySwap32(&x);
    }
    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianUInt64(volatile uint64 &x) {
        EndianitySwap64(&x);
    }

    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianUInt32(volatile uint32 &x) {
        EndianitySwap32(&x);
    }

    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianUInt16(volatile uint16 &x) {
        EndianitySwap16(&x);
    }

    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianInt64(volatile int64 &x) {
        EndianitySwap64(&x);
    }

    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianInt32(volatile int32 &x) {
        EndianitySwap32(&x);
    }

    /**
     * @brief Converts a number from little endian to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndianInt16(volatile int16 &x) {
        EndianitySwap16(&x);
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianDouble(volatile float64 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianFloat(volatile float32 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianUInt64(volatile uint64 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianUInt32(volatile uint32 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianUInt16(volatile uint16 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianInt64(volatile int64 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianInt32(volatile int32 &x) {
    }

    /**
     * @brief NOOP since the system is already little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndianInt16(volatile int16 &x) {
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianDouble(float64 *dest, const float64 *src, uint32 size) {
        EndianityMemCopySwap64(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
        EndianityMemCopySwap64(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianInt64(int64 *dest, const int64 *src, uint32 size) {
        EndianityMemCopySwap64(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianFloat(float32 *dest, const float32 *src, uint32 size) {
        EndianityMemCopySwap32(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
        EndianityMemCopySwap32(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
        EndianityMemCopySwap16(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianInt32(int32 *dest, const int32 *src, uint32 size) {
        EndianityMemCopySwap32(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from big endian to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromBigEndianInt16(int16 *dest, const int16 *src, uint32 size) {
        EndianityMemCopySwap16(dest, src, size);
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianDouble(float64 *dest, const float64 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianInt64(int64 *dest, const int64 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianFloat(float32 *dest, const float32 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianInt32(int32 *dest, const int32 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyFromLittleEndianInt16(int16 *dest, const int16 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianDouble(float64 *dest, const float64 *src, uint32 size) {
        EndianityMemCopySwap64(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
        EndianityMemCopySwap64(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianInt64(int64 *dest, const int64 *src, uint32 size) {
        EndianityMemCopySwap64(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianFloat(float32 *dest, const float32 *src, uint32 size) {
        EndianityMemCopySwap32(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
        EndianityMemCopySwap32(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
        EndianityMemCopySwap16(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianInt32(int32 *dest, const int32 *src, uint32 size) {
        EndianityMemCopySwap32(dest, src, size);
    }

    /**
     * @brief Copies a block of memory and converts from little endian to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToBigEndianInt16(int16 *dest, const int16 *src, uint32 size) {
        EndianityMemCopySwap16(dest, src, size);
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianDouble(float64 *dest, const float64 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianUInt64(uint64 *dest, const uint64 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianInt64(int64 *dest, const int64 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianFloat(float32 *dest, const float32 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianUInt32(uint32 *dest, const uint32 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianUInt16(uint16 *dest, const uint16 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianInt32(int32 *dest, const int32 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    /**
     * @brief Copies a block of memory but performs no endianity swap since both source and destinations are already little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     */
    static inline void MemCopyToLittleEndianInt16(int16 *dest, const int16 *src, uint32 size) {
        for (uint32 i = 0; i < size; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

private:

    /**
     * @brief EndianitySwaps the 4 bytes in a 32 bit number.
     * @param[in,out] x the number to be swapped
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
     * @param[in,out]  x the number to be swapped
     * @param[in] sizer the number of elements in the vector
     */
    static inline void EndianitySwap32(volatile void *x, uint32 sizer) {
        int32 *xx = (int32 *) x;
        for (uint32 i = 0; i < sizer; i++) {

            void* xptr = (void *) xx;
            EndianitySwap32(xptr);

            xx++;
        }
    }

    /**
     * @brief EndianitySwaps the 4 bytes while copying a vector of 32 bit numbers
     * @param[out] dest the destination vector (must be allocated in memory)
     * @param[in] src the source vector
     * @param[in] sizer the number of elements in the vector
     */
    static inline void EndianityMemCopySwap32(volatile void *dest, volatile const void *src, uint32 sizer) {
        int32 *s = (int32 *) src;
        int32 *d = (int32 *) dest;
        for (uint32 i = 0; i < sizer; i++) {
            void* sPtr = (void *) s;
            EndianitySwap32(sPtr);
            *d++ = *s++;
        }
    }

    /**
     * @brief EndianitySwaps the 2 bytes in a 16 bit number.
     * @param[in,out] x the number to be swapped
     */
    static inline void EndianitySwap16(volatile void *x) {
        asm(
                "movw (%0), %%dx\n"
                "xchgb %%dl, %%dh\n"
                "movw %%dx, (%0)"
                : : "r" (x) :"dx"
        );
    }

    /**
     * @brief EndianitySwaps the 2 bytes in a 16 bit number for all the elements
     * of a vector
     * @param[in,out] x the number to be swapped
     * @param[in] sizer the number of elements in the vector
     */
    static inline void EndianitySwap16(volatile void *x, uint32 sizer) {
        int16 *xx = (int16 *) x;

        for (uint32 i = 0; i < sizer; i++) {
            void* xptr = (void *) xx;
            EndianitySwap16(xptr);
            xx++;
        }
    }

    /**
     * @brief EndianitySwaps the 2 bytes while copying a vector of 16 bit numbers
     * @param[out] dest the destination vector (must be allocated in memory)
     * @param[in] src the source vector
     * @param[in] sizer the number of elements in the vector
     */
    static inline void EndianityMemCopySwap16(volatile void *dest, volatile const void *src, uint32 sizer) {
        int16 *s = (int16 *) src;
        int16 *d = (int16 *) dest;
        for (uint32 i = 0; i < sizer; i++) {
            void* sPtr = (void *) s;
            EndianitySwap16(sPtr);
            *d++ = *s++;
        }
    }

    /**
     * @brief EndianitySwaps the 8 bytes in a 64 bit number.
     * @param[in,out] x the number to be swapped
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
     * @param[out] dest the destination vector (must be allocated in memory)
     * @param[in] src the source vector
     * @param[in] sizer the number of elements in the vector
     */
    static inline void EndianityMemCopySwap64(volatile void *dest, volatile const void *src, uint32 sizer) {
        int64 *s = (int64 *) src;
        int64 *d = (int64 *) dest;
        for (uint32 i = 0; i < sizer; i++) {
            *d = *s;
            EndianitySwap64(d);
            d++;
            s++;
        }
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ENDIANITYA_H_ */

