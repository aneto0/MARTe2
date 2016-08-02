/**
 * @file RealTimeThreadGTest.cpp
 * @brief Source file for class RealTimeThreadGTest
 * @date 29/07/2016
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
 * the class RealTimeThreadGTest (public, protected, and private). Be aware that some
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
#include "RealTimeThreadTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(RealTimeThreadGTest,TestConstructor) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(RealTimeThreadGTest,TestInitialise) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(RealTimeThreadGTest,TestInitialiseFalse_NoFunctions) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoFunctions());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestConfigureArchitecture());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_OrphanGAM) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestConfigureArchitecture_OrphanGAM());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_InvalidGAMType) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestConfigureArchitecture_InvalidGAMType());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_InvalidGAMPath) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestConfigureArchitecture_InvalidGAMPath());
}

TEST(RealTimeThreadGTest,TestGetNumberOfFunctions) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetNumberOfFunctions());
}

TEST(RealTimeThreadGTest,TestGetFunctions) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetFunctions());
}

TEST(RealTimeThreadGTest,TestGetNumberOfGAMs) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetNumberOfGAMs());
}

TEST(RealTimeThreadGTest,TestGetGAMs) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetGAMs());
}

TEST(RealTimeThreadGTest,TestGetStackSize) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(RealTimeThreadGTest,TestGetCPU) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetCPU());
}
/*
TEST(RealTimeThreadGTest,TestGetGAMs) {
    RealTimeThreadTest test;
    ASSERT_TRUE(test.TestGetGAMs());
}*/
