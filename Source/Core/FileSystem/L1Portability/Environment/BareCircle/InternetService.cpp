/**
 * @file InternetService.cpp
 * @brief Source file for class InternetService
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

 * @details This source file contains the definition of all the methods for
 * the class InternetService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "InternetService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * As we are dealing with baremetal implementation, there are no other OS managed
 * services / ports which can be searched for. 
 * This is why this class implementation is totally blank.
 */
namespace MARTe {

    InternetService::InternetService() {
    }

    bool InternetService::SearchByName(const char8 * const name,
                                    const char8 * const protocol) {
        return false;
    }

    bool InternetService::SearchByPort(const uint16 port,
                                    const char8 * const protocol) {

        return false;
    }

    uint16 InternetService::Port() const {
        return 0;
    }

    const char8 *InternetService::Name() const {
        return NULL_PTR(char8*);
    }

    const char8 *InternetService::Protocol() const {
        return NULL_PTR(char8*);
    }

}
