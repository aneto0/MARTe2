/**
 * @file BrokerIGTest.cpp
 * @brief Source file for class BrokerIGTest
 * @date 27/07/2016
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
 * the class BrokerIGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BrokerITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L5GAMs_BrokerIGTest,TestConstructor) {
    BrokerITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestInitFunctionPointers) {
    BrokerITest test;
    ASSERT_TRUE(test.TestInitFunctionPointers());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetNumberOfCopies) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetNumberOfCopies());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetNumberOfCopiesOptim) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetNumberOfCopiesOptim());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetCopyByteSize) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetCopyByteSize());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetCopyByteSizeOptim) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetCopyByteSizeOptim());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetCopyOffset) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetCopyOffset());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetCopyOffsetOptim) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetCopyOffsetOptim());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetFunctionPointer) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetFunctionPointer());
}


TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetFunctionPointerOptim) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetFunctionPointerOptim());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetOwnerFunctionName) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetOwnerFunctionName());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestGetOwnerDataSourceName) {
    BrokerITest test;
    ASSERT_TRUE(test.TestGetOwnerDataSourceName());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestInterleavedDSMemory) {
    BrokerITest test;
    ASSERT_TRUE(test.TestInterleavedDSMemory());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestInterleavedDSMemoryOptim) {
    BrokerITest test;
    ASSERT_TRUE(test.TestInterleavedDSMemoryOptim());
}

TEST(BareMetal_L5GAMs_BrokerIGTest,TestFlatDSMemory) {
    BrokerITest test;
    ASSERT_TRUE(test.TestFlatDSMemory());
}

