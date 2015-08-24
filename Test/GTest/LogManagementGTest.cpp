/**
 * @file LogManagementGTest.cpp
 * @brief Source file for class LogManagementGTest
 * @date 24/08/2015
 * @author Giuseppe Ferrò
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
 * the class LogManagementGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "LogManagementTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(LogManagementGTest,TestToName) {
    LogManagementTest errorManTest;
    ASSERT_TRUE(errorManTest.TestToName());
}

TEST(LogManagementGTest,TestReportLogMessage) {
    LogManagementTest errorManTest;
    ASSERT_TRUE(errorManTest.TestReportLogMessage(Information,"Information","This is an error","FileError", 1, "TestReportError"));
}

TEST(LogManagementGTest,TestReportLogMessageFullContext) {
    LogManagementTest errorManTest;
    ASSERT_TRUE(errorManTest.TestReportLogMessageFullContext(Information,"Information","Error!!","FileError2", 1, "TestReportErrorFullContext",32));
}


TEST(LogManagementGTest,TestReportLogMessageMacro) {
    LogManagementTest errorManTest;
    ASSERT_TRUE(errorManTest.TestReportLogMessageMacro(Information,"This is an error","Information"));
}

TEST(LogManagementGTest,TestReportLogMessageMacroFullContext) {
    LogManagementTest errorManTest;
    ASSERT_TRUE(errorManTest.TestReportLogMessageMacroFullContext(Information,"Error!!","Information",32));
}

TEST(LogManagementGTest,TestSetLogMessageProcessFunction) {
    LogManagementTest errorManTest;
    ASSERT_TRUE(errorManTest.TestSetLogMessageProcessFunction());
}
