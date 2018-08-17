/**
 * @file RealTimeApplicationGTest.cpp
 * @brief Source file for class RealTimeApplicationGTest
 * @date 04/ago/2016
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
 * the class RealTimeApplicationGTest (public, protected, and private). Be aware that some 
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
#include "RealTimeApplicationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(RealTimeApplicationGTest,TestConstructor) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(RealTimeApplicationGTest,TestInitialise) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(RealTimeApplicationGTest,TestInitialiseFalse_NoStatesFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoStatesFound());
}

TEST(RealTimeApplicationGTest,TestInitialiseFalse_NoDataFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoDataFound());
}

TEST(RealTimeApplicationGTest,TestInitialiseFalse_NoFunctionsFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoFunctionsFound());
}

TEST(RealTimeApplicationGTest,TestInitialiseFalse_NoSchedulerFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoSchedulerFound());
}

TEST(RealTimeApplicationGTest,TestConfigureApplication) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication());
}

TEST(RealTimeApplicationGTest,TestConfigureApplication_False_ConfigureAfterInitialisation) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_ConfigureAfterInitialisation());
}

TEST(RealTimeApplicationGTest,TestConfigureApplication_False_PostConfigureDataSources) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_PostConfigureDataSources());
}

TEST(RealTimeApplicationGTest,TestConfigureApplication_False_ConfigureScheduler) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_ConfigureScheduler());
}

TEST(RealTimeApplicationGTest,TestConfigureApplication_False_AllocateGAMMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_AllocateGAMMemory());
}

TEST(RealTimeApplicationGTest,TestConfigureApplication_False_AllocateDataSourceMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_AllocateDataSourceMemory());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationNoInit) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationNoInit());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AssignBrokersToFunctions) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AssignBrokersToFunctions());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_PostConfigureDataSources) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_PostConfigureDataSources());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AllocateGAMMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AllocateGAMMemory());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AllocateDataSourceMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AllocateDataSourceMemory());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AddBrokersToFunctions) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AddBrokersToFunctions());
}

TEST(RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_ConfigureScheduler) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_ConfigureScheduler());
}

TEST(RealTimeApplicationGTest,TestPrepareNextState) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(RealTimeApplicationGTest,TestPrepareNextState_Message) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Message());
}

TEST(RealTimeApplicationGTest,TestStartNextStateExecution) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution());
}

TEST(RealTimeApplicationGTest,TestStopCurrentStateExecution) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution());
}

TEST(RealTimeApplicationGTest,TestStartNextStateExecution_Message) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution_Message());
}

TEST(RealTimeApplicationGTest,TestStopCurrentStateExecution_Message) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution_Message());
}
