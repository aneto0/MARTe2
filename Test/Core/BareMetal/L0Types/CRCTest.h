/**
 * @file CRCTest.h
 * @brief Header file for class CRCTest
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

 * @details This header file contains the declaration of the class CRCTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_BAREMETAL_L0TYPES_CRCTEST_H_
#define TEST_CORE_BAREMETAL_L0TYPES_CRCTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CRC.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the CRC public methods
 */
class CRCTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the compute table with the polynomial 0x3D65.
     */
    bool TestComputeTable_0x3D65();

    /**
     * @brief Tests the compute table with the polynomial 0x3D65 and an inverted input.
     */
    bool TestComputeTable_0x3D65_inverted();

    /**
     * @brief Tests the compute table with the polynomial 0x3D65 and initial crc != 0.
     */
    bool TestComputeTable_0x3D65_initialCRC();

    /**
     * @brief Tests the compute table with the polynomial 0x3D65, an inverted input and initial crc != 0.
     */
    bool TestComputeTable_0x3D65_inverted_initialCRC();

    /**
     * @brief Tests that after computing the CRC of the data, recomputing the CRC using the computed CRC will output zero.
     */
    bool TestCompute_uint16_CRC_0();

    /**
     * @brief Tests that after computing the CRC of the data (with inverted input) with the computed CRC is zero.
     */
    bool TestCompute_uint16_CRC_0_inverted();

    /**
     * @brief Tests that after computing the CRC of the data, recomputing the CRC using the computed CRC will output zero.
     */
    bool TestCompute_uint16_CRC_0_initialCRC();

    /**
     * @brief As TestCompute_uint16_CRC_0_initialCRC with inverted inputs.
     */
    bool TestCompute_uint16_CRC_0_inverted_initialCRC();

    /**
     * @brief Tests that after computing the CRC of the data, recomputing the CRC using the computed CRC will output zero.
     */
    bool TestCompute_uint8_CRC_0();

    /**
     * @brief Tests the compute table with the polynomial 0x9B and the initial value 0xFF.
     */
    bool TestCompute_CRC8_CDMA2000();

    /**
     * @brief Tests the compute table with the polynomial 0x9B and the initial value 0xFF and inverted input.
     */
    bool TestCompute_CRC8_CDMA2000_inverted();

    /**
     * @brief Tests the compute table with the polynomial 0x4C11DB7 and the initial value 0xFFFFFFFF.
     */
    bool TestCompute_CRC32_MPEG2();

    /**
     * @brief Tests the compute table with the polynomial 0x4C11DB7 and the initial value 0xFFFFFFFF and inverted input.
     */
    bool TestCompute_CRC32_MPEG2_inverted();

private:
    /**
     * @brief Computes the CRC against four different data arrays.
     */
    template <typename T>
    bool TestComputeCRC(T pol, T initialCRC, bool inputInverted, T *expectedCRC);

    /**
     * @brief Tests that after computing the CRC of the data with the computed CRC is zero.
     */
    template <typename T>
    bool TestComputeCRC0(T pol, T initialCRC, bool inputInverted, MARTe::uint8 *data, MARTe::uint32 dataSize = 16);
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template <typename T>
bool CRCTest::TestComputeCRC0(T pol, T initialCRC, bool inputInverted, MARTe::uint8 *data, MARTe::uint32 dataSize) {
    using namespace MARTe;

    uint32 nBytes = sizeof(T);

    CRC<T> crc;
    crc.ComputeTable(pol);
    uint32 size = (dataSize - nBytes);
    T resultCRC;
    uint8 *pcrc = reinterpret_cast<uint8 *>(&resultCRC);
    resultCRC = crc.Compute(&data[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    uint32 n;
    for (n=0; n<nBytes; n++) {
        if (inputInverted) {
            data[n] = pcrc[n];
        }
        else {
            data[dataSize - n - 1] = pcrc[n];
        }
    }
    size = dataSize;
    resultCRC = crc.Compute(&data[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);

    return (resultCRC == 0);
}

template <typename T>
bool CRCTest::TestComputeCRC(T pol, T initialCRC, bool inputInverted, T* expectedCRC) {
    using namespace MARTe;
    const uint32 dataSize = 16u;
    uint8 data1[dataSize] = { 0xF, 0x0, 0xF, 0x0, 0xAA, 0x55, 0xAA, 0x55, 0xBB, 0xDD, 0xFF, 0xEE, 0xCC, 0xDD, 0xEE, 0xFF };
    uint8 data2[dataSize] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    uint8 data3[dataSize] = { 0x1, 0xFF, 0x2, 0xFE, 0x3, 0xFD, 0x4, 0xFC, 0x5, 0xFB, 0x6, 0xFA, 0x7, 0xEF, 0x8, 0xEE };
    uint8 data4[dataSize] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    CRC<T> crc;
    crc.ComputeTable(pol);
    //Force delete of existent table (for coverage).
    crc.ComputeTable(pol);
    uint32 size = dataSize;
    T crcResult[8];
    crcResult[0] = crc.Compute(&data1[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    crcResult[1] = crc.Compute(&data2[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    crcResult[2] = crc.Compute(&data3[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    crcResult[3] = crc.Compute(&data4[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    size = 10;
    crcResult[4] = crc.Compute(&data1[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    crcResult[5] = crc.Compute(&data2[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    crcResult[6] = crc.Compute(&data3[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    crcResult[7] = crc.Compute(&data4[inputInverted ? (dataSize - 1) : 0], size, initialCRC, inputInverted);
    bool ok = true;
    uint32 i;
    for (i = 0; (i < 8) && (ok); i++) {
        ok = (crcResult[i] == expectedCRC[i]);
    }
    return ok;
}

#endif /* TEST_CORE_BAREMETAL_L0TYPES_CRCTEST_H_ */
