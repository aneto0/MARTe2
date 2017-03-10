/**
 * @file LoggerService.cpp
 * @brief Source file for class LoggerService
 * @date 10/03/2017
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "LoggerService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

LoggerService::LoggerService() :
        ReferenceContainer(), EmbeddedServiceMethodBinderI(), logThreadService(*this) {
    consumers = NULL_PTR(LoggerConsumerI *);
    logger = NULL_PTR(Logger *);
}

LoggerService::~LoggerService() {
    if (consumers != NULL_PTR(LoggerConsumerI *)) {
        delete[] consumers;
    }
}

bool LoggerService::Initialise(StructuredDataI &data) {
    bool ok = ReferenceContainer::Initialise(data);
    uint32 cpuMask = 0xf;
    uint32 stackSize = THREADS_DEFAULT_STACKSIZE;
    uint32 numberOfLogPages = DEFAULT_NUMBER_OF_LOG_PAGES;
    if (ok) {
        ok = data.Read("CPUs", cpuMask);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Warning, "No CPUs defined");
        }
    }
    if (ok) {
        (void) data.Read("StackSize", stackSize);
        ok = (stackSize > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Warning, "StackSize must be > 0");
        }
    }
    if (ok) {
        (void) data.Read("NumberOfLogPages", numberOfLogPages);
        ok = (numberOfLogPages > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Warning, "NumberOfLogPages must be > 0");
        }
    }
    if (ok) {
        logger = Logger::Instance(numberOfLogPages);
        //logThreadService.
    }
    return ok;
}

}

