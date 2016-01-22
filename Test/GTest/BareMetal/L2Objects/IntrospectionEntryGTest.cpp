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

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "IntrospectionEntryTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(IntrospectionEntryGTest,TestConstructor) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestConstructor("memberx", "char8", true, "*C", sizeof(char8*), 4));
}

TEST(IntrospectionEntryGTest,TestGetMemberName) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberName("memberx"));
}

TEST(IntrospectionEntryGTest,TestGetMemberModifiers) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberModifiers("memberx"));
}

TEST(IntrospectionEntryGTest,TestGetMemberSize) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberSize(10));
}

TEST(IntrospectionEntryGTest,TestGetMemberByteOffset) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberByteOffset(10));
}

TEST(IntrospectionEntryGTest,TestGetMemberTypeDescriptor) {
    IntrospectionEntryTest IntrospectionEntryTest;
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberTypeDescriptor());
}


TEST(IntrospectionEntryGTest,TestIsConstant) {
    IntrospectionEntryTest IntrospectionEntryTest;
    static const IntrospectionEntryTestTable table[]{
            {true, "", 0, true},
            {false, "", 0, false},
            {true, "*", 1, false},
            {false, "*C", 1, true},
            {true, "**C", 1, false},
            {false, "**C", 2, true},
            {false, "*C*C", 1, true},
            {false, "*C*C", 2, true},
            {true, "**", 0, true},
            {true, "**", 1, false},
            {true, "**", 2, false},
            {true, "**", 3, true},
            {false, "**", 3, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(IntrospectionEntryTest.TestIsConstant(table));
}


TEST(IntrospectionEntryGTest,TestGetMemberPointerLevel) {
    IntrospectionEntryTest IntrospectionEntryTest;
    static const IntrospectionEntryTestTable table[]{
            {true, "", 0, true},
            {true, "*", 1, false},
            {false, "*C", 1, true},
            {true, "**C", 2, false},
            {false, "*C*C", 2, true},
            {true, "**", 2, true},
            {0,0,0,0}
    };
    ASSERT_TRUE(IntrospectionEntryTest.TestGetMemberPointerLevel(table));
}

