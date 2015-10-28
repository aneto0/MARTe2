/**
 * @file InternetAddress.cpp
 * @brief Source file for class InternetAddress
 * @date 26/ott/2015
 * @author pc
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
 * the class InternetAddress (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
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

#include "InternetAddress.h"
#include "Sleep.h"
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

class InternetAddressInfo {

public:
    //

    static InternetAddressInfo *Instance() {
        static InternetAddressInfo instance;
        return &instance;
    }
    //
    ~InternetAddressInfo() {
        if (ipNumber != NULL) {
            /*lint -e{586} -e{1551} [MISRA C++ Rule 18-4-1]. Justification: Use of free required. */
            free(reinterpret_cast<void *>(const_cast<char8 *>(ipNumber)));
        }
        if (localAddress != NULL) {
            /*lint -e{586} -e{1551} [MISRA C++ Rule 18-4-1]. Justification: Use of free required. */
            free(reinterpret_cast<void *>(const_cast<char8 *>(localAddress)));
        }
    }
    //
    const char8 *GetLocalAddress() {
        Init();
        return localAddress;
    }
    ///
    const char8 *GetIpNumber() {
        Init();
        return ipNumber;
    }

    bool Initialized() const {
        return internetAddressInfoInitialised;
    }

private:
    const char8 *localAddress;
    const char8 *ipNumber;
    bool internetAddressInfoInitialised;
    FastPollingMutexSem internalFastSem;

    /*lint -e{1704} .Justification: The constructor is private because this is a singleton.*/
    InternetAddressInfo():localAddress(static_cast<const char8*>(NULL)),ipNumber(static_cast<const char8*>(NULL)),internetAddressInfoInitialised(false),internalFastSem() {
        Init();
    }

    void Init() {
        if (!internetAddressInfoInitialised) {

            if(internalFastSem.FastLock()!=ErrorManagement::NoError) {
                REPORT_ERROR(ErrorManagement::FatalError,"Error: Failed FastPollingMutexSem::FastLock() in initialization of local address");
            }

            localAddress = static_cast<const char8*>(NULL);
            ipNumber = static_cast<const char8*>(NULL);

            char8 hostname[128];
            int32 ret = gethostname(&hostname[0], static_cast<osulong>(128u));
            struct hostent *h = static_cast<struct hostent *>(NULL);
            if (ret == 0) {
                h = gethostbyname(&hostname[0]);
            }
            if (h != NULL) {
                localAddress = strdup(h->h_name);
                struct in_addr sin_addr;
                sin_addr.s_addr = *(reinterpret_cast<uint32 *> (h->h_addr_list[0]));

                // Convert the ip number in a dot notation string
                ipNumber = strdup(inet_ntoa(sin_addr));
                internetAddressInfoInitialised = true;
                internalFastSem.FastUnLock();
            }
            else{
                REPORT_ERROR(ErrorManagement::FatalError,"Error: Failed local address initialization");
            }
        }
        return;
    }

};


String InternetAddress::GetHostName() const {

    if (hostnameFastSem.FastLock() != ErrorManagement::NoError) {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: Failed FastPollingMutexSem::FastLock() in initialization of local address");
    }
    String hostName = GetDotName();

    /*lint -e{923} [MISRA C++ Rule 5-2-7], [MISRA C++ Rule 5-2-8]. Justification: Cast from unsigned integer to pointer required by this implementation.*/
    struct hostent *h = gethostbyaddr(reinterpret_cast<char8 *>(address.sin_addr.s_addr), 4u, AF_INET);

// what's the point ??
// and it crashes if h is NULL
//       ia.address.sin_addr.s_addr = *((int *)(h->h_addr_list[0]));

    if (h != NULL) {
        hostName = h->h_name;
    }
    else{
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed gethostbyaddr()");
    }
    hostnameFastSem.FastUnLock();

    return (hostName.Size() == 0u) ? (static_cast<const char8 *>(NULL)):(hostName);
}

void InternetAddress::SocketInit() {
}

const char8 *InternetAddress::GetLocalAddress() {
    return InternetAddressInfo::Instance()->GetLocalAddress();
}

const char8 *InternetAddress::GetLocalIpNumber() {
    return InternetAddressInfo::Instance()->GetIpNumber();
}

uint32 InternetAddress::GetLocalAddressAsNumber() {

    uint32 ret = 0u;
    uint32 comp[4];
    const char8* name = InternetAddressInfo::Instance()->GetIpNumber();
    if (name != NULL) {
        sscanf(name, "%u.%u.%u.%u", &comp[3], &comp[2], &comp[1], &comp[0]);
        uint32 addressN = (comp[0] + (256u * (comp[1] + (256u * (comp[2] + (256u * comp[3]))))));
        ret= addressN;
    }
    return ret;
}

InternetAddress::InternetAddress(const uint16 port,
                                 const char8 * const addr) {

    address.sin_family = static_cast<uint16>(AF_INET);
    SetPort(port);
    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: The C-style cast is made by the operating system API.*/
    address.sin_addr.s_addr = INADDR_ANY;
    if (addr != NULL) {
        address.sin_addr.s_addr = inet_addr(const_cast<char8 *>(addr));
    }
}

uint16 InternetAddress::GetPort() const {
    return htons(address.sin_port);
}


String InternetAddress::GetDotName() const {
    String dotName(inet_ntoa(address.sin_addr));
    return dotName;
}

/**  returns the host number associated to this InternetAddress*/
uint32 InternetAddress::GetHostNumber() const {
    return static_cast<uint32>(address.sin_addr.s_addr);
}

/** sets the port value  */
void InternetAddress::SetPort(const uint16 port) {
    address.sin_port = htons(port);
}


bool InternetAddress::SetAddressByDotName(const char8 * const addr) {
    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: The C-style cast is made by the operating system API.*/
    address.sin_addr.s_addr = INADDR_ANY;
    bool ret = (addr != NULL);

    if (ret) {
        uint32 iaddr = inet_addr(const_cast<char8 *>(addr));

        if (iaddr != 0xFFFFFFFFu) {
            address.sin_addr.s_addr = iaddr;
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError,"Error: Failed inet_addr()");

            ret = false;
        }
    }
    return ret;
}

bool InternetAddress::SetAddressByName(const char8 * hostName) {
    if (hostName == NULL) {
        hostName = "localhost";
    }
    //  hostName can be NULL meaning localhost
    struct hostent *h = gethostbyname(hostName);
    bool ret = false;
    if (h != NULL) {
        address.sin_addr.s_addr = *(reinterpret_cast<uint32 *>(h->h_addr_list[0]));
        ret= true;
    }
    else{
        REPORT_ERROR(ErrorManagement::OSError,"Error: Failed gethostbyname()");

    }
    return ret;
}

void InternetAddress::SetAddressByNumber(const uint32 number) {
    address.sin_addr.s_addr = number;
}

/**  The address of the local host */
bool InternetAddress::SetLocalAddress() {
    return SetAddressByName(static_cast<const char8*>(NULL));
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: sockets will change this attribute then the full access to this
 * member is allowed.
 */
InternetAddressCore *InternetAddress::GetAddress() {
    return &address;
}

uint32 InternetAddress::Size() const {
    return static_cast<uint32>(sizeof(address));
}

}
