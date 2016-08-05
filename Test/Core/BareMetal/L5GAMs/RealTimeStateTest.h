/**
 * @file RealTimeStateTest.h
 * @brief Header file for class RealTimeStateTest
 * @date 07/03/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class RealTimeStateTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMESTATETEST_H_
#define REALTIMESTATETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeState.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeState functions.
 */
class RealTimeStateTest {

public:

    /**
     * @brief Creates a configuration database used in tests.
     */
    RealTimeStateTest();

    /**
     * @brief Destroys the ObjectRegistryDatabase.
     */
    ~RealTimeStateTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests if the the function inserts statefuls in the internal container
     * checking if they already are present to avoi duplications
     */
    bool TestAddStatefuls();

    /**
     * @brief Tests if the function fails trying to add a not StatefulI reference
     */
    bool TestAddStatefulsFalse_InvalidStateful();

    /**
     * @brief Tests if the function refreshes the context of the StatefulI involved in the next state
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests if the function returns how many StatefulI are involved in this state
     */
    bool TestGetNumberOfStatefuls();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMESTATETEST_H_ */

