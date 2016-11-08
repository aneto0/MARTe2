/**
 * @file LoggerServiceTest.cpp
 * @brief Source file for class LoggerServiceTest
 * @date 07/11/2016
 * @author Andre' Neto
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
 * the class LoggerServiceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LoggerService.h"
#include "LoggerServiceTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool LoggerServiceTest::TestConstructor() {
    using namespace MARTe;
    LoggerService *logger = LoggerService::Instance();
    return (logger != NULL);
}

bool LoggerServiceTest::TestGetLogEntry() {
    using namespace MARTe;
    LoggerService *logger = LoggerService::Instance();
    LoggerPage *page = logger->GetPage();
    bool ok = (page != NULL);
    if (ok) {
        logger->AddLogEntry(page);
        LoggerPage *entry = logger->GetLogEntry();
        ok = (entry != NULL);
        logger->ReturnPage(entry);
    }
    return ok;
}

bool LoggerServiceTest::TestReturnPage() {
    return TestGetLogEntry();
}

bool LoggerServiceTest::TestGetPage() {
    return TestGetLogEntry();
}

bool LoggerServiceTest::TestAddLogEntry() {
    return TestGetLogEntry();
}

bool LoggerServiceTest::TestLoggerServiceErrorProcessFunction() {
    using namespace MARTe;
    LoggerService *logger = LoggerService::Instance();
    REPORT_ERROR(ErrorManagement::Information, "Testing TestLoggerServiceErrorProcessFunction");

    LoggerPage *entry = logger->GetLogEntry();
    bool ok = (entry != NULL);
    if (ok) {
        ok = (entry->errorInfo.header.errorType == ErrorManagement::Information);
    }
    if (ok) {
        ok = (StringHelper::Compare(entry->errorStrBuffer, "Testing TestLoggerServiceErrorProcessFunction") == 0u);
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

