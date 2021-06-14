/**
 * @file MultiClientServiceGTest.cpp
 * @brief Source file for class MultiClientServiceGTest
 * @date 19/09/2016
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
 * the class MessageGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include <MultiClientServiceTest.h>
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestDefaultConstructor) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestDefaultConstructor_Template) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestInitialise) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestInitialise_False_Timeout) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_Timeout());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestInitialise_False_MaxNumberOfThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MaxNumberOfThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestInitialise_False_MinNumberOfThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MinNumberOfThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestInitialise_False_MinGreaterThanMax) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MinGreaterThanMax());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestInitialise_False_MinNumberOfThreads_0) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MinNumberOfThreads_0());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestStart_False) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestStart_Restart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestStart_NotEnoughThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart_NotEnoughThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestStop) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestStop_Kill) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestGetMinimumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestGetMinimumNumberOfPoolThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads_0) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads_0());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads_AfterStart());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads_MinGreaterThanMax) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads_MinGreaterThanMax());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestGetMaximumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestGetMaximumNumberOfPoolThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMaximumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMaximumNumberOfPoolThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMaximumNumberOfPoolThreads_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMaximumNumberOfPoolThreads_AfterStart());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestSetMaximumNumberOfPoolThreads_MinGreaterThanMax) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMaximumNumberOfPoolThreads_MinGreaterThanMax());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestTestGetNumberOfActiveThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestGetNumberOfActiveThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestAddThread) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestAddThread());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestRemoveThread) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestRemoveThread());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest,TestMoreThanEnoughThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestMoreThanEnoughThreads());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest, TestSetPriorityClass) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest, TestSetPriorityLevel) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest, TestSetCPUMask) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest, TestSetPriorityClass_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass_AfterStart());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest, TestSetPriorityLevel_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel_AfterStart());
}

TEST(Scheduler_L3Services_MultiClientServiceGTest, TestSetCPUMask_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask_AfterStart());
}

