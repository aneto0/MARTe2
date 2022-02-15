/**
 * @file LoggerServiceGTest.cpp
 * @brief Source file for class LoggerServiceGTest
 * @date 10/03/2017
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
 * the class LoggerServiceGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "LoggerServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestConstructor) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestConstructor());
}


TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise_Defaults) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise_Defaults());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise_False_NoCPUs) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_NoCPUs());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise_False_NoConsumers) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_NoConsumers());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise_False_NotLoggerConsumerI) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_NotLoggerConsumerI());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise_False_StackSize_Zero) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_StackSize_Zero());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestInitialise_False_NumberOfLoggerPages_Zero) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_NumberOfLoggerPages_Zero());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestGetCPUMask) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestGetCPUMask());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestGetNumberOfLogPages) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestGetNumberOfLogPages());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestGetStackSize) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestGetStackSize());
}

TEST(Scheduler_L4LoggerService_LoggerServiceGTest,TestExecute) {
    LoggerServiceTest target;
    ASSERT_TRUE(target.TestExecute());
}
