/**
 * @file StaticStackGTest.cpp
 * @brief Source file for class StaticStackGTest
 * @date 06/04/2020
 * @author Dídac Magriñá
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
 * the class StaticStackGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "StaticStackTest.h"
#include "StreamString.h"


using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

uint32 demoValues[] = { 3, 7, 2, 5, 45, 3, 6, 7, 47, 3, 74, 5, 32, 91, 8, 66, 9 };

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_StaticStackGTest,TestDefaultConstructor) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestDefaultConstructor());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestPushOnEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPushOnEmptyStack<25u>());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestPushOnNonEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPushOnNonEmptyStack<25u>());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestPopOnEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPopOnEmptyStack());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestPopOnNonEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPopOnNonEmptyStack());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestGetSize) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestGetSize());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestCleanOnEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, 0> tester;
    ASSERT_TRUE(tester.TestClean());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestCleanOnNonEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestClean());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestGetAllocatedMemoryConstOnEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, 0> tester;
    ASSERT_TRUE(tester.TestGetAllocatedMemoryConst());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestGetAllocatedMemoryConstOnNonEmptyStack) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestGetAllocatedMemoryConst());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestPeek) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPeek());
}

TEST(BareMetal_L1Portability_StaticStackGTest,TestPeekInvalidPosition) {
    StaticStackTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPeekInvalidPosition());
}
