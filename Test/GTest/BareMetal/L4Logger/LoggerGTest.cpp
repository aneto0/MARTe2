/**
 * @file LoggerGTest.cpp
 * @brief Source file for class LoggerGTest
 * @date 07/11/2016
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


#include "LoggerTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Logger_LoggerGTest,TestConstructor) {
    LoggerTest target;
    ASSERT_TRUE(target.TestConstructor());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestAddLogEntry) {
    LoggerTest target;
    ASSERT_TRUE(target.TestAddLogEntry());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestGetLogEntry) {
    LoggerTest target;
    ASSERT_TRUE(target.TestGetLogEntry());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestGetPage) {
    LoggerTest target;
    ASSERT_TRUE(target.TestGetPage());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestReturnPage) {
    LoggerTest target;
    ASSERT_TRUE(target.TestReturnPage());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestGetNumberOfPages) {
    LoggerTest target;
    ASSERT_TRUE(target.TestGetNumberOfPages());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestGetNumberOfLogs) {
    LoggerTest target;
    ASSERT_TRUE(target.TestGetNumberOfLogs());
}

TEST(BareMetal_L4Logger_LoggerGTest,TestLoggerErrorProcessFunction) {
    LoggerTest target;
    ASSERT_TRUE(target.TestLoggerErrorProcessFunction());
}
