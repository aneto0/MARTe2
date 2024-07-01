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

#ifndef PLAINLOGGERSERVICE_H_
#define PLAINLOGGERSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
#include "FastPollingMutexSem.h"
#include "Logger.h"
#include "LoggerConsumerI.h"
#include "ReferenceContainer.h"

#define PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS 4

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class PlainLoggerBinderSingleton;

/**
 * @brief The LoggerService registers itself as a callback for the logging messages and
 * synchronously offer them to all the registered LoggerConsumerI. To accomplish this, the ErrorProcessFunction
 * is catched to divert all the logging traffic synchronously on each print call.
 * @details The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +LoggerService = {
 *     Class = PlainLoggerService
 *     +LoggerConsumer1 = {
 *         Class = ALoggerConsumer
 *         ...
 *     }
 *     +LoggerConsumer2 = {
 *         Class = AnotherLoggerConsumer
 *         ...
 *     }
 * }
 * </pre>
 */
class DLL_API PlainLoggerService: public ReferenceContainer {
    public:
        CLASS_REGISTER_DECLARATION()
        /**
        * @brief Default Constructor. Initialises internal structures.
        */
        PlainLoggerService();

        /**
        * @brief Destructor. 
        */
        virtual ~PlainLoggerService();

        /**
        * @brief Calls ReferenceContainer::Initialise
        * @param[in] data see ReferenceContainer::Initialise.
        * @return true if CPUs is specified and at least one valid ConsumerI is registered. If one of the child elements does not implement
        *  the LoggerConsumerI interface it will return false.
        */
        virtual bool Initialise(StructuredDataI &data);

        void Log(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription);

        virtual void Purge(ReferenceContainer &purgeList);
        
    private:
        /**
        * Accelerated list of all the consumers.
        */
        LoggerConsumerI **consumers;

        /**
        * Number of consumers.
        */
        uint32 nOfConsumers;
};

class PlainLoggerBinderSingleton {
    
    public:

        static PlainLoggerBinderSingleton& Instance()
        {
            static PlainLoggerBinderSingleton instance;
            return instance;
        }

        ~PlainLoggerBinderSingleton();

        bool RegisterPlainLoggerService(PlainLoggerService *plainLoggerService);

        void UnRegisterPlainLoggerService(PlainLoggerService *plainLoggerService);

        void PropagateLog(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription);

    private:
        FastPollingMutexSem mux;

        uint32 registeredLoggerArrayIndex;

        PlainLoggerService* registeredPlainLoggers[PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS];

        PlainLoggerBinderSingleton();


        //Do not implement to follow singleton pattern
        PlainLoggerBinderSingleton(PlainLoggerBinderSingleton const&);
        void operator=(PlainLoggerBinderSingleton const&);
};

}

#endif /* PLAINLOGGERSERVICE_H_ */