/**
 * @file LoggerTest.cpp
 * @brief Source file for class LoggerTest
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
 * the class LoggerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Logger.h"
#include "LoggerTest.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool LoggerTest::TestConstructor() {
    using namespace MARTe;
    Logger *logger = Logger::Instance();
    return (logger != NULL);
}

bool LoggerTest::TestGetLogEntry() {
    using namespace MARTe;
    Logger *logger = Logger::Instance();
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

bool LoggerTest::TestReturnPage() {
    return TestGetLogEntry();
}

bool LoggerTest::TestGetPage() {
    return TestGetLogEntry();
}

bool LoggerTest::TestAddLogEntry() {
    return TestGetLogEntry();
}

bool LoggerTest::TestGetNumberOfPages() {
    using namespace MARTe;
    Logger *logger = Logger::Instance(DEFAULT_NUMBER_OF_LOG_PAGES);
    return (logger->GetNumberOfPages() == DEFAULT_NUMBER_OF_LOG_PAGES);
}

bool LoggerTest::TestLoggerErrorProcessFunction() {
    using namespace MARTe;
    Logger *logger = Logger::Instance();
    LoggerPage *entry = logger->GetLogEntry();
    //Empty all the entries
    while (entry != NULL){
        logger->ReturnPage(entry);
        entry = logger->GetLogEntry();
    }
    uint32 i;
    for (i = 0; i < 100; i++) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Testing TestLoggerErrorProcessFunction %d", i)
    }
    bool ok = true;
    for (i = 0; (i < 100) && (ok); i++) {
        LoggerPage *entry = logger->GetLogEntry();
        if (ok) {
            ok = (entry != NULL);
        }
        if (ok) {
            ok = (entry->errorInfo.header.errorType == ErrorManagement::Information);
        }
        if (ok) {
            StreamString toCompare;
            toCompare.Printf("Testing TestLoggerErrorProcessFunction %d", i);
            ok = (toCompare == entry->errorStrBuffer);
        }
        if (ok) {
            logger->ReturnPage(entry);
        }
    }
    for (i = 0; (i < (DEFAULT_NUMBER_OF_LOG_PAGES * 2)) && (ok); i++) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Testing TestLoggerErrorProcessFunction %d", i)
        LoggerPage *entry = logger->GetLogEntry();
        if (ok) {
            ok = (entry != NULL);
        }
        if (ok) {
            ok = (entry->errorInfo.header.errorType == ErrorManagement::Information);
        }
        if (ok) {
            StreamString toCompare;
            toCompare.Printf("Testing TestLoggerErrorProcessFunction %d", i);
            ok = (toCompare == entry->errorStrBuffer);
        }
        if (ok) {
            logger->ReturnPage(entry);
        }
    }
    return ok;
}

bool LoggerTest::TestGetNumberOfLogs() {
    using namespace MARTe;
    Logger *logger = Logger::Instance();
    uint32 numberOfLogs = logger->GetNumberOfLogs();
    uint32 initNumberOfLogs = numberOfLogs;
    REPORT_ERROR(ErrorManagement::Information, "Testing TestLoggerErrorProcessFunction");
    numberOfLogs = logger->GetNumberOfLogs();
    bool ok = (numberOfLogs == (initNumberOfLogs + 1));
    LoggerPage *entry = logger->GetLogEntry();
    if (ok) {
        ok = (entry != NULL);
    }
    if (ok) {
        ok = (entry->errorInfo.header.errorType == ErrorManagement::Information);
    }
    if (ok) {
        ok = (StringHelper::Compare(entry->errorStrBuffer, "Testing TestLoggerErrorProcessFunction") == 0u);
    }
    if (ok) {
        logger->ReturnPage(entry);
    }
    if (ok) {
        numberOfLogs = logger->GetNumberOfLogs();
        ok = (numberOfLogs == initNumberOfLogs);
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

