/**
 * @file MemoryDataSourceITest.h
 * @brief Header file for class MemoryDataSourceITest
 * @date 11/05/2018
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

 * @details This header file contains the declaration of the class MemoryDataSourceITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5GAMS_MEMORYDATASOURCEITEST_H_
#define L5GAMS_MEMORYDATASOURCEITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/**
 * @brief Tests the MemoryDataSourceI public methods.
 */
class MemoryDataSourceITest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method with the default parameters.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method specifying the number of buffers.
     */
    bool TestInitialise_NumberOfBuffers();

    /**
     * @brief Tests the Initialise method specifying a bad heap name.
     */
    bool TestInitialise_BadHeapName();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L5GAMS_MEMORYDATASOURCEITEST_H_ */
