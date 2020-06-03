/**
 * @file IntrospectionStructureTest.h
 * @brief Header file for class IntrospectionStructureTest
 * @date 19/11/2018
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

 * @details This header file contains the declaration of the class IntrospectionStructureTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4CONFIGURATION_INTROSPECTIONSTRUCTURETEST_H_
#define L4CONFIGURATION_INTROSPECTIONSTRUCTURETEST_H_

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
 * @brief Tests the IntrospectionStructure public methods.
 */
class IntrospectionStructureTest {
public:
    /**
     * @brief Tests the registration of a complex structure.
     */
    bool TestStructureRegistration();

    /**
     * @brief Tests that the registration of a complex structure updates if the type is already registered.
     */
    bool TestStructureRegistration_TypeAlreadyRegistered();

    /**
     * @brief Tests that the registration of a complex structure fails if the type has no parameters.
     */
    bool TestStructureRegistration_False_TypeNoParameters();

    /**
     * @brief Tests that the registration of a complex structure fails to update if the type is from a class.
     */
    bool TestStructureRegistration_False_TypeAlreadyRegistered_Class();

    /**
     * @brief Tests the RegisterStructuredDataI method.
     */
    bool TestRegisterStructuredDataI();

    /**
     * @brief Tests the RegisterStructuredDataI with the type keyword declared.
     */
    bool TestRegisterStructuredDataI_TypeDeclared();

    /**
     * @brief Tests the RegisterStructuredDataI with arrays of nodes.
     */
    bool TestRegisterStructuredDataI_ArraysStructures();

    /**
     * @brief Tests the RegisterStructuredDataI with arrays of nodes and with the type keyword declared.
     */
    bool TestRegisterStructuredDataI_ArraysStructures_TypeDeclared();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4CONFIGURATION_INTROSPECTIONSTRUCTURETEST_H_ */
