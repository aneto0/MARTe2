/**
 * @file Logger.h
 * @brief Header file for class Logger
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

 * @details This header file contains the declaration of the class Logger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorManagement.h"
#include "FastResourceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Structure to hold information about a log event.
 */
struct LoggerPage {
    /**
     * Error information associated to the logging event.
     */
    ErrorManagement::ErrorInformation errorInfo;

    /**
     * Free format error message.
     */
    char8 errorStrBuffer[MAX_ERROR_MESSAGE_SIZE];

    /**
     * The page index.
     */
    uint32 index;
    /*lint -e{9150} [MISRA C++ Rule 11-0-1]. Justification this structure behaves like a POD.*/
};

/**
 * @brief The default number of pages.
 */
static const uint32 DEFAULT_NUMBER_OF_LOG_PAGES = 128u;

/**
 * @brief The Logger class registers a callback to the SetErrorProcessFunction and adds
 * the logs to a FastResourceContainer queue. These are expected to be consumed by
 * users of this class.
 */
class Logger {
public:

    /**
     * @brief Singleton access to the Logger.
     * @param[in] numberOfPages to set for the logger. This value can only be set the first time this method is called.
     * @details Registers the error callback function to the logger.
     * @return a pointer to the Logger.
     */
    static Logger *Instance(const uint32 numberOfPages = DEFAULT_NUMBER_OF_LOG_PAGES);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~Logger();

    /**
     * @brief Returns the oldest LoggerPage available or NULL if no LoggerPage is available.
     * @return the oldest LoggerPage available or NULL if no LoggerPage is available.
     * @warning this page must be later returned to the Logger (see ReturnPage)
     */
    LoggerPage *GetLogEntry();

    /**
     * @brief Returns the LoggerPage to the Logger.
     * @param[in] page the page to be returned.
     */
    void ReturnPage(const LoggerPage * const page);

    /**
     * @brief Returns a page that is used to write the log message received in the callback function.
     * @return the a free page to write the log message or NULL if no page is available.
     */
    LoggerPage *GetPage();

    /**
     * @brief Adds a page to be consumed (see GetLogEntry) by a user of this class.
     * @param[in] page the page to be consumed.
     */
    void AddLogEntry(const LoggerPage * const page);

    /**
     * @brief Gets the number of configured logger pages.
     * @return the number of configured logger pages.
     */
    uint32 GetNumberOfPages() const;

    /**
     * @brief Gets the number of LoggerPage elements that were not consumed yet (i.e. returned with ReturnPage).
     * @return the number of LoggerPage elements that were not consumed.
     */
    uint32 GetNumberOfLogs() const;
private:

    /**
     * @brief Default constructor.
     * @param[in] numberOfPages the number of pages to set for the logger.
     */
    /*lint -e{1704} private constructor for singleton implementation*/
    Logger(const uint32 numberOfPages);

    /**
     * The number of log pages.
     */
    uint32 nOfPages;

    /**
     * Array of LoggerPages.
     */
    LoggerPage *pages;

    /**
     * When a new log arrives a page is returned to this container (which is full in the beginning).
     * When a log is returned by the consumer a page is taken from this container. So that in the beginning
     * logsIndex is fully Taken and pagesIndex is fully returned.
     */
    FastResourceContainer logsIndex;

    /**
     * When a new log arrives a page is taken from this container.
     * When a log is returned by the consumer a page is returned to this container.
     */
    FastResourceContainer pagesIndex;

    /*lint -e{1712} This class does not have a default constructor because
     * the numberOfPages must be defined on construction and remain constant
     * during object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGER_H_ */

