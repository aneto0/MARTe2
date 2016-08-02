/**
 * @file MemoryMapStatefulOutputBrokerGTest.cpp
 * @brief Source file for class MemoryMapStatefulOutputBrokerGTest
 * @date 30/07/2016
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
 * the class MemoryMapStatefulOutputBrokerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulOutputBrokerTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MemoryMapStatefulOutputBrokerGTest,TestConstructor) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_0) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_1) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_ChangeState) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_ChangeState());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_Ranges_0) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Ranges_0());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_Ranges_1) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Ranges_1());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_Ranges_ChangeState) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Ranges_ChangeState());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_Samples_0) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Samples_0());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_Samples_1) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Samples_1());
}

TEST(MemoryMapStatefulOutputBrokerGTest,TestExecute_Samples_ChangeState) {
    MemoryMapStatefulOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Samples_ChangeState());
}
