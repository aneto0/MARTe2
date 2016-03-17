/**
 * @file RealTimeStateGTest.cpp
 * @brief Source file for class RealTimeStateGTest
 * @date 07/mar/2016
 * @author pc
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
 * the class RealTimeStateGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeStateTest.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeStateGTest,TestConstructor) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestConstructor());
}

TEST(RealTimeStateGTest,TestConfigureArchitecture) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestConfigureArchitecture());
}

TEST(RealTimeStateGTest,TestConfigureArchitectureFalse_NoThreads) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestConfigureArchitectureFalse_NoThreads());
}

TEST(RealTimeStateGTest,TestConfigureArchitecture_SingleInGAMGroup) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestConfigureArchitecture_SingleInGAMGroup());
}

TEST(RealTimeStateGTest,TestInsertFunction) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestInsertFunction());
}

TEST(RealTimeStateGTest,TestAddGAMGroup) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestAddGAMGroup());
}

TEST(RealTimeStateGTest,TestGetGAMGroups) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestGetGAMGroups());
}

TEST(RealTimeStateGTest,TestGetNumberOfGAMGroups) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestGetNumberOfGAMGroups());
}

TEST(RealTimeStateGTest,TestChangeState) {
    RealTimeStateTest rtsTest;
    ASSERT_TRUE(rtsTest.TestChangeState());
}

