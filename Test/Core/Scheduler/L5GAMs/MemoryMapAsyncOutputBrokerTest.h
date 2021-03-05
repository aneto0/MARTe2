/**
 * @file MemoryMapAsyncOutputBrokerTest.h
 * @brief Header file for class MemoryMapAsyncOutputBrokerTest
 * @date 24/01/2017
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

 * @details This header file contains the declaration of the class MemoryMapAsyncOutputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPASYNCOUTPUTBROKERTEST_H_
#define MEMORYMAPASYNCOUTPUTBROKERTEST_H_

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
 * @brief Tests the MemoryMapAsyncOutputBroker public methods.
 */
class MemoryMapAsyncOutputBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Init method.
     */
    bool TestInit();

    /**
     * @brief Tests the InitWithBufferParameters method.
     */
    bool TestInitWithBufferParameters();

    /**
     * @brief Tests the InitWithBufferParameters method with NumberOfBuffers = 0.
     */
    bool TestInitWithBufferParameters_False_0_NumberOfBuffers();

    /**
     * @brief Tests the InitWithBufferParameters method with more than one GAM writing to the DataSourceI.
     */
    bool TestInitWithBufferParameters_False_MoreThanOneGAM();

    /**
     * @brief Tests the InitWithBufferParameters method signals that have Samples =  0.
     */
    bool TestInitWithBufferParameters_False_Samples();

    /**
     * @brief Tests the Execute method with many buffers.
     */
    bool TestExecute_N_Buffers();

    /**
     * @brief Tests the Execute method with only one buffer.
     */
    bool TestExecute_1_Buffer();

    /**
     * @brief Tests that a buffer overrun is detected.
     */
    bool TestExecute_Buffer_Overrun();

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the GetNumberOfBuffers method.
     */
    bool TestGetNumberOfBuffers();

    /**
     * @brief Tests the IsIgnoringBufferOverrun method.
     */
    bool TestIsIgnoringBufferOverrun();

    /**
     * @brief Tests the TestSetIgnoreBufferOverrun method.
     */
    bool TestSetIgnoreBufferOverrun();

    /**
     * @brief Tests the TestSetIgnoreBufferOverrun method.
     */
    bool TestFlush();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPASYNCOUTPUTBROKERTEST_H_ */

