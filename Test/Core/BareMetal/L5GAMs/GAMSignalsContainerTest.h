/**
 * @file GAMSignalsContainerTest.h
 * @brief Header file for class GAMSignalsContainerTest
 * @date 13/04/2016
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

 * @details This header file contains the declaration of the class GAMSignalsContainerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSIGNALSCONTAINERTEST_H_
#define GAMSIGNALSCONTAINERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAMSignalsContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the GAMSignalsContainer functions.
 */
class GAMSignalsContainerTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function intialises correctly all the definitions from a StructuredDataI in input.
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function performs correctly the merging of all definitions contained with the StructuredDataI in input.
     */
    bool TestMergeWithLocal();

    /**
     * @brief Tests if the function can add a new definition.
     */
    bool TestMergeWithLocal_AddDefinition();

    /**
     * @brief Tests if the merging fails if the definition is declared final.
     */
    bool TestMergeWithLocalFalse_GlobalFinal();

    /**
     * @brief Tests if the function adds the IsInput flag to the definition reading from the local data.
     */
    bool TestMergeWithLocal_AddInput();

    /**
     * @brief Tests if the function adds the IsOutput flag to the definition reading from the local data.
     */
    bool TestMergeWithLocal_AddOutput();

    /**
     * @brief Tests if the function returns true if all the definitions matches with their introspections.
     */
    bool TestVerify();

    /**
     * @brief Tests if the function returns false if a definition does not match with its introspection.
     */
    bool TestVerify_DefInvalid();

    /**
     * @brief Tests if the function returns true if the definition container is for input and false otherwise.
     */
    bool TestIsInput();
    /**
     * @brief Tests if the function returns true if the definition container is for output and false otherwise.
     */
    bool TestIsOutput();

    /**
     * @brief Tests if the conversion to a StructuredData will be performed correctly.
     */
    bool TestToStructuredData();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSIGNALSCONTAINERTEST_H_ */

