/**
 * @file SocketStartup.cpp
 * @brief Source file for class InternetHost
 * @date June, 2019
 * @author Filippo Sartori
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
 * the class InternetHost (public, protected, and private). Be aware that some
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
#include "StartupManager.h"
#include "ErrorManagement.h"
#include "InternetHost.h"
#include "DynamicCString.h"
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(Socket,("SocketStartup",emptyString),(emptyString));

static DynamicCString localHostName;
static DynamicCString ipAddressString;
static uint32 ipAddress = 0;
static WSADATA wsaData;


ErrorManagement::ErrorType SocketStartup::Init(){
	ErrorManagement::ErrorType ret;

    // Initialize Winsock
    ret.OSError = (WSAStartup(MAKEWORD(2,2), &wsaData)!=0);
    REPORT_ERROR(ret,"SocketStartup: Failed WSAStartup");

    char8 hostName[128];
    if (ret){
        ret.OSError = (gethostname(&hostName[0], sizeof(hostName))!=0);
        REPORT_ERROR(ret,"SocketStartup: Failed gethostname");
    }

    struct hostent *h = static_cast<struct hostent *>(NULL);
    if (ret){
        h = gethostbyname(&hostName[0]);
        ret.OSError = (h == NULL);
        COMPOSITE_REPORT_ERROR(ret,"SocketStartup: Failed gethostbyname(",CCString(&hostName[0]),")");
    }

    char8* addr = NULL;
    if (ret){
        localHostName = CCString(h->h_name);

        addr = h->h_addr_list[0];
        ret.OSError = (addr == static_cast<char8 *>(NULL));
        REPORT_ERROR(ret,"SocketStartup: h_addr_list is NULL");
    }

    if (ret){
        struct in_addr sin_addr;
        sin_addr.S_un.S_addr = *(reinterpret_cast<uint32 *> (addr));
    	ipAddress = sin_addr.S_un.S_addr;
        ipAddressString = CCString(inet_ntoa(sin_addr));
    }
	return ret;
}

ErrorManagement::ErrorType SocketStartup::Finish(){
	ErrorManagement::ErrorType ret;
	WSACleanup();
	return ret;
}


CCString InternetHost::GetLocalHostName() {
    return localHostName;
}

CCString InternetHost::GetLocalAddress() {
    return ipAddressString;
}

uint32 InternetHost::GetLocalAddressAsNumber() {
    return ipAddress;
}


} //MARTe
