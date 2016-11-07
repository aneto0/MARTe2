/**
 * @file LoggerService.h
 * @brief Header file for class LoggerService
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
#include "ErrorManagement.h"
#include "LoggerPage.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief TODO
 */
class LoggerService {
public:
    /**
     * @brief Singleton access to the GlobalObjectsDatabase.
     * @return a pointer to the GlobalObjectsDatabase.
     */
    static LoggerService *Instance();

    virtual ~LoggerService();

    LoggerPage *GetLogEntry();

    void ReturnPage(LoggerPage *page);

    LoggerPage *GetPage();

    void AddLogEntry(LoggerPage *page);

private:
    LoggerService();

    uint32 nOfPages;
    LoggerPage *pages;
    FastResourceContainer logsIndex;
    FastResourceContainer pagesIndex;


};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERSERVICE_H_ */

