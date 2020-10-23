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
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

LoggerService::LoggerService() :
        ReferenceContainer(), EmbeddedServiceMethodBinderI(), logThreadService(*this) {
    consumers = NULL_PTR(LoggerConsumerI **);
    logger = NULL_PTR(Logger *);
    nOfConsumers = 0u;
}

/*lint -e{1551} -e{1740} the destructor must guarantee that the SingleThreadService. The logger is a singleton and is freed by the Logger class at the end of program execution*/
LoggerService::~LoggerService() {
    if (logThreadService.Stop() != ErrorManagement::NoError) {
        if (logThreadService.Stop() != ErrorManagement::NoError) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not Stop the logThreadService");
        }
    }
    if (consumers != NULL_PTR(LoggerConsumerI **)) {
        delete[] consumers;
    }
}

bool LoggerService::Initialise(StructuredDataI &data) {
    bool ok = ReferenceContainer::Initialise(data);
    uint32 cpuMask = 0x1u;
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
        nOfConsumers = Size();
        ok = (nOfConsumers > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Warning, "At least one LoggerConsumerI must be added to the container");
        }
    }
    if (ok) {
        uint32 i;
        consumers = new LoggerConsumerI*[nOfConsumers];
        for (i = 0u; (i < nOfConsumers) && (ok); i++) {
            ReferenceT<LoggerConsumerI> consumer = Get(i);
            ok = (consumer.IsValid());
            if (ok) {
                /*lint -e{613} consumers is allocated before entering the for loop */
                consumers[i] = consumer.operator ->();
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Found a child which is not a LoggerConsumerI");
            }
        }
    }
    if (ok) {
        logger = Logger::Instance(numberOfLogPages);
        logThreadService.SetStackSize(stackSize);
        logThreadService.SetCPUMask(cpuMask);
        logThreadService.SetName(GetName());
        ok = logThreadService.Start();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not start the embedded thread.");
        }
    }
    return ok;
}

/*lint -e{715} the consuming algorithm is independent of the thread state*/
ErrorManagement::ErrorType LoggerService::Execute(ExecutionInfo & info) {
    if (logger != NULL_PTR(Logger *)) {
        bool terminate = (info.GetStage() == ExecutionInfo::TerminationStage);
        if (!terminate) {
            terminate = (info.GetStage() == ExecutionInfo::BadTerminationStage);
        }

        if (terminate){
            //If terminating wait a couple of seconds for the log to flush before...
            Sleep::Sec(1.0F);
        }
        uint32 i;
        if (consumers != NULL_PTR(LoggerConsumerI **)) {
            LoggerPage *page = logger->GetLogEntry();
            while (page != NULL_PTR(LoggerPage *)) {
                for (i = 0u; (i < nOfConsumers); i++) {
                    consumers[i]->ConsumeLogMessage(page);
                }
                logger->ReturnPage(page);
                page = logger->GetLogEntry();
            }
        }
        if (terminate) {
            //.. and after
            Sleep::Sec(1.0F);
        }
    }
    Sleep::Sec(1e-3F);
    return ErrorManagement::NoError;
}

uint32 LoggerService::GetNumberOfLogPages() const {
    uint32 n = 0u;
    if (logger != NULL_PTR(Logger *)) {
        n = logger->GetNumberOfPages();
    }
    return n;
}

ProcessorType LoggerService::GetCPUMask() const {
    return logThreadService.GetCPUMask();
}

uint32 LoggerService::GetStackSize() const {
    return logThreadService.GetStackSize();
}

CLASS_REGISTER(LoggerService, "1.0")

}

