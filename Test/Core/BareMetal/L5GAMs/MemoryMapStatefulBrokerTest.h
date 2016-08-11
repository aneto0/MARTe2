/**
 * @file MemoryMapStatefulBrokerTest.h
 * @brief Header file for class MemoryMapStatefulBrokerTest
 * @date 28/07/2016
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

 * @details This header file contains the declaration of the class MemoryMapStatefulBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSTATEFULBROKERTEST_H_
#define MEMORYMAPSTATEFULBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the MemoryMapStatefulBroker public methods.
 */
class MemoryMapStatefulBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Init method for an input basic signal (i.e. no ranges and no samples) in buffer index = 0
     */
    bool TestInit_Input_0();

    /**
     * @brief Tests the Init method for an input basic signal (i.e. no ranges and no samples) in buffer index = 1
     */
    bool TestInit_Input_1();

    /**
     * @brief Tests the Init method for an input signal with ranges in buffer index = 0
     */
    bool TestInit_Input_Ranges_0();

    /**
     * @brief Tests the Init method for an input signal with ranges in buffer index = 1
     */
    bool TestInit_Input_Ranges_1();

    /**
     * @brief Tests the Init method for an input signal with samples in buffer index = 0
     */
    bool TestInit_Input_Samples_0();

    /**
     * @brief Tests the Init method for an input signal with samples in buffer index = 1
     */
    bool TestInit_Input_Samples_1();

    /**
     * @brief Tests the Init method for an output basic signal (i.e. no ranges and no samples) in buffer index = 0
     */
    bool TestInit_Output_0();

    /**
     * @brief Tests the Init method for an output basic signal (i.e. no ranges and no samples) in buffer index = 1
     */
    bool TestInit_Output_1();

    /**
     * @brief Tests the Init method for an output signal with ranges in buffer index = 0
     */
    bool TestInit_Output_Ranges_0();

    /**
     * @brief Tests the Init method for an output signal with ranges in buffer index = 1
     */
    bool TestInit_Output_Ranges_1();

    /**
     * @brief Tests the Init method for an output signal with samples in buffer index = 0
     */
    bool TestInit_Output_Samples_0();

    /**
     * @brief Tests the Init method for an output signal with samples in buffer index = 0
     */
    bool TestInit_Output_Samples_1();

    /**
     * @brief Tests the Init method and verifies that the buffers are correctly setup after state change is requested.
     */
    bool TestInit_PrepareNextState();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPBROKERTEST_H_ */

