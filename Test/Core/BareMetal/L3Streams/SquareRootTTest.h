/**
 * @file SquareRootTTest.h
 * @brief Header file for class SquareRootTTest
 * @date 20/04/2017
 * @author Andre' Torres
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

 * @details This header file contains the declaration of the class SquareRootTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SQUAREROOTTTEST_H_
#define SQUAREROOTTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SquareRootT.h"
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class SquareRootTTest {

public:
    /**
     * @brief Tests the the function for uint8.
     */
    bool TestUint8(const uint8 testVal,
                   const uint8 expectedRes);

    /**
     * @brief Tests the the function for uint16.
     */
    bool TestUint16(const uint16 testVal,
                    const uint16 expectedRes);

    /**
     * @brief Tests the the function for uint32.
     */
    bool TestUint32(const uint32 testVal,
                    const uint32 expectedRes);
    /**
     * @brief Tests the the function for uint64.
     */
    bool TestUint64(const uint64 testVal,
                    const uint64 expectedRes);

    /**
     * @brief Tests the the function for int8.
     */
    bool TestInt8(const int8 testVal,
                  const int8 expectedRes);

    /**
     * @brief Tests the the function for int16.
     */
    bool TestInt16(const int16 testVal,
                   const int16 expectedRes);

    /**
     * @brief Tests the the function for int32.
     */
    bool TestInt32(const int32 testVal,
                   const int32 expectedRes);

    /**
     * @brief Tests the the function for int64.
     */
    bool TestInt64(const int64 testVal,
                   const int64 expectedRes);

    /**
     * @brief Tests the the function for float32.
     */
    bool TestFloat32(const float32 testVal,
                     const float32 expectedRes);

    /**
     * @brief Tests the the function for float64.
     */
    bool TestFloat64(const float64 testVal,
                     const float64 expectedRes);
};
} /* namespace MARTe*/
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SQUAREROOTTTEST_H_ */

