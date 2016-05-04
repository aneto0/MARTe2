/**
 * @file GAMSampledSignalTest.h
 * @brief Header file for class GAMSampledSignalTest
 * @date 12/04/2016
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

 * @details This header file contains the declaration of the class GAMSampledSignalTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSAMPLEDSIGNALTEST_H_
#define GAMSAMPLEDSIGNALTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSampledSignal.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeSampledData functions
 */
class GAMSampledSignalTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function initialises correctly the definition from a StructuredDataI in input.
     */
    bool TestInitialise(StructuredDataI & data);

    /**
     * @brief Tests if the function sets the number of samples from local data.
     */
    bool TestMergeWithLocal_AddSamples();

    /**
     * @brief Tests if the function sets the number of samples per cycle from local data.
     */
    bool TestMergeWithLocal_AddCycles();

    /**
     * @brief Tests if the function sets the path from local data.
     */
    bool TestMergeWithLocal_AddPath();

    /**
     * @brief Tests if the function sets the type reading from local data.
     */
    bool TestMergeWithLocal_AddType();

    /**
     * @brief Tests if the function sets the dimensions field reading from local data.
     */
    bool TestMergeWithLocal_AddDimensions();

    /**
     * @brief Tests if the function sets the modifiers reading from local data.
     */
    bool TestMergeWithLocal_AddOperation();

    /**
     * @brief Tests if the function fails if the definition is declared final.
     */
    bool TestMergeWithLocalFalse_FinalGlobal();

    /**
     * @brief Tests if when the number of samples is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentSamples();

    /**
     * @brief Tests if when the number of samples per cycle is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentCycles();

    /**
     * @brief Tests if when the type is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentType();

    /**
     * @brief Tests if when the path is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentPath();

    /**
     * @brief Tests if when the modifiers is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentOperation();

    /**
     * @brief Tests if when the dimensions is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentDimensions();

    /**
     * @brief Tests if the function returns true if the type matches its introspection.
     */
    bool TestVerify();

    /**
     * @brief Tests if the function fails if the type is not registered.
     */
    bool TestVerifyFalse_UnregisteredType();

    /**
     * @brief Tests if the function fails if the introspection for the type is does not exist.
     */
    bool TestVerifyFalse_UnintrospectableType();

    /**
     * @brief Tests if the function fails if the type is empty.
     */
    bool TestVerifyFalse_EmptyType();

    /**
     * @brief Tests if the function fails for multi-dimensional structured types.
     */
    bool TestVerifyFalse_UnsupportedMultiDim();

    /**
     * @brief Tests if the function returns the number of samples defined.
     */
    bool TestGetSamples();

    /**
     * @brief Tests if the function returns the number of samples per cycle defined.
     */
    bool TestGetCycles();

    /**
     * @brief Tests if the function performs correctly the conversion to a StructuredDataI.
     */
    bool TestToStructuredData();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSAMPLEDSIGNALTEST_H_ */

