/**
 * @file InternetService.h
 * @brief Header file for class InternetService
 * @date 27/10/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class InternetService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERNETSERVICE_H_
#define INTERNETSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,InternetServiceCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief Class which allows to query the system for "well-known" services
     * by name/protocol or by port/protocol.
     */
    class DLL_API InternetService {

    public:

        /**
         * @brief Initializes the InternetServiceCore structure.
         */
        InternetService();

        /**
         * @brief Sets the InternetService handle structure searching in the
         * database the service name and the protocol provided.
         * @param[in] name is the internet service name to search.
         * @param[in] protocol is the protocol used in the search.
         * @details if protocol is NULL any protocol will be matched.
         */
        bool SearchByName(const char8 * const name,
                const char8 * const protocol = static_cast<const char8 *>(NULL));

        /**
         * @brief Sets the InternetService handle structure searching in the
         * database by port.
         * @param[in] port is the port to search.
         * @param[in] protocol is the protocol used in the search.
         * @details if protocol is NULL any protocol will be matched.
         */
        bool SearchByPort(const uint16 port,const char8 *const protocol=static_cast<char8 *>(NULL));

        /**
         * @brief Retrieves the internet service port.
         */
        uint16 Port() const;

        /**
         * @brief Retrieves the internet service name.
         */
        const char8 *Name() const;

        /**
         * @brief Retrieves the internet service protocol.
         */
        const char8 *Protocol() const;

    private:

        /**
         * The InternetService handle.
         */
        InternetServiceCore service;
    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_FILESYSTEM_L1PORTABILITY_INTERNETSERVICE_H_ */

