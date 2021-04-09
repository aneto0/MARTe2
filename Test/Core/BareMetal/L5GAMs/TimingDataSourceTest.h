/**
 * @file TimingDataSourceTest.h
 * @brief Header file for class TimingDataSourceTest
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

 * @details This header file contains the declaration of the class TimingDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMINGDATASOURCETEST_H_
#define TIMINGDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimingDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the DataSourceI public methods.
 */
class TimingDataSourceTest {
public:
    /**
     * @brief Tests the Constructor method.
     */
    bool TestConstructor();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method and verifies that an error is issued if a producer was added to the TimingDataSource.
     */
    bool TestSetConfiguredDatabase_False_Producers();

    /**
    * @brief Tests the CurrentState ancillary signal generation and value change across states
    */
    bool TestCurrentStateValue();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMINGDATASOURCETEST_H_ */

