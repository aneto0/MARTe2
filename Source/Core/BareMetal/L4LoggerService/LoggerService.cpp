/**
 * @file LoggerService.cpp
 * @brief Source file for class LoggerService
 * @date 05/11/2016
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
 * the class LoggerService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FastResourceContainer.h"
#include "LoggerService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
void LoggerServiceErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                       const char * const errorDescription) {
    LoggerService *loggerService = LoggerService::Instance();
    if (loggerService != NULL_PTR(LoggerService *)) {
        LoggerPage *page = loggerService->GetPage();
        if (page != NULL_PTR(LoggerPage *)) {
            page->errorInfo = errorInfo;
            MemoryOperationsHelper::Copy(page->errorStrBuffer, errorDescription, MAX_ERROR_MESSAGE_SIZE);
            loggerService->AddLogEntry(page);
        }
    }
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
LoggerService *LoggerService::Instance() {
    static LoggerService instance;
    return &instance;
}

LoggerService::LoggerService() :
        nOfPages(128u),
        logsIndex(nOfPages, true),
        pagesIndex(nOfPages, false) {
    pages = new LoggerPage[nOfPages];
    SetErrorProcessFunction(&LoggerServiceErrorProcessFunction);
}

LoggerService::~LoggerService() {

}

LoggerPage *LoggerService::GetPage() {
    LoggerPage *page = NULL_PTR(LoggerPage *);
    if (pages != NULL) {
        uint32 pageNo = pagesIndex.Take();
        if ((pageNo != 0xFFFFFFFFu) && (pageNo < nOfPages)) {
            page = &pages[pageNo];
        }
    }
    return page;
}

void LoggerService::ReturnPage(LoggerPage *page) {
    if (pages != NULL) {
        if (page != NULL) {
            int32 pageNo = page - pages;
            if ((pageNo > 0) && (pageNo < static_cast<int32>(nOfPages))) {
                pagesIndex.Return(pageNo);
            }
        }
    }
}

void LoggerService::AddLogEntry(LoggerPage *page) {
    if (pages != NULL) {
        uint pageNo = page - pages;
        if ((pageNo > 0) && (pageNo < nOfPages)) {
            logsIndex.Return(pageNo);
        }
    }
}

LoggerPage *LoggerService::GetLogEntry() {
    LoggerPage *page = NULL_PTR(LoggerPage *);
    if (pages != NULL) {
        uint32 pageNo = logsIndex.Take();
        if ((pageNo != 0xFFFFFFFFu) && (pageNo < nOfPages)) {
            page = &pages[pageNo];
        }
    }
    return page;
}

}

