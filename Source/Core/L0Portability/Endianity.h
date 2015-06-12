/**
 * @file Endianity.h
 * @brief Header file for class Endianity
 * @date 05/06/2015
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

#ifndef Endianity_H_
#define Endianity_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,EndianityA.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Converts any basic type to (from) the target architecture, from (to)
 * the requested endianity. 
 *
 * @details The target architecture is the one of the machine where the code is being executed.
 * Most of the implementation is therefore delegated to the EndianityA.h implementation, 
 * which will be specific for each type of supported architecture. 
 *
 * @details These methods are particularly useful in the implementation of functionality that requires
 * the exchange of data with external producers that might have a different endianity. Examples
 * are drivers and network interfaces.
 */
class Endianity {
private:
    /**
     * Set to True when the Endianity::Type() is called for the first time
     */
    static bool endianityTypeChecked;
    /**
     * The target machine endianity
     */
    static uint8 endianityType;
public:

    /**
     * Definition of little endian
     */
    static const uint8 ENDIANITY_LITTLE_ENDIAN = 1;
    /**
     * Definition of big endian
     */
    static const uint8 ENDIANITY_BIG_ENDIAN = 2;
    /**
     * @brief Checks the target (i.e. of where the code is being executed) architecture endianity.
     * @return LITTLE_ENDIAN if the target architecture is little endian. BIG_ENDIAN if the target architecture is big endian.
     */
    static inline uint8 Type();

    /**
     * @brief Converts a number (double) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile double &x);

    /**
     * @brief Converts a number (float) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile float &x);

    /**
     * @brief Converts a number (uint64) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile uint64 &x);

    /**
     * @brief Converts a number (uint32) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile uint32 &x);

    /**
     * @brief Converts a number (uint16) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile uint16 &x);

    /**
     * @brief Converts a number (int64) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile int64 &x);

    /**
     * @brief Converts a number (int32) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile int32 &x);

    /**
     * @brief Converts a number (int16) from big endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromBigEndian(volatile int16 &x);

    /**
     * @brief Converts a number (double) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile double &x);

    /**
     * @brief Converts a number (float) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile float &x);

    /**
     * @brief Converts a number (uint64) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile uint64 &x);

    /**
     * @brief Converts a number (uint32) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile uint32 &x);

    /**
     * @brief Converts a number (uint16) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile uint16 &x);

    /**
     * @brief Converts a number (int64) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile int64 &x);

    /**
     * @brief Converts a number (int32) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile int32 &x);

    /**
     * @brief Converts a number (int16) from little endian to the target operating system endianity
     * @param[in,out] x the number to convert
     */
    static inline void FromLittleEndian(volatile int16 &x);

    /** 
     * @brief Converts a number (double) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile double &x);

    /** 
     * @brief Converts a number (float) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile float &x);

    /** 
     * @brief Converts a number (uint64) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile uint64 &x);

    /** 
     * @brief Converts a number (uint32) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile uint32 &x);

    /** 
     * @brief Converts a number (uint16) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile uint16 &x);

    /** 
     * @brief Converts a number (int64) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile int64 &x);

    /** 
     * @brief Converts a number (int32) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile int32 &x);

    /** 
     * @brief Converts a number (int16) from the target operating system endianity to big endian
     * @param[in,out] x the number to convert
     */
    static inline void ToBigEndian(volatile int16 &x);

    /** 
     * @brief Converts a number (double) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile double &x);

    /** 
     * @brief Converts a number (float) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile float &x);

    /** 
     * @brief Converts a number (uint64) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile uint64 &x);

    /** 
     * @brief Converts a number (uint32) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile uint32 &x);

    /** 
     * @brief Converts a number (uint16) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile uint16 &x);

    /** 
     * @brief Converts a number (int64) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile int64 &x);

    /** 
     * @brief Converts a number (int32) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile int32 &x);

    /** 
     * @brief Converts a number (int16) from the target operating system endianity to little endian
     * @param[in,out] x the number to convert
     */
    static inline void ToLittleEndian(volatile int16 &x);

