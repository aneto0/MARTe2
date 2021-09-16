/**
 * @file ConfigurationDatabaseGTest.cpp
 * @brief Source file for class ConfigurationDatabaseGTest
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

 * @details This source file contains the definition of all the methods for
 * the class ConfigurationDatabaseGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "IntrospectionStructureTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestStructureRegistration) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestStructureRegistration());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestStructureRegistration_TypeAlreadyRegistered) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestStructureRegistration_TypeAlreadyRegistered());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestStructureRegistration_False_TypeAlreadyRegistered_Class) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestStructureRegistration_False_TypeAlreadyRegistered_Class());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestStructureRegistration_False_TypeNoParameters) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestStructureRegistration_False_TypeNoParameters());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestRegisterStructuredDataI) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestRegisterStructuredDataI());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestRegisterStructuredDataI_TypeDeclared) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestRegisterStructuredDataI_TypeDeclared());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestRegisterStructuredDataI_ArraysStructures) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestRegisterStructuredDataI_ArraysStructures());
}

TEST(BareMetal_L4Configuration_IntrospectionStructureGTest,TestRegisterStructuredDataI_ArraysStructures_TypeDeclared) {
    IntrospectionStructureTest test;
    ASSERT_TRUE(test.TestRegisterStructuredDataI_ArraysStructures_TypeDeclared());
}
