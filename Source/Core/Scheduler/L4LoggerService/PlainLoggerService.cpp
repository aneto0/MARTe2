/**
 * @file PlainLoggerService.h
 * @brief Header file for class PlainLoggerService
 * @date 28/06/2024
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class PlainLoggerService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorManagement.h"
#include "PlainLoggerService.h"
#include "ReferenceT.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
    
    PlainLoggerService::PlainLoggerService() :
            ReferenceContainer() {
        consumers = NULL_PTR(LoggerConsumerI **);
        nOfConsumers = 0u;
    }

    PlainLoggerService::~PlainLoggerService() {
        if (consumers != NULL_PTR(LoggerConsumerI **)) {
            delete[] consumers;
        }
        nOfConsumers = 0u;
    }

    bool PlainLoggerService::Initialise(StructuredDataI &data) {
        bool ok = ReferenceContainer::Initialise(data);
        if (ok) {
            uint32 tmpSize = Size();
            ok = (tmpSize > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::Warning, "At least one LoggerConsumerI must be added to the container");
                nOfConsumers = 0u;
            } else {
                nOfConsumers = tmpSize;
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

        if(ok) {
            PlainLoggerBinderSingleton& pLoggerSingleton = PlainLoggerBinderSingleton::Instance();
            ok = pLoggerSingleton.RegisterPlainLoggerService(this);
            if(!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Cannot register PlainLoggerService to the binder instance");
            }
        }

        return ok;
    }

    void PlainLoggerService::Log(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription) {
        if(nOfConsumers > 0) {
            LoggerPage tempPage;

            tempPage.errorInfo = errorInfo;
            tempPage.index = 0u;

            StringHelper::CopyN(&tempPage.errorStrBuffer[0], errorDescription, MAX_ERROR_MESSAGE_SIZE);

            for (uint32 i = 0u; (i < nOfConsumers); i++) {
                consumers[i]->ConsumeLogMessage(&tempPage);
            }
        }
    }

    void PlainLoggerService::Purge(ReferenceContainer &purgeList) {
        PlainLoggerBinderSingleton& pLoggerSingleton = PlainLoggerBinderSingleton::Instance();
        pLoggerSingleton.UnRegisterPlainLoggerService(this);

        ReferenceContainer::Purge(purgeList);
    }

    void PlainLoggerErrorProcessFunction(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription) {
        PlainLoggerBinderSingleton& pLoggerSingleton = PlainLoggerBinderSingleton::Instance();
        pLoggerSingleton.PropagateLog(errorInfo, errorDescription);
    }

    PlainLoggerBinderSingleton::PlainLoggerBinderSingleton() {
        mux.FastLock();
        plainLoggersList.Clean();
        mux.FastUnLock();

        SetErrorProcessFunction(&PlainLoggerErrorProcessFunction);
    }

    bool PlainLoggerBinderSingleton::RegisterPlainLoggerService(PlainLoggerService *plainLoggerService) {
        mux.FastLock();
        bool goodNOfLoggers = (plainLoggersList.GetSize() < PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS);
        bool addSuccessful = false;

        if(goodNOfLoggers) {
            addSuccessful = plainLoggersList.Add(plainLoggerService);
        }
        mux.FastUnLock();

        if(!goodNOfLoggers) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::FatalError, "Bind request of PlainLoggerService exceeds maximum (%u) number of bindable items", PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS);
        } else {
            if(!addSuccessful) {
                REPORT_ERROR_STATIC(MARTe::ErrorManagement::FatalError, "Cannot add PlainLoggerService to the binder list");
            }
        }

        return (goodNOfLoggers && addSuccessful);
    }

    void PlainLoggerBinderSingleton::UnRegisterPlainLoggerService(PlainLoggerService *plainLoggerService) {
        bool found = false;
        uint32 positionToRemove = 0u;
        mux.FastLock();
        for(uint32 i = 0; (i < plainLoggersList.GetSize()) && !(found); i++) {
            if(plainLoggersList[i] == plainLoggerService) {
                found = true;
                positionToRemove = i;
            }
        }

        if(found) {
            plainLoggersList.Remove(positionToRemove);
        }
        mux.FastUnLock();
    }

    void PlainLoggerBinderSingleton::PropagateLog(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription) {
        mux.FastLock();
        for(uint32 i = 0; (i < plainLoggersList.GetSize()); i++) {
            plainLoggersList[i]->Log(errorInfo, errorDescription);
        }
        mux.FastUnLock();
    }

    PlainLoggerBinderSingleton::~PlainLoggerBinderSingleton() {
        mux.FastLock();
        plainLoggersList.Clean();
        mux.FastUnLock();
    }

    CLASS_REGISTER(PlainLoggerService, "1.0")
}