    /** 
     * @brief Copies a block of memory (double) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromBigEndian(double *dest,const double *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint64) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromBigEndian(uint64 *dest,const uint64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int64) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromBigEndian(int64 *dest,const int64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (float) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromBigEndian(float *dest,const float *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint32) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromBigEndian(uint32 *dest,const uint32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint16) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     *
     */
    static inline void MemCopyFromBigEndian(uint16 *dest,const uint16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int32) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     *
     */
    static inline void MemCopyFromBigEndian(int32 *dest,const int32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int16) and converts from big endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromBigEndian(int16 *dest,const int16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (double) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(double *dest,const double *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint64) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(uint64 *dest,const uint64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int64) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(int64 *dest,const int64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (float) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(float *dest,const float *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint32) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(uint32 *dest,const uint32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint16) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(uint16 *dest,const uint16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int32) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(int32 *dest,const int32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int16) and converts from little endian to the target operating system endianity
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyFromLittleEndian(int16 *dest,const int16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (double) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(double *dest,const double *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint64) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(uint64 *dest,const uint64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int64) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(int64 *dest,const int64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (float) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(float *dest,const float *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint32) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(uint32 *dest,const uint32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint16) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(uint16 *dest,const uint16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int32) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(int32 *dest,const int32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int16) and converts from the target operating system endianity to big endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToBigEndian(int16 *dest,const int16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (double) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(double *dest,const double *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint64) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(uint64 *dest,const uint64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int64) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(int64 *dest,const int64 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (float) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(float *dest,const float *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint32) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(uint32 *dest,const uint32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (uint16) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(uint16 *dest,const uint16 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int32) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(int32 *dest,const int32 *src,uint32 size);

    /** 
     * @brief Copies a block of memory (int16) and converts from the target operating system endianity to little endian
     * @param[out] dest the destination
     * @param[in] src the source
     * @param[in] size the number of elements
     * @pre src != NULL && size >= 0.
     */
    static inline void MemCopyToLittleEndian(int16 *dest,const int16 *src,uint32 size);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
uint8 Endianity::Type() {
    if (!endianityTypeChecked) {
        uint16 value = 0xAABB;
        uint16 test = value;
        ToLittleEndian(test);
        (test == value) ? endianityType = ENDIANITY_LITTLE_ENDIAN : endianityType = ENDIANITY_BIG_ENDIAN; //If the value has not changed then this platform is little endian
        endianityTypeChecked = True;
    }
    return endianityType;
}

void Endianity::FromBigEndian(volatile double &x) {
    EndianityA::FromBigEndianDouble(x);
}

void Endianity::FromBigEndian(volatile float &x) {
    EndianityA::FromBigEndianFloat(x);
}

void Endianity::FromBigEndian(volatile uint64 &x) {
    EndianityA::FromBigEndianUInt64(x);
}

void Endianity::FromBigEndian(volatile uint32 &x) {
    EndianityA::FromBigEndianUInt32(x);
}

void Endianity::FromBigEndian(volatile uint16 &x) {
    EndianityA::FromBigEndianUInt16(x);
}

void Endianity::FromBigEndian(volatile int64 &x) {
    EndianityA::FromBigEndianInt64(x);
}

void Endianity::FromBigEndian(volatile int32 &x) {
    EndianityA::FromBigEndianInt32(x);
}

void Endianity::FromBigEndian(volatile int16 &x) {
    EndianityA::FromBigEndianInt16(x);
}

void Endianity::FromLittleEndian(volatile double &x) {
    EndianityA::FromLittleEndianDouble(x);
}

void Endianity::FromLittleEndian(volatile float &x) {
    EndianityA::FromLittleEndianFloat(x);
}

void Endianity::FromLittleEndian(volatile uint64 &x) {
    EndianityA::FromLittleEndianUInt64(x);
}

void Endianity::FromLittleEndian(volatile uint32 &x) {
    EndianityA::FromLittleEndianUInt32(x);
}

void Endianity::FromLittleEndian(volatile uint16 &x) {
    EndianityA::FromLittleEndianUInt16(x);
}

void Endianity::FromLittleEndian(volatile int64 &x) {
    EndianityA::FromLittleEndianInt64(x);
}

void Endianity::FromLittleEndian(volatile int32 &x) {
    EndianityA::FromLittleEndianInt32(x);
}

void Endianity::FromLittleEndian(volatile int16 &x) {
    EndianityA::FromLittleEndianInt16(x);
}

void Endianity::ToBigEndian(volatile double &x) {
    EndianityA::ToBigEndianDouble(x);
}

void Endianity::ToBigEndian(volatile float &x) {
    EndianityA::ToBigEndianFloat(x);
}

void Endianity::ToBigEndian(volatile uint64 &x) {
    EndianityA::ToBigEndianUInt64(x);
}

void Endianity::ToBigEndian(volatile uint32 &x) {
    EndianityA::ToBigEndianUInt32(x);
}

void Endianity::ToBigEndian(volatile uint16 &x) {
    EndianityA::ToBigEndianUInt16(x);
}

void Endianity::ToBigEndian(volatile int64 &x) {
    EndianityA::ToBigEndianInt64(x);
}

void Endianity::ToBigEndian(volatile int32 &x) {
    EndianityA::ToBigEndianInt32(x);
}

void Endianity::ToBigEndian(volatile int16 &x) {
    EndianityA::ToBigEndianInt16(x);
}

void Endianity::ToLittleEndian(volatile double &x) {
    EndianityA::ToLittleEndianDouble(x);
}

void Endianity::ToLittleEndian(volatile float &x) {
    EndianityA::ToLittleEndianFloat(x);
}

void Endianity::ToLittleEndian(volatile uint64 &x) {
    EndianityA::ToLittleEndianUInt64(x);
}

void Endianity::ToLittleEndian(volatile uint32 &x) {
    EndianityA::ToLittleEndianUInt32(x);
}

void Endianity::ToLittleEndian(volatile uint16 &x) {
    EndianityA::ToLittleEndianUInt16(x);
}

void Endianity::ToLittleEndian(volatile int64 &x) {
    EndianityA::ToLittleEndianInt64(x);
}

void Endianity::ToLittleEndian(volatile int32 &x) {
    EndianityA::ToLittleEndianInt32(x);
}

void Endianity::ToLittleEndian(volatile int16 &x) {
    EndianityA::ToLittleEndianInt16(x);
}

void Endianity::MemCopyFromBigEndian(double *dest,
                                     const double *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianDouble(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(uint64 *dest,
                                     const uint64 *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianUInt64(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(int64 *dest,
                                     const int64 *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianInt64(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(float *dest,
                                     const float *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianFloat(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(uint32 *dest,
                                     const uint32 *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianUInt32(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(uint16 *dest,
                                     const uint16 *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianUInt16(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(int32 *dest,
                                     const int32 *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianInt32(dest, src, size);
}

void Endianity::MemCopyFromBigEndian(int16 *dest,
                                     const int16 *src,
                                     uint32 size) {
    EndianityA::MemCopyFromBigEndianInt16(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(double *dest,
                                        const double *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianDouble(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(uint64 *dest,
                                        const uint64 *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianUInt64(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(int64 *dest,
                                        const int64 *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianInt64(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(float *dest,
                                        const float *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianFloat(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(uint32 *dest,
                                        const uint32 *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianUInt32(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(uint16 *dest,
                                        const uint16 *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianUInt16(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(int32 *dest,
                                        const int32 *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianInt32(dest, src, size);
}

void Endianity::MemCopyFromLittleEndian(int16 *dest,
                                        const int16 *src,
                                        uint32 size) {
    EndianityA::MemCopyFromLittleEndianInt16(dest, src, size);
}

void Endianity::MemCopyToBigEndian(double *dest,
                                   const double *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianDouble(dest, src, size);
}

void Endianity::MemCopyToBigEndian(uint64 *dest,
                                   const uint64 *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianUInt64(dest, src, size);
}

void Endianity::MemCopyToBigEndian(int64 *dest,
                                   const int64 *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianInt64(dest, src, size);
}

void Endianity::MemCopyToBigEndian(float *dest,
                                   const float *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianFloat(dest, src, size);
}

void Endianity::MemCopyToBigEndian(uint32 *dest,
                                   const uint32 *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianUInt32(dest, src, size);
}

void Endianity::MemCopyToBigEndian(uint16 *dest,
                                   const uint16 *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianUInt16(dest, src, size);
}

void Endianity::MemCopyToBigEndian(int32 *dest,
                                   const int32 *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianInt32(dest, src, size);
}

void Endianity::MemCopyToBigEndian(int16 *dest,
                                   const int16 *src,
                                   uint32 size) {
    EndianityA::MemCopyToBigEndianInt16(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(double *dest,
                                      const double *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianDouble(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(uint64 *dest,
                                      const uint64 *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianUInt64(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(int64 *dest,
                                      const int64 *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianInt64(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(float *dest,
                                      const float *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianFloat(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(uint32 *dest,
                                      const uint32 *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianUInt32(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(uint16 *dest,
                                      const uint16 *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianUInt16(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(int32 *dest,
                                      const int32 *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianInt32(dest, src, size);
}

void Endianity::MemCopyToLittleEndian(int16 *dest,
                                      const int16 *src,
                                      uint32 size) {
    EndianityA::MemCopyToLittleEndianInt16(dest, src, size);
}
/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ENDIANITY_H_ */

