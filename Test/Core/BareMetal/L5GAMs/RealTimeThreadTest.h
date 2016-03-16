/**
 * @file RealTimeThreadTest.h
 * @brief Header file for class RealTimeThreadTest
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

 * @details This header file contains the declaration of the class RealTimeThreadTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADTEST_H_
#define REALTIMETHREADTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeThread.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class RealTimeThreadTest {

public:

    RealTimeThreadTest();

    ~RealTimeThreadTest();

    bool TestConstructor();

    bool TestInitialise();

    bool TestConfigureArchitecture();

    bool TestConfigureArchitecture_GAMGroup();

    bool TestConfigureArchitecture_ReferenceContainer();

    bool TestConfigureArchitecture_SingleInGAMGroup();

    bool TestGetNumberOfFunctions();

    bool TestGetFunctions();

    bool TestGetGAMs();

    bool TestGetNumberOfGAMs();

    bool TestToStructuredData();

private:
    ConfigurationDatabase cdb;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADTEST_H_ */

