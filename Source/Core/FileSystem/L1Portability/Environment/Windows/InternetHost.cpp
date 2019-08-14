/**
 * @file InternetHost.cpp
 * @brief Source file for class InternetHost
 * @date Nov 24, 2015
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
 * the class InternetHost (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <windows.h>
//#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompositeErrorManagement.h"
#include "FastPollingMutexSem.h"
#include "InternetHost.h"
#include "DynamicCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


void InternetHost::GetAddress(DynamicCString &addrAsString) const {
	char8 buffer[256];
	inet_ntop(AF_INET,const_cast<PVOID>(reinterpret_cast<const void *>(&address.sin_addr)),&buffer[0],sizeof(buffer));
	CCString converted(&buffer[0]);

	addrAsString = converted;
}

bool InternetHost::GetHostName(DynamicCString &hostName) const {
	ErrorManagement::ErrorType ret;
	char8 buffer[512];

    ret.OSError = (getnameinfo(reinterpret_cast<const SOCKADDR *>(&address),sizeof (address),&buffer[0],sizeof(buffer), NULL, 0, 0)!=0);
    REPORT_ERROR(ret,"getnameinfo failed");

    if (!ret){
    	GetAddress(hostName);
    }

    return ret;
}


uint16 InternetHost::GetPort() const {
    return htons(address.sin_port);
}

void InternetHost::SetPort(const uint16 port) {
    address.sin_port = htons(port);
}

InternetHost::InternetHost(const uint16 port,CCString addr) {
	ErrorManagement::ErrorType ret;
    address.sin_family = static_cast<uint16>(AF_INET);
    address.sin_port = htons(port);

    ret.fatalError = SetAddress(addr);
    REPORT_ERROR(ret, "InternetHost: Failed SetAddress");
}

uint32 InternetHost::GetAddressAsNumber() const {
    return static_cast<uint32>(address.sin_addr.s_addr);
}

bool InternetHost::SetAddress(CCString addr) {
	ErrorManagement::ErrorType ret;
    address.sin_addr.s_addr = INADDR_ANY;

    ret.parametersError = (addr.GetSize() ==0);
    REPORT_ERROR(ErrorManagement::OSError, "InternetHost: SetAddres(empty)");

    uint32 iaddr = INADDR_NONE;
    if (ret) {
        iaddr = inet_addr(addr.GetList());

        ret.fatalError = (iaddr == INADDR_NONE);
        COMPOSITE_REPORT_ERROR(ret, "InternetHost: Failed inet_addr(",addr,"), address=0xFFFFFFFF");
    }

    if (ret){
        address.sin_addr.S_un.S_addr = iaddr;
    }

    return ret;
}

bool InternetHost::SetAddressByHostName(CCString hostName) {
	ErrorManagement::ErrorType ret;
    //  hostName can be NULL meaning localhost

    if (hostName.GetSize() == 0) {
        hostName = CCString("localhost");
    }

    // memory is in the TLS
    struct hostent *h = gethostbyname(hostName);
    ret.OSError = (h==NULL);
    COMPOSITE_REPORT_ERROR(ret,"InternetHost: Failed gethostbyname(",hostName,")");

    if (ret) {
        address.sin_addr.s_addr = *(reinterpret_cast<uint32 *>(h->h_addr_list[0]));
    }
    return ret;
}

void InternetHost::SetAddressByNumber(const uint32 number) {
    address.sin_addr.s_addr = number;
}

bool InternetHost::SetLocalAddress() {
    return SetAddressByHostName(static_cast<const char8*>(NULL));
}

InternetHostCore *InternetHost::GetInternetHost() {
    return &address;
}

uint32 InternetHost::Size() const {
    return static_cast<uint32>(sizeof(address));
}

}
