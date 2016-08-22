/**
 * @file MemoryMapStatefulOutputBrokerTest.h
 * @brief Header file for class MemoryMapStatefulOutputBrokerTest
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

 * @details This header file contains the declaration of the class MemoryMapStatefulOutputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSTATEFULOUTPUTROKERTEST_H_
#define MEMORYMAPSTATEFULOUTPUTROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the MemoryMapStatefulOutputBroker public methods.
 */
class MemoryMapStatefulOutputBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute method for RealTimeApplication::index = 0.
     */
    bool TestExecute_0();

    /**
     * @brief Tests the Execute method for RealTimeApplication::index = 1.
     */
    bool TestExecute_1();

    /**
     * @brief Tests the Execute method after changing state.
     */
    bool TestExecute_ChangeState();

    /**
     * @brief Tests the Execute method with a signal with ranges for RealTimeApplication::index = 0.
     */
    bool TestExecute_Ranges_0();

    /**
     * @brief Tests the Execute method with a signal with ranges for RealTimeApplication::index = 1.
     */
    bool TestExecute_Ranges_1();

    /**
     * @brief Tests the Execute method with a signal with ranges after changing state.
     */
    bool TestExecute_Ranges_ChangeState();

    /**
     * @brief Tests the Execute method with a signal with samples for RealTimeApplication::index = 0.
     */
    bool TestExecute_Samples_0();

    /**
     * @brief Tests the Execute method with a signal with samples for RealTimeApplication::index = 1.
     */
    bool TestExecute_Samples_1();

    /**
     * @brief Tests the Execute method with a signal with samples after changing state.
     */
    bool TestExecute_Samples_ChangeState();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPOUTPUTROKERTEST_H_ */

