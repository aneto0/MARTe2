/**
 * @file InternetService.cpp
 * @brief Source file for class InternetService
 * @date Nov 25, 2015
 * @author Frank Perez Paz
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
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"
#include "InternetService.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

InternetService::InternetService() {
    service.s_name=static_cast<char8*>(NULL);
    service.s_aliases = static_cast<char8 **>(NULL);
    service.s_port=0;
    service.s_proto=static_cast<char8*>(NULL);
}

bool InternetService::SearchByName(const char8 * const name,
                                   const char8 * const protocol) {
    servent *serv = getservbyname(name, protocol);
    bool ret = (serv != NULL);
    if (ret) {
        service = *serv;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetService: Failed getservbyname()");
    }
    return ret;

}

bool InternetService::SearchByPort(const uint16 port,
                                   const char8 * const protocol) {

    servent *serv = getservbyport(static_cast<int32>(htons(port)), protocol);
    bool ret = (serv != NULL);
    if (ret) {
        service = *serv;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetService: Failed getservbyport()");

    }
    return ret;

}

uint16 InternetService::Port() const {
    return htons(static_cast<uint16>(service.s_port));
}

const char8 *InternetService::Name() const {
    return service.s_name;
}

const char8 *InternetService::Protocol() const {
    return service.s_proto;
}


}


	
