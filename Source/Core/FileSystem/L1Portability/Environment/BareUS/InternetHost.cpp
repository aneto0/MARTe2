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
#ifdef LWIP_ENABLED
#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "InternetHost.h"
#include "FastPollingMutexSem.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class LocalHostInfo {

    public:
        static LocalHostInfo& Instance() {
            static LocalHostInfo instance;
            return instance;
        }

        ~LocalHostInfo() {
            #ifdef LWIP_ENABLED
                if (ipAddress != NULL) {
                    /*lint -e{586} -e{1551} [MISRA C++ Rule 18-4-1]. Justification: Use of free required. */
                    free(reinterpret_cast<void *>(const_cast<char8 *>(ipAddress)));
                }
                if (localHostName != NULL) {
                    /*lint -e{586} -e{1551} [MISRA C++ Rule 18-4-1]. Justification: Use of free required. */
                    free(reinterpret_cast<void *>(const_cast<char8 *>(localHostName)));
                }
            #endif
        }

        const char8 *GetLocalHostName() {
            Init();
            return localHostName;
        }

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

        /*lint -e{1704} .Justification: The constructor is private because this is a singleton.*/
        LocalHostInfo():localHostName(static_cast<const char8*>(NULL)),
                        ipAddress(static_cast<const char8*>(NULL)),
                        internetAddressInfoInitialised(false) {
            Init();
        }

        void Init() {
        #ifdef LWIP_ENABLED
            if (!internetAddressInfoInitialised) {
                #if LWIP_NETIF_HOSTNAME
                    localHostName = netif_default->hostname;
                #else
                    localHostName = "localhost";
                #endif
                ipAddress = StringHelper::StringDup(ip4addr_ntoa(&netif_default->ip_addr));
                internetAddressInfoInitialised = true;
            }
        #else   
        internetAddressInfoInitialised = true;
        #endif
        }
        LocalHostInfo(LocalHostInfo const&);              // Singleton! Do not Implement!
        void operator=(LocalHostInfo const&); // Singleton! Do not implement"
};

void InternetHost::SetMulticastGroup(const char8 *const addr) {
    #ifdef LWIP_ENABLED

    #endif
    //TODO: Implement set multicast group for the lwIP raw scenario
}

uint32 InternetHost::MulticastSize() const {
    #ifdef LWIP_ENABLED 
        
    #else
    //TODO: Implement MulticastSize for the lwIP raw scenario
        return 0u;
    #endif
}

StreamString InternetHost::GetMulticastGroup() const {
    #ifdef LWIP_ENABLED

    #else
    //TODO Implement the GetMulticastGroup
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "InternetHost::GetMulticastGroup() not implemented, returns empty string.");
    return "";
    #endif
}

InternetMulticastCore *InternetHost::GetInternetMulticastHost() {
    #ifdef LWIP_ENABLED

    #else
    //TODO Implement the GetInternetMulticastHost
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "InternetHost::GetInternetMulticastHost() not implemented, returns null ptr.");
    return NULL_PTR(InternetMulticastCore*);
    #endif
}

StreamString InternetHost::GetHostName() const {
    //TODO: Do something better here (hook/cfg)
    StreamString hostName = "";
    return hostName;
}

const char8 *InternetHost::GetLocalHostName() {
    return LocalHostInfo::Instance().GetLocalHostName();
}

const char8 *InternetHost::GetLocalAddress() {
    return LocalHostInfo::Instance().GetIpAddress();
}

uint32 InternetHost::GetLocalAddressAsNumber() {

    uint32 ret = 0u;
    uint32 comp[4];
    const char8* name = LocalHostInfo::Instance().GetIpAddress();
    if (name != NULL) {
        #ifdef LWIP_ENABLED
            sscanf(name, "%u.%u.%u.%u", &comp[3], &comp[2], &comp[1], &comp[0]);
            uint32 addressN = (comp[3] + (256u * (comp[2] + (256u * (comp[1] + (256u * comp[0]))))));
            ret= addressN;
        #endif
    }
    return ret;
}

InternetHost::InternetHost(const uint16 port,
                           const char8 * const addr) {
#ifdef LWIP_ENABLED
    SetPort(port);

    /*lint -e{1924} [MISRA C++ Rule 5-2-4]. Justification: The C-style cast is made by the operating system API.*/
    if(!SetAddress(addr)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "InternetHost::InternetHost() SetAddress() failed.");
    }
#endif
}

uint16 InternetHost::GetPort() const {
    uint16 port = 0u;
#ifdef LWIP_ENABLED
    port = htons(address.port);
#endif
    return port;
}

StreamString InternetHost::GetAddress() const {
    StreamString dotName;
#ifdef LWIP_ENABLED
    dotName = (inet_ntoa(address.addr));
#endif
    return dotName;
}

/**  returns the host number associated to this InternetHost*/
uint32 InternetHost::GetAddressAsNumber() const {
    uint32 ipAddrUInt32 = 0u;

    #ifdef LWIP_ENABLED
        //InternetHostCore wraps ip_addr_t which for IPv4 is a struct with a single u32
        ipAddrUInt32 = address.addr.addr;
    #endif
        return ipAddrUInt32;
}

/** sets the port value  */
void InternetHost::SetPort(const uint16 port) {
    #ifdef LWIP_ENABLED
        address.port = htons(port);
    #endif
}

bool InternetHost::SetAddress(const char8 * const addr) {
    bool ret = (addr != NULL);
#ifdef LWIP_ENABLED
    if(ret) {
        address.addr.addr = inet_addr(addr);
    }
#endif

    return ret;
}

bool InternetHost::SetAddressByHostName(const char8 * hostName) {
    bool ret = false;
    //  hostName can be NULL meaning localhost

    if (hostName == NULL) {
        hostName = "localhost";
        ret = true;
    }
#ifdef LWIP_ENABLED
    //TODO
    /*err_t err = netconn_gethostbyname(hostName, &address.addr);
    ret = (err == 0u);
    if (!ret) {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError,"InternetHost: Failed gethostbyname()");
    }*/
#endif
    return ret;
}

void InternetHost::SetAddressByNumber(const uint32 number) {
#ifdef LWIP_ENABLED
    address.addr.addr = htons(number);
#endif
}

/**  The address of the local host */
bool InternetHost::SetLocalAddress() {
    return SetAddressByHostName(static_cast<const char8*>(NULL));
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: sockets will change this attribute then the full access to this
 * member is allowed.
 */
InternetHostCore *InternetHost::GetInternetHost() {
    return &address;
}

uint32 InternetHost::Size() const {
    return static_cast<uint32>(sizeof(address));
}

}

