/**
 * @file BasicUARTGTest.cpp
 * @brief Source file for class BasicUARTGTest.
 * @date 17/08/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class BasicUARTGTest (public, protected, and private). Be aware that
 * some methods, such as those inline could be defined on the header file,
 * instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicUARTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L1Portability_BasicUARTGTest,TestConstructor) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestRead) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestRead());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestReadTimeout) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestReadTimeout());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestClose) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestClose());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestOpen) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestOpen());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestOpen_WrongName) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestOpen_WrongName());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestSetSpeed) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestSetSpeed());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestSetSpeed_False) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestSetSpeed_False());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestSetSpeed_False_Open) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestSetSpeed_False_Open());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestWaitRead) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestWaitRead());
}

TEST(FileSystem_L1Portability_BasicUARTGTest,TestWrite) {
    BasicUARTTest test;
    ASSERT_TRUE(test.TestWrite());
}
