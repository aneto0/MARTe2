/**
 * @file IntrospectionEntryGTest.cpp
 * @brief Source file for class IntrospectionEntryGTest
 * @date 20/01/2016
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
 * the class IntrospectionEntryGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "IntrospectionEntryTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestConstructor) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestConstructor("memberx", "char8",  "*C", "max=2", sizeof(char8*), 4));
}

TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberName) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberName("memberx"));
}

TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberModifiers) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberModifiers("memberx"));
}

TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberTypeName) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberTypeName("memberx"));
}

TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberSize) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberSize(10));
}

TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberByteOffset) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberByteOffset(10));
}

TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberTypeDescriptor) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberTypeDescriptor());
}


TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberAttributes) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberAttributes("memberx"));
}


TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestIsConstant) {
    IntrospectionEntryTest IntrospectionEntryTest;
    const IntrospectionEntryTestTable table[] = {
            {"C", 0, true},
            {"", 0, false},
            {"C*", 1, false},
            {"*C", 1, true},
            {"C**C", 1, false},
            {"**C", 2, true},
            {"*C*C", 1, true},
            {"*C*C", 2, true},
            {"C**", 0, true},
            {"C**", 1, false},
            {"C**", 2, false},
            {"C**", 3, true},
            {"**", 3, false},
            {0,0,0}
    };
    ASSERT_TRUE(IntrospectionEntryTest.TestIsConstant(table));
}


TEST(BareMetal_L2Objects_IntrospectionEntryGTest,TestGetMemberPointerLevel) {
    IntrospectionEntryTest IntrospectionEntryTest;
    const IntrospectionEntryTestTable table[] = {
            {"C", 0, true},
            {"C*", 1, true},
            {"*C", 1, true},
            {"C**C", 2, true},
            {"*C*C", 2, true},
            {"C**", 2, true},
            {0,0,0}
    };
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberPointerLevel(table));
}

