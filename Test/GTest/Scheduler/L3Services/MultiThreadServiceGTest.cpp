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
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MultiThreadServiceGTest,TestDefaultConstructor) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(MultiThreadServiceGTest,TestDefaultConstructor_Template) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(MultiThreadServiceGTest,TestInitialise) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_Timeout) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_Timeout());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_TypeConvert) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_TypeConvert());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_OutOfRangeIndex());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_Invalid) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_Invalid());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_NotMatrix());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesClass_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesClass_Columns());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesLevel_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesLevel_OutOfRangeIndex());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesLevel_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesLevel_NotMatrix());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_PrioritiesLevel_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_PrioritiesLevel_Columns());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_StackSizes_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSizes_OutOfRangeIndex());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_StackSizes_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSizes_NotMatrix());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_StackSizes_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSizes_Columns());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_CPUMasks_OutOfRangeIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_CPUMasks_OutOfRangeIndex());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_CPUMasks_NotMatrix) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_CPUMasks_NotMatrix());
}

TEST(MultiThreadServiceGTest,TestInitialise_False_CPUMasks_Columns) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_CPUMasks_Columns());
}

TEST(MultiThreadServiceGTest,TestStart) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(MultiThreadServiceGTest,TestStart_False) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(MultiThreadServiceGTest,TestStart_Restart) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(MultiThreadServiceGTest,TestCreateThreads) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestCreateThreads());
}

TEST(MultiThreadServiceGTest,TestCreateThreads_False) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestCreateThreads_False());
}

TEST(MultiThreadServiceGTest,TestCreateThreads_Restart) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestCreateThreads_Restart());
}

TEST(MultiThreadServiceGTest,TestStop) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(MultiThreadServiceGTest,TestStop_Kill) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(MultiThreadServiceGTest,TestGetStatus) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStatus());
}

TEST(MultiThreadServiceGTest,TestSetTimeout) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetTimeout());
}

TEST(MultiThreadServiceGTest,TestGetTimeout) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetTimeout());
}

TEST(MultiThreadServiceGTest,TestGetNumberOfPoolThreads) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetNumberOfPoolThreads());
}

TEST(MultiThreadServiceGTest,TestSetNumberOfPoolThreads) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetNumberOfPoolThreads());
}

TEST(MultiThreadServiceGTest, TestSetPriorityClass) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(MultiThreadServiceGTest, TestSetPriorityLevel) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(MultiThreadServiceGTest, TestSetStackSize) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSize());
}

TEST(MultiThreadServiceGTest, TestSetCPUMask) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(MultiThreadServiceGTest, TestSetPriorityClass_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass_Start());
}

TEST(MultiThreadServiceGTest, TestSetPriorityLevel_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel_Start());
}

TEST(MultiThreadServiceGTest, TestSetStackSize_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSize_Start());
}

TEST(MultiThreadServiceGTest, TestSetCPUMask_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask_Start());
}

TEST(MultiThreadServiceGTest, TestSetPriorityClassThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClassThreadPool_Start());
}

TEST(MultiThreadServiceGTest, TestSetPriorityLevelThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevelThreadPool_Start());
}

TEST(MultiThreadServiceGTest, TestSetStackSizeThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSizeThreadPool_Start());
}

TEST(MultiThreadServiceGTest, TestSetCPUMaskThreadPool_Start) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMaskThreadPool_Start());
}

TEST(MultiThreadServiceGTest, TestSetPriorityClassThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestSetPriorityClassThreadPool());
}

TEST(MultiThreadServiceGTest, TestSetPriorityLevelThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestSetPriorityLevelThreadPool());
}

TEST(MultiThreadServiceGTest, TestSetStackSizeThreadPool) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSizeThreadPool());
}

TEST(MultiThreadServiceGTest, TestSetCPUMaskThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestSetCPUMaskThreadPool());
}

TEST(MultiThreadServiceGTest, TestGetPriorityClassThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityClassThreadPool());
}

TEST(MultiThreadServiceGTest, TestGetPriorityLevelThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityLevelThreadPool());
}

TEST(MultiThreadServiceGTest, TestGetStackSizeThreadPool) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStackSizeThreadPool());
}

TEST(MultiThreadServiceGTest, TestGetCPUMaskThreadPool) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetCPUMaskThreadPool());
}

TEST(MultiThreadServiceGTest, TestGetPriorityClassThreadPool_OutOfIndex) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityClassThreadPool_OutOfIndex());
}

TEST(MultiThreadServiceGTest, TestGetPriorityLevelThreadPool_OutOfIndex) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetPriorityLevelThreadPool_OutOfIndex());
}

TEST(MultiThreadServiceGTest, TestGetCPUMaskThreadPool_OutOfIndex) {
    MultiThreadServiceTest target; 
    ASSERT_TRUE(target.TestGetCPUMaskThreadPool_OutOfIndex());
}

TEST(MultiThreadServiceGTest, TestGetStackSizeThreadPool_OutOfIndex) {
    MultiThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStackSizeThreadPool_OutOfIndex());
}

