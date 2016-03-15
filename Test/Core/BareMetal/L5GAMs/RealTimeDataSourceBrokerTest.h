/**
 * @file RealTimeDataSourceBrokerTest.h
 * @brief Header file for class RealTimeDataSourceBrokerTest
 * @date 11/03/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCEBROKERTEST_H_
#define REALTIMEDATASOURCEBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeDataSourceBroker.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class RealTimeDataSourceBrokerTest {

public:

    RealTimeDataSourceBrokerTest();

    bool TestConstructor();

    bool TestSetApplication();

    bool TestAddVariable_Allocation();

    bool TestAddVariable_Static();

    bool TestAddVariableFalse_NoApplicationSet();

    bool TestAddVariableFalse_InvalidType();

    bool TestAddVariableFalse_UnintrospectableType();

    bool TestAddVariableFalse_NotInDataSource();

    bool TestAddVariable_MultiDimensional_Static_Vector();

    bool TestAddVariable_MultiDimensional_Allocation_Vector();

    bool TestAddVariable_MultiDimensional_Static_Matrix();

    bool TestAddVariable_MultiDimensional_Allocation_Matrix();

    bool TestGetData_Allocation();

    bool TestGetData_Static();

    bool TestGetMemoryPointer_Allocation();

    bool TestGetMemoryPointer_Static();

    bool TestFinalise();

private:
    ConfigurationDatabase cdb;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCEBROKERTEST_H_ */

