/**
 * @file GAMDataSourceTest.h
 * @brief Header file for class GAMDataSourceTest
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

 * @details This header file contains the declaration of the class GAMDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMDATASOURCETEST_H_
#define GAMDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the GAMDataSource public methods.
 */
class GAMDataSourceTest {
public:
    /**
     * @brief Tests the Constructor method.
     */
    bool TestConstructor();

    /**
     * @brief Tests the PrepareNextState method without any signal Default values.
     */
    bool TestPrepareNextState_NoDefault();

    /**
     * @brief Tests the PrepareNextState method using signal Default values which are basic types.
     */
    bool TestPrepareNextState_Default_Basic();

    /**
     * @brief Tests the PrepareNextState method using signal Default values which are multi-dimensional.
     */
    bool TestPrepareNextState_Default_Dimensions();

    /**
     * @brief Tests the PrepareNextState method using signal char8[].
     */
    bool TestPrepareNextState_Default_Char8Arr();

    /**
     * @brief Tests the PrepareNextStatemethod using signal Default values which have a dimension mismatch with respect to the signal dimension (number of rows).
     */
    bool TestPrepareNextState_Default_Dimensions_Mismatch_1();

    /**
     * @brief Tests the PrepareNextStatemethod using signal Default values which have a dimension mismatch with respect to the signal dimension (number of columns).
     */
    bool TestPrepareNextState_Default_Dimensions_Mismatch_2();

    /**
     * @brief Tests the PrepareNextStatemethod using signal Default values which have a number of elements mismatch with respect to the signal number of elements.
     */
    bool TestPrepareNextState_Default_Elements_Mismatch();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that an error is issued if a signal has no producers in one state.
     */
    bool TestSetConfiguredDatabase_NoProducer_State_1();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that an error is issued if a signal has no producers in another state.
     */
    bool TestSetConfiguredDatabase_NoProducer_State_2();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that an error is issued if a signal has more than one producer in one state.
     */
    bool TestSetConfiguredDatabase_MoreThanOneProducer_State_1();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that an error is issued if a signal has more than one producer in another state.
     */
    bool TestSetConfiguredDatabase_MoreThanOneProducer_State_2();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that an error is NOT issued if a GAM is not executed in any of the states.
     */
    bool TestSetConfiguredDatabase_GAM_Never_Executed();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that no error is issued if a signal has no producers in one state and AllowNoProducers is set.
     */
    bool TestSetConfiguredDatabase_NoProducer_State_1_AllowNoProducers();

    /**
     * @brief Tests the PrepareNextState method without reseting the default values at state transition.
     */
    bool TestPrepareNextState_NoResetUnusedVariables();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that no error is issued if a the signal is produced by different producers in different states.
     */
    bool TestSetConfiguredDatabase_MoreThanOneProducer_Different_States();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMDATASOURCETEST_H_ */

