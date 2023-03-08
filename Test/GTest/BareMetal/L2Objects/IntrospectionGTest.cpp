/**
 * @file IntrospectionGTest.cpp
 * @brief Source file for class IntrospectionGTest
 * @date 18/01/2016
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

 * @details This source file contains the definition of all the methods for
 * the class IntrospectionGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "IntrospectionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(BareMetal_L2Objects_IntrospectionGTest,TestConstructor) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestConstructor());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestPositionOperator) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestPositionOperator());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestMacroToAddBasicInClassRegistryDatabase) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestMacroToAddBasicInClassRegistryDatabase());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestMacroToAddStructuredInClassRegistryDatabase) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestMacroToAddStructuredInClassRegistryDatabase());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestGetNumberOfMembers) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestGetNumberOfMembers());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestGetClassSize) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestGetClassSize());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestMacroToAddObjectClassRegistryDatabase) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestMacroToAddObjectClassRegistryDatabase());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestSetIntrospectionEntryMemory) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestSetIntrospectionEntryMemory());
}

TEST(BareMetal_L2Objects_IntrospectionGTest,TestGetIntrospectionEntryMemory) {
    IntrospectionTest IntrospectionTest;
    ASSERT_TRUE(IntrospectionTest.TestGetIntrospectionEntryMemory());
}

