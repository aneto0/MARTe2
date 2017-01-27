/**
 * @file MemoryMapSynchronisedOutputBroker.h
 * @brief Header file for class MemoryMapSynchronisedOutputBroker
 * @date Nov 18, 2016 TODO Verify the value and format of the date
 * @author aneto TODO Verify the name and format of the author
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

 * @details This header file contains the declaration of the class MemoryMapSynchronisedOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_BAREMETAL_L5GAMS_MEMORYMAPSYNCHRONISEDOUTPUTBROKERTEST_H_
#define TEST_CORE_BAREMETAL_L5GAMS_MEMORYMAPSYNCHRONISEDOUTPUTBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the MemoryMapSynchronisedOutputBroker public methods.
 */
class MemoryMapSynchronisedOutputBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with the DataSourceI::Synchronise returning false.
     */
    bool TestExecute_False();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_BAREMETAL_L5GAMS_MEMORYMAPSYNCHRONISEDOUTPUTBROKERTEST_H_ */

