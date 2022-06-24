/**
 * @file StructuredDataIHelperTest.h
 * @brief Header file for class StructuredDataIHelperTest
 * @date 09/06/2022
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

 * @details This header file contains the declaration of the class StructuredDataIHelperTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRUCTURUREDDATAIHELPERTEST_H_
#define STRUCTURUREDDATAIHELPERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StructuredDataIHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ConfigurationDatabase functions.
 */
class StructuredDataIHelperTest {
public:
    /**
     * @brief Tests that the default constructor has no errors if the reference is valid.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the ResetErrors method.
     */
    bool TestResetErrors();

    /**
     * @brief Tests the HasErrors method.
     */
    bool TestHasErrors();

    /**
     * @brief Tests the Read method.
     */
    bool TestRead();

    /**
     * @brief Tests the Read method against a parameter that does not exist.
     */
    bool TestRead_False();

    /**
     * @brief Tests the Read method using a default value.
     */
    bool TestRead_DefaultValue();

    /**
     * @brief Tests the Read method using a default value that cannot be converted.
     */
    bool TestRead_DefaultValue_BadConversion();

    /**
     * @brief Tests the ReadEnum method.
     */
    bool TestReadEnum();

    /**
     * @brief Tests the ReadEnum method with a default value
     */
    bool TestReadEnum_DefaultValue();

    /**
     * @brief Tests the ReadEnum method with no default value.
     */
    bool TestReadEnum_False_NoDefaultValue();

    /**
     * @brief Tests the ReadEnum method with an invalid value (an option that does not exist).
     */
    bool TestReadEnum_False_InvalidOption();

    /**
     * @brief Tests the ReadEnum method with a size mismatch on the option values.
     */
    bool TestReadEnum_False_OptionSizeMismatch();

    /**
     * @brief Tests the ReadEnum method with forcing a failed conversion.
     */
    bool TestReadEnum_False_BadConversion();

    /**
     * @brief Tests the ReadValidated method.
     */
    bool TestReadValidated();

    /**
     * @brief Tests that the ReadValidated method detects an invalid condition.
     */
    bool TestReadValidated_InvalidCondition();

    /**
     * @brief Tests the ReadArray method.
     */
    bool TestReadArray();

    /**
     * @brief Tests the ReadArray method without specifying an input parameter.
     */
    bool TestReadArray_False();

    /**
     * @brief Tests the ReadMatrix method.
     */
    bool TestReadMatrix();

    /**
     * @brief Tests the ReadMatrix method without specifying an input parameter.
     */
    bool TestReadMatrix_False();

    /**
     * @brief Tests the ReadMatrix method with a wrong number of dimensions.
     */
    bool TestReadMatrix_False_Dimensions();

    /**
     * @brief Tests the Write method.
     */
    bool TestWrite();

    /**
     * @brief Tests the Copy method.
     */
    bool TestCopy();

    /**
     * @brief Tests the GetType method.
     */
    bool TestGetType();

    /**
     * @brief Tests the MoveToRoot method.
     */
    bool TestMoveToRoot();

    /**
     * @brief Tests the MoveToAncestor method.
     */
    bool TestMoveToAncestor();

    /**
     * @brief Tests the MoveAbsolute method.
     */
    bool TestMoveAbsolute();

    /**
     * @brief Tests the MoveRelative method.
     */
    bool TestMoveRelative();

    /**
     * @brief Tests the MoveToChild method.
     */
    bool TestMoveToChild();

    /**
     * @brief Tests the CreateAbsolute method.
     */
    bool TestCreateAbsolute();

    /**
     * @brief Tests the CreateRelative method.
     */
    bool TestCreateRelative();

    /**
     * @brief Tests the AddToCurrentNode method.
     */
    bool TestAddToCurrentNode();

    /**
     * @brief Tests the Delete method.
     */
    bool TestDelete();

    /**
     * @brief Tests the GetName method.
     */
    bool TestGetName();

    /**
     * @brief Tests the GetChildName method.
     */
    bool TestGetChildName();

    /**
     * @brief Tests the GetNumberOfChildren method.
     */
    bool TestGetNumberOfChildren();
};
#endif /* STRUCTURUREDDATAIHELPERTEST_H_ */

