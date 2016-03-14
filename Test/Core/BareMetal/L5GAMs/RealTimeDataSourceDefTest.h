/**
 * @file RealTimeDataSourceDefTest.h
 * @brief Header file for class RealTimeDataSourceDefTest
 * @date 04/02/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceDefTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCEDEFTEST_H_
#define REALTIMEDATASOURCEDEFTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeDataSourceDef.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class RealTimeDataSourceDefTest {

public:

    bool TestConstructor();

    bool TestAddConsumer();

    bool TestAddProducer();

    bool TestGetNumberOfConsumers();

    bool TestGetNumberOfProducers();

    bool TestVerifyTrue();

    bool TestVerifyFalse_MoreThanOneProducer();

    bool TestVerifyNoConsumers();

    bool TestGetType();

    bool TestSetType();

    bool TestAllocate_Basic();

    bool TestAllocate_Structured();

    bool TestAllocate_Unintrospectable();

    bool TestAllocateFalse_Invalid();

    bool TestGetDataSourcePointer();

    bool TestPrepareNextState_Basic_ContinueVar();

    bool TestPrepareNextState_Basic_DeadVar();

    bool TestPrepareNextState_Structured_ContinueVar();

    bool TestPrepareNextState_Structured_DeadVar();

    bool TestSetDefaultValue();

    bool TestGetDefaultValue();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCEDEFTEST_H_ */

