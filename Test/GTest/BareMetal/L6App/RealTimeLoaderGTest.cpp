/**
 * @file RealTimeLoaderGTest.cpp
 * @brief Source file for class RealTimeLoaderGTest
 * @date 05/04/2018
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
 * the class RealTimeLoaderGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "RealTimeLoaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L6App_RealTimeLoaderGTest,TestConstructor) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestConfigure) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestConfigure());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestConfigure_MoreApplications) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestConfigure_MoreApplications());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestConfigure_False_FailedConfiguration) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedConfiguration());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestConfigure_False_FailedRealTimeApplicationConfiguration) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedRealTimeApplicationConfiguration());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestStart_False_FailedRealTimeApplicationStartNextStateExecutionConfiguration) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestStart_False_FailedRealTimeApplicationStartNextStateExecutionConfiguration());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestStart_False_FailedRealTimeApplicationPrepareNextStateConfiguration) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestStart_False_FailedRealTimeApplicationPrepareNextStateConfiguration());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestStart_FirstState) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestStart_FirstState());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestStart_Message) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestStart_Message());
}

TEST(BareMetal_L6App_RealTimeLoaderGTest,TestStop) {
    RealTimeLoaderTest test;
    ASSERT_TRUE(test.TestStop());
}

