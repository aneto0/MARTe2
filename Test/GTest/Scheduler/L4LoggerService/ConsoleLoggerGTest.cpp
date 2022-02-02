/**
 * @file ConsoleLoggerGTest.cpp
 * @brief Source file for class ConsoleLoggerGTest
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
 * the class ConsoleLoggerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "ConsoleLoggerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(Scheduler_L4LoggerService_ConsoleLoggerGTest,TestConstructor) {
    ConsoleLoggerTest target;
    ASSERT_TRUE(target.TestConstructor());
}

TEST(Scheduler_L4LoggerService_ConsoleLoggerGTest,TestInitialise) {
    ConsoleLoggerTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(Scheduler_L4LoggerService_ConsoleLoggerGTest,TestConsumeLogMessage) {
    ConsoleLoggerTest target;
    ASSERT_TRUE(target.TestConsumeLogMessage());
}

