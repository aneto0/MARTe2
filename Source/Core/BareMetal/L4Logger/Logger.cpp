/**
 * @file Logger.cpp
 * @brief Source file for class Logger
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
 * the class Logger (public, protected, and private). Be aware that some
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
#include "../../BareMetal/L4Logger/Logger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Callback function for the logger. Add the logs to the FastResourceContainer queue.
 * @param[in] errorInfo the error information.
 * @param[in] errorDescription the error textual description.
 */
/*lint -estring(459, "*LoggerErrorProcessFunction*") this function is supposed to have access to the Logger singleton and to the error information.*/
void LoggerErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                       const char8 * const errorDescription) {
    Logger *loggerService = Logger::Instance();
    if (loggerService != NULL_PTR(Logger *)) {
        LoggerPage *page = loggerService->GetPage();
        if (page != NULL_PTR(LoggerPage *)) {
            page->errorInfo = errorInfo;
            (void)MemoryOperationsHelper::Copy(&page->errorStrBuffer[0u], errorDescription, MAX_ERROR_MESSAGE_SIZE);
            loggerService->AddLogEntry(page);
        }
    }
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
Logger *Logger::Instance(const uint32 numberOfPages) {
    static Logger instance(numberOfPages);
    return &instance;
}

Logger::Logger(const uint32 numberOfPages) :
        nOfPages(numberOfPages),
        logsIndex(nOfPages, true),
        pagesIndex(nOfPages, false) {
    /*lint -e{1732} -e{1733} new in constructor safe as this class can only be used as a singleton*/
    pages = new LoggerPage[nOfPages];
    SetErrorProcessFunction(&LoggerErrorProcessFunction);
}

Logger::~Logger() {
    if (pages != NULL_PTR(LoggerPage *)) {
        delete [] pages;
    }

}

LoggerPage *Logger::GetPage() {
    LoggerPage *page = NULL_PTR(LoggerPage *);
    if (pages != NULL_PTR(LoggerPage *)) {
        uint32 pageNo = pagesIndex.Take();
        if ((pageNo != 0xFFFFFFFFu) && (pageNo < nOfPages)) {
            page = &pages[pageNo];
            page->index = pageNo;
        }
    }
    return page;
}

void Logger::ReturnPage(const LoggerPage * const page) {
    if (pages != NULL_PTR(LoggerPage *)) {
        if (page != NULL_PTR(LoggerPage *)) {
            pagesIndex.Return(page->index);
        }
    }
}

void Logger::AddLogEntry(const LoggerPage * const page) {
    if (pages != NULL_PTR(LoggerPage *)) {
        logsIndex.Return(page->index);
    }
}

LoggerPage *Logger::GetLogEntry() {
    LoggerPage *page = NULL_PTR(LoggerPage *);
    if (pages != NULL_PTR(LoggerPage *)) {
        uint32 pageNo = logsIndex.Take();
        if ((pageNo != 0xFFFFFFFFu) && (pageNo < nOfPages)) {
            page = &pages[pageNo];
        }
    }
    return page;
}

uint32 Logger::GetNumberOfPages() const {
    return nOfPages;
}

uint32 Logger::GetNumberOfLogs() const {
    return logsIndex.GetSize();
}

}

