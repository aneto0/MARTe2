/**
 * @file LoggerConsumerIGTest.cpp
 * @brief Source file for class LoggerConsumerIGTest
 * @date 13/03/2017
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
 * the class LoggerConsumerIGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"
#include "LoggerConsumerITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(Scheduler_L4LoggerService_LoggerConsumerIGTest,TestLoadPrintPreferences) {
    LoggerConsumerITest target;
    ASSERT_TRUE(target.TestLoadPrintPreferences());
}

TEST(Scheduler_L4LoggerService_LoggerConsumerIGTest,TestLoadPrintPreferences_SomeKeys) {
    LoggerConsumerITest target;
    ASSERT_TRUE(target.TestLoadPrintPreferences_SomeKeys());
}

TEST(Scheduler_L4LoggerService_LoggerConsumerIGTest,TestLoadPrintPreferences_False_InvalidKey) {
    LoggerConsumerITest target;
    ASSERT_TRUE(target.TestLoadPrintPreferences_False_InvalidKey());
}

TEST(Scheduler_L4LoggerService_LoggerConsumerIGTest,TestLoadPrintPreferences_False_TooLarge) {
    LoggerConsumerITest target;
    ASSERT_TRUE(target.TestLoadPrintPreferences_False_TooLarge());
}

TEST(Scheduler_L4LoggerService_LoggerConsumerIGTest,TestPrintToStream) {
    LoggerConsumerITest target;
    ASSERT_TRUE(target.TestPrintToStream());
}

TEST(Scheduler_L4LoggerService_LoggerConsumerIGTest,TestPrintToStream_WithKeys) {
    LoggerConsumerITest target;
    ASSERT_TRUE(target.TestPrintToStream_WithKeys());
}
	
