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
#include "ErrorManagement.h"
#include "FastPollingMutexSem.h"
#include "LoggerConsumerI.h"
#include "ReferenceContainer.h"
#include "StaticList.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

const uint32 PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS = 4u;

/**
 * Forward declaration of the PlainLoggerBinderSingleton class.
*/
class PlainLoggerBinderSingleton;

/**
 * @brief The LoggerService registers itself as a callback for the logging messages and
 * synchronously offer them to all the registered LoggerConsumerI. To accomplish this, the ErrorProcessFunction
 * is catched to divert all the logging traffic synchronously on each print call.
 * An ancillary class, based on the GoF singleton pattern (PlainLoggerBinderSingleton) is used to allow binding the SetErrorProcessFunction
 * to the real Log() function of the PlainLoggerService. Consider that the binding helper can serve up to PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS
 * instances of PlainLoggerService.
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

        /**
         * @brief Concrete call to produce the log output, used by the binder upon being triggered by the SetErrorProcessFunction results.
         *        Builds a LoggerPage on its own and propagates it to the logger children.
         * @param[in] errorInfo Information structure associated to the error or message being logged.
         * @param[in] errorDescription Message string associated to the error or message being logged.
         * 
        */
        void Log(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription) const;


        /**
         * @brief Allows the PlainLoggerService to do its own purge routine before going on with the purge process.
         * @param[in] purgeList The container with all the elements to be purged.
        */
        virtual void Purge(ReferenceContainer &purgeList);
        
    private:
        /**
        * @brief Accelerated list of all the consumers.
        */
        LoggerConsumerI **consumers;

        /**
        * @brief Number of consumers kept into the accelerated list.
        */
        uint32 nOfConsumers;
};

/**
 * @brief Helper singleton class to allow binding the SetErrorProcessFunction with the instance of the PlainLoggerService.
 * Intercepts the SetErrorProcessFunction and propagates it to the registered PlainLoggerService instances, up to a 
 * maximum PLAINLOGGER_MAX_NO_OF_REGISTRABLE_LOGGERS number of instances.
*/
/*lint -e{756} The singleton is referenced into the PlainLoggerService class */
class PlainLoggerBinderSingleton {
    
    public:
        /**
         * @brief Singleton instance method.
        */
        static PlainLoggerBinderSingleton& Instance()
        {
            static PlainLoggerBinderSingleton instance;
            return instance;
        }

        /**
         * @brief Default destructor
        */
        ~PlainLoggerBinderSingleton();

        /**
         * @brief Registers a PlainLoggerService to the array where the subscribers of SetErrorProcessFunction are kept
         * @param[in] plainLoggerService pointer to the instance being registered
         * @return true if the maximum number of registrable PlainLoggerServices has not reached, false otherwise
        */
        bool RegisterPlainLoggerService(PlainLoggerService * const plainLoggerService);

        /**
         * @brief Unregister a PlainLoggerService from the singleton array. Used during the purging process, to help
         * @param[in] plainLoggerService pointer to the instance being registered
         * avoid conditions where the logger is still produced while the PlainLoggerService is being purged.
        */
        /*lint -e{952} The plainLoggerService argument is used into a subsequent non-const call */
        void UnRegisterPlainLoggerService(PlainLoggerService * const plainLoggerService);

        /**
         * @brief Propagates the SetErrorProcessFunction to all the registered PlainLoggerService instances
         * @param[in] errorInfo Information structure associated to the error or message being logged.
         * @param[in] errorDescription Message string associated to the error or message being logged.
        */
        void PropagateLog(const ErrorManagement::ErrorInformation &errorInfo, const char8 * const errorDescription);

    private:

        /**
         * @brief Mux guarding the registered loggers array operations.
        */
        FastPollingMutexSem mux;

        /**
         * @brief Static list of PlainLoggerService instances that are registered to this binder facility
        */
        StaticList<PlainLoggerService*> plainLoggersList;

        /**
         * @brief Default constructor. Private for singleton class definition.
        */
        /*lint -e{1704} Default constructor is private per singleton pattern implementation. */
        PlainLoggerBinderSingleton();


        /**
         * @brief Copy constructor. Private for singleton class definition.
        */
        /*lint -e{1704} Copy constructor is private per singleton pattern implementation. */
        PlainLoggerBinderSingleton(PlainLoggerBinderSingleton const&);

        /**
         * @brief Assignment operator. Private for singleton class definition.
        */
        /*lint -e{1722} Assignment operator does not return anything and is private per singleton pattern implementation. */
        void operator=(PlainLoggerBinderSingleton const&);
};

}

#endif /* PLAINLOGGERSERVICE_H_ */
