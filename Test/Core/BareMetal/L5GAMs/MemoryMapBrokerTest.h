/**
 * @file MemoryMapBrokerTest.h
 * @brief Header file for class MemoryMapBrokerTest
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

 * @details This header file contains the declaration of the class MemoryMapBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPBROKERTEST_H_
#define MEMORYMAPBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the MemoryMapBrokerTest public methods.
 */
class MemoryMapBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Init method for an input basic signal (i.e. no ranges and no samples)
     */
    bool TestInit_Input();

    /**
     * @brief Tests the Init method for an input signal with ranges
     */
    bool TestInit_Input_Ranges();

    /**
     * @brief Tests the Init method for an input signal with samples
     */
    bool TestInit_Input_Samples();

    /**
     * @brief Tests the Init method for an output basic signal (i.e. no ranges and no samples)
     */
    bool TestInit_Output();

    /**
     * @brief Tests the Init method for an output signal with ranges
     */
    bool TestInit_Output_Ranges();

    /**
     * @brief Tests the Init method for an output signal with samples
     */
    bool TestInit_Output_Samples();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPBROKERTEST_H_ */

