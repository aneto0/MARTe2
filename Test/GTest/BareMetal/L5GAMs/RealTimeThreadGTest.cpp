/**
 * @file RealTimeThreadGTest.cpp
 * @brief Source file for class RealTimeThreadGTest
 * @date 07/03/2016
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
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeThreadGTest,TestConstructor) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConstructor());
}

TEST(RealTimeThreadGTest,TestInitialise) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestInitialise());
}

TEST(RealTimeThreadGTest,TestInitialiseFalse_NoFunctions) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestInitialiseFalse_NoFunctions());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitecture());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_GAMGroup) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitecture_GAMGroup());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_ReferenceContainer) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitecture_ReferenceContainer());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_SingleInGAMGroup) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitecture_SingleInGAMGroup());
}

TEST(RealTimeThreadGTest,TestConfigureArchitecture_MoreGAMs) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitecture_MoreGAMs());
}

TEST(RealTimeThreadGTest,TestConfigureArchitectureFalse_OrphanGAM) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitectureFalse_OrphanGAM());
}

TEST(RealTimeThreadGTest,TestConfigureArchitectureFalse_InvalidGAMType) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitectureFalse_InvalidGAMType());
}

TEST(RealTimeThreadGTest,TestConfigureArchitectureFalse_InvalidGAMPath) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestConfigureArchitectureFalse_InvalidGAMPath());
}

TEST(RealTimeThreadGTest,TestGetNumberOfFunctions) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestGetNumberOfFunctions());
}

TEST(RealTimeThreadGTest,TestGetFunctions) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestGetFunctions());
}

TEST(RealTimeThreadGTest,TestGetGAMs) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestGetGAMs());
}

TEST(RealTimeThreadGTest,TestGetNumberOfGAMs) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestGetGAMs());
}

TEST(RealTimeThreadGTest,TestToStructuredData) {
    RealTimeThreadTest rttTest;
    ASSERT_TRUE(rttTest.TestToStructuredData());
}
