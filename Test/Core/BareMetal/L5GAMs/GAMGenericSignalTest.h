/**
 * @file GAMGenericSignalTest.h
 * @brief Header file for class GAMGenericSignalTest
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

 * @details This header file contains the declaration of the class GAMGenericSignalTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMGENERICSIGNALTEST_H_
#define GAMGENERICSIGNALTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMGenericSignal.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the GAMGenericSignal functions.
 */
class GAMGenericSignalTest {
public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function intialises correctly the definition from a StructuredDataI in input.
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function merges correctly the definition with the local data.
     */
    bool TestMergeWithLocal();

    /**
     * @brief Tests if the function fails if the definition is declared final.
     */
    bool TestMergeWithLocalFalse_FinalGlobal();

    /**
     * @brief Tests if the function fails if definition and local data are declare both final.
     */
    bool TestMergeWithLocalFalse_BothFinal();

    /**
     * @brief Tests if the function sets the type from the local data.
     */
    bool TestMergeWithLocal_AddType();

    /**
     * @brief Tests if the function sets the path from the local data.
     */
    bool TestMergeWithLocal_AddPath();

    /**
     * @brief Tests if the function sets the dimensions from the local data.
     */
    bool TestMergeWithLocal_AddDimensions();

    /**
     * @brief Tests if the function sets the modifiers form the local data.
     */
    bool TestMergeWithLocal_AddOperation();

    /**
     * @brief Tests if the function sets the default values from local data.
     */
    bool TestMergeWithLocal_AddDefaultValues();

    /**
     * @brief Tests if when the type is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentTypes();

    /**
     * @brief Tests if when the path is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentPaths();

    /**
     * @brief Tests if when the default value is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentDefaultValues();

    /**
     * @brief Tests if when the dimensions field is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentDimensions();

    /**
     * @brief Tests if when the modifiers is specified in both local and global definitions,
     * the global one will be considered.
     */
    bool TestMergeWithLocal_DifferentOperation();

    /**
     * @brief Tests if the function returns the default value.
     */
    bool TestGetDefaultValue();

    /**
     * @brief Tests if the function converts correctly the definition to a StructuredDataI.
     */
    bool TestToStructuredData();

    /**
     * @brief Tests if the function returns true if all the definitions math with their
     * introspection.
     */
    bool TestVerify();

    /**
     * @brief Tests the function with vector variables.
     */
    bool TestVerify_Vector();

    /**
     * @brief Tests the function with matrix variables.
     */
    bool TestVerify_Matrix();

    /**
     * @brief Tests if the function fails if the type defined is not registered.
     */
    bool TestVerifyFalse_UnregisteredType();

    /**
     * @brief Tests if the function fails if the introspection for the type does not exist.
     */
    bool TestVerifyFalse_UnintrospectableType();

    /**
     * @brief Tests if the function fails if the member name does not match with the introspection.
     */
    bool TestVerifyFalse_InvalidMemberName();

    /**
     * @brief Tests if the function fails if the member type does not match with the introspection.
     */
    bool TestVerifyFalse_InvalidMemberType();

    /**
     * @brief Tests if the function fails if the number of members does not match with the introspection.
     */
    bool TestVerifyFalse_InvalidNumberOfMembers();

    /**
     * @brief Tests if the function fails if the number of dimensions does not match with the introspection.
     */
    bool TestVerifyFalse_NDims();

    /**
     * @brief Tests if the function fails if the number of Elements does not match with the introspection.
     */
    bool TestVerifyFalse_NElements();

    /**
     * @brief Tests if the function if the definition represents a multi-dimensiona structured type.
     */
    bool TestVerifyFalse_UnsupportedMultiDim();

    /**
     * @brief Tests if the function fails if the type field is empty.
     */
    bool TestVerifyFalse_EmptyType();

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMGENERICSIGNALTEST_H_ */

