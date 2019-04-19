/**
 * @file LoggerService.h
 * @brief Header file for class LoggerService
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

 * @details This header file contains the declaration of the class LoggerService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERSERVICE_H_
#define LOGGERSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
#include "Logger.h"
#include "LoggerConsumerI.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief The LoggerService registers itself as a callback for the logging messages and
 * asynchronously offer these messages to all the registered LoggerConsumerI.
 * @details The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +LoggerService = {
 *     Class = LoggerService
 *     CPUs = 0x1 //Compulsory. The CPU mask where the asynchronous thread will run.
 *     StackSize = 32768 //Optional. The stack size of the asynchronous thread.
 *     NumberOfLogPages = 128 //Optional. The number of log pages.
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
class LoggerService: public ReferenceContainer, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. Register the SingleThreadService callback function.
     */
LoggerService    ();

    /**
     * @brief Destructor. Stops the SingleThreadService callback function.
     */
    virtual ~LoggerService();

    /**
     * @brief Calls ReferenceContainer::Initialise and reads the CPUs and StackSize parameters info.
     * @param[in] data see ReferenceContainer::Initialise.
     * @return true if CPUs is specified and at least one valid ConsumerI is registered. If one of the child elements does not implement
     *  the LoggerConsumerI interface it will return false.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Callback function for the EmbeddedThread that polls data from the Logger.
     * @details Polls data from the Logger and if a new log message is available calls ConsumeLogMessage on all
     *  the registered consumers.
     * @param[in] info see EmbeddedServiceMethodBinderI
     * @return ErrorManagement::NoError.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Gets the configured number of log pages.
     * @return the configured number of log pages.
     * @pre
     *   Initialise()
     */
    uint32 GetNumberOfLogPages() const;

    /**
     * @brief Gets the configured CPU mask.
     * @return the configured CPU mask.
     * @pre
     *   Initialise()
     */
    ProcessorType GetCPUMask() const;

    /**
     * @brief Gets the configured stack size.
     * @return the configured stack size.
     * @pre
     *   Initialise()
     */
    uint32 GetStackSize() const;
private:

    /**
     * The thread that will Poll for new messages.
     */
    SingleThreadService logThreadService;

    /**
     * Pointer to the logger
     */
    Logger *logger;

    /**
     * Accelerated list of all the consumers.
     */
    LoggerConsumerI **consumers;

    /**
     * Number of consumers.
     */
    uint32 nOfConsumers;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERSERVICE_H_ */

