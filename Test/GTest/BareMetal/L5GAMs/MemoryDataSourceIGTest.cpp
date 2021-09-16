/**
 * @file MemoryDataSourceIGTest.cpp
 * @brief Source file for class MemoryDataSourceIGTest
 * @date 11/05/2018
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
 * the class MemoryDataSourceIGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryDataSourceITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestConstructor) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestInitialise) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestInitialise_NumberOfBuffers) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_NumberOfBuffers());
}

TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestInitialise_BadHeapName) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_BadHeapName());
}

TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestAllocateMemory) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestGetNumberOfMemoryBuffers) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(BareMetal_L5GAMs_MemoryDataSourceIGTest,TestGetSignalMemoryBuffer) {
    MemoryDataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

