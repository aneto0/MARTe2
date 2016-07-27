/**
 * @file DataSourceITest.h
 * @brief Header file for class DataSourceITest
 * @date 24/07/2016
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

 * @details This header file contains the declaration of the class DataSourceITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCEITEST_H_
#define DATASOURCEITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

class DataSourceITest {
public:
    bool TestConstructor();

    bool TestInitialise();

    bool TestAddSignals();

    //This is tested implicitly
    bool TestSetConfiguredDatabase();

    bool TestGetNumberOfSignals();

    bool TestGetSignalIndex();

    bool TestGetSignalName();

    bool TestGetSignalType();

    bool TestGetSignalNumberOfDimensions();

    bool TestGetSignalNumberOfElements();

    bool TestGetSignalByteSize();

    bool TestGetSignalNumberOfStates();

    bool TestGetSignalStateName();

    bool TestGetSignalNumberOfConsumers();

    bool TestGetSignalNumberOfProducers();

    bool TestGetSignalConsumerName();

    bool TestGetSignalProducerName();

    bool TestGetSignalDefaultValue();

    bool TestGetNumberOfFunctions();

    bool TestGetFunctionName();

    bool TestGetFunctionIndex();

    bool TestGetFunctionNumberOfSignals();

    bool TestGetFunctionSignalsByteSize();

    bool TestGetFunctionSignalName();

    bool TestGetFunctionSignalIndex();

    bool TestGetFunctionSignalAlias();

    bool TestGetFunctionSignalNumberOfByteOffsets();

    bool TestGetFunctionSignalByteOffsetInfo();

    bool TestGetFunctionSignalSamples();

    bool TestGetFunctionSignalReadFrequencyInput();

    bool TestGetFunctionSignalReadFrequencyOutput();

    bool TestGetFunctionSignalGAMMemoryOffset();

    bool TestGetAddBrokers();

    bool TestIsSupportedBroker();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCEITEST_H_ */

