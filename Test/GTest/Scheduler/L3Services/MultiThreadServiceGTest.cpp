/**
 * @file MultiThreadServiceGTest.cpp
 * @brief Source file for class MultiThreadServiceGTest
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

#include <MultiThreadServiceTest.h>
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestDefaultConstructor) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestDefaultConstructor_Template) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_Timeout) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_Timeout());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_TypeConvert) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_TypeConvert());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_OutOfRangeIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_Invalid) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_Invalid());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_NotMatrix());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_Columns());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesLevel_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesLevel_OutOfRangeIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesLevel_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesLevel_NotMatrix());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_PrioritiesLevel_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesLevel_Columns());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_StackSizes_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSizes_OutOfRangeIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_StackSizes_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSizes_NotMatrix());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_StackSizes_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSizes_Columns());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_CPUMasks_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_CPUMasks_OutOfRangeIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_CPUMasks_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_CPUMasks_NotMatrix());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestInitialise_False_CPUMasks_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_CPUMasks_Columns());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestStart) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestStart_False) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestStart_Restart) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestCreateThreads) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestCreateThreads());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestCreateThreads_False) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestCreateThreads_False());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestCreateThreads_Restart) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestCreateThreads_Restart());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestStop) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestStop_Kill) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestGetStatus) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStatus());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestSetTimeout) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetTimeout());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestGetTimeout) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetTimeout());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestGetNumberOfPoolThreads) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetNumberOfPoolThreads());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest,TestSetNumberOfPoolThreads) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetNumberOfPoolThreads());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityClass) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityLevel) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetStackSize) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSize());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetCPUMask) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityClass_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityLevel_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetStackSize_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSize_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetCPUMask_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityClassThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClassThreadPool_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityLevelThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevelThreadPool_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetStackSizeThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSizeThreadPool_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetCPUMaskThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMaskThreadPool_Start());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityClassThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestSetPriorityClassThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetPriorityLevelThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestSetPriorityLevelThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetStackSizeThreadPool) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSizeThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetCPUMaskThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestSetCPUMaskThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetPriorityClassThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityClassThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetPriorityLevelThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityLevelThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetStackSizeThreadPool) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStackSizeThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetCPUMaskThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetCPUMaskThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetPriorityClassThreadPool_OutOfIndex) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityClassThreadPool_OutOfIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetPriorityLevelThreadPool_OutOfIndex) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityLevelThreadPool_OutOfIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetCPUMaskThreadPool_OutOfIndex) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetCPUMaskThreadPool_OutOfIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetStackSizeThreadPool_OutOfIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStackSizeThreadPool_OutOfIndex());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestGetThreadNameThreadPool) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetThreadNameThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetThreadNameThreadPool) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetThreadNameThreadPool());
}

TEST(Scheduler_L3Services_MultiThreadServiceGTest, TestSetThreadNameThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetThreadNameThreadPool_Start());
}

