/**
 * @file CRC.h
 * @brief Header file for class CRC
 * @date 13/10/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class CRC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L0TYPES_CRC_H_
#define SOURCE_CORE_BAREMETAL_L0TYPES_CRC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Computes the expected CRC of any given input sequence of bytes (see ComputeCRC).
 * @details The implementation is generic (for a 8/16/32 and bit CRC) and can be used for any input.
 *
 * The CRC table shall be pre-computed by calling ComputeTable. The initial CRC value shall be set using SetInitialCRC, otherwise zero is assumed.
 *
 * The implementation is based on the algorithm implementation described in http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
 * and on https://github.com/meetanthony/crcjava
 */
template<typename T>
class CRC {
public:
    /**
     * @brief NOOP.
     */
    CRC();

    /**
     * @brief Destroys the previously allocated polynomial division lookup table.
     */
    ~CRC();

    /**
     * @brief Computes a lookup table for a given polynomial divisor.
     * @param[in] pol the polynomial divisor to be used for this CRC instance.
     */
    void ComputeTable(T pol);

    /**
     * @brief Computes the CRC for \a size bytes in \a data.
     * @param[in] data the bytes against which the CRC will be computed.
     * @param[in] size the number of bytes in \a data.
     * @param[in] initCRC the initial CRC value.
     * @param[in] inputInverted if true, the order of the bytes to compute the CRC will be data[0], data[-1], data[-2] ... data[-size + 1].
     * This implies that, when using inputInverted, the data must be pointing at the last byte (since negative indexes will be used)
     * @return the CRC value.
     */
    T Compute(const uint8 * const data, int32 size, T initCRC, bool inputInverted) const;

private:
    /**
     * Lookup table for a given polynomial divisor.
     */
    T *crcTable;

    /**
     * The type dependent mask
     */
    uint32 mask;

    /**
     * The type dependent inverted mask
     */
    uint32 maskInv;

    /**
     * The type dependent shift
     */
    uint32 shift;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename T>
CRC<T>::CRC() {
    crcTable = NULL_PTR(T *);
    mask = 1u;
    mask = mask << ((sizeof(T) * 8u) - 1u); //0x80 for uint8, 0x8000 for uint16 ...
    maskInv = 0xFFFFFFFFu;
    if (sizeof(T) < 4u) {
        maskInv = static_cast<uint32>(1ULL << (sizeof(T) * 8u));
        maskInv -= 1u;
    }
    shift = sizeof(T) - 1u;
    shift *= 8u;
}

template<typename T>
CRC<T>::~CRC() {
    if (crcTable != NULL_PTR(T *)) {
        delete[] crcTable;
        crcTable = NULL_PTR(T *);
    }
}

template<typename T>
void CRC<T>::ComputeTable(const T pol) {
    if (crcTable != NULL_PTR(T *)) {
        delete[] crcTable;
        crcTable = NULL_PTR(T *);
    }
    crcTable = new T[256u];

    uint32 d; //divisor
    for (d = 0u; d < 256u; d++) {

        T curByte = static_cast<T>(d << shift);

        uint8 bit;
        for (bit = 0u; bit < 8u; bit++) {
            if ((curByte & mask) != 0u) {
                curByte <<= 1;
                curByte ^= pol;
            }
            else {
                curByte <<= 1;
            }
        }

        crcTable[d] = curByte;
    }
}

template<typename T>
T CRC<T>::Compute(const uint8 * const data, const int32 size, T initCRC, const bool inputInverted) const {
    using namespace MARTe;
    int32 b;
    T crc = initCRC;

    for (b = 0; b < size; b++) {

        uint8 pos = static_cast<uint8>(((crc >> shift) ^ data[inputInverted ? -b : b]) & 0xFF);
        /*lint -e{613} crcTable is not NULL if pre-condition is met*/
        crc = static_cast<T>(static_cast<T>(crcTable[pos]) ^ (crc << 8u));
        crc &= maskInv;
    }

    return crc;
}

}

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_CRC_H_ */

