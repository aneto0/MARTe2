/**
 * @file MemoryMapInterpolatedInputBrokerTest.h
 * @brief Header file for class MemoryMapInterpolatedInputBrokerTest
 * @date 24/08/2017
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

 * @details This header file contains the declaration of the class MemoryMapInterpolatedInputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKERTEST_H_
#define L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the MemoryMapInterpolatedInputBroker public methods.
 */
class MemoryMapInterpolatedInputBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute method with over sampling
     */
    bool TestExecute_1();

    /**
     * @brief Tests the Execute method with under sampling
     */
    bool TestExecute_2();

    /**
     * @brief Tests the Execute method with arrays
     */
    bool TestExecute_3();

    /**
     * @brief Tests that the Execute method returns false when the DataSource time does not change.
     */
    bool TestExecute_False();

    /**
     * @brief Tests that the Execute method returns false when the DataSource time does not change.
     */
    bool TestInit();

    /**
     * @brief Tests that the Execute method returns false when the DataSource time does not change.
     */
    bool TestReset();

    /**
     * @brief Tests that the Execute method returns false when the DataSource time does not change.
     */
    bool TestSetInterpolationSignal();


private:
    /**
     * @brief Helper to test the Execute method.
     */
    bool TestExecuteP(MARTe::StreamString config, MARTe::uint32 nExecutes = 5);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKERTEST_H_ */
