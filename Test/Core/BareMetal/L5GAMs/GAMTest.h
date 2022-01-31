/**
 * @file GAMTest.h
 * @brief Header file for class GAMTest
 * @date 25/04/2016
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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the GAM public methods.
 */
class GAMTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function initialises the gam from a StructuredDataI in input.
     */
    bool TestInitialise();

    /**
     * @brief Tests the AddSignals method.
     */
    bool TestAddSignals();

    /**
     * @brief Tests the GetQualifiedName method.
     */
    bool TestGetQualifiedName();

    /**
     * @brief Tests the SetConfiguredDatabase method (this is tested implicitly)
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the GetNumberOfInputSignals method.
     */
    bool TestGetNumberOfInputSignals();

    /**
     * @brief Tests the GetNumberOfOutputSignals method.
     */
    bool TestGetNumberOfOutputSignals();

    /**
     * @brief Tests the GetSignalName method.
     */
    bool TestGetSignalName();

    /**
     * @brief Tests the GetSignalIndex method.
     */
    bool TestGetSignalIndex();

    /**
     * @brief Tests the GetSignalDataSourceName method.
     */
    bool TestGetSignalDataSourceName();

    /**
     * @brief Tests the GetSignalType method.
     */
    bool TestGetSignalType();

    /**
     * @brief Tests the GetSignalNumberOfDimensions method.
     */
    bool TestGetSignalNumberOfDimensions();

    /**
     * @brief Tests the GetSignalNumberOfElements method.
     */
    bool TestGetSignalNumberOfElements();

    /**
     * @brief Tests the GetSignalByteSize method.
     */
    bool TestGetSignalByteSize();

    /**
     * @brief Tests the GetSignalDefaultValue method.
     */
    bool TestGetSignalDefaultValue();

    /**
     * @brief Tests the GetSignalNumberOfByteOffsets method.
     */
    bool TestGetSignalNumberOfByteOffsets();

    /**
     * @brief Tests the GetSignalByteOffsetInfo method.
     */
    bool TestGetSignalByteOffsetInfo();

    /**
     * @brief Tests the GetSignalNumberOfRanges method.
     */
    bool TestGetSignalNumberOfRanges();

    /**
     * @brief Tests the GetSignalRangesInfo method.
     */
    bool TestGetSignalRangesInfo();

    /**
     * @brief Tests the GetSignalNumberOfSamples method.
     */
    bool TestGetSignalNumberOfSamples();

    /**
     * @brief Tests the GetSignalFrequency_Input method.
     */
    bool TestGetSignalFrequency_Input();

    /**
     * @brief Tests the GetSignalFrequency_Output method.
     */
    bool TestGetSignalFrequency_Output();

    /**
     * @brief Tests the AllocateInputSignalsMemory method.
     */
    bool TestAllocateInputSignalsMemory();

    /**
     * @brief Tests the AllocateOutputSignalsMemory method.
     */
    bool TestAllocateOutputSignalsMemory();

    /**
     * @brief Tests the GetInputSignalsMemory method.
     */
    bool TestGetInputSignalsMemory();

    /**
     * @brief Tests the GetOutputSignalsMemory method.
     */
    bool TestGetOutputSignalsMemory();

    /**
     * @brief Tests the GetInputSignalMemory method.
     */
    bool TestGetInputSignalMemory();

    /**
     * @brief Tests the GetOutputSignalMemory method.
     */
    bool TestGetOutputSignalMemory();

    /**
     * @brief Tests the GetInputSignalMemory_Samples method.
     */
    bool TestGetInputSignalMemory_Samples();

    /**
     * @brief Tests the GetOutputSignalMemory_Samples method.
     */
    bool TestGetOutputSignalMemory_Samples();

    /**
     * @brief Tests the AddInputBrokers method.
     */
    bool TestAddInputBrokers();

    /**
     * @brief Tests the AddInputBrokers with an invalid broker.
     */
    bool TestAddInputBrokers_InvalidBroker();

    /**
     * @brief Tests the AddOutputBrokers method.
     */
    bool TestAddOutputBrokers();

    /**
     * @brief Tests the AddOutputBrokers with an invalid broker.
     */
    bool TestAddOutputBrokers_InvalidBroker();

    /**
     * @brief Tests the SortBrokers method.
     */
    bool TestSortBrokers();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the ExportData method.
     */
    bool TestExportData();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMTEST_H_ */

