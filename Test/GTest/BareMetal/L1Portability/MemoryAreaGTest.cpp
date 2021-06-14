/**
 * @file MemoryAreaGTest.cpp
 * @brief Source file for class MemoryAreaGTest
 * @date 10/03/2016
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
 * the class MemoryAreaGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "MemoryAreaTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestConstructor) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestConstructor());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestAdd_OnlySize) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestAdd_OnlySize());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestAdd_Element) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestAdd_Element());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestFree) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestFree());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestGetMemoryStart) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestGetMemoryStart());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestGetMemorySize_32) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestGetMemorySize(32));
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestGetMemorySize_0) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestGetMemorySize(0));
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestGetPointer) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestGetPointer());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestInitMemory) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestInitMemory());
}

TEST(BareMetal_L1Portability_MemoryAreaGTest,TestInitMemoryFalse_AlreadyInit) {
    MemoryAreaTest MyMemoryAreaTest;
    ASSERT_TRUE(MyMemoryAreaTest.TestInitMemoryFalse_AlreadyInit());
}
