/**
 * @file InternetHost.cpp
 * @brief Source file for class InternetHost
 * @date 28/10/2015
 * @author Giuseppe Ferro
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
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetHost.h"
#include "FastPollingMutexSem.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

FastPollingMutexSem hostnameFastSem;

class LocalHostInfo {

public:
    //

    static LocalHostInfo* Instance() {
        static LocalHostInfo instance;
        return &instance;
    }
    //
    ~LocalHostInfo() {
        if (ipAddress != NULL) {
            /*lint -e{586} -e{1551} [MISRA C++ Rule 18-4-1]. Justification: Use of free required. */
            free(reinterpret_cast<void *>(const_cast<char8 *>(ipAddress)));
        }
        if (localHostName != NULL) {
            /*lint -e{586} -e{1551} [MISRA C++ Rule 18-4-1]. Justification: Use of free required. */
            free(reinterpret_cast<void *>(const_cast<char8 *>(localHostName)));
        }
    }
    //
    const char8 *GetLocalHostName() {
        Init();
        return localHostName;
    }
    ///
    const char8 *GetIpAddress() {
        Init();
        return ipAddress;
    }

    bool Initialized() const {
        return internetAddressInfoInitialised;
    }

private:
    const char8 *localHostName;
    const char8 *ipAddress;
    bool internetAddressInfoInitialised;
    FastPollingMutexSem internalFastSem;

    /*lint -e{1704} .Justification: The constructor is private because this is a singleton.*/
    LocalHostInfo():localHostName(static_cast<const char8*>(NULL)),ipAddress(static_cast<const char8*>(NULL)),internetAddressInfoInitialised(false),internalFastSem() {
        Init();
    }

    void Init() {
        if (!internetAddressInfoInitialised) {

            if(internalFastSem.FastLock()!=ErrorManagement::NoError) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed FastPollingMutexSem::FastLock() in initialization of local address");
            }

            localHostName = static_cast<const char8*>(NULL);
            ipAddress = static_cast<const char8*>(NULL);

            char8 hostname[128];
            int32 ret = gethostname(&hostname[0], static_cast<size_t>(128u));
            struct hostent *h = static_cast<struct hostent *>(NULL);
            if (ret == 0) {
                h = gethostbyname(&hostname[0]);
            }
            if (h != NULL) {
                localHostName = strdup(h->h_name);
                struct in_addr sin_addr;
                char8* addr=h->h_addr_list[0];
                if(addr!=static_cast<char8 *>(NULL)) {
                    sin_addr.s_addr = *(reinterpret_cast<uint32 *> (addr));

                    // Convert the ip number in a dot notation string
                    ipAddress = strdup(inet_ntoa(sin_addr));
                    internetAddressInfoInitialised = true;
                    internalFastSem.FastUnLock();
                }
                else {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed local address initialization");
                }
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed local address initialization");
            }
        }
        return;
    }

};

StreamString InternetHost::GetHostName() const {

    if (hostnameFastSem.FastLock() != ErrorManagement::NoError) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "InternetHost: Failed FastPollingMutexSem::FastLock() in initialization of local address");
    }
    StreamString hostName = GetAddress();

    struct hostent *h = gethostbyaddr((&address.sin_addr.s_addr), 4u, AF_INET);

// what's the point ??
// and it crashes if h is NULL
//       ia.address.sin_addr.s_addr = *((int *)(h->h_addr_list[0]));

    if (h != NULL) {
        hostName = h->h_name;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError,"InternetHost: Failed gethostbyaddr()");
    }
    hostnameFastSem.FastUnLock();

    return (hostName.Size() == 0u) ? (static_cast<const char8*>(NULL)):(hostName);
}

const char8* InternetHost::GetLocalHostName() {
    return LocalHostInfo::Instance()->GetLocalHostName();
}

const char8* InternetHost::GetLocalAddress() {
    return LocalHostInfo::Instance()->GetIpAddress();
}

uint32 InternetHost::GetLocalAddressAsNumber() {

    uint32 ret = 0u;
    uint32 comp[4];
    const char8 *name = LocalHostInfo::Instance()->GetIpAddress();
    if (name != NULL) {
        sscanf(name, "%u.%u.%u.%u", &comp[3], &comp[2], &comp[1], &comp[0]);
        uint32 addressN = (comp[3] + (256u * (comp[2] + (256u * (comp[1] + (256u * comp[0]))))));
        ret= addressN;
    }
    return ret;
}

InternetHost::InternetHost(const uint16 port,
                           const char8 *const addr) {
    mreq.imr_interface.s_addr = htonl(static_cast<in_addr_t>(0x00000000)); //INADDR_ANY

    address.sin_family = static_cast<uint16>(AF_INET);
    SetPort(port);
    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: The C-style cast is made by the operating system API.*/
    if (!SetAddress(addr)) {
    }
}

uint16 InternetHost::GetPort() const {
    return htons(address.sin_port);
}

StreamString InternetHost::GetAddress() const {
    StreamString dotName(inet_ntoa(address.sin_addr));
    return dotName;
}

/**  returns the host number associated to this InternetHost*/
uint32 InternetHost::GetAddressAsNumber() const {
    return static_cast<uint32>(address.sin_addr.s_addr);
}

/** sets the port value  */
void InternetHost::SetPort(const uint16 port) {
    address.sin_port = htons(port);
}

bool InternetHost::SetAddress(const char8 *const addr) {
    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: The C-style cast is made by the operating system API.*/
    address.sin_addr.s_addr = INADDR_ANY;
    bool ret = (addr != NULL);

    if (ret) {
        uint32 iaddr = inet_addr(const_cast<char8*>(addr));

        if (iaddr != 0xFFFFFFFFu) {
            address.sin_addr.s_addr = iaddr;
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetHost: Failed inet_addr(), address=0xFFFFFFFF");
            ret = false;
        }
    }
    return ret;
}

bool InternetHost::SetAddressByHostName(const char8 *hostName) {
    bool ret = false;
    //  hostName can be NULL meaning localhost

    if (hostName == NULL) {
        hostName = "localhost";
    }
    struct hostent *h = gethostbyname(hostName);

    if (h != NULL) {
        address.sin_addr.s_addr = *(reinterpret_cast<uint32 *>(h->h_addr_list[0]));
        ret= true;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError,"InternetHost: Failed gethostbyname()");
    }
    return ret;
}

void InternetHost::SetAddressByNumber(const uint32 number) {
    address.sin_addr.s_addr = number;
}

/**  The address of the local host */
bool InternetHost::SetLocalAddress() {
    return SetAddressByHostName(static_cast<const char8*>(NULL));
}

void InternetHost::SetMulticastGroup(const char8 *const addr) {
    mreq.imr_multiaddr.s_addr = inet_addr(const_cast<char8*>(addr));
}

StreamString InternetHost::GetMulticastGroup() const {
    StreamString dotName(inet_ntoa(mreq.imr_multiaddr));
    return dotName;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: sockets will change this attribute then the full access to this
 * member is allowed.
 */
InternetMulticastCore *InternetHost::GetInternetMulticastHost() {
    return &mreq;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: sockets will change this attribute then the full access to this
 * member is allowed.
 */
InternetHostCore* InternetHost::GetInternetHost() {
    return &address;
}

uint32 InternetHost::Size() const {
    return static_cast<uint32>(sizeof(address));
}

uint32 InternetHost::MulticastSize() const {
    return static_cast<uint32>(sizeof(mreq));
}

}

