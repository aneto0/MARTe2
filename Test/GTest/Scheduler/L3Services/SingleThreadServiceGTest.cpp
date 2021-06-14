/**
 * @file SingleThreadServiceGTest.cpp
 * @brief Source file for class SingleThreadServiceGTest
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


#include <SingleThreadServiceTest.h>
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestConstructor) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestDefaultConstructor_Template) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestInitialise) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestInitialise_False) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestStart) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestStart_False) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestStart_Restart) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestStop) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestStop_Kill) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}


TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestSetTimeout) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetTimeout());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest,TestGetTimeout) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetTimeout());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestGetPriorityClass) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetPriorityClass());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestGetPriorityLevel) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetPriorityLevel());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestGetStackSize) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetStackSize());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestGetCPUMask) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetCPUMask());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetPriorityClass) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetPriorityLevel) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetStackSize) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSize());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetCPUMask) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetPriorityClass_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass_Start());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetPriorityLevel_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel_Start());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetStackSize_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetStackSize_Start());
}

TEST(Scheduler_L3Services_SingleThreadServiceGTest, TestSetCPUMask_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask_Start());
}
