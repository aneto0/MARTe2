/**
 * @file EmbeddedThreadIGTest.cpp
 * @brief Source file for class EmbeddedThreadIGTest
 * @date 20/09/2016
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

#include "MARTe2UTest.h"
#include "EmbeddedThreadITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestConstructor) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestConstructorThreadNumber) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestConstructorThreadNumber());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetThreadNumber) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetThreadNumber());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetThreadId) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetThreadId());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetCommands) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetCommands());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestSetCommands) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetCommands());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetPriorityClass) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetPriorityClass());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetPriorityLevel) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetPriorityLevel());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetStackSize) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetStackSize());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestGetCPUMask) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetCPUMask());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestSetPriorityClass) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestSetPriorityLevel) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestSetStackSize) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetStackSize());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestSetCPUMask) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestResetThreadId) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestResetThreadId());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest, TestExecute) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestExecute());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestGetStatus) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetStatus());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStart) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStart_False) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStart_Restart) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStop) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStop_Kill) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStart_StartCommand) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_StartCommand());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestStart_StartCommandTimeout) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_StartCommandTimeout());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestGetTimeout) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetTimeout());
}

TEST(Scheduler_L3Services_EmbeddedThreadIGTest,TestSetTimeout) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetTimeout());
}
