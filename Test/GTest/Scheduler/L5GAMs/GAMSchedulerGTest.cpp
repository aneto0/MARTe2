/**
 * @file GAMSchedulerGTest.cpp
 * @brief Source file for class GAMSchedulerGTest
 * @date 09/08/2018
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
 * the class GAMSchedulerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestConstructor) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestStartNextStateExecution) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestStartNextStateExecution_False_PrepareNextState) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution_False_PrepareNextState());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestStopCurrentStateExecution) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestIntegrated) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestIntegrated());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestIntegrated_TriggerErrorMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestIntegrated_TriggerErrorMessage());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestPurge) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestPurge());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestInitialise) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestInitialise_ErrorMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise_ErrorMessage());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestInitialise_False_InvalidMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise_False_InvalidMessage());
}

TEST(Scheduler_L5GAMs_GAMSchedulerGTest,TestInitialise_False_MoreThanOneErrorMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise_False_MoreThanOneErrorMessage());
}
