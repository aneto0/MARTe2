/**
 * @file RealTimeApplicationGTest.cpp
 * @brief Source file for class RealTimeApplicationGTest
 * @date 04/08/2016
 * @author Giuseppe Ferro
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

#include "MARTe2UTest.h"

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
TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConstructor) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestInitialise) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestInitialiseFalse_NoStatesFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoStatesFound());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestInitialiseFalse_NoDataFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoDataFound());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestInitialiseFalse_NoFunctionsFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoFunctionsFound());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestInitialiseFalse_NoSchedulerFound) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestInitialiseFalse_NoSchedulerFound());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplication) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplication_False_ConfigureAfterInitialisation) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_ConfigureAfterInitialisation());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplication_False_PostConfigureDataSources) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_PostConfigureDataSources());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplication_False_ConfigureScheduler) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_ConfigureScheduler());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplication_False_AllocateGAMMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_AllocateGAMMemory());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplication_False_AllocateDataSourceMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplication_False_AllocateDataSourceMemory());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationNoInit) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationNoInit());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AssignBrokersToFunctions) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AssignBrokersToFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_PostConfigureDataSources) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_PostConfigureDataSources());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AllocateGAMMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AllocateGAMMemory());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AllocateDataSourceMemory) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AllocateDataSourceMemory());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_AddBrokersToFunctions) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_AddBrokersToFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestConfigureApplicationAfterInitialisation_False_ConfigureScheduler) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestConfigureApplicationAfterInitialisation_False_ConfigureScheduler());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestPrepareNextState) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestPrepareNextState_Message) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Message());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestStartNextStateExecution) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestStopCurrentStateExecution) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestStartNextStateExecution_Message) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution_Message());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestStopCurrentStateExecution_Message) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution_Message());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestCheckSameGamInMoreThreads) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestCheckSameGamInMoreThreads());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationGTest,TestCheckMultipleProducersWrites) {
    RealTimeApplicationTest test;
    ASSERT_TRUE(test.TestCheckMultipleProducersWrites());
}

