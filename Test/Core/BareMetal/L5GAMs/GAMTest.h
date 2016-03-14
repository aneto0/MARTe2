/**
 * @file GAMTest.h
 * @brief Header file for class GAMTest
 * @date 18/02/2016
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

 * @details This header file contains the declaration of the class GAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMTEST_H_
#define GAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class GAMTest {

public:

    GAMTest();

    ~GAMTest();


    bool TestConstructor();

    bool TestInitialise();


    bool TestSetApplication();

    bool TestSetGAMGroup();

    bool TestAddState();

    bool TestGetNumberOfSupportedStates();

    bool TestGetNumberOfSupportedStates_GAMGroup();

    bool TestGetSupportedStates();

    bool TestGetSupportedStates_GAMGroup();

    bool TestConfigureDataSource();

    bool TestConfigureDataSource_NotFinal();

    bool TestConfigureDataSourceFalse_Final();

    bool TestConfigureDataSourceLinks();

    bool TestExecute();
private:
    ConfigurationDatabase cdb;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMTEST_H_ */

