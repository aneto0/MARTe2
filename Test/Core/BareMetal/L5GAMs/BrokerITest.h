/**
 * @file BrokerITest.h
 * @brief Header file for class BrokerITest
 * @date 27/07/2016
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

 * @details This header file contains the declaration of the class BrokerITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BROKERITEST_H_
#define BROKERITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the BrokerI public methods.
 */
class BrokerITest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the InitFunctionPointers method.
     */
    bool TestInitFunctionPointers();

    /**
     * @brief Tests the GetNumberOfCopies method.
     */
    bool TestGetNumberOfCopies();

    /**
     * @brief Tests the GetNumberOfCopies method.
     */
    bool TestGetNumberOfCopiesOptim();

    /**
     * @brief Tests the GetCopyByteSize method.
     */
    bool TestGetCopyByteSize();

    /**
     * @brief Tests the GetCopyByteSize method.
     */
    bool TestGetCopyByteSizeOptim();

    /**
     * @brief Tests the GetCopyOffset method.
     */
    bool TestGetCopyOffset();

    /**
     * @brief Tests the GetCopyOffset method.
     */
    bool TestGetCopyOffsetOptim();

    /**
     * @brief Tests the GetFunctionPointer method.
     */
    bool TestGetFunctionPointer();

    /**
     * @brief Tests the GetFunctionPointer method.
     */
    bool TestGetFunctionPointerOptim();

    /**
     * @brief Tests the GetOwnerFunctionName method.
     */
    bool TestGetOwnerFunctionName();

    /**
     * @brief Tests the GetOwnerFunctionName method.
     */
    bool TestGetOwnerDataSourceName();

    /**
     * @brief Tests that the query functions to BrokerI are not overlapping.
     * @details This test is design to demonstrate that the BrokerI fails when the DS memory is interleaved (Signal1_sample1, Signal2_sample1, Signal1_sample2, Signal2_sample2).... and samples is greater than 1.
     * The test check that the memory to be copied from the dataSource is overlapping itself since the DS memory is interleaved while the BrokerI assumes flat memory.
     */
    bool TestInterleavedDSMemory();

    /**
     * @brief Tests that the query functions to BrokerI are not overlapping.
     * @details This test is design to demonstrate that the BrokerI fails when the DS memory is interleaved (Signal1_sample1, Signal2_sample1, Signal1_sample2, Signal2_sample2).... and samples is greater than 1.
     * The test check that the memory to be copied from the dataSource is overlapping itself since the DS memory is interleaved while the BrokerI assumes flat memory.
     */
    bool TestInterleavedDSMemoryOptim();

    /**
     * @brief Tests that the query functions to BrokerI are not overlapping.
     * @details This test is design to demonstrate that the BrokerI fails when the DS memory is interleaved and samples is greater than 1.
     * The test check that the memory to be copied from the dataSource is overlapping itself since the DS memory is interleaved while the BrokerI assumes flat memory.
     */
    bool TestFlatDSMemory();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BROKERITEST_H_ */

