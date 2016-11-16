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

#include "gtest/gtest.h"
#include "EmbeddedThreadITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(EmbeddedThreadIGTest, TestConstructor) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}


TEST(EmbeddedThreadIGTest, TestGetThreadId) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetThreadId());
}

TEST(EmbeddedThreadIGTest, TestGetCommands) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetCommands());
}

TEST(EmbeddedThreadIGTest, TestSetCommands) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetCommands());
}

TEST(EmbeddedThreadIGTest, TestGetPriorityClass) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetPriorityClass());
}

TEST(EmbeddedThreadIGTest, TestGetPriorityLevel) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetPriorityLevel());
}

TEST(EmbeddedThreadIGTest, TestGetCPUMask) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetCPUMask());
}

TEST(EmbeddedThreadIGTest, TestSetPriorityClass) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(EmbeddedThreadIGTest, TestSetPriorityLevel) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(EmbeddedThreadIGTest, TestSetCPUMask) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(EmbeddedThreadIGTest, TestResetThreadId) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestResetThreadId());
}

TEST(EmbeddedThreadIGTest, TestExecute) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestExecute());
}

TEST(EmbeddedThreadIGTest,TestGetStatus) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestGetStatus());
}

TEST(EmbeddedThreadIGTest,TestStart) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(EmbeddedThreadIGTest,TestStart_False) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(EmbeddedThreadIGTest,TestStart_Restart) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(EmbeddedThreadIGTest,TestStop) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(EmbeddedThreadIGTest,TestStop_Kill) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(EmbeddedThreadIGTest,TestStart_StartCommand) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_StartCommand());
}

TEST(EmbeddedThreadIGTest,TestStart_StartCommandTimeout) {
    EmbeddedThreadITest target;
    ASSERT_TRUE(target.TestStart_StartCommandTimeout());
}
