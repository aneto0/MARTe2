/**
 * @file Endianity.h
 * @brief Header file for module Endianity
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

 * @details This header file contains the declaration of the module Endianity
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ENDIANITY_H_
#define ENDIANITY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(BareMetal,L1Portability,ARCHITECTURE,EndianityA.h)

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    namespace Endianity {

        /**
         * Definition of little endian
         */
        const uint8 ENDIANITY_LITTLE_ENDIAN = 1u;

        /**
         * Definition of big endian
         */
        const uint8 ENDIANITY_BIG_ENDIAN = 2u;

        /**
         * @brief Checks the target (i.e. of where the code is being executed) architecture endianity.
         * @return MARTe::Endianity::ENDIANITY_LITTLE_ENDIAN if the target architecture is little endian. MARTe::Endianity::ENDIANITY_BIG_ENDIAN if the target architecture is big endian.
         */
        inline uint8 Type();

        /**
         * @brief Converts a number (float64) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        /*lint -e(766) the header file EndianityA.h is not used here. This file is just for declaration and documentation. */
        /*lint -e(762) these declarations are redundant. */
        inline void FromBigEndian(volatile float64 &x);

        /**
         * @brief Converts a number (float32) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile float32 &x);

        /**
         * @brief Converts a number (uint64) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile uint64 &x);

        /**
         * @brief Converts a number (uint32) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile uint32 &x);

        /**
         * @brief Converts a number (uint16) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile uint16 &x);

        /**
         * @brief Converts a number (int64) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile int64 &x);

        /**
         * @brief Converts a number (int32) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile int32 &x);

        /**
         * @brief Converts a number (int16) from big endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromBigEndian(volatile int16 &x);

        /**
         * @brief Converts a number (float64) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile float64 &x);

        /**
         * @brief Converts a number (float32) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile float32 &x);

        /**
         * @brief Converts a number (uint64) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile uint64 &x);

        /**
         * @brief Converts a number (uint32) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile uint32 &x);

        /**
         * @brief Converts a number (uint16) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile uint16 &x);

        /**
         * @brief Converts a number (int64) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile int64 &x);

        /**
         * @brief Converts a number (int32) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile int32 &x);

        /**
         * @brief Converts a number (int16) from little endian to the target operating system endianity
         * @param[in,out] x the number to convert
         */
        inline void FromLittleEndian(volatile int16 &x);

        /**
         * @brief Converts a number (float64) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile float64 &x);

        /**
         * @brief Converts a number (float32) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile float32 &x);

        /**
         * @brief Converts a number (uint64) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile uint64 &x);

        /**
         * @brief Converts a number (uint32) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile uint32 &x);

        /**
         * @brief Converts a number (uint16) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile uint16 &x);

        /**
         * @brief Converts a number (int64) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile int64 &x);

        /**
         * @brief Converts a number (int32) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile int32 &x);

        /**
         * @brief Converts a number (int16) from the target operating system endianity to big endian
         * @param[in,out] x the number to convert
         */
        inline void ToBigEndian(volatile int16 &x);

        /**
         * @brief Converts a number (float64) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile float64 &x);

        /**
         * @brief Converts a number (float32) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile float32 &x);

        /**
         * @brief Converts a number (uint64) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile uint64 &x);

        /**
         * @brief Converts a number (uint32) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile uint32 &x);

        /**
         * @brief Converts a number (uint16) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile uint16 &x);

        /**
         * @brief Converts a number (int64) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile int64 &x);

        /**
         * @brief Converts a number (int32) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile int32 &x);

        /**
         * @brief Converts a number (int16) from the target operating system endianity to little endian
         * @param[in,out] x the number to convert
         */
        inline void ToLittleEndian(volatile int16 &x);

        /**
         * @brief Copies a block of memory (float64) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromBigEndian(float64 *dest,
                const float64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint64) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromBigEndian(uint64 *dest,
                const uint64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int64) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromBigEndian(int64 *dest,
                const int64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float32) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromBigEndian(float32 *dest,
                const float32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint32) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromBigEndian(uint32 *dest,
                const uint32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint16) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         *
         */
        inline void MemCopyFromBigEndian(uint16 *dest,
                const uint16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int32) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         *
         */
        inline void MemCopyFromBigEndian(int32 *dest,
                const int32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int16) and converts from big endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromBigEndian(int16 *dest,
                const int16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float64) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(float64 *dest,
                const float64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint64) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(uint64 *dest,
                const uint64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int64) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(int64 *dest,
                const int64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float32) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(float32 *dest,
                const float32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint32) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(uint32 *dest,
                const uint32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint16) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(uint16 *dest,
                const uint16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int32) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(int32 *dest,
                const int32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int16) and converts from little endian to the target operating system endianity
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyFromLittleEndian(int16 *dest,
                const int16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float64) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(float64 *dest,
                const float64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint64) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(uint64 *dest,
                const uint64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int64) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(int64 *dest,
                const int64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float32) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(float32 *dest,
                const float32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint32) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(uint32 *dest,
                const uint32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint16) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(uint16 *dest,
                const uint16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int32) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(int32 *dest,
                const int32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int16) and converts from the target operating system endianity to big endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToBigEndian(int16 *dest,
                const int16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float64) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(float64 *dest,
                const float64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint64) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(uint64 *dest,
                const uint64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int64) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(int64 *dest,
                const int64 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (float32) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(float32 *dest,
                const float32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint32) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(uint32 *dest,
                const uint32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (uint16) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(uint16 *dest,
                const uint16 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int32) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(int32 *dest,
                const int32 *src,
                uint32 size);

        /**
         * @brief Copies a block of memory (int16) and converts from the target operating system endianity to little endian
         * @param[out] dest the destination
         * @param[in] src the source
         * @param[in] size the number of elements
         * @pre src != NULL && size >= 0.
         */
        inline void MemCopyToLittleEndian(int16 *dest,
                const int16 *src,
                uint32 size);

    }

    /*---------------------------------------------------------------------------*/
    /*                        Inline method definitions                          */
    /*---------------------------------------------------------------------------*/

    inline uint8 Endianity::Type() {
        static bool endianityTypeChecked = false;
        static uint8 endianityType;

        if (!endianityTypeChecked) {
            uint16 value = 0xAABBu;
            uint16 test = value;
            ToLittleEndian(test);
            if (test == value) {
                endianityType = ENDIANITY_LITTLE_ENDIAN;
            }
            else {
                endianityType = ENDIANITY_BIG_ENDIAN; //If the value has not changed then this platform is little endian
            }
            endianityTypeChecked = true;
        }
        return endianityType;
    }

}
#endif /* ENDIANITY_H_ */
