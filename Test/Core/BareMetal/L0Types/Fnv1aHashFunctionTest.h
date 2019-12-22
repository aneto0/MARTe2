/**
 * @file Fnv1aHashFunctionTest.h
 * @brief Header file for class Fnv1aHashFunctionTest
 * @date 19/08/2019
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class Fnv1aHashFunctionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_BAREMETAL_L0TYPES_FNV1AHASHFUNCTIONTEST_H_
#define TEST_CORE_BAREMETAL_L0TYPES_FNV1AHASHFUNCTIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Fnv1aHashFunction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** 
 * @brief Test the Fnv1aHashFunction public methods.
 */
class Fnv1aHashFunctionTest {
public:
    /**
     * @brief NOOP.
     */
    Fnv1aHashFunctionTest();
    
    /**
     * @brief NOOP.
     */
    virtual ~Fnv1aHashFunctionTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the compute method.
     */
    bool TestCompute(const char8 *in, uint32 size, uint32 result);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_BAREMETAL_L0TYPES_FNV1AHASHFUNCTIONTEST_H_ */

